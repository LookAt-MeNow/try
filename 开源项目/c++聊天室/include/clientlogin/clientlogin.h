#ifndef _CLIENTLOGIN_
#define _CLIENTLOGIN_

typedef struct{
    char online[303][20]; // 在线用户
    int len; // 用户数量
    char msg[1024];//消息内容
    char name[20];//用户账号
    char passwd[20];//用户密码
    int cmd;//消息类型
}Messges;

//界面
void Interface();
//注册
void Reg(int sockfd);
//登陆
void Entry(int sockfd);
//客户端向服务器发送数据
Messges ask_ser(int sockfd);
#endif