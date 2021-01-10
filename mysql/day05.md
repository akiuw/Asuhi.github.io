#MySQL

##存储过程
>保存在mysql上的一个别名,里面包括一些sql语句
> 以后使用别名就能得到结果
> 使用：别名()

**存储过程保存在数据库中**
**程序员不再写sql语句，只需要调用存储过程就能达到目的**

简单举例:
```
delimiter //
	create procedure p1()
	begin
		select * from student;
		insert into teacher(tname) values('aaa')
	end//
	delimiter ;
```
`call p1();`这里存储过程里面的语句都能执行了


##传参
> 存储过程参数主要有三个 -- in out inout

```
create procedure p2(
		in n1 int,
		in n2 int
	)
begin
	select * from student where id > n1;
end
call p2(10,2);
```

```
create procedure p3(
		in n1 int,
		out n2 int
	)
begin
	set n2 = 12;
	select * from student where id > n1;
end
set @v1 = 0; -- 创建一个session级别的变量v1
call p3(10,@v1);
select @v1; -- 查看一个变量
```

> inout 既能传入也能传出

pymysql中使用存储过程:
```
conn = pymysql.connect(...,charset='utf8')
cursor = conn.cursor
cursor.callproc("p1")
cursor.callproc("p2",(10,2))
cursor.callproc("p3",(10,2))
conn.commit()
cursor.execute('select @_p3_0,@_p3_1') 
# _p3_0是10，_p3_1 是12 _p3_1被修改了
r = cursor.fetchall()
rint(r) # ((10,12),)
```

##事务

```
delimiter \\
	create procedure p4(
		out status tinyint
	)
	begin
		declare exit handler for sqlexception
		-- 如果出现异常执行下面代码块
		begin
			set status = 1; -- 输出参数置1
			rollback; -- 回滚到原始状态
		end;		
		-- 成功执行下面
		start transaction; -- 开始事务
			delete from tb1;
			insert into tb2(name) values('aaa');
		commit; -- 提交结果
		
		set  status = 0;
		end\\
delimiter ;
```

##游标
1. 声明游标
2. 获取A表中的数据
	my_cursor select id,num from A

3
```
delimiter //
	create procedure p5()
	begin
		declare row_id int;
		declare row_num varchar(32);
		declare done int default false;
		-- 声明变量 done是判断循环是否结束的条件
		
		declare my_cursor cursor select id,num from A;
		-- 声明游标 my_cursor A表里面的每一行的id和num列
		declare continue handler for found set done = true;
		-- 检测到循环结束把done设置成true
		
		open my_cursor; -- 打开游标
			aaaa: loop -- 创建循环 'aaaa'
			-- 取出游标中的的一行数据
			fetch my_cursor into row_id,row_num;
			-- 判断循环条件是否结束
			if done then
			-- 如果结束就退出循环
				leave aaaa;
			end if;
			-- 设置临时变量tmp 为 row_id + row_num
			set tmp = row_id + row_num;
			-- 将tmp插入到表B中
			insert into B(num) values(tmp);
		end loop aaaa;
	-- 关闭游标
	close my_cursor;	
	end //
delimiter ;
```

游标使用场景：
需要对每一行进行操作和计算时使用游标

```
delimiter //
		create procedure p6(
		
		)
		begin
			
		end //
		delimiter ;
```
