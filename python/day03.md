#函数

##定义函数
**定义一个计算字符串长度的函数**
```
def my_len(s):
    i = 0
    for k in s:
        i += 1
    return i
```

* 封装成函数 把测到到长度值返回
* 如果不写return 函数会返回 None
* s 是传递的参数
* 函数内部为局部变量

###有多个返回值

```
def fun():
    return 1,2,3

print(fun()) #(1,2,3) 会得到一个这些返回值组成的元祖
#分配
a,b,c = fun() #将元祖里面的值分配给三个变量
```
##函数调用
假如我们定义了一个函数
```
def my_sum(a,b):
    return a+b
```

**有如下调用方式：**

1. `my_sum(1,2) # 这里a的值是1 b的值是2`
2. `my_sum(a=2,b=1) # 这里的传参按照关键字传参`
3. `my_sum(2,b=1) #混合 必须先按照位置传参 再按照关键字传参`

```
# my_sum(1,a=2) #报错
# my_sum(a=1,2) #报错
```
* 不能给同一个变量传多个值
* my_sum(1) #报错
* 位置参数：必须传

##默认参数
```
def class_info(name,sex='男'):
    print('%s : %s'%(name,sex))

class_info('tom')
class_info('alice','女')
```
###open函数举例
```
def open(file, mode='r', buffering=None, encoding=None, errors=None, newline=None, closefd=True):
```
对于open函数 file是文件路径 是位置参数必须传 mode 可以不传默认为只读 可以指明后面某个参数的值 如`encoding='utf-8'`

##动态参数
```
def sum(*args):
    #这里的args是一个元组 它将动态参数组合成一个元祖
    res = 0
    for i in args:
        res += i
    return res

sum(1,2)
sum(1,2,3,4)
sum(1,3,4,5,6)
```
```
def print(self, *args, sep=' ', end='\n', file=None):
```
print就是一个动态参数的函数

***args 不能接受关键字传参**

```
def func2(**kwargs):
    print(kwargs)

func2(a = 1,b = 2,c = 3)    #{'a': 1, 'b': 2, 'c': 3}
func2(a = 1,b = 2)          #{'a': 1, 'b': 2}
```
****kwargs 传进去一个字典 其元素里面的key为变量名，data是传进来的值**

对比 `*args` 和 `**kwargs`
* *args 按照位置传参的值组成一个元组 
* **kwargs 接受按照关键字传参的值组合成一个字典
* 它们两个可以接受任意数量的参数

```
def func3(*args,**kwargs):
    print(args,kwargs)

func3(1,2,3,4,a = '1',b = '2',c = 5) # (1, 2, 3, 4) {'a': '1', 'b': '2', 'c': 5}
```

1. 我们传参的时候约定 先传位置参数 再传关键字参数
2. 定义函数的时候args 也必须在 kwargs 之前

##传参位置顺序
####func(位置参数,*args,默认参数,**kwargs)
```
# 举例：
def func4(*args):
    print(args)

l = [1,2,3,4]
#如果要把l的元素作为一个个参数传入函数func
func4(*l) #在类似地如l这个序列前面加上*号 就按照顺序分别传入函数

#在函数的角度上 在args上加上*就意味着组合一个序列

def func5(**kwargs):
    print(kwargs)

dr = {'a':1,'b':2}
func5(**dr) #打散一个字典 #对于函数func5会把这些参数组合成一个字典
#效果如上
```

##匿名函数

**对于普通函数：**

```
def calc(n,m):
    return n**m
```
把这个函数写成匿名函数

`calc = lambda n,m:n**m`

**其形式为：**

####函数名 = lambda 参数:返回值

* ####匿名函数关键字：lambda
* ####在:号左边是输入参数, 多个参数用','号分割，右边是返回结果
* ####整个匿名函数不能换行

###匿名函数的调用:
`calc(2,3)`

```
dic = {'k1':10,'k2':100,'k3':30}
print(max(dic)) # 默认取key值最大的

print(max(dic,key = lambda k:dic[k])) # 找出value值最大的key值

```


