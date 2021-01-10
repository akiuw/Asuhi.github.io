#信号和槽

###信号和槽函数通过connect函数建立联系

```
//...
    MySignal signal;
    MySlot slot;
	QObject::connect(&signal,SIGNAL(signal()),&slot,SLOT(slot()));
//...
```
###我们自定义一个信号和槽

* MySIgnal.h

```
#ifndef MYSIGNAL_H
#define MYSIGNAL_H
#include <QObject>
class MySignal : public QObject
{
    Q_OBJECT
public:
    explicit MySignal(QObject *parent = 0);

signals:
    void signal();
public slots:
};
#endif // MYSIGNAL_H
```

* MySlot.h

```
#include <QObject>
#include <QDebug>
class MySlot : public QObject
{
    Q_OBJECT
public:
    explicit MySlot(QObject *parent = 0);

signals:

public slots:
    void slot();
};
```

* MySlot.cpp

```
#include "myslot.h"

MySlot::MySlot(QObject *parent) : QObject(parent){}

void MySlot::slot(){
    qDebug() << "slot send";
}
```

* main.cpp

```
int main(int argc,char ** argv){
    QCoreApplication app(argc,argv);
    MySignal signal;
    MySlot slot;
    QObject::connect(&signal,SIGNAL(signal()),&slot,SLOT(slot()));
    emit signal.signal();//emit关键字发出一个信号
    return app.exec();
}
```


总结：

1. 
> 信号定义必须在signal:保留字下，不需要实现
> 槽定义必须在slot下，需要实现

2. 
> 信号和槽通过QObject::connect函数链接
> 当信号被触发时曹函数被调用

3. 
> 实现信号和槽的类必须是QObject的子类，且必须以宏Q_OBJECT开始
> 链接信号和槽要用到SIGNAL和SLOT宏转换函数为字符串(反射)

4. 
> 一个信号可以和多个槽链接,此时槽函数的调用顺序是不确定的
> 多个信号可以同时链接一个槽
> 信号可以链接信号,形成信号传导

5. 
> 信号和槽的参数应该一样多,并且类型必须相同
> 信号可以重载，槽可以重载，且都可以有默认参数

6. 
> 槽函数可以和普通函数一样被调用
> 槽函数中调用sender可以获得信号拥有者(调用者)

7. 记忆：
> 一个类:QObject
> 三个宏:Q_OBJECT,SIGNAL,SLOT
> 三个保留字:signals,slots,emits

