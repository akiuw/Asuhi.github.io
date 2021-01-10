id 用户名id，主键，自增
name 用户名
password 密码
phone 手机号
email 邮箱
createtime 创建时间


create table userinfo(
	id int not null auto_increment primary key ,
	name varchar(128) not null,
	password varchar(128) not null,
	phone varchar(15) not null,
	createtime varchar(128),
	email varchar(128),
	constraint uq_name unique(name)
) engine=innodb default charset=utf8;

file info

md5 文件md5
fileid 文件id /group1/M00/00/00/xxx
url 文件url ip/group1/M00/00/00/xxx
size 文件大小 字节为单位
type 文件类型 png mp4 ....
count 文件引用计数 拥有此文件的用户数 重复上传的文件服务器不用再下载

create table file_info(
	md5 varchar(256) not null primary key,
	file_id varchar(256) not null,
	url varchar(512) not null,
	size bigint,
	type varchar(20),
	count int
)engine=innodb default charset=utf8;


文件用户信息表：

user 文件所属用户
md5 
createtime
filename
shared_status 共享状态，0有共享，1没用
pv	文件下载量

create table user_file_info(
	user varchar(128) not null,
	md5 varchar(256) not null,
	createtime varchar(128),
	size bigint,
	filename varchar(128),
	shared_status int,
	pv int
)engine=innodb default charset=utf8;

文件数量表
记录每个用户有多少个文件
user 
count 

create table user_file_count(
	user varchar(128) not null primary key,
	count int
)engine=innodb default charset=utf8;

文件共享表
create table file_shared_info(
	user varchar(128) not null,
	md5 varchar(256) not null,
	createtime varchar(128),
	filename varchar(128),
	pv int
)engine=innodb default charset=utf8;















user_file_info






