# 常用模块

##collections

> **python 的扩展数据类型模块**

##可命名元组——namedtuple
```
from collections import namedtuple
Point = namedtuple('point',['x','y']) # '名字',[]
```
####我们定义了一个元组名字为point 其元素有x和y两个属性
```
p = Point(1,2) # 给x和y赋值
print(p) # point(x=1, y=2)
print(p.x,p.y) # 1 2
```
####定义了一个card元组 有花色和数字两个属性
```
Card = namedtuple('card',['suits','number'])
```
```
c = Card('红桃',3)
print(c) # card(suits='红桃', number=3)

```

##队列queue
###元素入队列
```
import queue
q = queue.Queue() # 得到句柄
q.put(10)
q.put(6)
```
```
print(q.get())  # 得到第一个元素
# 如果队列中没有值的时候就阻塞
print(q.qsize()) # 得到队列大小
```
##双端队列 deque

> `from collections import deque`

和c++里面的deque类似
```
dq = deque([1,2])
dq.append(10) # 从后面放数据
# deque.appendleft()# 从前面放数据
dq.append(6)
dq.insert(1,3)
print(dq) # deque([1, 3, 2, 10, 6]) 查看队列内容
dq.pop() # 从后面取数据
dq.popleft() # 从前面取数据
dq.reverse() # 翻转队列
```

##有序字典 OrderDict
### 用法和dict完全一样只是key变得有序
```
od = OrderedDict([('a',1),('b',2),('c',3)])
print(od)
print(od['a'])
# 可以用for访问
for i in od:
    print(i,':',od[i])
```
### defaultdict
> `from collections import defaultdict`

### 场景：讲大于66的数和小与66的数分开
```
values = [11,22,33,44,55,66,77,88,99]

d = defaultdict(list)# 创建句柄制定默认值是list类型

for value in values:
    if value >= 66:
        d['k1'].append(value)
    else:
        d['k2'].append(value)
```

* 在使用defaultdict的时候没有这个key的话就创建它
* 默认值是list类型的在创建句柄对象时指定

```
# 默认值是5
d = defaultdict(lambda :5)
```



##time模块

**基本方法：**
```
time.sleep(0.1) # 停一段时间
time.time() # 1970到现在的秒数
```

###时间字符串

> Y 代表年 m表示月 d代表天 H代表时 M代表分 S代表秒 他们直接被-和:隔开

`print(time.strftime("%Y-%m-%d %H:%M:%S")) ` `# 2018-12-21 17:23:50`

`print(time.strftime("%m/%d %H:%M"))` `# 12/21 17:26`

> a简化星期 A完整名字

`print(time.strftime("%m/%d %a %H:%M")) # 12/21 Fri 17:27`


`ret = time.localtime() `

> 所有的时间内容存放在元组中这是一个可命名元组`

```
print(ret)
#time.struct_time(tm_year=2018, tm_mon=12,\
# tm_mday=21, tm_hour=17, tm_min=29, tm_sec=21,\
# tm_wday=4, tm_yday=355, tm_isdst=0)
```
###时间戳和结构化时间的转换
```
print(time.strptime('2000-12.31','%Y-%m.%d'))
print(time.strftime('%m/%d/%Y %H:%M:%S',time.localtime(30000000000))) # 08/30/2920 13:20:00
print(time.asctime()) # Fri Dec 21 17:38:35 2018
```

##random

```
import  random

random.random() # 大于0小与1的小数
random.uniform(1,3) # 大于1小于3的小数

random.randint(1,5) # 大于1小于等于5的整数
random.randrange(1,10,2) #步长为2 # 大于1小于10的奇数
random.choice([1,'23',[4,5]]) #接受一个可迭代的数据类型 随机返回其中的一个元素
l = [1,2,3,4,5]
random.shuffle(l) #打乱一个列表的顺序

```
