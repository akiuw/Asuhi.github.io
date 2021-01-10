#pragma once
#include "head.h"
using namespace std;

class TCP_base {
public:
	TCP_base() :listenfd(-1) {}
	TCP_base(const char* address, short port);
	bool check();
protected:
	int listenfd;
};

class TCP_com :public  TCP_base {

public:
	TCP_com() :TCP_base(), conn(-1), events(16) {}
	TCP_com(const char* address, short port) :TCP_base(address, port), conn(-1), events(16) {
		epollfd = epoll_create1(EPOLL_CLOEXEC);
		struct epoll_event event;
		event.data.fd = listenfd;
		event.events = EPOLLIN | EPOLLET;
		epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);
	}
	void wait_connect();
	ssize_t readline(int sockfd, void *buf, size_t maxline);
	virtual int do_svr(int conn) { return 0; }//检测到conn有数据传输过来就进行服务
	void for_each_conn();
	ssize_t writen(int fd, const void* buf, size_t count);
	ssize_t readn(int fd, void* buf, size_t count);
private:
	void activate_nonblock(int fd);
	ssize_t recv_peek(int sockfd, void *buf, size_t len);

private:
	typedef vector<struct epoll_event> EventList;
	int conn;
	int epollfd;
	int nready;
	EventList events;
	vector<int> clients;//改用poll函数不受最大数量影响
};
