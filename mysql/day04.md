#MySQL

##视图

> 给某个查询语句设置个别名

####创建视图

> `create view [视图名称] as [SQL语句]`

####修改视图

> `alter view [视图名称] as [SQL语句]`

####删除

> `drop view [视图名称]`


##触发器
> 在执行某个SQL语句前后进行一些操作

trigger(触发器)
创建触发器t1在插入之前，作用于表student
	
```
delimiter //  -- 修改终止符为'//'
create trigger t1 before insert on student for each row -- for each row表示对每一条都执行

begin
	-- begin和end之间添加代码块
		insert into teacher(tname) values('aaaa');
end//

delimiter ; -- 然后修改回来
```


将上面内容改成动态的

```
delimiter //
	create trigger t1 before insert on student for each row
	begin
		insert into teacher(tname) values(NEW.sname);-- NEW代指新数据(插入) --OLD代指老数据(删除，更新) 
	end//
	delimiter ;
	insert into student(grender,class_id,sname) values('女',5,'bbb'); -- teacher表里面会多一行 sname为bbb的行
	
```

##函数
> mysql里面提供了一些函数
> 执行函数:`select CURDATE() -- select + 函数名`

####常用内置函数

得到字符串长度
`select CHAR_LENGTH('abcd');` 

字符串拼接str1str2
`select CONCAT('str1','str2');`

时间格式化
`select DATE_FORMAT('2019-11-11 11:11','%Y-%m');` 只取年月
格式化占位符及选项查看表格


####自定义函数
函数里面不能写查询创建等表的操作
```
delimiter \\
	create function f1(
		i1 int,
		i2 int
	) -- 函数头
	returns int; -- 返回类型
	begin -- 函数体
		declare num int default 0; -- 创建变量默认值为0
		set mum = i1 + i2;
		return(num);
	end \\
	delimiter ;
```













