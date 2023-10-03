# Gin Web开发

## GO语言的hello world

### 环境安装：

https://golang.google.cn/dl/

在这个页面下载go语言文件，要注意下载的版本 macos是darwin内核然后arm64架构（m1）

然后下载idea，我选用的是goland 同时安装好中文插件

### 基础配置

首先 喜闻乐见环境变量：

```bash
vim ~/.zshrc
```

然后添加环境变量在文件末尾：

```bash
export GOPATH=$HOME/gopath
export PATH=$PATH:$HOME/go/bin:$GOPATH/bin
```

### 换源

由于历史原因，go的软件包会通过GOPATH和module两种方式去管理，而不同管理方式下go get所下载的源也不同，因此go的换源会比其他语言更加麻烦，要用两步来完成：

```
go env -w GO111MODULE=on
go env -w GOPROXY=https://goproxy.cn
```

第一个命令，是将GO111MODULE从auto模式修改为on模式。如前面提到的，go有两种包管理方式

GOPATH方式：早期方式，会将下载的包放入GOPATH/src目录下，然后只有GOPATH/src中的包是能被程序导入的 module方式：更现代的方式，通过在项目目录中生成go.mod文件来管理需要的包，此时go还可以导入网络上的包、本目录的包，然后缺少的包会被缓存到GOPATH/pkg目录下

GOPROXY只能修改module方式的下载源。而go get是通过哪种方式下载包，go怎样管理包依赖，取决于GO111MODULE的设置：

auto模式下，有`go.mod`文件的目录下执行go get会走module方式，而没有的目录会走GOPATH方式。 on模式下，全部都走`module`方式

### helloworld

```go
package main

import "fmt"
import "net/http"

func sayHello(w http.ResponseWriter, r *http.Request) {
  _, _ = fmt.Fprintln(w, "Hello Golang")
}
func main() {
  http.HandleFunc("/hello", sayHello)
  err := http.ListenAndServe(":8080", nil)
  if err != nil {
    fmt.Printf("http serve falid,err\n", err)
    return
  } else {
    fmt.Printf("serve running at 127.0.0.1:8080")
  }
}

```

然后访问8080/hello 就可以看到helloworld页面了

注释：nil代表空类

## GO后端处理api简易版本

代码如下

```go
package main

import (
  "github.com/gin-gonic/gin"
  "net/http"
)

func main() {
  r := gin.Default() // 默认返回的路由引擎
  //下面是采用RESTful风格的 API设计
  r.GET("/api", func(context *gin.Context) {
    context.JSON(http.StatusOK, gin.H{
      "method": "GET",
    })
  })
  r.POST("/api", func(context *gin.Context) {
    context.JSON(http.StatusOK, gin.H{
      "method": "POST",
    })
  })
  r.PUT("/api", func(context *gin.Context) {
    context.JSON(http.StatusOK, gin.H{
      "method": "PUT",
    })
  })
  r.DELETE("/api", func(context *gin.Context) {
  context.JSON(http.StatusOK, gin.H{
      "method": "DELETE",
    })
  })
    r.Run(":8000")
}
```

采用RESTful模式的api 然后调试的话不方便使用浏览器，所以还是先通过apifox进行建议调试，检查json传输的是否正确

## GO 语言内置模板引擎

GO语言内置了模板引擎，有点类似于Django 

Go语言内置了文本模板引擎`text/template`和用于HTML文档的`html/template`。它们的作用机制可以简单归纳如下：

1. 模板文件通常定义为`.tmpl`和`.tpl`为后缀（也可以使用其他的后缀），必须使用`UTF8`编码。
2. 模板文件中使用`{{`和`}}`包裹和标识需要传入的数据。
3. 传给模板这样的数据就可以通过点号（`.`）来访问，如果数据是复杂类型的数据，可以通过{ { .FieldName }}来访问它的字段。
4. 除`{{`和`}}`包裹的内容外，其他内容均不做修改原样输出。

例子 首先写一个模板：

```html
<!doctype html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
    <p>hello{{.}}</p>
</body>
</html>
```

然后在go里面调用模板：

```go
package main

import (
  "fmt"
  "html/template"
  "net/http"
)

func sayhello(w http.ResponseWriter, r *http.Request) {
  files, err := template.ParseFiles("./hello.tmpl") //解析模板
  if err != nil {
    fmt.Printf("err:%v", err)
    return
  }
  err = files.Execute(w, "hi")
  if err != nil { //处理错误
    fmt.Printf("err:%v", err)
    return
  }
}

func main() {
  http.HandleFunc("/", sayhello)
  err := http.ListenAndServe(":8080", nil)
  if err != nil {
    fmt.Printf("http err:%v\n", err)
    return
  }

}
```

然后运行就得到一个模板网页

### GO语言模板语法

模板语法都包含在`{{`和`}}`中间，其中`{{.}}`中的点表示当前对象。

当我们传入一个结构体对象时，我们可以根据`.`来访问结构体的对应字段。例如：

```go
// main.go
package main

import (
  "fmt"
  "html/template"
  "net/http"
)

type UserInfo struct {  //进行封装,定义结构体
Name   string
Gender string
Age    int
}

func sayHello(w http.ResponseWriter, r *http.Request) {
// 解析指定文件生成模板对象
tmpl, err := template.ParseFiles("./hello.tmpl")
if err != nil {
fmt.Println("create template failed, err:", err)
return
}
// 利用给定数据渲染模板，并将结果写入w
user := UserInfo{
Name:   "小王子",
Gender: "男",
Age:    18,
}
tmpl.Execute(w, user)
}
```

模板文件hello.tmpl内容如下：

```html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Hello</title>
</head>
<body><!--用点表示结构体对象的方法-->
    <p>Hello {{.Name}}</p>
    <p>性别：{{.Gender}}</p>
    <p>年龄：{{.Age}}</p>
</body>
</html>
```

同理，当我们传入的变量是map时，也可以在模板文件中通过.根据key来取值。

### pipeline管道操作

`pipeline`是指产生数据的操作。比如`{{.}}`、`{{.Name}}`等。Go的模板语法中支持使用管道符号`|`链接多个命令，用法和unix下的管道类似：`|`前面的命令会将运算结果(或返回值)传递给后一个命令的最后一个位置。

**注意：** 并不是只有使用了`|`才是pipeline。Go的模板语法中，`pipeline的`概念是传递数据，只要能产生数据的，都是`pipeline`。

### 变量

我们还可以在模板中声明变量，用来保存传入模板的数据或其他语句生成的结果。具体语法如下：

```html
$obj := {{.}}
```

### 模板函数

```html
and
    函数返回它的第一个empty参数或者最后一个参数；
    就是说"and x y"等价于"if x then y else x"；所有参数都会执行；
or
    返回第一个非empty参数或者最后一个参数；
    亦即"or x y"等价于"if x then x else y"；所有参数都会执行；
not
    返回它的单个参数的布尔值的否定
len
    返回它的参数的整数类型长度
index
    执行结果为第一个参数以剩下的参数为索引/键指向的值；
    如"index x 1 2 3"返回x[1][2][3]的值；每个被索引的主体必须是数组、切片或者字典。
print
    即fmt.Sprint
printf
    即fmt.Sprintf
println
    即fmt.Sprintln
html
    返回与其参数的文本表示形式等效的转义HTML。
    这个函数在html/template中不可用。
urlquery
    以适合嵌入到网址查询中的形式返回其参数的文本表示的转义值。
    这个函数在html/template中不可用。
js
    返回与其参数的文本表示形式等效的转义JavaScript。
call
    执行结果是调用第一个参数的返回值，该参数必须是函数类型，其余参数作为调用该函数的参数；
    如"call .X.Y 1 2"等价于go语言里的dot.X.Y(1, 2)；
    其中Y是函数类型的字段或者字典的值，或者其他类似情况；
    call的第一个参数的执行结果必须是函数类型的值（和预定义函数如print明显不同）；
    该函数类型值必须有1到2个返回值，如果有2个则后一个必须是error接口类型；
    如果有2个返回值的方法返回的error非nil，模板执行会中断并返回给调用模板执行者该错误；
```

这些个函数只能放在模板中使用，但是 我们其实还能自己定义

例如：

```go
// main.go
package main

import (
  "fmt"
  "html/template"
  "net/http"
)

func f1(w http.ResponseWriter, r *http.Request) {
  //定义一个模板函数hello
  hello := func(arg string) (string, error) { // 定义函数 要么只有一个返回值 要么两个返回值第二个必须为error类型
    return "hello", nil
  }
  //定义模板
  t := template.New("f.tmpl") //用new函数创建模板 返回一个指针指向文件
  t.Funcs(template.FuncMap{
    "hello_name": hello, //前面是模板内函数的名字，后面是在go中定义的函数名
    //通过键值对的方式导入
  })
  //解析模板
  _, err := t.ParseFiles("./f.tmpl")
  //新建对象的名字要与模板的名字对应上

  if err != nil {
    fmt.Printf("template error:%v", err)
    return
  }
  name := "小明"
  //渲染模板
  t.Execute(w, name)

}

func main() {
  http.HandleFunc("/", f1)
  err := http.ListenAndServe(":9000", nil)
  if err != nil {
    fmt.Printf("HTTP server start failed , err :%v\n", err)
    return
  }
}
```

这里我是定义了一个自己的模板函数go 这个函数

```bash
<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
<p>{{hello_name .}}</p>
</body>
</html>
```

然后在模板中使用

### 模板嵌套

首先新建一个tmpl模板文件

```html
<h1>这是被嵌套的模板</h1>
<h2>t.tmpl</h2>
```

然后在母模板中导入：

```html
<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
<p>{{hello_name .}}</p>
{{template "t.tmpl"}} <!--导入模板-->
</body>
</html>
```

然后在go中导入模板，母模板在前 子模板在后

```go
_, err := t.ParseFiles("./f.tmpl", "./t.tmpl")
```

还可以在模板中直接自定义子模板

```html
<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
<p>{{hello_name .}}</p>
{{template "t.tmpl"}}
{{template "p.tmpl"}}
</body>
</html>
{{define "p.tmpl"}}
    <ol>
        <li>这是在模板中定义的子模板</li>
        <li>子模板名字叫p.tmpl</li>
    </ol>
{{end}}
```

如果是直接在模板文件中定义的话，就无需在go中导入。

## GIN的后端登录操作与ORM

```go
package main

import (
  "fmt"
  "github.com/gin-gonic/gin"
  _ "github.com/go-sql-driver/mysql"
  "github.com/jinzhu/gorm"
  "log"
  "math/rand"
  "net"
  "net/http"
  "time"
)

type User struct {
  gorm.Model
  Name      string `gorm:"type:varchar(20);not null"`
  Telephone string `gorm:"type:varchar(11);not null;unique"`
  Password  string `gorm:"type:varchar(20);not null"`
}

func main() {
  db := InitDB()
  defer db.Close() //关闭函数后析构db

  r := gin.Default()
  r.GET("/api/auth/register", func(c *gin.Context) {
    //获取参数
    name := c.PostForm("name") //获取注册名
    println("name:", name)
    telephone := c.PostForm("telephone")
    println("telephone:", telephone)
    password := c.PostForm("password")
    println("password:", password)
    //数据验证
    if len(telephone) != 11 { //手机号数据验证
      c.JSON(http.StatusUnprocessableEntity, gin.H{"code": 422, "msg": "手机号码不合法"})
      return
    } //H是一个别名：type H map[string]interface{}
    if len(password) < 6 {
      c.JSON(http.StatusUnprocessableEntity, gin.H{"code": 422, "msg": "密码长度至少为6位"})
      return
    }
    if len(name) == 0 { ///
      name = RandomString(10)
    }
    log.Println(name, telephone, password)
    //查询手机号是否存在
    if isTelephoneExist(db, telephone) {
      c.JSON(http.StatusUnprocessableEntity, gin.H{"code": 422, "msg": "用户存在"})
      return
    }
    newUser := User{
      Name:      name,
      Telephone: telephone,
      Password:  password,
    }
    db.Create(&newUser)

    c.JSON(200, gin.H{
      "message": "注册成功",
    })
  })
  panic(r.Run())

}

func RandomString(n int) string { //生成随机字符串
  var letters = []byte("asdfghjklzxcvbnmQWSDFGHJM")
  result := make([]byte, n)

  rand.Seed(time.Now().Unix())
  for i := range result {
    result[i] = letters[rand.Intn(len(letters))]
  }

  return string(result)
}

func InitDB() *gorm.DB {
  ip := getIP("xzlnas.myqnapcloud.com")
  drivername := "mysql"
  fmt.Println("ip:", ip)
  host := ip
  database := "go"
  username := "root"
  password := "1037981455Xx"
  charset := "utf8"
  args := username + ":" + password + "@(" + host + ")/" + database + "?charset=" + charset + "&&parseTime=True&loc=Local"
  fmt.Println("args:", args)
  db, err := gorm.Open(drivername, args)
  if err != nil {
    panic("failed to connect database err:" + err.Error())

  }
  db.AutoMigrate(&User{})
  return db
}

func getIP(addr string) string {
  ip, err := net.ResolveIPAddr("ip4", addr)
  
```

如你所见  我这个登录操作全是放在同一个main文件下 应该对其进行一定量的优化才能达到MVVM设计模式

所以对其进行拆分