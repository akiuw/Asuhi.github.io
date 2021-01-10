#MySQL

##基本操作

###数据库基本操作

对数据库的一些操作
```
create database db2; -- 创建数据库
create database db2 default charset utf8; -- 指定编码
show databases; -- 显示数据库
drop database db2; -- 删除数据库
```
```
use db2; -- 进入db2数据库
```

创建表
```
create table t1(id int, name char(10));
create table t1(id int, name char(10)) default charset=utf8; -- 指定编码
create table t1(id int, name char(10)) engine=innodb default charset=utf8; -- 指定引擎
```

## 创建表

> create table t1(
	列名 类型 null,
	列名 类型 not null, -- 可否为空
	列名 类型 not null default 1, -- 不可为空但是默认是1
	列名 类型 not null auto_increment primary key, -- 不为空，并自增（自增量必须为索引）
	 -- auto_increment 自增
	 -- primary key	 做约束：不能重复且不能为空。加速查找
	id int,
	name char(10)
) engine=innodb default charset=utf8;

修改表t10的自增序列从2开始自增:
> alter table t10 auto_increment=2

自增步长：*基于会话级别(不能给每个表设置步长)
每建立一次会话要重新设置步长

`set  session auto_increment_increment=2`

设置起始值
`set session autu_increment_offset=2`

全局级别：(尽量别使用)
`set globle auto_increment_increment=2`

1. innodb 支持事务，原子操作
2. mysiam 支持全局索引，存储速度快


```
create table t2(id int auto_increment primary key,
	name char(10)
)engine=innodb default charset=utf8;

```

##基本数据类型

###数字

前加unsigned表示无符号
数字：

类型|含义
--:|:--
int|整数
tinyint|小整数(-128~127   0~255)
bigint|大整数
float|单精度浮点
double|双精度浮点
decimal|精确小数(内部使用字符串存储)

```
create table t2(
	num decimal(10,5), -- 10表示总位数5表示小数点后最多5位
	id int auto_increment primary key,
	name char(10)
)engine=innodb default charset=utf8;
```
字符串：

类型|含义
--:|:--
char(10)|字符定长字符串，如果不到10个填充到10个
varchar(10)|不到10个不进行填充 最大是255个字符
text|存储较大字符串 65535个
mediutext|更大字符串
longtext|最大字符串


对比char和varchar：
>查找速度 char > varchar
>空间节省 varchar > char

PS:创建数据表时定长(char)放置在前面，不定长(varchar)的放在后面

时间类型：

类型|含义
--:|:--
DATA|YYYY-MM-DD
TIME|HH:MM:SS
YEAR|YYYY
DATETIME|YYYY-MM-DD HH:MM:SS
TIMESTAMP|YYYYMMDD HHMMSS

enum 枚举只能选择指定选项内容的数据
例如：
```
CREATE TABLE shirts(
	name VARCHAR(40),
	size ENUM('x-small','small','medium','large','x-large')
);

```
size只能在指定后面的选项选择

set 集合只能在选项中的选区任意组合
```
CREATE TABLE myset(
	name VARCHAR(40),
	col SET('a','b','c','d')
);
insert into myset(name,col) values('root','a');
insert into myset(name,col) values('root','a,b,c');
```

##外键
> 在一个表中存在的另一个表的主键称此表的外键。

```
CREATE TABLE userinfo(
	uid int auto_increment primary key,
	name varchar(32),
	department_id int,
	constraint fk_user_depar foreign key ("department_id",) refrences department("id")
)engine=innodb default charset=utf8;

CREATE TABLE department(
	id int auto_increment primary key,
	title char(15)
)engine=innodb default charset=utf8;


```


##修改自增值

修改自增起始位
```
desc t10;
show create table t10 \G;  # 查看表的创建方式
alter table t10 AUTO_INCREMENT=20; # 修改自增列的值为20
```

修改自增步长

`set session auto_increment_increment=2; # 修改自增步长为2`
`show session variables like 'auto_inc%';`


步长是基于会话级别的，如果推出登录步长将会重置成1

如果要修改全局的需要加上 global 

`set global auto_increment_increment=`
`show global variables like 'auto_inc%';`
