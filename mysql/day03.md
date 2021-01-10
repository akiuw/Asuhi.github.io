#MySQL

##PyMySQL

####安装

> `pip3 install pymysql -i `

####1.使用python 实现用户登入

day03-1.py

```
import pymysql
user = input("username")
pwd = input("password")
conn = pymysql.connec(host="localhost",user="root".password='',database="db2")
cursor = conn.cursor()
# 以列表[字典] 的方式查找
cursor = conn.cursor(cursor=pymysql.cursor.DictCursor)

# 不要自己做字符拼接，写好占位符后使用excute拼接,防止sql注入
# sql = "select * frome userinfo where username = '%s' and password = '%s'" %(user,pwd)
# cursor.excute(sql)
# %s占位
# sql = "select * frome userinfo where username = %s and password = %s"
# cursor.excute(sql,user,pwd)
# 字典拼接
sql = "select * frome userinfo where username = %(u)s and password = %(p)s"
cursor.excute(sql,{'u':user,'p':pwd})

res = cursor.fetchone() # 只拿第一条
print(res)

# res = cursor.fetchmany(4) # 一次取4条数据
res = cursor.fetchall() 
cursor.close()
conn.close()

```

day03-2.py

```
import pymysql
	
conn = pymysql.connec(host="localhost",user="root".password='',database="db2")	
cursor = conn.cursor()	
sql = "insert into userinfo(username,password) values('root','123')"
cursor.excute(sql)
# 提交多个,以元组的方式传入
# 返回值r代表受影响的行数
r = cursor.excutemany(sql,[('root2','123'),('root3','456')])

# 增删改 必须commit提交结果
conn.commit()
cursor.close()
```

####pymysql中使用存储过程

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
	print(r) # ((10,12),)
```


