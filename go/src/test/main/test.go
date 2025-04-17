// package main //声明文件所在的包
// import "fmt" //引入程序需要的包
// func main(){
//     fmt.Println("hello worlaaad")
//         fmt.Println("hello worlaaad")

// }   
//变量

package main
import "fmt"

var n11 = 11
var {
    n99 = 10
    n109 = "jack"
}

func main(){
    var age int
    age=18
    fmt.Println("age = ",age)

    var age2 int = 19
    fmt.Println("age2 = ",age2)

    // var num int = 12.56
    // fmt.Println(num)

    // var age3=1
    // age4 := 10
    

    // var m1,m2 int
    var n4,name,n5 = 1,"tom",2
    fmt.Println(n4);
    fmt.Println(name)
    fmt.Println(n5);

    n6,hight  := 1,22
    fmt.Println(n6);
    fmt.Println(hight);

    fmt.Println(n99);
    fmt.Println(n10);
    fmt.Println(n11);



}