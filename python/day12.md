# 模块和包

##模块
###在同目录下创建一个文件 `demo.py`

**demo.py:**

```
print("in demo")
def test():
    print('test')
st = 'aaa'
def name():
    print(__name__)
```
我们执行语句
`import demo`

> * 当我们import一个文件时就是执行了里面的内容
> * 这个文件里面的所以变量和方法都被加载到这个模块单独的命名空间里面

###调用demo文件里面的方法
```
demo.test() # test
print(demo.st) # aaa
```
###python模块的导入方式
我们调用`sys.modules.keys()`方法可以查看到
```
import sys
print(sys.modules.keys()) 
# ...., 'sitecustomize', 'demo']) sys.modules里面存放导入的模块
```

> * import一个模块时 会先从sys.modules里面查看模块是否被导入
> * 如果已经导入就不会第二次导入了
> * 如果没有被导入就依据sys.path路径取寻找模块 没找到就报错

给一个模块取别名 提高代码的兼容性
```
import time as t
print(t.time())
```
###单独从某个模块中导入一个方法

> 被导入的变量已经属于文件本地的变量了就脱离了命名空间

```
from time import sleep
sleep(0.5)
```

###导入全部的方法和变量到本地
```
from demo import *
test()
print(st)  # aaa
st = 'sss'
print(st) # sss
# 导入本地的变量可以被修改
```
###\__main__
```
print(__name__) # 运行的这个文件就会打印 __main__
demo.name() # demo 会打印模块名
```

##包
> 把解决一类问题的模块放在同一个**文件夹**里
> 这个**文件夹**就是包

**我们先创建一个目录结构如下:**

```
import os
os.makedirs('glance/api')
os.makedirs('glance/cmd')
os.makedirs('glance/db')
l = []
l.append(open('glance/__init__.py','w'))
l.append(open('glance/api/__init__.py','w'))
l.append(open('glance/api/policy.py','w'))
l.append(open('glance/api/versions.py','w'))
l.append(open('glance/cmd/__init__.py','w'))
l.append(open('glance/cmd/manage.py','w'))
l.append(open('glance/db/__init__.py','w'))
l.append(open('glance/db/modles.py','w'))
for f in l:
    f.close()
```

###两种导入模块的方式

####1
```
import glance.api.policy as policy
policy.get()
```

####2
```
from glance.api import policy
policy.get()
```

> 我们在这当前文件里面导入下面的包
> 但是在导入glance下的包的时候执行的文件路径没有变,`sys.path`没有变
> 所以还需要在glance下面的包的**__init__.py**中导入完整的路径

### api/\__init__.py :
```
from glance.api import policy
from glance.api import versions
glance.api.policy.get()
```

**问题：**
> 当我们在glance上层加上dir顶层包时 -> `dir/glance`
> 我们必须修改glance的**\__init__.py**文件成这个路径才能导入成功
> 也就是意味着允许py文件和包的路径这件的相对路径不能改变
> 我们在**__init__.py**文件里面写这个包的相对路径就可以了

###glance/\__init__.py :

```
from . import api
from . import cmd
from . import db
```
> 注意：如果这么写的话**不能**在这个**包的范围下**使用**包里面的模块**







