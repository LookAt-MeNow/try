#ifndef _SERVRELOGIN_
#define _SERVERLOGIN_

#include <mysql/mysql.h>
#define MAXLINE 1024

typedef struct{
    char online[303][20]; // 在线用户
    int len; // 用户数量
    char msg[1024];//消息内容
    char name[20];//用户账号
    char passwd[20];//用户密码
    int cmd;//消息类型
}Messges;

//注册
void Reg(int client_socket, Messges msg,MYSQL *conndb); 
//登录
void Entry(int client_socket,Messges msg,MYSQL *condb);

#endif