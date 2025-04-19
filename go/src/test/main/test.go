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

package main

import "fmt"
import "test/test2"

func test(num int)(){
    fmt.Println(num)
}

func test2(num int , testfunc func(int)){
    testfunc(num)
}
func main(){
    test2(10,test)

    fmt.Printf(test2.Add(1,2))
}