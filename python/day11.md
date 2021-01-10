#序列化

> 序列化：**转向一个字符串数据类型**
> 所谓序列就是指字符串
> 反序列化 ： 将字符串转成具体数据类型

##json
> **通用序列化格式**
> 少量数据类型可以序列化

###序列化方法dumps

```
import json
dic = {'k1':'v1'}
s_d = json.dumps(dic)
print(type(s_d),s_d) # <class 'str'> {"k1": "v1"}
```
> 使用json序列化后 引号会变成`"`**双引号** 数据类型会变成字符串

###反序列化loads

```
s = json.loads(s_d)
print(type(s),s) # <class 'dict'> {'k1': 'v1'}
```
> 只有int str list dict 能够序列化
> tuple会被转换成列表在序列化 反序列化后也是列表

###dump和load

* 后面有s的时直接对内存进行操作
* 没有s的是对文件进行操作

```
dic = {1:'a',2:'b'}
f = open('file','w',encoding='utf-8')
json.dump(dic,f) # 给一个可序列化类型 和文件句柄 作为参数
f.close()
```
* 在这之后 dic的内容被写到file文件中了

```
f = open('file',encoding='utf-8')
ret = json.load(f) # 从文件中反序列化
print(type(ret),ret) # <class 'dict'> {'1': 'a', '2': 'b'}
```
> 当数据中有中文时 序列化后会变成byte类型
> 这时候在序列化时`dump(dicc,f,ensure_ascii=False)` 就行了


**注意：**往文件里面写的时候使用dumps序列化后使用write往文件里面写每一句加上换行符`\n`

```
l = [{'k1':'v1'},{'k2':'v2'},{'k3':'v3'}]
f = open('file','w')
for dic in l:
    str = json.dumps(dic)
    f.write(str+'\n')
f.close()
```

* 读的时候使用先读出每一行再用loads反序列化

```
l = []
f = open('file')
for line in f:
    dic = json.loads(line.strip())
    l.append(dic)
f.close()
print(l) # [{'k1': 'v1'}, {'k2': 'v2'}, {'k3': 'v3'}]
```
> json尽量绕过dump和load

##pickle

> 所有的python的数据类型都可以序列化

```
import pickle
dic = {'k1':'v1','k2':'v2','k3':'v3'}
str_dic = pickle.dumps(dic)
print(str_dic) # 输出结果是一串二进制
dic2 = pickle.loads(str_dic)
print(dic2) # {'k1': 'v1', 'k2': 'v2', 'k3': 'v3'} # 字典
```
> * pickle也有dump和load 在打开文件读写的时候需要用二进制打开
> * pickle支持分布dump和load但json不支持

##shelve
> 序列化句柄
> 使用句柄直接操作 使用方便
> python 版本要在3.x

```
import shelve

f1 = shelve.open('file2') # 直接打开文件得到句柄 # 这文件后缀名是.db
f1['key'] = {'k1':'v1','k2':'v2','k3':'v3'} # 索引为key
f1.close()
# shelve 文件存储方式不透明
f = shelve.open('file2') #打开文件进行反序列化
ret = f['key']
f.close()

print(ret) # {'k1': 'v1', 'k2': 'v2', 'k3': 'v3'}
```





