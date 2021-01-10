#C++进阶 设计模式day01

##组建协作模式
框架与运用 通过晚期绑定来实现框架和运用程序至今的松耦合
经典代表：
`Templat Method`
`Strategy`
`Observer/Event`


##模板模式 Templat Method


**对于某一项任务有稳定的整体结构，但各个子步骤有很多的改变需求无法任务整体同时实现**

**在稳定的操作结构下对各个子步骤的变化实现晚绑定**




用法举例:
Run方法是固定不变的，某些步骤的组合是不变的，但其中的某些步骤是在未来变化的
框架实现：
```
class Library{
public:
//步骤1 3 5已经在框架实现
//2 4 在未来可以改变
	void Run(){
		Step1();
		if(Step2())//变化步骤
			Step3();
		for(int i = 0;i<4; i++)
			Step4();//变化步骤
		Step5();
	}

	virtual ~Library(){}

protected:
	bool Step1(){
		//....		
	}
	bool Step3(){
		//....		
	}
	bool Step5(){
		//....		
	}
	virtual void Step2(){}
	virtual void Step4(){}
	
};
```
运用实现：
```
//这时候就只要去确定Step2和4并不会关系到1 3 5和run的实现
class Application:public Library{

//重写步骤 2 4 
	virtual void Step2(){
		//...	
	}
	virtual void Step2(){
		//...	
	}
};

int main(){
	Application* app = new Application();
	app->Run();
	return 0;
}
```

上面的例子就是晚绑定，步骤2,4在未来实现被框架调用
算法骨架是稳定的(Run)将一些步骤延迟到子类（复用）
Templat Method可以重定义算法中的一些特定步骤

不适合的情况：Run假设是稳定的，当Run方法不是稳定的时候就不适合使用该模式


##观察者模式Observer/Event


我们需要建立一种通知依赖关系，一个目标发生变化，所有的依赖对象将得到通知
如果这种依赖比较紧密，就不能抵御变化，使用面向对象技术弱化这种通知

举例：文件分割器FileSplitter,有一个界面MainForm，点击按钮就分割文件Button1_click
场景：提供进度条需求,
变化：进度条样式可能变化(进度条，打点等)
```
class IProgress{
public:
	virtual void DoProgress(float value) = 0;
	virtual ~IProgress(){}

};
```
```
class FileSplitter{
	string m_filePath;
	int m_fileNumber;
	//ProgressBar* m_progressbar; // 这里的ProgressBar提供具体进度条更新的方法
	//是一个通知控件，如果过度依赖通知控件，不能抵御变化
	
	//IProgress* m_iprogress;//这是抽象的通知机制
	List<IProgress*> m_iprogressList;//支持多个观察者

public:
	FileSplitter(const string& filePath,int fileNumber):m_filePath(filePath),
	m_fileNumber(fileNumber){}

	void add_IProgress(IProgress* iprogress){
			m_iprogressList.add(iprogress);
	}
	void remove_IProgress(IProgress* iprogress){
			m_iprogressList.remove(iprogress);
	}


	void split(){
	//1.读文件

	//2.分批次写入小文件
	for(int i = 0;i< m_fileNumber;i++){
		
		//....
		
		//m_progressbar->setValue((i+1)/m_fileNumber);//更新进度条
		onProgress((i+1)/m_fileNumber);
		}
	}
	
//通知所有的观察者
protected:
	virtual void onProgress(float value){
		
		for(auto it = m_iprogressList.begin();it!=m_iprogressList.end();it++)
		it->DoProgress();
	}
};
```
```
//form是主继承类，IProgress是接口类
class MainForm : public Form,public IProgress{

	TextBox* txtFilePath;
	TextBox* txtFileNumber;

	ProgressBar* progressbar;//这里的ProgressBar就是一个观察者

public:
	void Button1_click(){
		string filePath = txtFilePath->getText();
		int number = atoi(txtFileNumber->getText().c_ctr());

		ConsoleNotifier cn;

		FileSplitter splitter(filePath,number);//文件分割器

		splitter.addIProgress(this);//第一个观察者
		splitter.addIProgress(&cn);//第二个观察者

		splitter.split();
	}

/*
	//以前的m_progressbar->setValue((i+1)/m_fileNumber);从基类延迟到子类
	virtual void DoProgress(float value){
		progressBar->setValue(value);	
	}
*/

};
//第二个观察者是使用打点来表示进度的
class ConsoleNotifier:public IProgress{
public:
	virtual void DoProgress(float value){
		cout << ".";
	}


};
```
这个例子中完成进度条具体显示的类progressBar或者ConsoleNotifier提供
它们的父类IProgress是抽象方法类
FileSplitter去调用子类实现的进度条显示方法来完成进度条显示
onProgress方法是稳定不变的MainForm是未来的运用，它加入不同的观察者，但这些观察者都要继承基类IProgress





