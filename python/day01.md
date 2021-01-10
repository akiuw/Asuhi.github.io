
#基本类型及操作

##字符串

###大小写变化
```
s = 'aABbCcddeeff'
s1 = s.capitalize() #首字母大写
s2 = s.upper() #所以的字母变成大写
s21 = s.lower() #全部小写
#大小写变化对数字无影响
```
**验证码输入不区分大小写**
```
s_str = 'ac2E1Q'
your_input = input('输入验证码不区分大小写')
if s_str.lower() == your_input.lower():
    print('输入成功')
else:
    print('验证码错误')
```
**大小写反转** 
```
s3 = s.swapcase() #大小写翻转
```
**首字母大写**
```
s = 'dabcde def*abc'
s4 = s.title() #每个被空格，数字或特殊字符隔开的英文单词首字母大写

```
**居中**
```
s = 'abcWWaaa'
s5 = s.center(20,'#') #让字符串居中并且使用#填充，如果不填写使用空格填充

s = ‘aaa亿元’

l = len(s) #l长度为5 字母和汉字都算一个元素

```
###增，删，改，查

**相关元素判断**
```
s = 'alswsde'

s7 = s.startswith('al') #s7为true 判断s是不是以al开头的

s8 = s.find('ls') #找字符串中是否有该元素 如果存在返回该元素的下标
s81 = s.index('A')#找到指定元素的索引，找不到就报错
```
**去除元素**
```
s = '   dsadas   '
s9 = s.strip() #默认去掉s中前后的空格
s91 = s.strip('%*')#去除前后的%和*字符 rstrip只删除右边
```
**查找统计元素**
```
s10 = s.count('a')#计算字符串中有多少个a
```
**分割字符串**
```
s = ':abc :de :fg'
l = s.split()#默认以空格分割
l1 = s.split(':')#以:号分割
print(l1)# ['','abc ','de ','fg']
#这里s被转换成list
```
**格式化**
```
name = 'aaa'
age = 15
hobby = 'game'
s = '我叫%s,今年%d,爱好%s'%(name,age,hobby) # 使用%格式化 和c语言相似

s = '我叫{},今年{},爱好{}'.format('张三',33,'玩游戏')#按顺序替换

s = '我叫{0},今年{1},爱好{2},我叫{0}'.format('张三',33,'玩游戏') #按索引值替换

s = '我叫{name},今年{age},爱好{hobby},我叫{name}'.format(name = '张三',hobby = '玩游戏',age = 15)

s = ‘大赛地方的萨格地方法四大地方’
```
**替换**
```
s11 = s.replace('地方','三四')#将地方全部替换成三四
s11 = s.replace('地方','三四',1)#将第一个地方替换成三四

#每一个字符一个个打印for循环的用法
s = 'sadsadsad'
for i in s:
    print(i)
```
**查找元素**
```
#通过索引来得到字符
s = 'ABCDEFG'

s1 = s[0] # A
s2 = s[2] # C

s3 = s[0:4] # ABCD 
#如果要取ABCD 必须要取到4 其范围表示[0,4)

s4 = s[-1] # G
# 从后往前数第一个 

s5 = s[-2] # F

s6 = s[0:-1] # ABCDEF 不会取到G

s7 = s[:] # ABCDEFG
s8 = s[0:] # ABCDEFG
#这两种方法都能取到所以的

s9 = s[0:0] #空字符串

s10 = s[0:5:2] # ACE 
#[0,5)每隔两个取一个

s11 = s[4:0:-1] #从后往前 (0,4] 每隔一个 
#EDCB
s12 = s[3::-1] # DCBA 从第3个往前取完
s13 = s[3::-2] # DB
```

##列表

列表是有序的，有索引，可切片

**增**
```
lis = {'alice','mood','echo']

#尾插
lis.append('mono')
lis.append(23)

lis.insert(3,'zhangsan') #第3个后面插入元素

lis.extend('lisi') # lisi会被分解成'l' 'i' 's' 'i' 添加到最后 
#能被迭代的只能是字符串和列表
```
**删除**
```
s = lis.pop(1) # 删除第2个元素 并返回这个元素  
#不填参数表示删除第一个元素

lis.remove('alice') #按照元素删除

lis.clear() #清空列表

del lis[1:] # 1和它后面的元素全部删除
```
**按索引修改**
```
lis[0] = 'hhhh' #直接修改0
```
**查找**
```
lis.count() #长度 元素个数
lis.index('echo') #返回元素的索引

```
**排序和反转**
```
lis = [1,5,3,2,7,3,4,5]
lis.sort() #排序默认从小到大

lis.reverse() #反转顺序
```

##元组
只读列表：可查询，循环，切片

**定义**
```
tu = (1,2,3,'alice',[1,2,3,'event'],'eval')
# 元素可以是各种数据类型
```

**其中 1,2,3,'alice' 这些元素不能修改**

**但是其中列表`[1,2,3,'event']`可以修改**
```
tu[4][3] = tu[4][3].upper() #修改'event'为大写字母
```
**list ———> string**
```
s = 'alex'
s1 = '_'.join(s) # s1 == 'a_l_e_x'
lis = ['alice','mood','echo']
s2 = ''.join(lis) # s2 == 'alicemoodecho' #直接用''中的字符拼接
```
**string ———> list**
```
#range
for i in range(0,100) #0~99 全部打印
	print(i)

for i in range(0,10,2) #0~10 偶数全部打印
	print(i)

for i in range(10,0,-1) #0~10 倒叙打印
	print(i)
```

##字典
**定义**
```
dic = {
	'name':['张三','李四'],
	'py':[{'num':12,'age':17,}],
	True:1,
	(1,2,3):'yiersan',
	2:'二',
}
```

key只能是string int bool类型 data可以是任何类型
意味着key是不可变的，而data是可变的
字典查找数据是使用的二分法查找的
并且储存着大量的数据之间的关系

`dic1 = {'age':18,'name':'lisi','sex':'male',}`

**增加键值对**
```
dic1['high'] = 185
dic1['age'] = 20 #age已经存在这里会直接覆盖它的值

dic1.setdefault('weight') # 'weight':None #没有指明值就默认为None
dic1.setdefault('weight',150) # 'weight':150
dic1.setdefault('name','zhangsan') # 'name' :lisi 如果存在就不做任何改变 没有才增加
```
**删除**
```
#1 按照键值删除
s = dic1.pop('age') #删除键值为'age'的数据,返回值是被删除值的data

dic1.pop('abc')#如果没有这个键就会报错
dic1.pop('abc',None) #这样就不会报错了并且返回None

dic1.clear() #清空字典

del dic1['name'] #指定删除没有就报错
del dic1 #删除整个字典
```
**修改**
```
dic1['name'] = '张三'#直接覆盖

dic2 = {'age':18,'name':'张三','sex':'male','abc':'abcdefg'}
dic2.update(dic1) #dic1的所有的键值对更新到dic2里面 dic2有的就覆盖，没有的就添加
```
**查找**
```
print(dic1.keys()) #以列表的方式打印所有的键
print(dic1.values()) #以列表的方式打印所有的value
print(dic1.items()) #使用元祖的方式组合键值对，并且以列表的方式组合输出

for i in dic1:
	print(i) #打印键

for i in dic1.values():
	print(i) #打印所有的值

v1 = dic1['name']

dic1.get('name1','没有这个建')
```
**对应赋值**
```
a,b = 1,2
a,b = b,a
a,b = [1,2]
a,b = [1,2],[3,4]

for k,v in dic1.items():
	print(k,v)
```
##集合

**集合是可变数据类型，里面的元素必须是不可变数据类型，集合是无序的不可重复的**

**创建方式**
```
set1 = set({1,2,3})
set2 = {1,2,3}
```
**增加元素 **
```
set1.add('nnn')
set1.update('abc') #将abc分别加入集合
```
**删除**
```
set1.pop() #随机弹出一个元素
set1.remove('alex') #删除指定元素
set1.clear() #清空 set()表示空集合
```

定义俩个集合
`set1 = {1,2,3,4,5}`
`set2 = {4,5,6,7,8}`

**交集**
```
print(set1 & set2) # {4,5}
print(set1.intersection(set2)) #{4,5}
```
**并集**
```
print(set1 | set2) #{1,2,3,4,5,6,7,8}
print(set2.union(set1)) #{1,2,3,4,5,6,7,8}
```
**反交集**
```
print(set1 ^ set2) #{1,2,3,6,7,8}
```
**差集**
```
print(set1 - set2) #{1,2,3} set1独有的
print(set2 - set1) #{6,7,8} set2独有的
```
set1 < set2 #如果是true 就说明set1是set2的子集 set2是set1的超集

**去重**
```
li = [1,2,33,33,2,1,4,5,6,6]
set = set(li)
li = list(set)
```

##小数据池

**在一定范围之内的创建的变量是共用一个地址**

**现象：**
```
i1 = 6
i2 = 6
print(id(i1),id(i2))
#这里的i1 和 i2的地址也是一样的
```
```
i1 = 300
i2 = 300
print(id(i1),id(i2))
#这里的i1 和 i2的地址是不一样的
```
**数字的范围是 -5 ~ 256**
**字符串是**
**不能含有特殊字符**
```
s1 = 'alex'
s2 = 'alex'
print(id(s1),id(s2)) #这里是一样的
s1 = 'alex@'
s2 = 'alex@'
print(id(s1),id(s2)) #这里是不一样的
```
####list dirct tuple set 没有小数据池


