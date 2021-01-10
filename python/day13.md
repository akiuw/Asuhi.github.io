# 面向对象
##类的定义

> `class 类名:`
   ` 属性 = 'a'`
`print(类名.属性) # a`


###我们定义一个类 Persson
```
class Persson:
    country = ['China'] # 静态属性
    def __init__(self,*args): # 这个函数定义了一些属性，并且把self返回出去
        self.name = args[0] # 名字
        self.hp = args[1]  # 血量
        self.aggr = args[2] # 攻击力
        self.sex = args[3] # 性别
        # print(self.__dict__)
        # print(id(self)) # 4349238408 self返回出去实例化
    def walk(self,n):
        print(self.name+'走了%s步'%n)

player1 = Persson('tom',1000,20,'男') # {'name': 'tom', 'hp': 1000, 'aggr': 20, 'sex': '男'}
```

* 调用这个函数的时候self意味着把player1当作参数传入

```
print(id(player1)) # 4349238408 这里的对象名就是self
print(player1)  # <__main__.Persson object at 0x102a59630> # 在__main__模块中的Persson 对象
player1.walk(5) # tom走了5步
```

> ####对于不可变数据类型来说，最好使用类名来操作
> ####对于不可变数据类型来说，最好使用类名来操作

```
Persson.country = 'US'
```

> ####对于可变数据类型,例如 country = ['China']

###类静态属性赋值

* ####利用索引来修改是共享的

```
player2 = Persson('jack',1000,10,'男')
```

* 当player1修改country[0]的时候player2也会改变

```
player1.country[0] = 'France'

print(player2.country) # ['France']
```

* ####重新赋值是独立的

* 当player1的country重新赋值成'UK'后 player2的country没有改变

```
player1.country = 'UK'
print(player2.country) # ['France']
```

**本质原因：**

* 当player1和player2创建时他们的country指向同一块list类型的内存地址
* 当使用country[0]修改时实际上时修改的country指向的内存地址的内容
* 所以两个player的属性都改变了
* 当给country重新赋值时只时改变了player1的country指向 并没有影响到player2


**任务：** 创建一个类每实例化一个对象就记录起来,并且每个对象都共享这个数据

```
class Foo:
    count = 0
    def __init__(self):
        Foo.count += 1 # 静态属性使用类名操作所以的类都会得到通知
f1 = Foo()
f2 = Foo()
f3 = Foo()
print(f1.count,f2.count,f3.count) # 3 3 3
```

##单继承

**语法：**
```
class A:pass
class B:pass

class C(A,B):pass # C继承了A和B
```

### 查看基类
```
print(C.__bases__) # (<class '__main__.A'>, <class '__main__.B'>)
print(A.__bases__) # (<class 'object'>,) 所有的没有父类的类都是object的子类

```
###子类的实例化
```
class Animal:
    def __init__(self,name,aggr,hp):
        self.name = name;
        self.aggr = aggr;
        self.hp = hp;

class Dog(Animal):
    def __init__(self,name,aggr,hp,kind):
        self.kind = kind;

dog = Dog('ll',100,500,'泰迪')
# print(dog.name) # 报错
# 原因 Dog类里面有init函数 Animal里面也有
# 当我们实例化的时候 直接进入了Dog的init函数 没有进入Animal的init函数
# 所以，name属性没有被赋值因此找不到该属性的值
```

###解决方法：

* 我们只要在Dog里面调用Animal的init函数就可以了
* 使用`super`关键字找到父类

```
class Animal:
    def __init__(self,name,aggr,hp):
        self.name = name
        self.aggr = aggr
        self.hp = hp
    def eat(self):
        print('吃')
        self.hp += 100

class Dog(Animal):
    def __init__(self,name,aggr,hp,kind):
        # Animal.__init__(self,name,aggr,hp)
        super().__init__(name,aggr,hp) # 使用super关键字代替父类并且不需要传self
        # python3中都可以用super找到父类
        self.kind = kind # 派生属性
    def bite(self):
        print('咬人') # 派生方法
    def eat(self):
        Animal.eat(self) # 先执行了父类的eat
        print(self.name + '吃了hp+%s 攻击力+%s'%(100,10))
        self.aggr += 10;

dog = Dog('abc',100,500,'泰迪')
print(dog.name)
dog.eat() # abc吃了hp+100 攻击力+10 # 使用继承的方法
print(dog.hp,dog.aggr) # 600 110

super(Dog,dog).eat() # 在类外面调用必须传类名和对象名
```


##多继承


```
class A:
    def func(self):
        print('A')
class B:
    def func(self):
        print('B')
class C:
    def func(self):
        print('C')

class D(A,B,C):
    pass

d = D()
d.func() # A 因为A是第一个继承的
# 按照顺序往上找就是A的func

```

###菱形继承问题
**情况1**

```
class A:
    def func(self):
        print('A')
class B(A):
    def func(self):
        print('B')
class C(A):
    def func(self):
        print('C')

class D(B,C):
    pass

d = D()
d.func()
# 广度优先
# 先取找B因为是先继承的B 如果B没有就取找C 最后找A
```

**情况2:**
```
class A:
    def func(self):
        print('A')
class B(A):
    def func(self):
        print('B')
class C:
    def func(self):
        print('C')
class D(C):
    def func(self):
        print('D')

class E(B,D):
e = E()
e.func()
# 顺序是：B A D C
# 如果找完B再找D的话就会错过A 以后就找不到了 #所以一定是要找A再找D
```

###经典类和新式类

> * 新式类： 继承了object的类
> * 经典类： py2中直接创建的类
> * 新式类是广度优先
> * 经典类是深度优先
> * super本质是：根据节点位置广度优先顺序找到方法的



