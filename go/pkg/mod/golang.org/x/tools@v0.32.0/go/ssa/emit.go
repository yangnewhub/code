// Copyright 2013 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package ssa

// Helpers for emitting SSA instructions.

import (
	"fmt"
	"go/ast"
	"go/token"
	"go/types"

	"golang.org/x/tools/internal/typeparams"
)

// emitAlloc emits to f a new Alloc instruction allocating a variable
// of type typ.
//
// The caller must set Alloc.Heap=true (for a heap-allocated variable)
// or add the Alloc to f.Locals (for a frame-allocated variable).
//
// During building, a variable in f.Locals may have its Heap flag
// set when it is discovered that its address is taken.
// These Allocs are removed from f.Locals at the end.
//
// The builder should generally call one of the emit{New,Local,LocalVar} wrappers instead.
func emitAlloc(f *Function, typ types.Type, pos token.Pos, comment string) *Alloc {
	v := &Alloc{Comment: comment}
	v.setType(types.NewPointer(typ))
	v.setPos(pos)
	f.emit(v)
	return v
}

// emitNew emits to f a new Alloc instruction heap-allocating a
// variable of type typ. pos is the optional source location.
func emitNew(f *Function, typ types.Type, pos token.Pos, comment string) *Alloc {
	alloc := emitAlloc(f, typ, pos, comment)
	alloc.Heap = true
	return alloc
}

// emitLocal creates a local var for (t, pos, comment) and
// emits an Alloc instruction for it.
//
// (Use this function or emitNew for synthetic variables;
// for source-level variables in the same function, use emitLocalVar.)
func emitLocal(f *Function, t types.Type, pos token.Pos, comment string) *Alloc {
	local := emitAlloc(f, t, pos, comment)
	f.Locals = append(f.Locals, local)
	return local
}

// emitLocalVar creates a local var for v and emits an Alloc instruction for it.
// Subsequent calls to f.lookup(v) return it.
// It applies the appropriate generic instantiation to the type.
func emitLocalVar(f *Function, v *types.Var) *Alloc {
	alloc := emitLocal(f, f.typ(v.Type()), v.Pos(), v.Name())
	f.vars[v] = alloc
	return alloc
}

// emitLoad emits to f an instruction to load the address addr into a
// new temporary, and returns the value so defined.
func emitLoad(f *Function, addr Value) *UnOp {
	v := &UnOp{Op: token.MUL, X: addr}
	v.setType(typeparams.MustDeref(addr.Type()))
	f.emit(v)
	return v
}

// emitDebugRef emits to f a DebugRef pseudo-instruction associating
// expression e with value v.
func emitDebugRef(f *Function, e ast.Expr, v Value, isAddr bool) {
	if !f.debugInfo() {
		return // debugging not enabled
	}
	if v == nil || e == nil {
		panic("nil")
	}
	var obj types.Object
	e = ast.Unparen(e)
	if id, ok := e.(*ast.Ident); ok {
		if isBlankIdent(id) {
			return
		}
		obj = f.objectOf(id)
		switch obj.(type) {
		case *types.Nil, *types.Const, *types.Builtin:
			return
		}
	}
	f.emit(&DebugRef{
		X:      v,
		Expr:   e,
		IsAddr: isAddr,
		object: obj,
	})
}

// emitArith emits to f code to compute the binary operation op(x, y)
// where op is an eager shift, logical or arithmetic operation.
// (Use emitCompare() for comparisons and Builder.logicalBinop() for
// non-eager operations.)
func emitArith(f *Function, op token.Token, x, y Value, t types.Type, pos token.Pos) Value {
	switch op {
	case token.SHL, token.SHR:
		x = emitConv(f, x, t)
		// y may be signed or an 'untyped' constant.

		// There is a runtime panic if y is signed and <0. Instead of inserting a check for y<0
		// and converting to an unsigned value (like the compiler) leave y as is.

		if isUntyped(y.Type().Underlying()) {
			// Untyped conversion:
			// Spec https://go.dev/ref/spec#Operators:
			// The right operand in a shift expression must have integer type or be an untyped constant
			// representable by a value of type uint.
			y = emitConv(f, y, types.Typ[types.Uint])
		}

	case token.ADD, token.SUB, token.MUL, token.QUO, token.REM, token.AND, token.OR, token.XOR, token.AND_NOT:
		x = emitConv(f, x, t)
		y = emitConv(f, y, t)

	default:
		panic("illegal op in emitArith: " + op.String())

	}
	v := &BinOp{
		Op: op,
		X:  x,
		Y:  y,
	}
	v.setPos(pos)
	v.setType(t)
	return f.emit(v)
}

// emitCompare emits to f code compute the boolean result of
// comparison 'x op y'.
func emitCompare(f *Function, op token.Token, x, y Value, pos token.Pos) Value {
	xt := x.Type().Underlying()
	yt := y.Type().Underlying()

	// Special case to optimise a tagless SwitchStmt so that
	// these are equivalent
	//   switch { case e: ...}
	//   switch true { case e: ... }
	//   if e==true { ... }
	// even in the case when e's type is an interface.
	// TODO(adonovan): opt: generalise to x==true, false!=y, etc.
	if x == vTrue && op == token.EQL {
		if yt, ok := yt.(*types.Basic); ok && yt.Info()&types.IsBoolean != 0 {
			return y
		}
	}

	if types.Identical(xt, yt) {
		// no conversion necessary
	} else if isNonTypeParamInterface(x.Type()) {
		y = emitConv(f, y, x.Type())
	} else if isNonTypeParamInterface(y.Type()) {
		x = emitConv(f, x, y.Type())
	} else if _, ok := x.(*Const); ok {
		x = emitConv(f, x, y.Type())
	} else if _, ok := y.(*Const); ok {
		y = emitConv(f, y, x.Type())
	} else {
		// other cases, e.g. channels.  No-op.
	}

	v := &BinOp{
		Op: op,
		X:  x,
		Y:  y,
	}
	v.setPos(pos)
	v.setType(tBool)
	return f.emit(v)
}

// isValuePreserving returns true if a conversion from ut_src to
// ut_dst is value-preserving, i.e. just a change of type.
// Precondition: neither argument is a named or alias type.
func isValuePreserving(ut_src, ut_dst types.Type) bool {
	// Identical underlying types?
	if types.IdenticalIgnoreTags(ut_dst, ut_src) {
		return true
	}

	switch ut_dst.(type) {
	case *types.Chan:
		// Conversion between channel types?
		_, ok := ut_src.(*types.Chan)
		return ok

	case *types.Pointer:
		// Conversion between pointers with identical base types?
		_, ok := ut_src.(*types.Pointer)
		return ok
	}
	return false
}

// emitConv emits to f code to convert Value val to exactly type typ,
// and returns the converted value.  Implicit conversions are required
// by language assignability rules in assignments, parameter passing,
// etc.
func emitConv(f *Function, val Value, typ types.Type) Value {
	t_src := val.Type()

	// Identical types?  Conversion is a no-op.
	if types.Identical(t_src, typ) {
		return val
	}
	ut_dst := typ.Underlying()
	ut_src := t_src.Underlying()

	// Conversion to, or construction of a value of, an interface type?
	if isNonTypeParamInterface(typ) {
		// Interface name change?
		if isValuePreserving(ut_src, ut_dst) {
			c := &ChangeType{X: val}
			c.setType(typ)
			return f.emit(c)
		}

		// Assignment from one interface type to another?
		if isNonTypeParamInterface(t_src) {
			c := &ChangeInterface{X: val}
			c.setType(typ)
			return f.emit(c)
		}

		// Untyped nil constant?  Return interface-typed nil constant.
		if ut_src == tUntypedNil {
			return zeroConst(typ)
		}

		// Convert (non-nil) "untyped" literals to their default type.
		if t, ok := ut_src.(*types.Basic); ok && t.Info()&types.IsUntyped != 0 {
			val = emitConv(f, val, types.Default(ut_src))
		}

		// Record the types of operands to MakeInterface, if
		// non-parameterized, as they are the set of runtime types.
		t := val.Type()
		if f.typeparams.Len() == 0 || !f.Prog.isParameterized(t) {
			addMakeInterfaceType(f.Prog, t)
		}

		mi := &MakeInterface{X: val}
		mi.setType(typ)
		return f.emit(mi)
	}

	// conversionCase describes an instruction pattern that maybe emitted to
	// model d <- s for d in dst_terms and s in src_terms.
	// Multiple conversions can match the same pattern.
	type conversionCase uint8
	const (
		changeType conversionCase = 1 << iota
		sliceToArray
		sliceToArrayPtr
		sliceTo0Array
		sliceTo0ArrayPtr
		convert
	)
	// classify the conversion case of a source type us to a destination type ud.
	// us and ud are underlying types (not *Named or *Alias)
	classify := func(us, ud types.Type) conversionCase {
		// Just a change of type, but not value or representation?
		if isValuePreserving(us, ud) {
			return changeType
		}

		// Conversion from slice to array or slice to array pointer?
		if slice, ok := us.(*types.Slice); ok {
			var arr *types.Array
			var ptr bool
			// Conversion from slice to array pointer?
			switch d := ud.(type) {
			case *types.Array:
				arr = d
			case *types.Pointer:
				arr, _ = d.Elem().Underlying().(*types.Array)
				ptr = true
			}
			if arr != nil && types.Identical(slice.Elem(), arr.Elem()) {
				if arr.Len() == 0 {
					if ptr {
						return sliceTo0ArrayPtr
					} else {
						return sliceTo0Array
					}
				}
				if ptr {
					return sliceToArrayPtr
				} else {
					return sliceToArray
				}
			}
		}

		// The only remaining case in well-typed code is a representation-
		// changing conversion of basic types (possibly with []byte/[]rune).
		if !isBasic(us) && !isBasic(ud) {
			panic(fmt.Sprintf("in %s: cannot convert term %s (%s [within %s]) to type %s [within %s]", f, val, val.Type(), us, typ, ud))
		}
		return convert
	}

	var classifications conversionCase
	underIs(ut_src, func(us types.Type) bool {
		return underIs(ut_dst, func(ud types.Type) bool {
			if us != nil && ud != nil {
				classifications |= classify(us, ud)
			}
			return classifications != 0
		})
	})
	if classifications == 0 {
		panic(fmt.Sprintf("in %s: cannot convert %s (%s) to %s", f, val, val.Type(), typ))
	}

	// Conversion of a compile-time constant value?
	if c, ok := val.(*Const); ok {
		// Conversion to a basic type?
		if isBasic(ut_dst) {
			// Conversion of a compile-time constant to
			// another constant type results in a new
			// constant of the destination type and
			// (initially) the same abstract value.
			// We don't truncate the value yet.
			return NewConst(c.Value, typ)
		}
		// Can we always convert from zero value without panicking?
		const mayPanic = sliceToArray | sliceToArrayPtr
		if c.Value == nil && classifications&mayPanic == 0 {
			return NewConst(nil, typ)
		}

		// We're converting from constant to non-constant type,
		// e.g. string -> []byte/[]rune.
	}

	switch classifications {
	case changeType: // representation-preserving change
		c := &ChangeType{X: val}
		c.setType(typ)
		return f.emit(c)

	case sliceToArrayPtr, sliceTo0ArrayPtr: // slice to array pointer
		c := &SliceToArrayPointer{X: val}
		c.setType(typ)
		return f.emit(c)

	case sliceToArray: // slice to arrays (not zero-length)
		ptype := types.NewPointer(typ)
		p := &SliceToArrayPointer{X: val}
		p.setType(ptype)
		x := f.emit(p)
		unOp := &UnOp{Op: token.MUL, X: x}
		unOp.setType(typ)
		return f.emit(unOp)

	case sliceTo0Array: // slice to zero-length arrays (constant)
		return zeroConst(typ)

	case convert: // representation-changing conversion
		c := &Convert{X: val}
		c.setType(typ)
		return f.emit(c)

	default: // The conversion represents a cross product.
		c := &MultiConvert{X: val, from: t_src, to: typ}
		c.setType(typ)
		return f.emit(c)
	}
}

// emitTypeCoercion emits to f code to coerce the type of a
// Value v to exactly type typ, and returns the coerced value.
//
// Requires that coercing v.Typ() to typ is a value preserving change.
//
// Currently used only when v.Type() is a type instance of typ or vice versa.
// A type v is a type instance of a type t if there exists a
// type parameter substitution σ s.t. σ(v) == t. Example:
//
//	σ(func(T) T) == func(int) int for σ == [T ↦ int]
//
// This happens in instantiation wrappers for conversion
// from an instantiation to a parameterized type (and vice versa)
// with σ substituting f.typeparams by f.typeargs.
func emitTypeCoercion(f *Function, v Value, typ types.Type) Value {
	if types.Identical(v.Type(), typ) {
		return v // no coercion needed
	}
	// TODO(taking): for instances should we record which side is the instance?
	c := &ChangeType{
		X: v,
	}
	c.setType(typ)
	f.emit(c)
	return c
}

// emitStore emits to f an instruction to store value val at location
// addr, applying implicit conversions as required by assignability rules.
func emitStore(f *Function, addr, val Value, pos token.Pos) *Store {
	typ := typeparams.MustDeref(addr.Type())
	s := &Store{
		Addr: addr,
		Val:  emitConv(f, val, typ),
		pos:  pos,
	}
	f.emit(s)
	return s
}

// emitJump emits to f a jump to target, and updates the control-flow graph.
// Postcondition: f.currentBlock is nil.
func emitJump(f *Function, target *BasicBlock) {
	b := f.currentBlock
	b.emit(new(Jump))
	addEdge(b, target)
	f.currentBlock = nil
}

// emitIf emits to f a conditional jump to tblock or fblock based on
// cond, and updates the control-flow graph.
// Postcondition: f.currentBlock is nil.
func emitIf(f *Function, cond Value, tblock, fblock *BasicBlock) {
	b := f.currentBlock
	b.emit(&If{Cond: cond})
	addEdge(b, tblock)
	addEdge(b, fblock)
	f.currentBlock = nil
}

// emitExtract emits to f an instruction to extract the index'th
// component of tuple.  It returns the extracted value.
func emitExtract(f *Function, tuple Value, index int) Value {
	e := &Extract{Tuple: tuple, Index: index}
	e.setType(tuple.Type().(*types.Tuple).At(index).Type())
	return f.emit(e)
}

// emitTypeAssert emits to f a type assertion value := x.(t) and
// returns the value.  x.Type() must be an interface.
func emitTypeAssert(f *Function, x Value, t types.Type, pos token.Pos) Value {
	a := &TypeAssert{X: x, AssertedType: t}
	a.setPos(pos)
	a.setType(t)
	return f.emit(a)
}

// emitTypeTest emits to f a type test value,ok := x.(t) and returns
// a (value, ok) tuple.  x.Type() must be an interface.
func emitTypeTest(f *Function, x Value, t types.Type, pos token.Pos) Value {
	a := &TypeAssert{
		X:            x,
		AssertedType: t,
		CommaOk:      true,
	}
	a.setPos(pos)
	a.setType(types.NewTuple(
		newVar("value", t),
		varOk,
	))
	return f.emit(a)
}

// emitTailCall emits to f a function call in tail position.  The
// caller is responsible for all fields of 'call' except its type.
// Intended for wrapper methods.
// Precondition: f does/will not use deferred procedure calls.
// Postcondition: f.currentBlock is nil.
func emitTailCall(f *Function, call *Call) {
	tresults := f.Signature.Results()
	nr := tresults.Len()
	if nr == 1 {
		call.typ = tresults.At(0).Type()
	} else {
		call.typ = tresults
	}
	tuple := f.emit(call)
	var ret Return
	switch nr {
	case 0:
		// no-op
	case 1:
		ret.Results = []Value{tuple}
	default:
		for i := range nr {
			v := emitExtract(f, tuple, i)
			// TODO(adonovan): in principle, this is required:
			//   v = emitConv(f, o.Type, f.Signature.Results[i].Type)
			// but in practice emitTailCall is only used when
			// the types exactly match.
			ret.Results = append(ret.Results, v)
		}
	}
	f.emit(&ret)
	f.currentBlock = nil
}

// emitImplicitSelections emits to f code to apply the sequence of
// implicit field selections specified by indices to base value v, and
// returns the selected value.
//
// If v is the address of a struct, the result will be the address of
// a field; if it is the value of a struct, the result will be the
// value of a field.
func emitImplicitSelections(f *Function, v Value, indices []int, pos token.Pos) Value {
	for _, index := range indices {
		if isPointerCore(v.Type()) {
			fld := fieldOf(typeparams.MustDeref(v.Type()), index)
			instr := &FieldAddr{
				X:     v,
				Field: index,
			}
			instr.setPos(pos)
			instr.setType(types.NewPointer(fld.Type()))
			v = f.emit(instr)
			// Load the field's value iff indirectly embedded.
			if isPointerCore(fld.Type()) {
				v = emitLoad(f, v)
			}
		} else {
			fld := fieldOf(v.Type(), index)
			instr := &Field{
				X:     v,
				Field: index,
			}
			instr.setPos(pos)
			instr.setType(fld.Type())
			v = f.emit(instr)
		}
	}
	return v
}

// emitFieldSelection emits to f code to select the index'th field of v.
//
// If wantAddr, the input must be a pointer-to-struct and the result
// will be the field's address; otherwise the result will be the
// field's value.
// Ident id is used for position and debug info.
func emitFieldSelection(f *Function, v Value, index int, wantAddr bool, id *ast.Ident) Value {
	if isPointerCore(v.Type()) {
		fld := fieldOf(typeparams.MustDeref(v.Type()), index)
		instr := &FieldAddr{
			X:     v,
			Field: index,
		}
		instr.setPos(id.Pos())
		instr.setType(types.NewPointer(fld.Type()))
		v = f.emit(instr)
		// Load the field's value iff we don't want its address.
		if !wantAddr {
			v = emitLoad(f, v)
		}
	} else {
		fld := fieldOf(v.Type(), index)
		instr := &Field{
			X:     v,
			Field: index,
		}
		instr.setPos(id.Pos())
		instr.setType(fld.Type())
		v = f.emit(instr)
	}
	emitDebugRef(f, id, v, wantAddr)
	return v
}

// createRecoverBlock emits to f a block of code to return after a
// recovered panic, and sets f.Recover to it.
//
// If f's result parameters are named, the code loads and returns
// their current values, otherwise it returns the zero values of their
// type.
//
// Idempotent.
func createRecoverBlock(f *Function) {
	if f.Recover != nil {
		return // already created
	}
	saved := f.currentBlock

	f.Recover = f.newBasicBlock("recover")
	f.currentBlock = f.Recover

	var results []Value
	// Reload NRPs to form value tuple.
	for _, nr := range f.results {
		results = append(results, emitLoad(f, nr))
	}

	f.emit(&Return{Results: results})

	f.currentBlock = saved
}
