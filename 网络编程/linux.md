#Linux网络编程
##前言
这个版块代码全部使用Visual Studio for Linux来完成
阿里云学生机只有1M带宽 请耐心等待。。。
笔记请用Notepad++打开
这里的dayX并不是意味着第X天的内容全部在这
使用dayX只是用来表示一个阶段分组。并没有在时间上有明确的安排
本模块正在更新中...已到day12
##day01--day03
###学习内容
[**传送门**](day01-day03.md)
1. 学会最简单的套接字创建
2. TCP客户端/服务器链接以及发送数据
3. 初步优化服务器，使之能一对多进行数据处理
4. 学会解决粘包问题
##day04--day06
[**传送门**](day04-day06.md)
1. 防止服务器程序出现僵尸进程利用`SIGCHLD`信号来解决
2. 了解TCP的11种状态了解`CLOSING`状态的产生以及`TIME_WAIT`状态的产生
3. 学习5种IO模型，重点关注`IO复用`
4. 掌握`select`函数的用法管理IO
5. 完善回射客户端程序，对标准输入IO和网络传输IO进行管理
##day07--day09
[**传送门**](day07-day09.md)
1. 使用`select`改进回射服务器程序
2. `close`与`shutdown`的区别
3. 引用计数的概念
4. 使用`shutdown`函数解决服务器因`SIGPIPE`信号崩溃的bug
5. 使用`select`完成超时设置函数
	`read_timeout` `write_timeout` `accept_timeout` `connect_timeout`
##day10--day12
[**传送门**](day10-day12.md)
1. `select`的限制使用`poll`改善服务器程序
2. `select`与`poll`的异同点
3. 理解`select`与`poll`的效率低下问题
4. 使用`epoll`挺好效率
5. 初步了解实现UDP回射服务器/客户端程序
6. 了解UDP协议产生的问题
##day13--day14
[**传送门**](day13-day14.md)

##ftp小程序
[**传送门**](ftp.md)
使用TCP实现稳定的文件下载客户/服务器程序

##网络编程常见问题整理
[**传送门**](extra.md)

