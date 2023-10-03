#ifndef _MYSQL_CONNECT_
#define _MYSQL_CONNECT_

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdbool.h>

// 创建用户信息结构体
typedef struct
{
    char username[20];
    char userpasswd[20];
} User;

// 创建用户在线信息
typedef struct
{
    char username[20];
    char userstate[20];
} Onlineuser;

// 定义一个结构体，用于存储MySQL连接的相关信息
typedef struct mysql_conn
{
    MYSQL *condb; // 定义一个MySQL连接指针
    char *hostIP; // 定义一个字符串指针，用于存储主机名
    char *user;   // 定义一个字符串指针，用于存储用户名
    char *passwd; // 定义一个字符串指针，用于存储密码
    char *db;     // 定义一个字符串指针，用于存储数据库名
} mysql_conn;

// 功能实现------------------------------------------

// 初始化数据库
// 返回值：MYSQL指针
// 参数：MYSQL指针
MYSQL *db_init(MYSQL *condb);

// 连接数据库
// 返回值：MYSQL指针，地址，用户名，密码，数据库名称
// 参数：MYSQL指针
MYSQL *db_connect(MYSQL *condb, char *hostIP, char *username, char *passwd, char *db);

// 显示表信息
// 返回值：bool
// 参数：MYSQL指针，SQL查询字符串
bool info_table(MYSQL *condb, char *query);

// 判断表中name字段是否存在
// 返回值：bool
// 参数：MYSQL指针，用户名
bool if_name_exist(MYSQL *condb, char *name);

// 判断用户名与密码是否匹配
// 返回值： bool
// 参数：MYSQL指针，用户名，密码
bool judge_user(MYSQL *condb, char *name, char *passwd);

// 用户注册
// 返回值：bool
// 参数：MYSQL指针，用户名，密码
bool insert_user(MYSQL *condb, char *name, char *passwd);

// 用户注销
// 返回值：bool
// 参数：MYSQL指针，用户名，密码
bool drop_user(MYSQL *condb, char *name);

// 功能：用户上线
// 参数：MYSQL指针，用户名，用户在线状态
// 返回值：空
bool insert_user_online(MYSQL *condb, char *name, char *state);

// 功能：用户下线
// 参数：con，所需删除用户的用户名
bool drop_user_online(MYSQL *con, char *name);

// 实现结束------------------------------------------

#endif