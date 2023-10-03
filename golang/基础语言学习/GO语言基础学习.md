# GO语言基础学习

## 1.hello go

```go
package main

import "fmt"

func main() {
  fmt.Println("hello go")
}
```

hello world

这里面包含的猫腻：

- 语句结尾没分号，不强制要求不加（类似js）默认不加
- import 和py一样是用来导入包的
- 注释和c一致
- go 语言要求定义函数的时候 左花括号必须和函数名一行

## 2.GO语言的变量以及定义

### 关键字和保留字

go语言的变量和其他语言作用是一样的，然后存在关键字和保留字

    break        default      func         interface    select
    case         defer        go           map          struct
    chan         else         goto         package      switch
    const        fallthrough  if           range        type
    continue     for          import       return       var

    Constants:    true  false  iota  nil
       Types:    int  int8  int16  int32  int64  
                  uint  uint8  uint16  uint32  uint64  uintptr
                  float32  float64  complex128  complex64
                  bool  byte  rune  string  error
    
    Functions:   make  len  cap  new  append  copy  close  delete
                 complex  real  imag
                 panic  recover

### 变量的定义

go语言的变量定义有点类似于JavaScript，但是其还是有数据类型作为约束存在

```go
var <变量名> <变量类型>
```

如果需要同时定义多个变量的时候

```go
var (
    a string
    b int
    c bool
    d float32
)
```

变量赋值方式有俩 ，一种是类似与 c语言一样 定义后赋值或者定义时同时赋值，一种就是语法糖 定义的时候同时赋值

```go
var a int  // 默认赋值为0 而不是c语言中的随机
a=10
var b int = 11

c:=12 //仅限在函数内部使用（局部变量）
```

数据类型可以被省略，编译器会通过类似Python的方式去推测变量的数据类型

在使用多重赋值时，如果想要忽略某个值，可以使用匿名变量（anonymous variable）。 匿名变量用一个下划线_表示，例如：

```go
func foo() (int, string) {
  return 10, "Q1mi"
}
func main() {
  x, _ := foo()
  _, y := foo()
  fmt.Println("x=", x)
  fmt.Println("y=", y)
}
```

匿名变量不占用命名空间，不会分配内存，所以匿名变量之间不存在重复声明。 (在Lua等编程语言里，匿名变量也被叫做哑元变量。)

注意事项：

函数外的每个语句都必须以关键字开始（var、const、func等） `:=`不能使用在函数外。 `_`多用于占位，表示忽略值。

### 打印数据类型：

```go
fmt.prinft("%T",a)//打印数据类型
```

### 常量的声明

和大多数语言一样 GO使用const来代替var定义常量

### iota计数器

`iota`是go语言的常量计数器，只能在常量的表达式中使用。

`iota`在const关键字出现时将被重置为0。const中每新增一行常量声明将使`iota`计数一次(iota可理解为const语句块中的行索引)。 使用iota能简化定义，在定义枚举时很有用。

举个例子：

```go
const (
    n1 = iota //0
    n2        //1
    n3        //2
    n4        //3
  )
```

#### 几个常见的`iota`示例:

使用_跳过某些值

```go
const (
    n1 = iota //0
    n2        //1
    _
    n4        //3
  )
```

#### iota声明中间插队

```go
const (
    n1 = iota //0
    n2 = 100  //100
    n3 = iota //2
    n4        //3
  )
  const n5 = iota //0
```

定义数量级 （这里的<<表示左移操作，1<<10表示将1的二进制表示向左移10位，也就是由1变成了10000000000，也就是十进制的1024。同理2<<2表示将2的二进制表示向左移2位，也就是由10变成了1000，也就是十进制的8。）

```go
const (
    _  = iota
    KB = 1 << (10 * iota)
    MB = 1 << (10 * iota)
    GB = 1 << (10 * iota)
    TB = 1 << (10 * iota)
    PB = 1 << (10 * iota)
  )
```

**多个iota定义在一行**

```go
const (
    a, b = iota + 1, iota + 2 //1,2
    c, d                      //2,3
    e, f                      //3,4
  )
```

## 3.GO中函数和返回值

### 函数定义

`func <函数名> (a string, b string) int {……}` a和b是传入参数，int是返回值

go语言中函数是支持 多返回值的，如果需要返回两个值的话 这么定义：

```go
func f2 ()(int, int){
return 1,2
}
```

也可以返回有名称的变量

```go
func f3 ()(a int, b int){
  a=1
  b=2
  return
}
```

传入参数与返回参数都是属于形式参数（作用域在函数范围内）

GO中所有定义的变量没有赋值之前默认值都是0

如果前面参数和后面参数数据类型一致 可以合并来写

```go
func f3 ()(a ,b int){
  a=1
  b=2
  return
}
```

## 4.import导包路径与init方法调用流程

先创建两个包 lib01 和lib10

```go
package lib01

import "fmt"

func Lib01Test() {
  fmt.Printf("lib01Test()\n")

}

func init() {
  fmt.Printf("lib01的init函数被执行\n")
}
```

```go
package lib10

import "fmt"

func Lib10Test() {
  fmt.Printf("lib10Test()\n")

}
func init() {
  fmt.Printf("lib10的init函数被执行\n")
}
```

定义的包都需要放在文件夹里面，然后 定义的函数首字母大写为公有方法小写为私有方法

```go
package main

import (
  "awesomeProject/lib01"
  "awesomeProject/lib10"
)

func main() {
  lib10.Lib10Test()
  lib01.Lib01Test()
  return

}
```

调用：

```go
lib01的init函数被执行
lib10的init函数被执行
lib10Test()
lib01Test()
```

### 匿名导包

如果只需要某个包的init方法 但是不需要其他方法，则使用匿名导包方案

比如把上述代码更改下：

```go
package main

import (
  _ "awesomeProject/lib01"
  "awesomeProject/lib10"
)

func main() {
  lib10.Lib10Test()
  return

}
```

运行得到的结果：

```go
lib01的init函数被执行
lib10的init函数被执行
lib10Test()
```

还可以给包指定别名

```go
package main

import (
  _ "awesomeProject/lib01"
  mylib "awesomeProject/lib10"
)

func main() {
  mylib.Lib10Test()
  return

}
```

如果用全局导入用. 做前缀

## 5.GO里面的指针

其实和c一样，都差不多 也是用* 来表示指针但 语法和c不一样

c语言里面我们需要定义一个指针 使用：

```c
int *p
```

但是在go里面 

```go
var a *int
```

取地址倒是一样的

## 6.GO中的defer函数

defer是用栈管理的 遵循先入后出，

```go
package main

import (
  "fmt"
)

func main() {
  defer fmt.Printf("1\n")
  defer fmt.Printf("2\n")
  return

}

```

执行结果

```
2
1
```

return比defer先执行，原因：return 不是一个原子操作，分三步，第一赋值，第二执行defer ，第三才是真正return出去

## 7.GO语言中数组与动态数组

### 固定长度数组

与c很像

```go
package main

import "fmt"

func main() {
  var a = [10]int{1, 2, 3, 4}
  for i := 0; i < len(a); i++ {
    fmt.Printf("%d", a[i])
  }

}
```

但是没有赋值的部分会被自动赋值为0

还可以这样输出

```go
package main

import "fmt"

func main() {
  var a = [10]int{1, 2, 3, 4}
  for index, value := range a {
    fmt.Printf("index = %d,value = %d \n", index, value)
  }

}
```

```go
index = 0,value = 1 
index = 1,value = 2 
index = 2,value = 3 
index = 3,value = 4 
index = 4,value = 0 
index = 5,value = 0 
index = 6,value = 0 
index = 7,value = 0 
index = 8,value = 0 
index = 9,value = 0 
```

有点类似于Python里面的for i in range

而且 不同数组长度的数组数据类型不同，如果需要传参的话也得指定数组长度（感觉比c还搞）

```go
package main

import "fmt"

func printArray(myarray [10]int) { // 传入参数指定长度
  for index, value := range myarray {
    fmt.Printf("index = %d,value = %d \n", index, value)
  }
}

func main() {
  var a = [10]int{1, 2, 3, 4}
  printArray(a)
  return

}
```

这种传递方式还是值拷贝，所以还是传地址 用地址传递靠谱点

### 动态数组

其实就是定义固定长度数组，但是不指定长度

```go
package main

import "fmt"

func printArray(myarray []int) {
//这是一种引用传递 而不是值传递
  for _, value := range myarray {
    fmt.Printf("value = %d \n", value)
  }
}

func main() {
  var a = []int{1, 2, 3, 4} //动态数组，用切片的方式
  printArray(a)
  return

}
```

下划线表示匿名接受，接受即刻被抛弃

动态数组本质就是指向头结点的一个指针，所以传递进函数也就是地址传递

### 切片

上面定义动态数组就是一个切片

```go
  a := []int{1, 2, 3, 4} //定义一个动态数组并且初始化
  var b []int            //定义一个空的数组 但是没给b分配内存空间 打印长度为0
  b = make([]int, 3)     // 分配3个int大小的内存空间，以0填充
  //上面俩可以合并
  var c []int = make([]int, 3) // 或者 c:=make([]int,3)
```

如果需要判断一个数组是否为空

```go
if a == nil {
  fmt.Print("为空"）
}
```

### 切片容量追加

make的第三个参数指定的是cap 容量

```go
  var b []int            //定义一个空的数组 但是没给b分配内存空间 打印长度为0
  b = make([]int, 3, 5)  // 分配3个int大小的内存空间，以0填充
```

但是容量和长度不是一个概念，容量指的是系统底层为我们开辟的空间大小，而长度是程序可以合法访问的空间大小

如果用append方法向数组中插入值，存在下面的情况：

- 如果插入值后 len<cap 容量不变 长度增加
- 如果插入后len>cap **容量增大一倍**长度增加

### 切片截取

和Python一模一样  

就是 截取后获得的数组其实是通过指针指向，不是通过值拷贝， 如果对原数组进行修改，截取产生的数组同样也会被修改
go语言提供了copy函数可以实现值拷贝

```go
s1 :=[]int{1,2,3}
s2:=make([]int,3}
copy(s2,s1)//值拷贝
```

## 8.GO语言中map类型

map 用的多啊，特别是算法方面

申明方式和数组差不多

```go
 var myMap1 map[string]string //key是string  value是string
  if myMap1 == nil {
    fmt.Printf("map空间为空\n")
  }
  myMap1 = make(map[string]string, 10) // 分配空间
  myMap1["one"] = "1"
  myMap1["two"] = "2"
  fmt.Println(myMap1) //使用哈希存储 不存在顺序
  //或者
  myMap2 := make(map[int]string, 10) //参数10可以不写 让底层自动分配大小
  fmt.Println(myMap2)
  //或者
  myMap3 := map[string]int{
    "one": 1,
    "two": 2,
  }
  fmt.Println(myMap3)
```

### 遍历

```go
 var myMap1 map[string]string //key是string  value是string
  if myMap1 == nil {
    fmt.Printf("map空间为空\n")
  }
  myMap1 = make(map[string]string, 10) // 分配空间
  myMap1["one"] = "1"
  myMap1["two"] = "2"
  fmt.Println(myMap1) //使用哈希存储 不存在顺序

  for key, value := range myMap1 {
    fmt.Printf("%s:%s\n", key, value)
  }
```

### 删除

```go
var myMap1 map[string]string //key是string  value是string
  if myMap1 == nil {
    fmt.Printf("map空间为空\n")
  }
  myMap1 = make(map[string]string, 10) // 分配空间
  myMap1["one"] = "1"
  myMap1["two"] = "2"
  delete(myMap1, "one")
```

只用给出key名就行

### 修改

直接赋值就行

### 函数引用

如果map作为函数的传入参数，使用的是引用传递，也就是传了一个指针过去了

## 9.GO的面向对象

go是一个面向对象的语言

### 结构体

go的结构体和c基本上一致

用type关键字进行取别名，struct定义结构体

```go
type myStruct struct{
  auth int
  title string
}
```

用`.`访问结构体的属性

**但是 go 语言不提供类（class）的方式实现面向对象 **

而是直接通过结构体实现面向对象

### 匿名结构体

```go
var s struct{
  x string
  y int 
} //声明 而不是定义
s.x = "hhh"
s.y = 111
```

匿名结构体多用于一些临时场景

### 封装

GO语言通过结构体来封装类的属性，但是类的方法需要在结构体之外进行定义

```go
func (recv receiver_type) <函数名>(parameter_list) (<返回值>) { ... }
```

示例：

```go
package main

import (
    "fmt"
)

// 矩形结构体
type Rectangle struct {
    Length int
    Width  int
}

// 计算矩形面积
func (r *Rectangle) Area() int {
    return r.Length * r.Width
}

func main() {
    r := Rectangle{4, 2}
    // 调用 Area() 方法，计算面积
    fmt.Println(r.Area())
}
```

如果传入的不是一个指针 则是通过拷贝传递

GO语言通过大小写区分私有公有，如果类的属性或者方法开头字母是大写 则为公有，小写为私有

当遇到**只能在包内使用的未导出字段**时，我们又该如何访问呢？

和其他面向对象语言一样，Go 语言也有实现 `getter` 和 `setter` 的方式：（友元？）

- 对于 `setter` 方法使用 `Set` 前缀
- 对于 `getter` 方法**只使用成员名**

例如我们现在有一个处于 `person` 包中的 `Person` 结构体：

```go
package person
type Person struct {
  firstName string
  lastName  string
}
```

我们可以看到，它的两个成员变量都是非大写字母开头，只能在包内使用，现在我们为其中的 firstName 来定义 setter 与 getter ：

```go
// 获取 firstName

func (p *Person) FirstName() string {
  return p.firstName
}

// 设置 firstName
func (p *Person) SetFirstName(newName string) {
  p.firstName = newName
}
```

这样一来，我们就可以在 main 包里设置和获取 firstName 的值了：

```go
package main
import (
  "fmt"
  "./person"
)

func main() {
  p := new(person.Person)
  p.SetFirstName("firstName")
  fmt.Println(p.FirstName())
}

/* Output:
firstName
*/
```

### 继承

就很简单 直接嵌套就完事，直接在子类属性嵌套父类结构体名就行

### 多态

go中多态是通过接口来定义，cpp实现多态的方式是通过函数重载

### 接口

`interface` 数据类型 本质是一个指针

(要不要看看kotlin的接口？：接口）

#### 定义接口：

```go
type Shape interface {
    Area() float64
    Perimeter() float64
}
```

上面的代码中，我们定义了Shape接口，它有两个方法Area和Perimeter，他们不接收任何参数并返回float64。 任何实现这两个方法的类型我们都认为它实现了Shape接口。

简单的说：

- interface是方法的集合
- interface是一种类型，并且是指针类型
- interface的更重要的作用在于多态实现

#### interface使用

- 接口的使用不仅仅针对结构体，自定义类型、变量等等都可以实现接口。
- 如果一个接口没有任何方法，我们称为空接口，由于空接口没有方法，所以任何类型都实现了空接口。
- 要实现一个接口，必须实现该接口里面的所有方法。

```go
package main

import "fmt"

type Skills interface {
    Running()
    Getname() string

}

type Student struct {
    Name string
    Age int
}


type Teacher struct {
    Name string
    Salary int
}

func (p Student) Getname() string{   //实现Getname方法
    fmt.Println(p.Name )
    return p.Name
}

func (p Student) Running()  {   // 实现 Running方法
    fmt.Printf("%s running",p.Name)
}


func (p Teacher) Getname() string{   //实现Getname方法
    fmt.Println(p.Name )
    return p.Name
}

func (p Teacher) Running()  {   // 实现 Running方法
    fmt.Printf("\n%s running",p.Name)
}
func main()  {
    var skill Skills
    var stu1 Student
    var t1 Teacher
    t1.Name = "wang"
    stu1.Name = "wd"
    stu1.Age = 22
    skill = stu1
    skill.Running()
    skill = t1
    t1.Running()
    //两个对象都有running方法
}
//wd running
//wang running
```

理解起来确实会有点困难

如果把Skills看做父类 teacher和student看成子类，通过子类实例化实现父类的接口定义

### 空接口

`interface{}` 空接口是一个万能类型，所有数据类型都实现了空接口，可以用空接口来实现传入任意数据类型

传入之后 需要区分传入的数据类型，就用到断言方式

比如 

```go
func myfunc(arg interface){
  value, ok :=arg.(string)//如果arg是string ok就为true value就是arg中保存的值
}
```

### 反射

什么是反射：

在计算机科学中，反射是指计算机程序在运行时（Run time）可以访问、检测和修改它本身状态或行为的一种能力。用比喻来说，反射就是程序在运行的时候能够“观察”并且修改自己的行为。

Go 语言提供了一种机制在运行时更新变量和检查它们的值、调用它们的方法，但是在编译时并不知道这些变量的具体类型，这称为反射机制。

golang中对变量的构造，分为类型和value 

#### 类型：

##### 静态类型：

基本静态类型：int string等基本数据类型

##### 具体类型：

系统可以看见的类型：interface所指向的局标题数据类型

#### 反射获取变量

```go
package main

import (
  "fmt"
  "reflect"
)

func reflectNum(arg interface{}) {
  fmt.Println("type:", reflect.TypeOf(arg))
  fmt.Println("value:", reflect.ValueOf(arg))
}
func main() {
  var num float64 = 1.2345
  reflectNum(num)

}
```

## 10.GO的结构体标签

结构体标签有点类似与其他语言中对函数或者结构体的注释，主要的作用的在于说明这个结构体（类）是干啥的

定义方式：

```go
type resume struct {
  Name string `info:"name" doc:"我的名字"`
  sex
```

如果需要读取还是通过反射的方式去读取

```go
package main

import (
  "fmt"
  "reflect"
)

type resume struct {
  Name string `info:"name" doc:"我的名字"`
  sex  string `info:"sex"`
}

func findType(str interface{}) {
  t := reflect.TypeOf(str).Elem() //得到返回的结构体上所有的元素
  for i := 0; i < t.NumField(); i++ {
    tag := t.Field(i).Tag.Get("info")
    tagdoc := t.Field(i).Tag.Get("doc")
    fmt.Println("info:", tag, "doc:", tagdoc)

  }
}

```

### 结构体标签在json中的应用

```go
package main

import (
  "encoding/json"
  "fmt"
)

type Movie struct {
  Title  string   `json:"title"`
  Year   int      `json:"year"`
  Price  int      `json:"price"`
  Actors []string `json:"actors"`
}

func main() {
  move := Movie{"人民的名义", 2020, 20, []string{"zhangsan", "lisi", "wangwu"}}
  //编码过程
  jsonStr, err := json.Marshal(move)
  if err != nil {
    fmt.Println("json marshal err", err)
    return
  }
  fmt.Printf("jsonStr = %s\n", jsonStr)
  
  //解码过程
  mymove := Movie{}
  err = json.Unmarshal(jsonStr, &mymove)
  if err != nil {
    fmt.Println("json unmarshal err", err)
    return
  }
  fmt.Printf("%v\n", mymove)

}
```

把结构体编程json

运行结果：

```
jsonStr = {"title":"人民的名义","year":2020,"price":20,"actors":["zhangsan","lisi","wangwu"]}
```

## 11.goroutine协程

早期操作系统是单进程的，只能一个任务一个任务去处理，而且有进程阻塞的问题

所以会出现多线程操作系统，使用轮询的方式进行控制，并发执行

多线程解决了线程阻塞的问题，但是会导致切换成本变高（进程和线程数量越多，切换成本越大，也就越浪费）

cpu可见的只有内核态 用户态和内核态是绑定的 用户线程如果能进行拆分，让线程绑定多个协程降低了切换成本 还可以改进一下可以有多对多的方式

协程调度器是通过语言底层是实现，线程调度是操作系统和硬件去实现

### Goroutine

内存占用小，灵活调度

早期golang调度器局部性不佳而且频繁线程阻塞和取消阻塞增加了系统开销

### gmp模型

![](https://ts1.cn.mm.bing.net/th/id/R-C.3a3ed056f4eb9d515907a915514fc814?rik=lADNtpPjBcbMjw&riu=http%3a%2f%2fv5blog.cn%2fassets%2fimg%2fimage-20210604153340855.3a3ed056.png&ehk=8GDfd4C7KsE0zk1GFjwkTPveyddN1R4KF2nl8pZ3dvU%3d&risl=&pid=ImgRaw&r=0)

### 调度器设计策略

#### 复用线程

work stealing：偷取机制 也就是空闲的p去从别的p的本地队列中偷取任务

hand off：交接机制 如果一个g任务阻塞 会唤醒一个新的线程 然后把剩下的任务交给新的线程接管 

#### 利用并行

通过GOMAXPROCS来限定p的个数 = CPU/2 

#### 抢占

对每个G限定一定运行时间，防止某个任务长时间占用

#### 全局G队列

对于一个空闲的P首先进行偷取机制，如果无法偷取，则从全局队列中获取新任务G

**goroutine代码**

```go
package main

import (
  "fmt"
  "time"
)

func newTask() {
  i := 0
  for {
    i++
    fmt.Printf("new Gorourine : i = %d\n", i)
    time.Sleep(1 * time.Second)
  }

}

func main() {
  go newTask() // 创建一个go程 去执行newTask 流程

  i := 0
  for {
    i++
    fmt.Printf("main Gorourine : i = %d\n", i)
    time.Sleep(1 * time.Second)
  }
}
```

执行效果：

```
main Gorourine : i = 1
new Gorourine : i = 1
new Gorourine : i = 2
main Gorourine : i = 2
main Gorourine : i = 3
new Gorourine : i = 3
new Gorourine : i = 4
main Gorourine : i = 4
main Gorourine : i = 5
new Gorourine : i = 5
new Gorourine : i = 6
main Gorourine : i = 6
main Gorourine : i = 7
new Gorourine : i = 7
new Gorourine : i = 8
main Gorourine : i = 8
```

main go程是父程 如果它退出 所有子程就会退出

如果需要在某一刻退出go程

```go
package main

import (
  "fmt"
  "runtime"
  "time"
)

func main() {
  go func() {
    defer fmt.Println("A defer")
    func() {
      defer fmt.Println("B defer")
      runtime.Goexit() //退出GO程
      fmt.Println("b")
    }()
    fmt.Println("A")
  }()

  for {
    time.Sleep(1 * time.Second)
  }
}
```

**runtime.GOMAXPROCS**

Go运行时的调度器使用GOMAXPROCS参数来确定需要使用多少个OS线程来同时执行Go代码。默认值是机器上的CPU核心数。例如在一个8核心的机器上，调度器会把Go代码同时调度到8个OS线程上（GOMAXPROCS是m:n调度中的n）。

Go语言中可以通过runtime.GOMAXPROCS()函数设置当前程序并发时占用的CPU逻辑核心数。

Go1.5版本之前，默认使用的是单核心执行。Go1.5版本之后，默认使用全部的CPU逻辑核心数。

我们可以通过将任务分配到不同的CPU逻辑核心上实现并行的效果，这里举个例子：

```go
func a() {
    for i := 1; i < 10; i++ {
        fmt.Println("A:", i)
    }
}

func b() {
    for i := 1; i < 10; i++ {
        fmt.Println("B:", i)
    }
}

func main() {
    runtime.GOMAXPROCS(1)
    go a()
    go b()
    time.Sleep(time.Second)
}
```

两个任务只有一个逻辑核心，此时是做完一个任务再做另一个任务。 

将逻辑核心数设为2，此时两个任务并行执行，代码如下。

```go
func a() {
    for i := 1; i < 10; i++ {
        fmt.Println("A:", i)
    }
}

func b() {
    for i := 1; i < 10; i++ {
        fmt.Println("B:", i)
    }
}

func main() {
    runtime.GOMAXPROCS(2)//双核运行
    go a()
    go b()
    time.Sleep(time.Second)
}
```

Go语言中的操作系统线程和goroutine的关系：

- 1.一个操作系统线程对应用户态多个goroutine。
- 2.go程序可以同时使用多个操作系统线程。
- 3.goroutine和OS线程是多对多的关系，即m:n。

## 12.channel管道，协程通讯

管道是一种数据类型，类似于套接字

make(chan Type) 

```go
package main

import "fmt"

func main() {
  c := make(chan int)
  go func() {
    defer fmt.Printf("goroutine 结束 \n")
    fmt.Println("goroutine 正在运行")
    c <- 666 // 将666 发送给c
  }()
  num := <-c // 从管道c中读取数据
  fmt.Println("num = ", num)
  fmt
```

go协程之间运行是异步的 但是如何保证能正确从管道中读取数据呢。其实在执行读取的时候 会发送一个阻塞信号直到值被写入，阻塞才被解除

- 在第1步，两个goroutine都到达通道，但哪个都没有开始执行发送或者接收。
- 在第2步，左侧的 goroutine 将它的手伸进了通道，这模拟了向通道发送数据的行为。这时，这个 goroutine 会在通道中被锁住，直到交换完成。
- 在第3步，右侧的goroutine将它的手放入通道，这模拟了从通道里接收数据。这个goroutine一样也会在通道中被锁住，直到交换完成。
- 在第4步和第5步，进行交换，并最终，在第6步，两个goroutine都将它们的手从通道里拿出来，这模拟了被锁住的 goroutine 得到释放。两个 goroutine 现在都可以去做其他事情了。

### 带缓冲的channel

- 在第1步，右侧的 goroutine 正在从通道接收一个值。
- 在第 2 步，右侧的这个 goroutine独立完成了接收值的动作，而左侧的goroutine正在发送一个新值到通道里。
- 在第 3 步，左侧的goroutine 还在向通道发送新值，而右侧的goroutine正在从通道接收另外一个值。这个步骤里的两个操作既不是同步的，也不会互相阻塞。
- 在第4步，所有的发送和接收都完成，而通道里还有几个值，也有一些空间可以存更多的值。

```go
package main

import (
  "fmt"
  "time"
)

func main() {
  c := make(chan int, 3) //定义缓存区大小
  fmt.Println("len c = ", len(c), ",cap(c)= ", cap(c))
  go func() {
    defer fmt.Printf("goroutine 结束 \n")
    for i := 0; i < 3; i++ {
      c <- i
      fmt.Println("子go程正在运行  发送元素：", i, "len c = ", len(c), ",cap(c)= ", cap(c))
    }
  }()
  time.Sleep(2 * time.Second) // 等待子go程写入缓存
  for i := 0; i < 3; i++ {
    num := <-c // 从管道c中读取数据
    fmt.Println("num = ", num)
  }
  fmt.Println("main goroutine 结束")
}
```

有缓存的时候 就可以避免阻塞，如果写入数据大于缓存区，也会造成阻塞 或者缓存区为空 从里面读数据也会造成阻塞

### 关闭channel

```
close(<管道名>)
```

判断管道是否关闭

```go
data, ok :=<-c 
```

如果c被关闭了 ok会被赋予一个false的bool值

- channel不像文件一样需要经常去关闭，只有当你确实没有任何发送数据了，或者你想显式的结束range循环之类的，才去关闭channel；
- 关闭channel后，无法向channel再发送数据(引发 panic 错误后导致接收立即返回零值);
- 关闭channel后，可以继续从channel接收数据；
- 对于nil channel，无论收发都会被阻塞。

### channel与range的配合

类似stl标准库中智能指针

```go
package main

import (
  "fmt"
  "time"
)

func main() {
  c := make(chan int, 3)
  fmt.Println("len c = ", len(c), ",cap(c)= ", cap(c))
  go func() {
    defer fmt.Printf("goroutine 结束 \n")
    for i := 0; i < 3; i++ {
      c <- i
      fmt.Println("子go程正在运行  发送元素：", i, "len c = ", len(c), ",cap(c)= ", cap(c))
    }
    close(c) // 必须要加关闭否则main会继续尝试读取导致死锁
  }()
  time.Sleep(2 * time.Second) // 等待子go程写入缓存
  for num := range c {

    fmt.Println("num = ", num)
  }
  fmt.Println("main goroutine 结束")
}
```

### channel与select的操作

select可以监控多个channel的状态（多路选择器？？）

### 超时

有时候会出现goroutine阻塞的情况，那么我们如何避免整个程序进入阻塞的情况呢？我们可以利用select来设置超时，通过如下的方式实现：

```go
func main() {
    c := make(chan int)
    o := make(chan bool)
    go func() {
        for {
            select {
                case v := <- c:
                    println(v)
                case <- time.After(5 * time.Second):
                    println("timeout")
                    o <- true
                    break
            }
        }
    }()
    <- o
}
```

### 通道实际意义

在c和c艹中 对于线程之间的通讯就是使用共享内存的方式进行，但是实际问题就存在锁的问题，比如在操作系统中学习的那一部分。