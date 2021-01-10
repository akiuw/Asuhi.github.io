# 常用模块2

##os模块

###打印执行的文件所在的目录
`print(os.getcwd())`

改变脚本的工作目录
```
os.chdir('/Users/mirror/PycharmProjects')
print(os.getcwd()) # /Users/mirror/PycharmProjects
```
```
print(os.curdir) # .
print(os.pardir) # ..
```
> 创建目录 如果存在则报错

`os.makedirs('dir1/dir2')`


> 递归删除空目录 如果上层目录为空就继续删除直到目录不为空为止

`os.removedirs('dir1/dir2')`

```
os.mkdir('dir') # 在指定路径下创建目录
os.rmdir('dir') # 删除单独的空目录
os.listdir('.') # 列出目录下的所有文件以及文件夹 以列表返回
os.remove('~/Desktop/test') # 删除指定文件
os.stat('day13.py') # 查看文件状态 返回stat结构
print(os.environ) # 返回系统的环境变量
```

> 与路径有关的操作 os.path

`print(os.path.getsize(os.path.join(os.getcwd(),'day13.py')))`

##sys模块
###与python解释器交互的模块

> 查看操作系统

```
print(sys.platform) #  darwin
```

> 查看python版本信息

```
print(sys.version) #3.6.3 (v3.6.3:2c5fed86e0, Oct  3 2017, 00:32:08)
                   #[GCC 4.2.1 (Apple Inc. build 5666) (dot 3)]
```

> 直接退出程序

```
# sys.exit() #  参数为错误码
print(sys.path) # 导入模块引用的路径
print(sys.argv)
ret = sys.argv  # py文件执行传入参数和c语言的main函数参数相似
```


