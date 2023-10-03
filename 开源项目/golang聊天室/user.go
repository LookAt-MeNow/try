package main

import "net"
import "strings"
//创建用户类

type User struct {
	Name string
	Addr string

	C chan string //管道
	conn net.Conn

	//添加服务器
	server *Server
}

//创建用户

func NewUser(conn net.Conn,server *Server) *User {
	//获取ip
	userAddr := conn.RemoteAddr().String()
	user := &User{
		Name: userAddr,
		Addr: userAddr,
		C: 	  make(chan string),
		conn: conn,
		server: server,
	}

	//监听管道的消息
	go user.ListenMessage()
	return user
}

//用户上线API
func (this *User) OnUser() {
	//将用户添加到上线表中
	this.server.mapLock.Lock()//锁
	this.server.OnlineMap[this.Name] = this
	this.server.mapLock.Unlock()//关锁
	this.server.BroadCast(this,"已上线")
}
//用户离线API
func (this *User) OffUser() {
	//将用户删除上线表中
	this.server.mapLock.Lock()//锁
	delete(this.server.OnlineMap,this.Name)
	this.server.mapLock.Unlock()//关锁
	this.server.BroadCast(this,"已下线")
}
//消息处理API
func (this *User) DoMessage(msg string) {
	if msg == "who" {
		//查看当前在线用户
		this.server.mapLock.Lock()
		for _,user := range this.server.OnlineMap {
			OnlineMsg := "[" + user.Addr + "]" +user.Name + ":" + "在线\n"
			this.sendMsg(OnlineMsg)
		}
		this.server.mapLock.Unlock()
	} else if len(msg)>7 && msg[:7] == "rename|" {
					//待分割的字符	分隔符
		newName := strings.Split(msg,"|")[1]
		//判断当前改的名字是否重复
		_ , ok := this.server.OnlineMap[newName]
		if ok {
			this.sendMsg("当前用户名不可用\n")
		} else {
			this.server.mapLock.Lock()
			delete(this.server.OnlineMap,this.Name)
			this.server.OnlineMap[newName] = this
			this.server.mapLock.Unlock()
			//更新
			this.Name = newName
			this.sendMsg("修改成功...\n")
		}
	} else if len(msg)>4 && msg[:3] == "to|" {
		//获取用户名
		SendName := strings.Split(msg,"|")[1]
		if SendName == "" {
			this.sendMsg("格式错误...\n正确格式:\"to|张三|你好\"\n")
			return
		}
		//根据用户名获取User对象
		SendUser , ok := this.server.OnlineMap[SendName]
		if !ok {
			this.sendMsg("该用户不存在...\n")
			return
		}
		//发送消息
		SendConten := strings.Split(msg,"|")[2] 
		if SendConten == ""{
			this.sendMsg("无效内容...\n")
			return
		}
		SendUser.sendMsg(this.Name+"说："+SendConten)
	} else {
	    this.server.BroadCast(this,msg)
	}
}

//超时强踢API




//给当前User对应的客户端发送消息，主要用于看例如在线用户这些消息
func (this *User) sendMsg(msg string) {
	this.conn.Write([]byte(msg))
}

//监听当前的User channel消息的goroutine
//有消息就发送给客户端
func (this *User) ListenMessage() {
	for {
		msg := <-this.C
		this.conn.Write([]byte(msg + "\n"))
	}
}