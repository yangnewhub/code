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


package main

import (
    "fmt"
)

func main(){
    var num int = 10
    fmt.Println(&num)

  
}