#装饰器

##定义
###假如我们要测量一个函数的执行时间：
```
import time
def timer(f):
    t = time.time()  # 返回1970年到现在过去的秒数
    f()              # f在未来被调用的被测试的函数
    print(time.time() - t)

def func():
    time.sleep(2)
    print('ok')

timer(func) # 测试函数func的运行时间
```
这样我们测到了func执行的时间
但是实际执行的函数是timer
func只是timer的一个参数
如果我们有大量类似func的函数
我们就要执行很多次timer

####如果为我们想直接调用func并测到时间

```
def timer(f): # 传入的参数也算是timer的局部变量
    def inner(*args,**kwargs):
        t = time.time()
        ret = f(*args,**kwargs)   # 在内部函数调用外部函数的变量形成包闭
        print(time.time() - t)
        return ret
    return inner
    
def func():
    time.sleep(1)
    print('ok')

```
```
func = timer(func) # 将func的地址修改成inner
func() # 实际上是运行的inner()
```
####这里 timer就是装饰器 里面的f()就是被装饰的函数

**原则：**开放封闭
开放：对于拓展开放
封闭：对于修改是封闭的

##语法糖

####在被装饰的函数上面写上装饰器的名字 就相当于使用了 func2 = timer(func2)
```
@timer
def func2():
    time.sleep(1)
    return '123'

s = func2() # 1.0039582252502441
print(s)
```
```
@timer
def func3(a,b):
    time.sleep(1)
    return a+b

s = func3(1,2)
print(s)
```

##装饰器的一般规则
```
def wrapper(f): # f是被装饰的函数
    def inner(*args,**kwargs):
        # 装饰内容
        ret = f(*args,**kwargs)
        # 装饰内容
        return ret
    return inner

@wrapper #在函数前加上@装饰名字
def func4():
    time.sleep(0.05)
    print('hello!!')
```

##装饰器存在的控制

假设有很多函数都要使用这个装饰器 要在每个函数前加上 [@装饰器]

#####使用三层装饰器将一个标记flag传进去
#####当flag变成true时启动装饰器
```
import time

FLAG = True
def timer_out(flag):
    def timer(f):
        def inner(*args,**kwargs):
            if flag:
                t = time.time()
                ret = f(*args,**kwargs)
                print(time.time() - t)
                return ret
            else:
                ret = f(*args, **kwargs)
                return ret
        return inner
    return timer
```
###相当于使用了
```
timer = timer_out(FLAG)
@timer
```
###这两行代码
#####最外面的一层只是为了把flag传入并没有其他作用 这就是带参装饰器
```
@timer_out(FLAG)
def func1():
    time.sleep(0.1)
@timer_out(FLAG)
def func2():
    time.sleep(0.1)
@timer_out(FLAG)
def func3():
    time.sleep(0.1)
@timer_out(FLAG)
def func4():
    time.sleep(0.1)
```

##装饰器嵌套调用
```
def wrapper1(func):
    def inner1():
        print('before wrapper1')
        ret = func()
        print('after wrapper1')
        return ret
    return inner1

def wrapper2(func):
    def inner2():
        print('before wrapper2')
        ret = func()
        print('after wrapper2')
        return ret
    return inner2

@wrapper2
@wrapper1
def func():
    print('in func')

func() #before wrapper2
       #before wrapper1
       #in func
       #after wrapper1
       #after wrapper2
```


