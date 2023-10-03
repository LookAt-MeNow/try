#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "../mysql_connection/mysql_connect.h"
#include "serverlogin.h"

//注册
void Reg(int client_socket, Messges msg,MYSQL *conndb){
    //检测用户名是否存在
    if(if_name_exist(conndb,msg.name)){
            msg.cmd = -3;//用户存在，注册失败
            return 0;
    }
    printf("用户%s开始注册...\n",msg.name);
    //插入到数据库中
    if(insert_user(conndb,msg.name,msg.passwd)){
        msg.cmd = 101;//注册成功状态码
        insert_user_online(conndb,msg.name,"online");
    }else{
        msg.cmd = -1;//注册失败
    }
    write(client_socket,&msg,sizeof(msg));
}
//登录
void Entry(int client_socket,Messges msg,MYSQL *conndb){
    //检测用户名与密码是否正确
    if(!judge_user(conndb,msg.name,msg.passwd)){
        msg.cmd = -1;//登陆失败
        //检测用户名是否存在
        if(!if_name_exist(conndb,msg.name)){
            msg.cmd = -2;//登陆失败
        }
    }else{
        drop_user_online(conndb,msg.name);
        insert_user_online(conndb,msg.name,"online");
        msg.cmd = 102;
    }
    write(client_socket,&msg,sizeof(msg));
}

