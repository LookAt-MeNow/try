#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../include/clientlogin/clientlogin.h"
#include "../include/mysql_connection/mysql_connect.h"

#define PORT   7990
const char * HostIP = "192.168.203.137";
int sockcd;//客户端socket
char user_name[20];//用户名
char pass_wd[20];//用户密码
Messges msg;//引入自定义的消息类型，用于连接的
Messges msg_talk;//用于聊天信息

void* recv_pthread(void*p);
void online_num(MYSQL *conndb);

int main(){
    int ret;
    int choice;
    pthread_t tid;
    MYSQL *mysql_handle = NULL;//引入数据库
    //连接数据库
    mysql_handle = db_init(mysql_handle);
    //连接远程数据库
    mysql_handle = db_connect(mysql_handle,"47.120.41.232","root","123456","chat");
    int create;//创建子线程专门用于接受消息
    sockcd = socket(AF_INET,SOCK_STREAM,0);
    typedef struct sockaddr SA;//引入结构体
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr)); //等同于memset(&ser_addr,0,sizeof(ser_addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, HostIP, &addr.sin_addr.s_addr);    //指定IP 字符串类型转换为网络字节序 参3:传出参数
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ret = connect(sockcd,(SA*)&addr,sizeof(addr));
    if(ret == -1){
        perror("无法连接\n");
        exit(1);
    }
    printf("连接成功\n");
    //客户端向服务器发送注册/登录的选择
    
    msg = ask_ser(sockcd);
    
    if(msg.cmd == -2){
        printf("\n");
        printf("%s用户还没有注册\n或者检查一下用户名是否输入错误吧\n",msg.name);
        printf("\n");
        exit(1);
    }
    if(msg.cmd == -1){
        printf("\n");
        printf("登陆失败！\n");
        printf("\n");
        exit(1);   
    }
    if(msg.cmd == -3){
        printf("\n");
        printf("%s用户已注册\n",msg.name);
        printf("\n");
        exit(1);
    }
    //获取用户名
    strcpy(user_name,msg.name);
    if(msg.cmd == 101 || msg.cmd == 102){
        sprintf(msg.msg,"%s进入聊天室",msg.name);
        printf("%s进入聊天室\n",msg.name);
        msg.cmd = 0;
        write(sockcd,&msg,sizeof(msg));//写给服务端
    }

    printf("=============================\n");
    printf("       1.输入#hello，可选择查看状态\n");
    printf("       2.输入#clear，可清除当前终端消息\n");
    printf("       3.输入#exit，可退出登陆\n");
    printf("       4.直接输入文字即可聊天（上三种特殊字符不可用于聊天）\n");
    printf("=============================\n");
    //create = pthread_create(&tid,0,recv_pthread,0);
    //if(create != 0){
    //    printf("create pthread error\n");
    //}
    //pthread_detach(tid);
    /*
    printf("选择聊天类型\n1：私聊\t2：群聊\n");
    int temp;
    scanf("%d",&temp);
    if(temp == 1){
        printf("当前在线的用户\n");
        online_num(mysql_handle);
        printf("请选择聊天的对象\n");
        char user_to_msg[20];
        scanf("%s",user_to_msg);
        if(!if_name_exist(mysql_handle,user_to_msg)){
            printf("%s用户不存在！\n",user_to_msg);
            exit(1);
        }
        strcpy(msg_talk.name,user_to_msg);
        printf("输入#exit退出登录\n");
        while(1){
            //输入发送的消息
            scanf("%s",msg_talk.msg);
            if(strcmp(msg_talk.msg ,"#exit")){
                sprintf(msg_talk.msg,"%s退出聊天室\n",user_name);
                strcpy(msg_talk.name,user_name);
                write(sockcd,&msg_talk,sizeof(msg_talk));
                close(sockcd);//关闭套接字
                exit(1);
            }
            msg_talk.cmd = 8;//私聊消息的状态码
            strcpy(msg.name,user_to_msg);
            write(sockcd,&msg_talk,sizeof(msg_talk));
            read(sockcd,&msg_talk,sizeof(msg_talk));
            if(msg_talk.cmd == 9){
            printf("%s:%s\n",msg_talk.name,msg_talk.msg);
            }
        }
    }*/
    create = pthread_create(&tid,0,recv_pthread,0);
    if(create != 0){
        printf("create pthread error\n");
    }
    pthread_detach(tid);
    while(1){
        //输入发送的消息
        scanf("%s",msg_talk.msg);
        msg_talk.cmd = 0;
        strcpy(msg_talk.name,user_name);
        //客户端退出
        if(strcmp(msg_talk.msg,"#exit") == 0){
            sprintf(msg_talk.msg,"%s退出聊天室\n",user_name);
            strcpy(msg_talk.name,user_name);
            write(sockcd,&msg_talk,sizeof(msg_talk));
            close(sockcd);//关闭套接字
            break;
        }else if(strcmp(msg_talk.msg,"#clear") == 0){
            system("clear");
        }else if(strcmp(msg_talk.msg,"#hello") == 0){
            printf("=============================\n");
            printf("       1.查看用户状态\n");
            printf("       2.设置在线状态\n");
            printf("       3.设置离线状态\n");
            printf("=============================\n");
            scanf("%d",&choice);
            switch(choice){
                case 1:
                    //msg_talk.cmd = 3;
                    //write(sockcd,&msg_talk,sizeof(msg_talk));
                    //read(sockcd,&msg_talk,sizeof(msg_talk));//从服务端读回信息
                    //msg_talk.cmd = 0;//重置状态码
                    /*printf("=============================\n");
                    for(int i =0 ;i<3;i++){
                        printf("%-10s",msg_talk.online[i]);
                    }
                    for(int j = 3;j<msg_talk.len;j++){

                    }
                    printf("\n");
                    printf("=============================\n");*/
                    online_num(mysql_handle);
                    break;
                case 2:
                    msg_talk.cmd = 4;
                    strcpy(msg_talk.name,user_name);
                    write(sockcd,&msg_talk,sizeof(msg_talk));
                    msg_talk.cmd = 0;
                    printf("设置成功\n");
                    break;
                case 3:
                    msg_talk.cmd = 5;
                    strcpy(msg_talk.name,user_name);
                    write(sockcd,&msg_talk,sizeof(msg_talk));
                    msg_talk.cmd = 0;
                    printf("设置成功\n");
                    break;
            }
        }else{
            write(sockcd,&msg_talk,sizeof(msg_talk));//没有选择就直接发送信息
        }
    }
    return 0;
}

void* recv_pthread(void*p){
    Messges msg_talk2;
    while(1){
        if(msg_talk.cmd !=3){
            read(sockcd,&msg_talk2,sizeof(msg_talk2));
        }
        if(msg_talk.cmd == 0){
            printf("%s:%s\n",msg_talk2.name,msg_talk2.msg);
        }
    }
}//启动子线程处理服务端消息

void online_num(MYSQL *conndb){
    if(info_table(conndb,"select *from user_online")){
        //printf("查询成功!\n");
    }
}

