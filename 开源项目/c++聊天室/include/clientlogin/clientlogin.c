#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include "clientlogin.h"

Messges msg;

//界面
void Interface(){
    printf("=============================\n");
    printf("       1.注册\t2. 登陆\n");
    printf("=============================\n");
}
//注册
void Reg(int sockfd){
    char name[20];
    char passwd[20];
    strcpy(msg.msg,"#hello");//发送消息hello
    printf("输入用户名：\n");
    scanf("%s",name);
    while(getchar()!='\n'){}
    strcpy(msg.name,name);

    printf("输入密码：\n");
    scanf("%s",passwd);
    while(getchar()!='\n'){}
    strcpy(msg.passwd,passwd);

    msg.cmd = 1;//定义为1，表示为注册消息
    write(sockfd,&msg,sizeof(msg));//将msg写入套接字，发送给服务器
    read(sockfd,&msg,sizeof(msg));//读取服务器的回复

    printf("msg.cmd = %d\n",msg.cmd);//打印服务回复的信息类型

    if(msg.cmd == 101){//回复码为101注册成功
        printf("注册成功！\n请稍后.......\n");
        sleep(3);
    }else if(msg.cmd == -1){
        printf("注册失败\n请稍后........\n");
        sleep(3);
    }else if(msg.cmd == -3){
        printf("用户名已存在！\n请稍后");
        sleep(3);
    }
}
//登陆
void Entry(int sockfd){
    char name[20];
    char passwd[20];
    strcpy(msg.msg,"#hello");//发送消息hello
    printf("输入用户名：\n");
    scanf("%s",name);
    while(getchar()!='\n'){}
    strcpy(msg.name,name);

    printf("输入密码：\n");
    scanf("%s",passwd);
    while(getchar()!='\n'){}
    strcpy(msg.passwd,passwd);

    msg.cmd = 2;//2表示为登陆类型
    write(sockfd,&msg,sizeof(msg));//将msg写入套接字，发送给服务器
    read(sockfd,&msg,sizeof(msg));//读取服务器的回复
    
    printf("msg.cmd = %d\n",msg.cmd);//打印服务回复的信息类型

    if(msg.cmd == 102){//回复码为102登陆成功
        printf("登陆成功！\n请稍后......\n");
        sleep(3);
    }else if(msg.cmd == -1){
        printf("登陆失败，请检查账号或者密码！\n请稍后......\n");
        sleep(3);
    }else if(msg.cmd == -2){
        printf("用户不存在！\n请稍后......\n");
        sleep(3);
    }
}
//客户端向服务器发送注册/登录的选择
Messges ask_ser(int sockfd){
    char choice;
    Interface();
    printf("请选择1或者2\n");
    scanf("%c",&choice);
    switch (choice){
        case '1'://注册
            Reg(sockfd);
                break;
            case '2'://登陆
                Entry(sockfd);
                break;
    }
	system("clear");//清理屏幕输出
	return msg;
}