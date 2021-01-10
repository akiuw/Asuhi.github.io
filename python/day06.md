# 迭代器与生成器

##迭代器
###这是一种固有的属性用于遍历元素

####得到列表的迭代器
```
l = [1,2,3]
iterator = l.__iter__() # 得到l的迭代器
```
####迭代器中的next方法可以得到元素
```
for i in range(3):
    print(iterator.__next__()) # 打印3次
```
#### for循环到实质是使用迭代器进行取值


##生成器函数 

* ###含有yield关键字的函数都是生成器函数
* ###并且yield不能和return 共用
* ###并且需要写在函数内部

```
def generator():
    print(1)
    yield 'a'
```
###生成器函数会返回一个生成器 不会执行函数内容
```
ret = generator() 
print(ret) # <generator object generator at 0x102bb3048>
```
###调用next的时候触发了函数代码
`print(ret.__next__()) `

##yield的返回值
```
def generator():
    print(1)
    yield 'a' # 作为next的返回值
    print(2)
    yield 'b' # 作为next的返回值

g = generator()
ret = g.__next__() # ret的值为a g.__next__()会执行一个语句print(1)
print(ret)
ret = g.__next__() # ret的值为b g.__next__()会执行print(2)
print(ret)
```
###用for循环效果和上面一样
```
for i in g:
    print(i)
```
##生成器的资源占用

###假如我们生成20000个字符串
```
def ppp():
    for i in range(20000):
        yield 'pp%s'%i
```

####使用生成器的话生成一个打印一个 这里只取了50个就在内存里面只生成了50个
####如果是想一般函数那样一次性返回的话就会全部生成在内存里面
####等我们取出50个再打印
####这样就会比较节省内存
```
g = ppp()
cont = 0
for i in g:
    cont += 1
    print(i)
    if cont > 50:
        break
```
####没有取到的字符串不会在内存中出现直到我们去取出它们
```
cont = 0
for i in g:
    cont += 1
    print(i)
    if cont > 50:
        break
# 再取50个
```
###任务：监听文件输入
```
def tail(filename):
    f = open(filename,encoding='utf-8')
    while True:
        line = f.readline()
        if line:
            yield line

g = tail('/Users/mirror/Desktop/test')

for i in g:
    print(i)
```

##send方法

```
def generator():
    print(123)
    num = yield 1
    print(num) # hello
    print(456)
    yield 2
    print(789)

g = generator()
ret = g.__next__() # 执行yield到时候就已经返回出去了，所以num在这没被赋值
print(ret)  # 1
ret = g.send('hello')  # send 到效果和next到效果一样（指获取下一个值）
# send  在获取下一个值到时候给上一个yield传递一个数据
# 在执行到下一个yield到时候会返回 2 到外面
print(ret) # 2

```
