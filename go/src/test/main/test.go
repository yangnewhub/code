// package main //声明文件所在的包
// import "fmt" //引入程序需要的包
// func main(){
//     fmt.Println("hello worlaaad")
//         fmt.Println("hello worlaaad")

// }
//变量

// package main
// import "fmt"

// var n11 = 11
// var (
//     n99 = 10
//     n109 = "jack"
// )

// func main(){
//     var age int
//     age=18
//     fmt.Println("age = ",age)

//     var age2 int = 19
//     fmt.Println("age2 = ",age2)

//     // var num int = 12.56
//     // fmt.Println(num)

//     // var age3=1
//     // age4 := 10

//     // var m1,m2 int
//     var n4,name,n5 = 1,"tom",2
//     fmt.Println(n4);
//     fmt.Println(name)
//     fmt.Println(n5);

//     n6,hight  := 1,22
//     fmt.Println(n6);
//     fmt.Println(hight);

//     fmt.Println(n99);
//     fmt.Println(n109);
//     fmt.Println(n11);

// }

// package main
// import (
//     "fmt"
//     "unsafe"
// )
// func main(){
//     var num int8 = 20
//     fmt.Println(unsafe.Sizeof(num))
// }

//浮点
// package main

// import "fmt"

// func main(){
//     var num float32 = 1.12
//     fmt.Println(num)
// }

//字符

// package main

// import "fmt"

// func main(){
//     var ch1 byte = 'a'
//     fmt.Println(ch1)
//     fmt.Printf("ch1: %c",ch1)

// }

// func main() {
// 	const name, age = "Kim", 22
// 	fmt.Printf("%s is %d years old.\n", name, age)

// 	// It is conventional not to worry about any
// 	// error returned by Printf.

// }

// //布尔

// package main
// import "fmt"

// func main(){
//     var tmp bool = true

// }

// //字符串

// package main

// import "fmt"

// func main(){
//     var s1 string = "aaaaaa"
//     var s2 string =`sss"ssx"`
//     var s3 string ="aa"+"bb"+
//     "cc"+"dd"

//     fmt.Println(s1,s2,s3)

//     var n1 int =10
//     var n2 float32 = float32(n1)
// }

//强制转化

// package main

// import "fmt"

// func main(){
//     // var n1 int =19
//     //var n2 float32 = 1.1
//     var n3 bool = false
//     // var n4 byte = 'a'

//     var s string =  fmt.Sprintf("%t",n3)
//     fmt.Printf("%v",s)

// }

//指针

//分包
// package main

// import (
//     "fmt"
//     "test/test2"
// )

// func main(){
//     fmt.Println(test2.StuNo)
// }

//输入

// package main

// import "fmt"

// func main(){

//     var age int
//     var name string
//     var score int
//     fmt.Scanln(&age,&name,&score)
//     fmt.Println(age,name,score)
// }

// package main

// import "fmt"

// func main(){

//     var age int
//     var name string
//     var score int
//     fmt.Scanf("%d %s %d",&age,&name,&score)
//     fmt.Println(age,name,score)
// }

//分支

// package main

// import "fmt"

// func main(){
//     // if(false){
//     //     fmt.Println("aa")
//     // }else if(true){
//     //     fmt.Println("abb")
//     // }else
//     // {
//     //     fmt.Println("cc")
//     // }

//     // var socre int = 88

//     // switch socre/10 {
//     //     case 8:
//     //         fmt.Println("8")

//     //     case 7:
//     //         fmt.Println("7")
//     //     default:

//     // }

//     // var sum int = 0
//     // for i:=1;i<=5;i++{
//     //     sum+=i
//     // }
//     // fmt.Println(sum)

//     //死循环
//     // for {
//     //     fmt.Println("q")
//     // }、

//     // var str string ="abcde"
//     // for i:=0 ;i<len(str);i++{
//     //     fmt.Printf("%c",str[i])
//     // }
//     // for i,value :=range str{
//     //     value='a'
//     //     fmt.Println(i,value)
//     // }
//     // fmt.Println(str)
//     lable:
//     for i:=1;i<=5;i++{
//         for j:=1;j<=5;j++{
//             fmt.Printf("i=%v,j=%v\n",i,j)
//             if(i==2&&j==2){
//                 break lable
//             }
//         }
//     }
// }

//函数

// package main

// import "fmt"

// func Add(a int, b int)(int){
//     return a+b
//     //fmt.Println(a+b)
// }

// func Add2(a int, b int)(int,int){
//     return a+b,b-a
// }
// func Swap(a *int,b *int)(){
//     var c int = *a
//     *a= *b
//     *b=c
// }

// func test(args...int)(){
//     //函数内部处理可变参数的时候,可以将参数切片处理
//     //遍历:
//     for i:=0;i<len(args);i++{
//         fmt.Println(args[i])
//     }

//     for i,value := range args{
//         fmt.Println(i,value)
//     }
// }

// func main(){
//     var ret int = Add(1,2)
//     fmt.Println(ret)

//     // Add(1,2)
//     sum1,sub2:=Add2(1,2)
//     fmt.Println(sum1,sub2)
//     var a,b  = 1,2
//     Swap(&a,&b)
//     fmt.Println(a,b)

//     test(1)
//     test(1,2)

// }

// package main

// import "fmt"
// import "test/test2"

// func test(num int)(){
//     fmt.Println(num)
// }

// func test2(num int , testfunc func(int)){
//     testfunc(num)
// }
// func main(){
//     test2(10,test)

//     fmt.Printf(test2.Add(1,2))
// }

//init
// package main
// import "fmt"

// func init(){
//     fmt.Println("init")
// }
// func main(){
//     fmt.Println("main")
// }

//匿名函数

// package main

// import "fmt"

// func main(){
//     result := func(num1 int,num2 int)(int){
//         return num1+num2
//     }
//     fmt.Println(result(10,20))
// }

//分包

// package main

// import (
// 	"fmt"
// 	"test/test2"
// )

// func main() {
// 	fmt.Println(test2.StuNo)
// }

//闭包

// package main

// import "fmt"

// func getSum() (func (int) (int)) {
//     var sum int = 0
//     return func (num int)(int){
//         sum+=num
//         return sum
//     }
// }
// func main(){
//     f := getSum()
//     fmt.Println(f(1))  //  ->1
//     fmt.Println(f(2))  //  ->3
// }

//defer

// package main

// import "fmt"

// func main(){
//     add(1,2)
// }
// func add(num1 int ,num2 int) (int){
//     defer fmt.Println("num1 =",num1)
//     defer fmt.Println("num2 =",num2)
//     num1+=1
//     num2+=2
//     fmt.Println("sum =",num1+num2)
//     return num1+num2
// }

//字符串

// package main

// import "fmt"
// import "strconv"
// import "strings"
// import "time"
// func main(){
//     str := "abcdef哈哈"  //go中一个汉字3个字节
//     //统计字符串长度
//     fmt.Println("str长度为",len(str))

//     //字符串遍历
//     //方式一
//     for i,value:= range str{
//         fmt.Printf("i = %d value = %c\n",i,value)
//     }
//     //方式二 切片
//     r := []rune(str)
//     for i:=0;i<len(r);i++{
//         fmt.Printf("%c",r[i])
//     }

//     //字符串转数字
//     num,_:=strconv.Atoi("666")
//     fmt.Println(num)
//     //数字转字符串
//     s:=strconv.Itoa(88)
//     fmt.Println(s)

//     //统计字符串中子串的个数

//     count := strings.Count(" ","abc")
//     fmt.Println(count)

//     //不区分大小写比较

//     flag := strings.EqualFold("hello","Hello")
//     fmt.Println(flag)

//     //返回子串在字符串第一次出现的位置

//     index := strings.Index("abcd","g")
//     fmt.Println(index)

//     //字符串替换
//     str1 := strings.Replace("gogogogogogog","go","goo",-1)
//     str2 := strings.Replace("gogogogogogog","go","goo",2)

//     fmt.Println(str1)
//     fmt.Println(str2)

//     //字符串切割
//     arr := strings.Split("ab-ac-dd","-")
//     fmt.Println(arr)
//     //大小写转化

//     fmt.Println(strings.ToLower("Go"))
//     fmt.Println(strings.ToUpper("Go"))

//     //将字符串左右两边空格去除
//     fmt.Println(strings.TrimSpace("  ss  "))
//     //去掉指定字符
//     fmt.Println(strings.Trim("--aaa-aaa--","-"))
//     //指定左边 右边   TrimLeft
//     //判断字符串是否以指定字符串开头 strings.HasPrefix("hh.a","hh")
//     //判断字符串是否以指定字符串结尾 strings.HaSuf`fix("hh.a","a")
//     now := time.Now()
//     fmt.Println(now)

// }

// package main

// import "fmt"

// func main() {
// 	fmt.Println(len("aaa"))

//		num := new(int)
//		fmt.Println((num))
//	}
// package main

// import (
// 	"errors"
// 	"fmt"
// )

// func main() {
// 	test()

// 	fmt.Println("执行成功")
// }

// func test() (err error) {

// 	num1 := 1
// 	num2 := 0
// 	if num2 == 0 {
// 		return errors.New("err")
// 	} else {
// 		fmt.Println(num1 / num2)
// 		return nil
// 	}
// }

// func test() {

// 	defer func() {
// 		err := recover()
// 		//如果没有捕获错误,返回值为nil
// 		if err != nil {
// 			fmt.Println("err是", err)
// 		}
// 	}()
// 	num1 := 1
// 	num2 := 0
// 	fmt.Println(num1 / num2)
// }

// package main

// import "fmt"

// func main() {
// 	// var scores [5]int
// 	// for i := 0; i < 5; i++ {
// 	// 	fmt.Scanf("%d", &scores[i])
// 	// }
// 	// for i := 0; i < len(scores); i++ {
// 	// 	fmt.Println(scores[i])
// 	// }
// 	// // for i,value:=range scores{

// 	// // }
// 	// fmt.Printf("%T", scores)
// 	var arr1 [3]int = [3]int{1, 2, 3}
// 	fmt.Println(arr1)

// 	var arr2 = [3]int{1, 2, 3}

// 	test(&arr2)
// 	fmt.Println(arr2)
// 	var arr3 = [2][3]int{{1, 2, 3}, {4, 5, 6}}
// 	fmt.Println(arr3)

// 	for _, v1 := range arr3 {
// 		for _, v2 := range v1 {
// 			fmt.Println(v2)
// 		}
// 	}
// }

// func test(arr *[3]int) {
// 	arr[0] = 3
// }

//切片

// func main() {
// 	//var arr [6]int = [6]int{1, 2, 3, 4, 5, 6}
// 	//定义一个切片，slic,[]表示动态的，所以长度不写,int表示类型
// 	//[1:3] 从1开始，到3，但是不包含3  [1,3)
// 	//var slic []int = arr[1:3]
// 	// arr2 := arr[1:3]
// 	// fmt.Println(slic)
// 	// fmt.Println(arr2)

// 	// arr3 := make([]int, 4, 20)
// 	// arr4 := []int{1, 2, 3}
// 	// fmt.Println(arr3, arr4)

// 	// arr5 := arr[1:4]
// 	// fmt.Println((arr5))
// 	// //fmt.Println((arr5[4]))
// 	// arr6 := arr5[0:2]
// 	// fmt.Println(arr6)
// 	var arr [6]int = [6]int{1, 2, 3, 4, 5, 6}
// 	var slic []int = arr[1:3]
// 	arr2 := append(slic, 1, 2)
// 	arr2[0] = 99
// 	fmt.Println("arr", arr)
// 	fmt.Println("slic", slic)
// 	fmt.Println("arr2", arr2)

// 	//arr3 := []int{8,8}
// 	arr3 := [2]int{8,8}
// 	slic= append(slic,arr3[0:len(arr3)]...)

// }

//map

// func main() {
// 	// var a map[int]string = make(map[int]string, 10)

// 	// a[1] = "张三"
// 	// a[10] = "张三"

// 	// a[0] = "张三"

// 	// fmt.Println(a)
// 	//创建
// 	b := make(map[int]string)
// 	//增加
// 	b[0] = "aa"
// 	b[1] = "aa"
// 	fmt.Println(b)
// 	//改
// 	b[0] = "bb"
// 	fmt.Println(b)
// 	//删
// 	delete(b, 0)
// 	fmt.Println(b)
// 	//查
// 	value, flag := b[1]
// 	fmt.Println(value, flag)
// 	//len
// 	fmt.Println(len(b))

// 	for k, v := range b {
// 		fmt.Println(k, v)
// 	}

// 	a := make(map[int]map[int]string)
// 	a[0] = make(map[int]string)
// 	a[0][1] = "aa"
// 	a[0][2] = "aa"

// 	fmt.Println(a)

// }

//结构体

// type Teacher struct {
// 	name string
// 	age  int
// }

// func (t *Teacher) String() string {
// 	str := fmt.Sprintf("name = %s,age = %d", t.name, t.age)
// 	return str
// }

// func main() {
// 	t := Teacher{"aaa", 11}
// 	fmt.Println(&t)
// }

// func (t *Teacher) test() {
// 	t.name = "aaa"
// 	t.age = 11
// 	fmt.Println(t)
// }

// func main() {
// 	var t Teacher = Teacher{"vvv", 1}
// 	//(&t).test()
// 	t.test()
// 	fmt.Println(t)
// }

// func main() {
// 	var a Teacher
// 	a.age = 10
// 	a.name = "aaa"
// 	fmt.Println(a)

// 	var b Teacher = Teacher{"bbb", 12}
// 	fmt.Println(b)

// 	var c *Teacher = new(Teacher)
// 	// (*c).age = 10
// 	// (*c).name = "ccc"
// 	c.age = 11
// 	c.name = "ccc"
// 	fmt.Println(*c)

// }
// package main
package main

import "fmt"

// import "test/test2"

// func main() {
// 	// s := test2.Teacher{"aaa", 11}
// 	// s.Test()

// 	// fmt.Println(s)

// 	// s := test2.NewTeacher("aaa", 11)
// 	// s.GetAge()

// 	cat := &test2.Cat{}
// 	//cat.Animal.Age=1
// 	cat.Age = 1
// 	cat.Weight = 2
// 	cat.Show()
// }

type SayHello interface {
	sayhello()
}

type Chinese struct {
}

func (c *Chinese) sayhello() {
	fmt.Println("你好")
}

type Amilen struct {
}

func (c *Amilen) sayhello() {
	fmt.Println("hi")
}

func hello(s SayHello) {
	s.sayhello()
}

func main() {
	a := Amilen{}
	c := Chinese{}
	hello(&a)
	hello(&c)
}
