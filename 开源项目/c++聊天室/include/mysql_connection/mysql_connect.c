    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include<stdbool.h>
    #include <mysql/mysql.h>
    #include "../erro/erro.h"
    #include "mysql_connect.h"

    //定义一个用于判断函数返回值的变量
    int ret;

    //初始化数据库
    //返回值：MYSQL指针
    //参数：MYSQL指针
    MYSQL* db_init(MYSQL *condb){
        //在多线程环境中需要调用这个函数的
        ret = mysql_library_init(0,NULL,NULL);
        if(ret != 0){
            info_erro("could not initialize MYSQL library");
            exit(1);
        }
        condb = mysql_init(NULL);
        return condb;
    }

    //连接数据库
    //返回值：MYSQL指针，地址，用户名，密码，数据库名称
    //参数：MYSQL指针
    MYSQL* db_connect(MYSQL* condb,char *hostIP,char* username,char* passwd,char* db){
        if(!(mysql_real_connect(condb,hostIP,username,passwd,db,3306,NULL,0))){
            mysql_error(condb);
            info_erro("连接错误\n可能是网络问题");//判断连接是否错误
            exit(1);
        }
        printf("连接成功！\n");
        return condb;    
    }

    //显示表信息
    //返回值：bool 1：成功 0：失败
    //参数：MYSQL指针，SQL查询字符串
    bool info_table(MYSQL* condb,char* query){
        MYSQL_RES *mysql_res; //指向结果集对象
        MYSQL_ROW *mysql_row; //指向结果集的一行
        MYSQL_FIELD *field;    //指向结果集的一个字段
        int i,j;//判断循环变量
        int num_row,num_col;//结果集中的行列
        //char display[100][20];//用于保存输出表的内容
        //执行查询的语句
        ret = mysql_real_query(condb,query,strlen(query));
        if(ret != 0){
            info_erro("查询失败！");
            return false;
        }
        //保存查询结果
        if((mysql_res = mysql_store_result(condb)) == NULL){
            //将查询结果集保存到mysql_res，如果失败返回NULL
            info_erro("结果保存失败！");
            return false;
        }
        num_row = mysql_num_rows(mysql_res);//获取行数2
        num_col = mysql_num_fields(mysql_res);//获取取列数3
        printf("======================================\n");
        int k=0; // 记录数组的字段
        for( i=0;i<num_row;i++){
            //遍历每一行
            if((mysql_row = mysql_fetch_row(mysql_res)) == NULL){
                break;//获取一行的数据，如果返回NULL则说明没有数据了
            }
            if( i == 0){
                //如果是表头，打印表头
                while(field = mysql_fetch_field(mysql_res)){
                    //获取每个字段的信息，如果返回空指针，表示没有字段了
                    printf("%-10s",field->name);//打印字段名
                    //strcpy(display[k],field->name);//将字段名写入display数组
                    //strcpy(display[0],field->name);
                    k++;
                }
                printf("\n");
            }
            mysql_fetch_lengths(mysql_res);//获取每个字段的长度
            for( j = 0;j<num_col;j++){
                printf("%-10s",mysql_row[j]);
                //strcpy(display[k2],mysql_row[j]);
            }
            printf("\n");
        }
        printf("======================================\n");
        mysql_free_result(mysql_res);
        return true;
    }

    //判断表中name字段是否存在
    //返回值：bool 1：存在 0：不存在
    //参数：MYSQL指针，用户名
    bool if_name_exist(MYSQL* condb,char* name){
        User user1;
        char query[1024];
        strcpy(user1.username,name);

        MYSQL_RES *res;
        MYSQL_ROW row;
        MYSQL_FIELD *field;

        int num_fields;//定义结果集列数
        //拼接查询语句
        sprintf(query,"select *from user where name = '%s'",user1.username);
        mysql_query(condb,query);//执行语句
        res = mysql_store_result(condb); //保存查询结果
        num_fields = mysql_num_fields(res);
        while((row = mysql_fetch_row(res))){
            //遍历每一行数据
            if(field = mysql_fetch_field(res)){
                //获取字段信息
                if(!strcmp(user1.username,row[1])){//strcmp匹配成功返回1
                    return true;//不能使用user1.username == row[1]
                }                           //这比较的只是指针
            }
        }
        mysql_free_result(res);
        mysql_commit(condb); //提交事务
        return false;
    }

    //判断用户名与密码是否匹配
    //返回值： bool 1：匹配 0：不匹配
    //参数：MYSQL指针，用户名，密码
    bool judge_user(MYSQL* condb,char* name,char* passwd){
        User user1;
        char query[1024];
        strcpy(user1.username,name);
        strcpy(user1.userpasswd,passwd);
        MYSQL_RES *res;
        MYSQL_ROW row;
        MYSQL_FIELD *field;

        int num_fields;//定义结果集列数
        //拼接查询语句
        sprintf(query,"select *from user where name = '%s'",user1.username);
        mysql_query(condb,query);//执行语句
        res = mysql_store_result(condb); //保存查询结果
        num_fields = mysql_num_fields(res);

        while((row = mysql_fetch_row(res))){
            if(field = mysql_fetch_field(res)){
                //strcmp如果匹配的返回值0，则使用！
                if(!strcmp(user1.username,row[1]) && !strcmp(user1.userpasswd ,row[2])){
                    printf("登陆成功\n");
                    return true;
                }
            }
        }
        mysql_free_result(res);
        mysql_commit(condb);
        return false;
    }

    //用户注册
    //返回值：bool
    //参数：MYSQL指针，用户名，密码
    bool insert_user(MYSQL* condb,char* name,char* passwd){
        User user1;
        char query[1024];
        
        MYSQL_RES *res;
        strcpy(user1.username,name);
        strcpy(user1.userpasswd,passwd);
        //判断用户是否存在
        if(if_name_exist(condb,name)){
            printf("用户存在，请登陆");
            return false;
        }
        sprintf(query,"insert into user (name,passwd) values('%s','%s')",
                        user1.username,user1.userpasswd);
        //执行查询的语句
        ret = mysql_real_query(condb,query,strlen(query));
        if(ret != 0){
            mysql_rollback(condb);//回转事物
            info_erro("注册失败！");
            return false;
        }
        mysql_commit(condb);
        return true;
    }

    //用户注销
    //返回值：bool
    //参数：MYSQL指针，用户名，密码
    bool drop_user(MYSQL* condb,char* name){
        char query[1024];
        if(!if_name_exist(condb,name)){
            info_erro("注销失败！请重新检查用户名\n");
            return false;
        }
        sprintf(query,"delete from user where name = '%s'",name);
        ret = mysql_real_query(condb,query,strlen(query));
        if(ret != 0){    if(!if_name_exist(condb,name)){
            info_erro("注销失败！请重新检查用户名\n");
            return false;
        }
            mysql_rollback(condb);//回转事物
            info_erro("注销失败！请重新检查用户名\n");
            return false;
        }
        mysql_commit(condb);
            if( mysql_affected_rows(condb)>0){
                printf("删除成功\n");
            }
        return true;
    }

    //功能：用户上线
//参数：MYSQL指针，用户名，用户在线状态
//返回值：空
bool insert_user_online(MYSQL *condb,char *name,char *state){
    Onlineuser user1;
    char query[1024];
	strcpy(user1.username,name);
	strcpy(user1.userstate,state);
    sprintf(query,"insert into user_online(name,state) values('%s','%s')",
		user1.username ,user1.userstate);
    ret = mysql_real_query(condb,query,strlen(query));
    if(ret != 0){
            mysql_rollback(condb);//回转事物
            info_erro("注册失败！");
            return false;
    }
    mysql_commit(condb);
    return true;
}

//功能：用户下线
//参数：con，所需删除用户的用户名
bool drop_user_online(MYSQL *condb,char *name){
    char query[1024];
    sprintf(query,"delete from user_online where name = '%s'",name);
    ret = mysql_real_query(condb,query,strlen(query));
    if(ret != 0){
        mysql_rollback(condb);//回转事物
        info_erro("注销失败！请重新检查用户名\n");
        return false;
    }
    mysql_commit(condb);
    if( mysql_affected_rows(condb)>0){
        printf("删除成功\n");
    }
    return true;
}

    //实现结束------------------------------------------
    /*
    //功能测试代码
       int main(){
        MYSQL *mysql_handle = NULL;
        int m,n;
        int len = 0;
        mysql_handle = db_init(mysql_handle);
        //连接测试
        mysql_handle = db_connect(mysql_handle,"47.120.41.232","root","123456","chat");
        //表查询测试
        info_table(mysql_handle,"select *from user");
        printf("\n*******************************\n");
        //插入测试
        insert_user(mysql_handle,"sfen","12");
        info_table(mysql_handle,"select *from user");
        printf("\n*******************************\n");
        //重复name测试
        if(if_name_exist(mysql_handle,"chen")){
            printf("该 用户存在\n");
        }else{
            printf("该用户不存在\n");
        }
        printf("\n*******************************\n");
        //name passwd不匹配测试
        if(judge_user(mysql_handle,"chen","12")){
            printf("匹配\n");
        }else{
            printf("不匹配\n");
        }
        printf("\n*******************************\n");
        //删除测试
        if(drop_user(mysql_handle,"sfen")){
            printf("注销成功\n");
        }else{
            printf("注销失败\n");
        }
        info_table(mysql_handle,"select *from user");
        printf("\n*******************************\n");
        printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
        //在线用户插入测试
        insert_user_online(mysql_handle,"zhang","online");
        info_table(mysql_handle,"select *from user_online");
        printf("\n*******************************\n");
        //在线用户删除测试
        drop_user_online(mysql_handle,"zhang");
        info_table(mysql_handle,"select *from user_online");
        printf("\n*******************************\n");
        mysql_close(mysql_handle);
        mysql_library_end();
        return 0;
        }
    
    */
   //`mysql_config --cflags --libs`
   // gcc mysql_connect.c ../erro/erro.c -o test `mysql_config --cflags --libs` 