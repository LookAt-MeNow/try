package main

	//这里要有空格的
import (
	"time"
	"sync"
	"fmt"
	"net"
)

//在golang中没有像c++一样的类的概念，通过结构来实现面向对象编程
//结构体变量名的首字母大写则代表public
type Server struct {
	Ip string //ip地址
	Port int  //端口
	OnlineMap map[string]*User	//在线用户表
	mapLock sync.RWMutex		//加一个锁
	Message chan string			//消息管道
}

//创建server对象					这个函数的返回值，顺便蛮怪
func NewServer(ip string ,port int) *Server {
	server :=&Server{
	Ip:		   ip,
	Port: 	   port,
	OnlineMap: make(map[string]*User),
	Message:   make(chan string),
	} //初始化列表
	return server
}

//启动server服务 这个就算是server类的类函数了
func (this *Server) Start() {
	//监听					连接类型	赋值给listen
	listen, err := net.Listen("tcp",fmt.Sprintf("%s:%d",this.Ip,this.Port))
	if err != nil {
		fmt.Println("listen err: ",err)
		return
	}
	//关闭server 	相当于c++中的析构函数
	defer listen.Close()

	//	监听message的管道
	go this.ListenMessager()
	for {
		//连接客户端
		conn, err := listen.Accept()
		if err != nil  {
			fmt.Println("listen accept err:", err)
			continue
		}
		//执行业务，go这里开了协程 ,协程！=线程
		go this.Handler(conn)
	}
}

func (this *Server) Handler(conn net.Conn){
	user := NewUser(conn,this)
	//调用用户上线API
	user.OnUser();
	
	//监听当前用户是否时活跃channel
	isLive := make(chan bool)

	//广播用户发的消息
	go func() {
		buf := make([]byte,4096)
		for {
			n,err := conn.Read(buf)
			if n==0 {
				//用户下线
				user.OffUser()
				return
			}
			if err != nil {
				fmt.Println("Conn Read err:",err)
			}
			//buf[:n-1]是去除输入时的\n
			msg := string(buf[:n-1])
			user.DoMessage(msg)
			//只用用户有消息发出就是活跃的
			isLive <- true
		}

	}()
	//堵塞当前用户
	for {
		select {
		case <-isLive:
			//如果是活的就不进行任何的操作
		case <-time.After(time.Second * 300):
			//给一个5min的计时器，如果超时了就会提出
			user.sendMsg("长时间无操作...\n")
			//释放管道
			close(user.C)
			//关闭套接字
			conn.Close()
			return
		}
	}
}

//写入广播消息

func (this *Server) BroadCast(user *User,msg string) {
	sendMsg := "[" + user.Addr + "]" +user.Name + ":" +msg
	//写入管道
	this.Message <- sendMsg
}

//广播消息到客户端

func (this *Server) ListenMessager() {
	for {
		msg := <-this.Message

		//发给在线的客户端
		this.mapLock.Lock()
		for _,cli := range this.OnlineMap {
			cli.C <- msg			
		}
		this.mapLock.Unlock()
	}
}