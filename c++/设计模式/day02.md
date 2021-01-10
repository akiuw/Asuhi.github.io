#C++进阶 设计模式day02

##对象创建模式
> 使用设计模式绕开new来创建对象

经典代表:
`Factory Method`
`Abstract Factory`
`Prototype`
`Builder`

##工厂方法Factory Method

> 使用Factory Method模式从而避免new创建对象过程中的紧耦合(依赖具体的类)
> 定义一个创建对象的接口，让子类决定创建哪个类的对象

**实例：分割文件的类**
> 我们可能需要更多的分割文件的类
> 比如二进制文件的分割，文本文件的分割，视频文件的分割...等

###代码如下：
**如果我们直接去创建对象使用new的方式，会依赖具体的实现细节导致不能应对需求变化**
```
//分割器类的抽象基类
class ISplitter {
public:
	virtual void split() = 0;
	virtual ~ISplitter() {}
};
class BinarySplitter :public ISplitter {//...
};
class TxtSplitter :public ISplitter {
public:
	void split() {
		//...
	}
};
class PictureSplitter :public ISplitter {//...
};
class VedioSplitter :public ISplitter {//...
};
class MainForm : public Form, public IProgress {
	TextBox* txtFilePath;
	TextBox* txtFileNumber;
	ProgressBar* progressbar;//这里的ProgressBar就是一个观察者
	
public:
	void Button1_click() {
		string filePath = txtFilePath->getText();
		int number = atoi(txtFileNumber->getText().c_ctr());
		ISplitter* splitter = new BinarySplitter(filePath, number);//这一行依赖具体实现细节BinarySplitter违背了设计原则
		splitter->split();
	}
};
```

###使用使用工厂方法来重构：

> 创建对象交给未来,从而支持扩展
> 将创建对象交给具体的factory
> 在构造器中传进来了各种工厂
> 调用这些工厂的CreateSplitter来创建对象


```
class ISplitter {
public:
	virtual void split() = 0;
	virtual ~ISplitter() {}
};

class SplitterFactory {
public:
	virtual ISplitter* CreateSplitter() = 0;
	virtual ~SplitterFactory(){}
};
class MainForm : public Form{

	TextBox* txtFilePath;
	TextBox* txtFileNumber;

	ProgressBar* progressbar;//这里的ProgressBar就是一个观察者
	SplitterFactory* factory;
public:
	MainForm(SplitterFactory* factory) {
		this->factory = factory;
	}
	void Button1_click() {
		string filePath = txtFilePath->getText();
		int number = atoi(txtFileNumber->getText().c_ctr());
		ISplitter * splitter = factory->CreateSplitter();
		//创建对象交给未来,从而支持扩展
		//将创建对象交给具体的factory
		//在构造器中传进来了各种工厂
		//调用这些工厂的CreateSplitter来创建对象
		splitter->split();
	}
};

//具体类
class BinarySplitter :public ISplitter {
};
class TxtSplitter :public ISplitter {
};
class PictureSplitter :public ISplitter {
};
class VedioSplitter :public ISplitter {
};
//具体的工厂
class BinarySplitterFacory:public SplitterFactory{
public:
	virtual ISplitter* CreateSplitter() {
		return new BinarySplitter();
	}
};
class TxtSplitterFacory :public SplitterFactory {
public:
	virtual ISplitter* CreateSplitter() {
		return new TxtSplitter();
	}
};
class PictureSplitterFacory :public SplitterFactory {
public:
	virtual ISplitter* CreateSplitter() {
		return new PictureSplitter();
	}
};
class VedioSplitterFacory :public SplitterFactory {
public:
	virtual ISplitter* CreateSplitter() {
		return new VedioSplitter();
	}
};
//未来使用
VedioSplitterFacory splitterFacory;
MainForm mainforme(&splitterFacory);
```


