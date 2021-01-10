#MySQL
## MySQL操作补充--数据行

###增加
####插入一行：

> insert into tb(name,age) values("root",12);

####多行插入

> insert into tb(name,age) values("root",12),("alice",11);

####将tbA表的数据插入到tbB表里面

> insert into tbB(name,age) select name,age from tbA;

###删除
> delete from tb;
delete from tb where id !=2;  -- 条件：id不等于2
delete from tb where id >=2;  -- <= = > < 都行
delete from tb where id !=2 and name="root"; -- and和or都行

[**drop、truncate、 delete区别**](几种删除操作的区别.md) ←点这里

###修改

> update tb set name="alice",age=19 where id>12 and name="root";


###查找

> select * from tb;//查找所有

####使用where和逻辑运算符

> select id,name from tb where id>10 and name="root";

####查找时给表头取个别名为cname
> select id,name as cname from tb where id>10 and name="root";

####查找时多加了一列都是1
> select name,age,1 from tb;

####不等于
> select * from tb where id != 1;
select * from tb where id <> 1;

####id是1,5,10的数据行
> select * from tb where id in (1,5,10); 
select * from tb where id not in (1,5,10); -- 上一句的非运算结果

####指定区间
> select * from tb where id between 5 and 12; -- 从5到12 闭区间 [5,12]

####从表2查出id在更具这个结果查找
> select * from tb where id in (select id from tb2);


####通配符：
> select * from tb where name like "a%";  -- 在name以a头的   ps："%a" 以a结尾
select * from tb where name like "a_"; -- 以a开头只匹配后面一个字符


####limit
> select * from tb limit 10; -- 查看前10条
select * from tb limit 0,2; -- 取第一条后面的2条，包括第一条
select * from tb limit 2 offset 0;  -- 从0开始往后取2条

##排序

> select * from tb order by id asc; -- 从小到大排序
select * from tb order by id desc; -- 从大到小排序
select * from tb order by id desc limit 10; -- 取后10条数据


##分组

####使用聚合函数：
> select uid,max(id) from userinfo group by uid; -- 将uid相同的合在一起，并且取id较大的那个
> min(取较小的)
> select uid,count(id) from userinfo group by uid;
> count将显示uid相同的行的个数 
>  sum 求和 avg求平均值
>  select uid,max(id),count(id) from userinfo group by uid;

**找到部门人数大于1的:**

如果对聚合函数的结果的二次筛选必须使用`having`
> select count(id),part_id from userinfo group by part_id having count(id)>1;

##连表操作

userinfo表中的part_id和department表中的id进行关联:

`select * from userinfo,department where userinfo.part_id = department.id;`

> 左边的数据会全部显示
> select * from userinfo left join department on userinfo.part_id = department.id;
> 右边的数据会全部显示
> select * from userinfo right join department on userinfo.part_id = department.id;

```
select * from userinfo 
left join department1 on userinfo.part_id = department.id
left join department2 on userinfo.part_id = department.id;
```


##外键变种

> 外键+唯一索引 -- fk不能重复

####一对一:

创建两个表：
```
create table userinfo1(
	id int auto_increment primary key, -- id 自增主键
	name char(10), -- 名字
	gender char(10), -- 性别
	email varchar(64)
)engine=innodb default charset=utf8;
```

```
create table admin(
	id int not null auto_increment primary key, -- id自增主键
	username varchar(64) not null, -- 用户名
	password varchar(64) not null, -- 密码
	user_id int not null, -- 用户id
	unique uq_u1 (user_id), -- 用户id为唯一索引
	constraint fk_admin_u1 foreign key (user_id) references userinfo1(id)
	-- 用户id为外键 指向userinfo1的id
)engine=innodb default charset=utf8;
```

####一对多
```
create table userinfo1(
	id int auto_increment primary key, -- id 自增主键
	name char(10), -- 名字
	gender char(10), -- 性别
	email varchar(64)
)engine=innodb default charset=utf8;
```

```
create table host(
	id int auto_increment primary key,
	hostname char(64)
)engine=innodb default charset=utf8;
```
```
create table user2host(
	id int auto_increment primary key,
	userid int not null,-- 用户id
	hostid int not null,-- 主机id
	unique uq_user_host (userid,hostid), -- 用户和主机id的组合关系是唯一的
	-- 用户id是外键指向userinfo1的id列
	constraint fk_u2h_user foreign key (userid) references userinfo1(id),
	-- 主机id是外键指向host的id列
	constraint fk_u2h_host foreign key (hostid) references host(id)
)engine=innodb default charset=utf8;
```

####多对多联合唯一

每一对只能出现一次

