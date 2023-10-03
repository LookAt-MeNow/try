-- 创建两个表
-- 用户信息存放表
create table user(
    id int auto_increment primary key comment  'id号',
    name varchar(20) not null unique comment '用户名',
    passwd varchar(20) comment '密码'
);

-- 在线用户表
create table user_online(
    id int auto_increment primary key comment  'id号',
    name varchar(20) not null unique comment '用户名',
    state varchar(20) not null comment '状态'
);
