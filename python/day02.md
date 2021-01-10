#文件操作

##打开文件只读
```
f = open('~/Desktop/ceshi/test.txt',mode='r',encoding = 'utf-8') 
```
###读取全部
将文件句柄f文件内容全部读到content里面
```
content = f.read()
#content = f.read(3) #指定读3个，不指定默认读取全部
print(content)
f.close()
```
###执行写
`mode = 'w'`
没有这个文件就直接创建
有的话就直接清除再写
```
f = open('~/Desktop/ceshi/test2.txt',mode='w',encoding = 'utf-8')
f.write('abcdefg')
f.close()
# mode = 'wb'
# 往里面写二进制 encode('utf-8')
```
###追加 
`mode = 'a'`

```
f = open('~/Desktop/ceshi/test.txt',mode='a',encoding = 'utf-8')
f.write('abcdefg')
f.close()
# 追加 mode = 'ab'
f = open('~/Desktop/ceshi/test.txt',mode='ab')
f.write('abcdefg',encode('utf-8'))
f.close()
```
##可读可写
```
f = open('/Users/mirror/Desktop/ceshi/test.txt',mode='r+')
print(f.read()) # 光标移到最后了
f.write('abcdefg')
print(f.read()) # 所以添加是在末尾添加
f.seek(0) #吧光标移到开头
print(f.read())
```
##光标调整
`f.seek(3)`
utf-8是三个字节一个汉字所以光标往后退一个
对于英文字母，如果跳3个 就移动3个字母
read()是按字节走的所以汉字和字母 read(3)都是读3个
```
f.tell()    # tell记录了光标读位置 它会返回此刻光标读位置
f.readline() # 读一行
f.readlines()# 每一行当作列表读一个元素 并返回
```

##重命名文件
```
#在吧test.txt 删除 并吧test.bak 重命名成test.txt
import os
os.remove('~/test.txt')
os.rename('~/test.bak','~/test.txt')
```

##with关键字
```
# 使用with 关键字命名 open返回的句柄 这样打开读文件不需要 调用close()
with open('~/test.txt',mode='r+',encoding = 'utf-8') as obj1,\
        open('~/text.bak',mode='r+',encoding = 'utf-8') as obj2:

    # 把所有的aaa换成bbb
    for line in obj1:
        if 'aaa' in line:
            line = line.replace('aaa','bbb')
        #写文件
        obj2.write(line)
```
