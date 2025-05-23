// Copyright 2021 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Package gen is used to generate command bindings from the gopls command
// interface.
package gen

import (
	"bytes"
	"fmt"
	"go/types"
	"log"
	"text/template"

	"golang.org/x/tools/gopls/internal/protocol/command/commandmeta"
	"golang.org/x/tools/internal/imports"
)

const src = `// Copyright 2024 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Don't include this file during code generation, or it will break the build
// if existing interface methods have been modified.
//go:build !generate
// +build !generate

// Code generated by gen.go from gopls/internal/protocol/command. DO NOT EDIT.

package command

import (
	{{range $k, $v := .Imports -}}
	"{{$k}}"
	{{end}}
)

// Symbolic names for gopls commands, corresponding to methods of [Interface].
//
// The string value is used in the Command field of protocol.Command.
// These commands may be obtained from a CodeLens or CodeAction request
// and executed by an ExecuteCommand request.
const (
{{- range .Commands}}
	{{.MethodName}} Command = "{{.Name}}"
{{- end}}
)

var Commands = []Command {
{{- range .Commands}}
	{{.MethodName}},
{{- end}}
}

func Dispatch(ctx context.Context, params *protocol.ExecuteCommandParams, s Interface) (any, error) {
	switch Command(params.Command) {
	{{- range .Commands}}
	case {{.MethodName}}:
		{{- if .Args -}}
			{{- range $i, $v := .Args}}
		var a{{$i}} {{typeString $v.Type}}
			{{- end}}
		if err := UnmarshalArgs(params.Arguments{{range $i, $v := .Args}}, &a{{$i}}{{end}}); err != nil {
			return nil, err
		}
		{{end -}}
		return {{if not .Result}}nil, {{end}}s.{{.MethodName}}(ctx{{range $i, $v := .Args}}, a{{$i}}{{end}})
	{{- end}}
	}
	return nil, fmt.Errorf("unsupported command %q", params.Command)
}
{{- range .Commands}}

{{if fallible .Args}}
func New{{.MethodName}}Command(title string, {{range $i, $v := .Args}}{{if $i}}, {{end}}a{{$i}} {{typeString $v.Type}}{{end}}) (*protocol.Command, error) {
	args, err := MarshalArgs({{range $i, $v := .Args}}{{if $i}}, {{end}}a{{$i}}{{end}})
	if err != nil {
		return nil, err
	}
	return &protocol.Command{
		Title: title,
		Command: {{.MethodName}}.String(),
		Arguments: args,
	}, nil
}
{{else}}
func New{{.MethodName}}Command(title string, {{range $i, $v := .Args}}{{if $i}}, {{end}}a{{$i}} {{typeString $v.Type}}{{end}}) *protocol.Command {
	return &protocol.Command{
		Title: title,
		Command: {{.MethodName}}.String(),
		Arguments: MustMarshalArgs({{range $i, $v := .Args}}{{if $i}}, {{end}}a{{$i}}{{end}}),
	}
}
{{end}}

{{end}}
`

type data struct {
	Imports  map[string]bool
	Commands []*commandmeta.Command
}

// Generate computes the new contents of ../command_gen.go from a
// static analysis of the command.Interface type.
func Generate() ([]byte, error) {
	cmds, err := commandmeta.Load()
	if err != nil {
		return nil, fmt.Errorf("loading command data: %v", err)
	}
	const thispkg = "golang.org/x/tools/gopls/internal/protocol/command"
	qual := func(p *types.Package) string {
		if p.Path() == thispkg {
			return ""
		}
		return p.Name()
	}
	tmpl, err := template.New("").Funcs(template.FuncMap{
		"typeString": func(t types.Type) string {
			return types.TypeString(t, qual)
		},
		"fallible": func(args []*commandmeta.Field) bool {
			var fallible func(types.Type) bool
			fallible = func(t types.Type) bool {
				switch t := t.Underlying().(type) {
				case *types.Basic:
					return false
				case *types.Slice:
					return fallible(t.Elem())
				case *types.Struct:
					for i := 0; i < t.NumFields(); i++ {
						if fallible(t.Field(i).Type()) {
							return true
						}
					}
					return false
				}
				// Assume all other types are fallible for now:
				log.Println("Command.Args has fallible type", t)
				return true
			}
			for _, arg := range args {
				if fallible(arg.Type) {
					return true
				}
			}
			return false
		},
	}).Parse(src)
	if err != nil {
		return nil, err
	}
	d := data{
		Commands: cmds,
		Imports: map[string]bool{
			"context": true,
			"fmt":     true,
			"golang.org/x/tools/gopls/internal/protocol": true,
		},
	}
	for _, c := range d.Commands {
		for _, arg := range c.Args {
			pth := pkgPath(arg.Type)
			if pth != "" && pth != thispkg {
				d.Imports[pth] = true
			}
		}
		if c.Result != nil {
			pth := pkgPath(c.Result.Type)
			if pth != "" && pth != thispkg {
				d.Imports[pth] = true
			}
		}
	}

	var buf bytes.Buffer
	if err := tmpl.Execute(&buf, d); err != nil {
		return nil, fmt.Errorf("executing: %v", err)
	}

	opts := &imports.Options{
		AllErrors:  true,
		FormatOnly: true,
		Comments:   true,
	}
	content, err := imports.Process("", buf.Bytes(), opts)
	if err != nil {
		return nil, fmt.Errorf("goimports: %v", err)
	}
	return content, nil
}

func pkgPath(t types.Type) string {
	type hasTypeName interface { // *Named or *Alias (or *TypeParam)
		Obj() *types.TypeName
	}
	if t, ok := t.(hasTypeName); ok {
		if pkg := t.Obj().Pkg(); pkg != nil {
			return pkg.Path()
		}
	}
	return ""
}
