#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <errno.h>
#include <sys/wait.h>
#include "../include/mysql_connection/mysql_connect.h"
#include "../include/erro/erro.h"
#include "../include/serverlogin/serverlogin.h"

#define PORT   7990
const char * HostIP = "127.0.0.1";

int online_count = 0;//在线人数
int sockcd[100];//客户端套接字
MYSQL *mysql_handle = NULL;//引入数据库
Messges msg;//引入自定义的消息类型
void server_thread(void* p);//启动子线程处理消息

//创建一个数组用于保存线程号与用户名的关系
char user_my[100][20];

int main(){
    //创建套接字
    int sockfd;//服务端套接字
    int size = 100;//最大支持人数
    typedef struct sockaddr SA;//引入结构体
    struct sockaddr_in addr;
    pid_t pid;
    int ret;
    //----------------
    //服务器建立
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        info_erro("创建socket失败\n");
        exit(1);
    }
    bzero(&addr,sizeof(addr)); //等同于memset(&ser_addr,0,sizeof(ser_addr));
    int opt = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const void*)&opt,opt);//端口复用
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定IP以及端口
    ret = bind(sockfd,(SA*)&addr,sizeof(addr));
    if(ret == -1){
        perror("绑定失败\n");
        exit(1);
    }
    //监听客户端连接数量
    ret = listen(sockfd,100);
    if(ret == -1){
        perror("设置监听失败\n");
        exit(1);
    }
    //-----------------
    //连接数据库
    mysql_handle = db_init(mysql_handle);
    //连接远程数据库
    mysql_handle = db_connect(mysql_handle,"47.120.41.232","root","123456","chat");
    printf("服务器设置成功！\n");
    //创建多线程
    while(1){
        //设置客户端
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int fd = accept(sockfd,(SA*)&cliaddr,&len);
        if(fd == -1){
            perror("客户端连接失败\n");
            continue;
        }else{
            printf("连接成功\n");
        }
        for(int i=0;i<size;i++){
            if(sockcd[i]==0){
                //记录客户端的socket
                sockcd[i] = fd;
                printf("fd = %d\n",fd);
                //启动多线程，给客户端服务
                pthread_t tid;
                pthread_create(&tid,0,server_thread,&fd);
                pthread_detach(tid);//线程分离
                online_count++;//连接客户端数量
                break;
            }
        }
    }
    return 0;
}

void server_thread(void* p){
    int fd = *(int*)p;
    printf("pthread = %d\n",fd);
    while(1){
        int ret = read(fd,&msg,sizeof(msg));
        strcpy(user_my[fd],msg.name);//绑定线程号与用户名
        if(ret == -1){
            perror("read error");
            break;
        }else if(ret == 0){
            printf("客户端退出\n");
            //printf("发送的消息码是：%d\n",msg.cmd);
            //printf("发送的信息类型是：%s\n",msg.msg);
            drop_user_online(mysql_handle,msg.name);
            insert_user_online(mysql_handle,msg.name,"offline");
            pthread_exit(0);
            online_count--;
        }
        /*
        if(msg.cmd == 8){
            for(int i=0;i<100;i++){
                if(!strcmp(user_my[i],msg.name)){
                    msg.cmd = 9;
                    write(sockcd[i],&msg,sizeof(msg));
                    break;
                    //printf("%s:%s\n",msg.name,msg.msg);
                }
            }
        }
        */
        if(!strcmp(msg.msg,"#hello") && (msg.cmd > 0 && msg.cmd < 6)){
            switch(msg.cmd){
                case 1://注册
                    Reg(fd,msg,mysql_handle);
                    break;
                case 2://登陆
                    Entry(fd,msg,mysql_handle);
                    break;
                case 3://打印在线人数
                //info_table(mysql_handle,"select *from user_online");
                break;
                case 4://设置在线
                drop_user_online(mysql_handle,msg.name);//删除在线状态
                insert_user_online(mysql_handle,msg.name,"online");//添加离线状态
                break;
                case 5://设置离线
                drop_user_online(mysql_handle,msg.name);//删除在线状态
                insert_user_online(mysql_handle,msg.name,"offline");//添加离线状态
                break;
                case 8://私聊消息
                //查找需要聊天用户的tid;
                /*
                for(int i=0;i<100;i++){
                    printf("走的这个\n");
                    if(!strcmp(user_my[i],msg.name)){
                        msg.cmd = 9;
                        write(sockcd[i],&msg,sizeof(msg));
                        //printf("%s:%s\n",msg.name,msg.msg);
                    }
                }*/
                break;
            }
        }else {
            for(int i=0;i<online_count;i++){
                if(sockcd[i] != fd){//群发消息
                    write(sockcd[i],&msg,sizeof(msg));
                    printf("%s:%s\n",msg.name,msg.msg);
                }
            }
        }
    }
}

//gcc server.c ../include/mysql_connection/mysql_connect.c ../include/erro/erro.c ../include/serverlogin/serverlogin.c -o server `mysql_config --cflags --libs`
//gcc client.c ../include/clientlogin/clientlogin.c -o client