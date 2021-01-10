#内置方法


## \_\_str\_\_ 和\_\_repr\_\_ 方法


> ####我们在调用str和repr函数时会打印出所对应类的 \_\_str\_\_ 和 \_\_repr\_\_ 方法 的结果

例如:
```
class A:pass
a = A()
print(str(a)) #出现a对象的内存地址
```

* 这是因为object类里面实现了\_\_str\_\_方法 (所有类的父类都是object)

> 如果我们在A类里面也实现了\_\_str\_\_方法

```
class A:
	def __init__(self,name):
		self.name = name
	def __str__(self):
		return "A's object name %s"%self.name # __str__ 不返回字符串时会报错
	def __repr__(self):
		return str(self.__dict__)
a = A('aaa')
```

```
print(str(a)) # A's object name aaa这里会调用A自己的__str__方法打印结果为其返回的字符串
print(a) # A's object name aaa打印一个对象时也就是在打印__str__的值
print('%s'%a) # A's object name aaa # 字符串格式化%s也是调用了__str__

print(repr(a)) # {'name':'aaa'} # 打印了__dict__返回的内容
print('%r'%a) # {'name':'aaa'} # %r调用了__repr__
```

> 当我们只写了\_\_repr\_\_ 没写 \_\_str\_\_时(或str没返回值) 会调用\_\_repr\_\_ 反过来不成立
> 如果两个都没有就会找父类的\_\_str\_\_

```
class A:
	def __init__(self,name):
		self.name = name
	# def __str__(self):
	# 	 return "A's object name : %s"%self.name
	def __repr__(self):
		return str(self.__dict__)
a = A('aaa')
print(a) # {'name':'aaa'}打印了__dict__返回的内容

```

##析构函数 \_\_del\_\_

```
class A:
	def __del__(self):
		print("delete A's obj")

a = A()
del a  # delete A's obj # 先执行了__del__ 方法 后删除了a对象
```

* 如果不手动删除，在整个程序结束时python解析器会调用\_\_del\_\_函数并且删除这个变量
* 一般我们要在对象消失前处理收尾工作

> del的实质：引用计数减1 当引用计数为零时才会真正删除变量

例如在类里面打开了文件
```
class B:
	def __init__(self,fd):
		self.f = fd
	def __del__(self):
		self.f.close()
b = B(open('File Path'))
del b # 在b对象消失之前会吧文件关闭 # 如果没有写析构函数这个文件描述符一直会存在与内存中
```

##\_\_call\_\_
类似于 c++的仿函数
```
class A:
	def __init__(self,name):
		self.name = name
	def __call__(self):
		print(self.name)
a = A('aaa')
a() # aaa # 会执行__call__方法 		
```

## item 系列
> 使用[]类似于字典一样操作对象

```
class Foo:
	del __init__(self,name,age,sex):
		self.name = name
		self.age = age
		self.sex = sex
	del __getitem__(self,item):
		if hasattr(self,item):
			return self.__dict__[item]
	def __setitem__(self,key,value):
		self.__dict__[key] = value
	def __delitem__(self,key):
		del self.__dict__[key]
	def __delattr__(self,item):
		self.__dict__.pop(item)

f = Foo('alice',18,'nv')
pritn(f['name']) # alice # []会调用getitem 'name' 作为参数 传入 经过hasattr得到name属性的值
f['hobby'] = 'eat' # 添加一个属性 key的值是[]中的数据 key 是 = 号右边的值
print(f['hobby']) # eat
def f['hobby'] # 删除 'hobby' 属性 # 需要自己实现
# 或者使用__delattr__删除
# del f.hobbby # 这个是py本来就支持的 # 而使用[]需要自己实现delitem
```

##构造函数 \_\_new\_\_

> \_\_init\_\_ 方法：初始化方法  在此之前调用\_\_new\_\_方法

**举例:单例模式**
```
class A:
	__instance = False
	def __init__(self,name,age):
		self.name = name
		self.age = age
	def __new__(cls,*args.**kwargs):
	 	if cls.__instance: # 判断__instance 是否为false 如果是就创建一个对象返回给__instance 
			return cls.__instance # 如果是就返回上个已经创建过的对象
		cls.__instance = object.__new__(cls,*args,**kwargs)
			return cls.__instance
```
```
a1 = A('aaa',19)
a2 = A('bbb',20)
print(a1,a2) # 两个对象的地址是一样的
print(a1.name) # bbb
print(a2.name) # bbb
```
## \_\_eq\_\_
类似于`==` 重载

```
class A:
	def __init__(self,name):
		self.name = name
	def __eq__(self,other):
		if self.name == other.name
			return True
		else:
			return False
a1 = A('aaa')
a2 = A('aaa')

print(a1 == a2) # True # 如果不重写__eq__解析器会默认比较两个对象的地址,当我们重写__eq__后会执行这个函数得到结果
```
## \_\_hash\_\_
```
class A:
	def __init__(self,name):
		self.name = name
	def __hash__()
		return hash(self.name)
a = A('aaa')
b = A('aaa')
# 如果没有重写__hash__
hash(a) # hash会根据a，b的内存地址进行hash 
hash(b) # 所以这两次的值是不同的
```
> 如果我们希望hash根据对象里面的属性进行hash
> 我们就要重写这个函数，hash所得的值是根据name属性的值变化

