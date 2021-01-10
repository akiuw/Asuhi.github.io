#命名空间

命名空间范围|所属
:--|:--
内置命名空间|python 解释器自带
全局命名空间|自己写的代码但不属于函数
局部命名空间|函数中但代码

* ####局但函数和内置但函数重名时 会使用全局的

##global关键字:

###global关键字声明的变量会影响全局的变量
```
a = 1
def func():
    global a
    a = 2
print(a) # 2
```


###global 只是影响全局作用域的变量 第一次定义的a的值会被inner2改变
```
a = 1
def outer():
    a = 10
    def inner():
        b = 2
        print(a)
        def inner2():
            global a
            a += 1
        inner2()
    inner()
    print(a) # 10

outer()
print(a) # 2 
```
####如果我们要在inner2里面改变outer声明的a的值的话就要使用nonlocal 关键字

```
a = 1
def outer():
     a = 10
    def inner():
        b = 2
        print(a)
        def inner2():
            nonlocal a
            a += 1
        inner2()
    inner()
    print(a) # 11

outer()
print(a) # 1
```
**nonloacl 关键字只能用于局部变量**
**影响上一层作用域距离最近的变量的值**

```
def func():
    print('aaa')
func2 = func # 函数名可以赋值
fucn2()
```

##包闭

####闭包条件：
* ####嵌套的函数
* ####且内部函数要调用外部函数的变量

```
def outer():
    a = 1
    def inner():
        print(a)
    print(inner.__closure__)
    return inner # 闭包常把内部函数传出去
inn = outer()
inn()
print(outer.__closure__)
```


