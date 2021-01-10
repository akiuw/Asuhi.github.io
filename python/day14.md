#反射

> ###程序可以访问、检测和修改它本身状态或行为的一种能力
> ###动态调用方法,动态构造对象,获取类型的相关信息,从程序集中获得类型
> ###通俗地来说:用字符串类型的名字去操作变量
> ###使用反射可以简化,优化代码，带来性能提升

在Python 中主要使用4个反射方法:

* `hasattr`
* `getattr`
* `setattr`
* `delattr`

## 反射对象中的属性和方法

定义一个类
```
class A:
	price = 20
	@classmethod
	def func2(cls):
		print('in func2')
	def func(self):
		print('in func')

a = A()
a.name = 'aaa'
```

###反射对象属性（getattr）
```
ret = getattr(a,'name') # 第一个传对象，第二个传属性名的字符串形式
print(ret) # aaa
```
这种取值方式是通过变量名的字符串形式取到了值

###反射对象方法（getattr）
```
f = getattr(a,'func') # 拿到了func方法
f() # 调用这个方法
```


##反射类属性和方法

###反射类属性
```
 ret = getattr(A,'price')
print(ret) # 20
```
###反射类方法(类名调用的方法)：`classmethod` `staticmethod `

```
if hasattr(A,'func2'): # 判断是否存在
	getattr(A,'func2')()
```
**getattr 和 hasattr 一般在一起使用并保持一致**

##反射模块属性和方法
模块中：
`module.py`
```
day = Monday
def func():
	print('in module')
```

```
import module
print(getattr(module,'day')) # Monday
getattr(module,'func')() # in module
```
###反射当前模块内容
```
import sys
year = 2019

def func():
	print('int func')

ret = getattr(sys.modules['__main__'],'year') 
print(ret) # 2019
getattr(sys.modules['__main__'],'func')() # in func
```

总结:

> **在类似于 xxx.xxx 的场景时基本上可以用getattr来反射**
> **getattr 和 hasattr 一般在一起使用并保持一致**



## setattr和delattr
```
class A:
	pass
a = A()
setattr(A,'name','bbb') # 给类 A 添加静态属性 name 其值为
setattr(a,'name','aaa') # 给对象 a 添加静态属性 name 其值为
```

```
delattr(a,'name') # 删除了对象a的属性name
print(a.name) # bbb # 这里应为对象a的name属性被删除了，就去找A对象的name属性 其值为'bbb'
delattr(A,'name') # 吧类A 的属性name删除
# print(A.name) # 报错
```
