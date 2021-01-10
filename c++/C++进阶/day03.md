
#C++进阶
## Complex类

Complex.h
```
#pragma once
#include <iostream>

template <typename T>
class Complex {

private:
	T _re, _im;
public:
	Complex() :_re(0), _im(0) {}
	Complex(T re, T im = 0) :_re(re), _im(im) {}

	//在operator<<后面加<T>
	friend std::ostream& operator<< <T> (std::ostream& os, const Complex& tmp);

	void Print();
	Complex& operator=(const Complex& tmp);
	Complex operator+(const Complex& d);
	Complex& operator+=(const Complex& d);
	Complex& operator++();//前置++
	Complex operator++(int);//后置++
};
```
Complex.cpp
```
#include "Complex.h"
//本质：模板是两次编译生成的，第一次生成的函数头和第二次生成的函数头不一样导致找不到函数体
template <typename T>
std::ostream& operator<<(std::ostream& os, const Complex<T>& tmp) {
	return os << tmp._re << " + " << tmp._im << "i";
}
template <typename T>
void Complex<T>::Print() {
	std::cout << this->_re << " + " << this->_im << std::endl;
}
template <typename T>
Complex<T>& Complex<T>::operator=(const Complex<T>& tmp) {
	if (&tmp != this) {
		_re = tmp._re;
		_im = tmp._im;
	}
	return *this;
}
template <typename T>
Complex<T> Complex<T>::operator+(const Complex<T>& d) {
	Complex ret;
	ret._re = (this->_re + d._re);
	ret._im = (this->_im + d._im);
	return ret;
}
template <typename T>
Complex<T>& Complex<T>::operator+=(const Complex<T>& d) {
	this->_re += d._re;
	this->_im += d._im;
	return *this;
}
template <typename T>
Complex<T>& Complex<T>::operator++() {
	this->_re++;
	return *this;
}
template <typename T>
Complex<T> Complex<T>::operator++(int) {
	Complex tmp(*this);
	this->_re += 1;
	return tmp;
}
```

###main.cpp
**调用自己写的模板时需要调用.cpp文件通常这个文件被写成.hpp**
这个意思是cpp和h的合称
```
#include <iostream>
#include "complex.h"
#include "complex.cpp"

int main(){
	//......
	return 0;
}
```

##String 类

###MyString.h

```
#pragma once
#include <iostream>

class MyString {
public:
	MyString() :str(NULL), lenth(0){}
	MyString(const char* _str);
	MyString(const MyString& _str);//拷贝构造
	~MyString();//析构函数
	void Print()const;
	friend std::ostream& operator<<(std::ostream& os, const MyString& _str);
	bool operator==(const MyString& _str)const;
	MyString& operator=(const MyString& _str);
	char operator[](int i)const;
	int size()const;
private:
	char* str;
	int lenth;
};
```

###MyString.cpp

```
#include "MyString.h"


MyString::MyString(const char* _str) {
	//如果传过来的是空指针，只将str赋值为\0 lenth为空即可
	if (_str == NULL) {
		str = new char;
		*(str) = '\0';
		lenth = 0;
	}
	//如果传过来一个字符串指针将new其大小的字节数后在将其拷贝过来
	else {
		lenth = strlen(_str);
		str = new char[lenth + 1];
		strcpy(str, _str);
	}
}
MyString::MyString(const MyString& _str) {
	lenth = _str.lenth;
	str = new char[lenth + 1];
	strcpy(str, _str.str);
}


void MyString::Print()const{
	if(str == NULL)
		std::cout << '\0' ;
	else
		std::cout << this->str << std::endl;
}

MyString::~MyString() {
	delete[] str;
	str = NULL;
	lenth = 0;
}

std::ostream& operator<<(std::ostream& os, const MyString& _str) {
	if (_str.str == NULL)
		return os << '\0';
	return os << _str.str;
}
bool MyString::operator==(const MyString& _str)const {
	return !strcmp(_str.str, str);
}
MyString& MyString::operator=(const MyString& _str) {
	if (&_str == this)
		return *this;
	if (lenth > 0) {
		delete[] str;
		lenth = 0;
	}
	str = new char[_str.lenth + 1];
	lenth = _str.lenth;
	strcpy(str, _str.str);
	return *this;
}
char MyString::operator[](int i)const {
	if (i < 0 || i >= lenth)
		return -1;
	else {
		return str[i];
	}
}

int MyString::size()const {
	return this->lenth;
}

```
