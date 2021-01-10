# 内置函数


##exec和eval
###嗲用方式相同
```
eval('1+2+3+4') #有返回值
exec('1+2+3+4') #无返回值
```
1. #####它们都可以执行字符串代码
2. #####eval适合执行有结果都简单计算
3. #####exec适合简单都流程控制
```
code = '''for i in range(10):
    print(i * '*')
    '''
exec(code)
print(eval('1+2+3+4'))
```

##绝对值，膜，幂运算

### 求绝对值
`print(abs(-5)) `
### (1,4) 求膜 商1余4
`print(divmod(9,5)) `
###幂运算
`print(pow(2,3)) # 8 == 2^3 `
###先幂运算在取余
`print(pow(2,3,3)) # 2 == 2^3%3 `


##reversed slice repr
###reversed会返回一个迭代器
```
l = [1,2,3,4,5]
l2 = reversed(l)

print(l2) #<list_reverseiterator object at 0x10395b550> 这是一个迭代器
# reversed返回一个反转之后都迭代器

```
###slice
```
# l = （1，22，33，44，55，66）
sli = slice(1,5,2)
print(l[sli]) # l[1:5:2] #和这个同效

```
###repr
```
print(repr(1)) # 1
print(repr('1')) # '1'
# repr 会告诉你类型 格式化输出是%r

```


##zip filter map sorted

###zip
```
l = [1,2,3]
l2 = ['a','b','c']
l3 = ('*','**',[1,2])
for i in zip(l,l2,l3):
    print(i)
# (1, 'a', '*')
# (2, 'b', '**')
# (3, 'c', [1, 2])

```
###filter
### 判断奇偶性 true 为奇数

* 第一个参数是一个函数 第二个参数是可迭代数据类型
* 这个函数返回的必须是bool值
* 将后面的列表的值代入函数如果是true就保留它

```
def is_odd(x):
    return x % 2 == 1

ret = filter(is_odd,[1,4,4,5,6,7,8,9,0]) 
```

```
for i in ret:
    print(i) # 1，5，7，9 拿到所以的奇数

def is_str(s):
    if type(s) == str:
        return True
    else:
        return False

ret = filter(is_str,[1,'wwww',333,'dasddasd'])

for i in ret:
    print(i)

```

###map
* 将 后面列表的所以元素代入函数中 算出值后返回原来列表的位置上
* filter只是赛选map会改变原来的值 元素个数不变

```
ret = map(abs,[1,-4,5,-2])
for i in ret:
    print(i)
```
###sorted
###它接受一个可迭代类型
```
l = [1,-4,5,2,3,7,2,3,5]
l.sort(key = abs) # sort是在原列表的基础上进行排序
# 这里按照绝对值的大小进行排序

print(sorted(l)) # sorted 也会排序 它会生成一个新列表
print(l)



```
#### 使用条件:l要比较短，sorted要占用内存，且要保留原来列表的顺序





