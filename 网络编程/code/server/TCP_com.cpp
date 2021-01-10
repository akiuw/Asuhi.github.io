#include "head.h"
#include "TCP_com.h"

TCP_base::TCP_base(const char* address, short port) {
	listenfd = -1;
	struct sockaddr_in servaddr; //结构体填充
	if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket()\n");
		return;
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; //IPV4
	servaddr.sin_port = static_cast<in_port_t>(htons(port));//端口号 使用htons()函数转化成网络字节序
	servaddr.sin_addr.s_addr = inet_addr(address);
	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {//sockaddr_in的变量强制类型转化成sockaddr类型
		perror("bind()\n");
		return;
	}
	if (listen(listenfd, SOMAXCONN) < 0)//SOMAXCONN宏表示队列的最大值
	{
		perror("listen()\n");
		return;
	}
}

bool TCP_base::check() {
	if (listenfd < 0)
		return false;
	else return true;
}

void TCP_com::wait_connect() {
	nready = epoll_wait(epollfd, &*events.begin(), static_cast<int>(events.size()), -1);
	//nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
	if (nready == -1) {
		if (errno == EINTR) //被信号中断
			return;
		perror("epoll_wait");
		exit(EXIT_FAILURE);
	}
	if (nready == 0)
		return;
	if (events.size() == (size_t)nready) {//如果nready与size()大小相等就将容器的大小扩大2倍
		events.resize(events.size() * 2);
	}
}

//将IO设置成非阻塞
void TCP_com::activate_nonblock(int fd) {
	int ret;
	int flags = fcntl(fd, F_GETFL);//得到标识符
	if (flags == -1) {
		perror("fcntl");
	}
	flags |= O_NONBLOCK; //将flags与O_NONBLOCK相或如果要设置成阻塞模式只需要将flags &= ~O_NOMBLOCK 即可函数名为deactivate_nonblock(int fd)
	ret = fcntl(fd, F_SETFL, flags);
	if (ret == -1) {
		perror("fcntl");
	}
}

ssize_t TCP_com::readn(int fd, void* buf, size_t count) {
	size_t nleft = count;//剩余的字节数nleft
	ssize_t nread = 0;		 //已经接收的字节数
	char *bufp = (char*)buf;
	while (nleft > 0) {
		if ((nread = read(fd, bufp, nleft)) < 0) {
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (nread == 0) {
			return count - nleft;
		}
		bufp += nread;
		nleft -= nread;
	}
	return count;
}

ssize_t TCP_com::writen(int fd, const void* buf, size_t count) {
	size_t nleft = count;	//剩余的字节数nleft
	ssize_t nwritten;
	char *bufp = (char*)buf;
	while (nleft > 0) {
		if ((nwritten = write(fd, bufp, nleft)) < 0) {
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (nwritten == 0) {
			continue;
		}
		bufp += nwritten;
		nleft -= nwritten;
	}
	return count;
}

ssize_t TCP_com::recv_peek(int sockfd, void *buf, size_t len) {

	while (true) {
		ssize_t ret = recv(sockfd, buf, len, MSG_PEEK);

		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

ssize_t TCP_com::readline(int sockfd, void *buf, size_t maxline) 
{
	int ret;
	int nread;
	char * bufp = static_cast<char*>(buf);
	int nleft = maxline;
	char tmp[1024] = { 0 };
	while (1)
	{
		ret = recv_peek(sockfd, buf, nleft);
		if (ret < 0)
			return ret;
		else if (ret == 0)
			return ret;
		nread = ret;
		int i;
		for (i = 0; i < nread; i++)
		{
			if (bufp[i] == '\n')
			{
				memset(bufp, 0, 1024);
				ret = readn(sockfd, bufp, i + 1);
				if (ret != i + 1)
				{
					exit(EXIT_FAILURE);
				}
				return ret;
			}
		}
		if (nread > nleft)
		{
			exit(EXIT_FAILURE);
		}
		nleft -= nread;
		ret = readn(sockfd, bufp, nread);
		if (ret != nread)
		{
			exit(EXIT_FAILURE);
		}
		bufp += nread;
	}
	return -1;
}

void TCP_com::for_each_conn() {
	struct epoll_event event;
	event.data.fd = listenfd;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

	struct sockaddr_in peeraddr;

	for (int i = 0; i < nready; i++) {
		if (events[i].data.fd == listenfd) {
			socklen_t peerlen = sizeof(peeraddr);
			conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen);
			if (conn == -1) {
				perror("accept");
				exit(EXIT_FAILURE);
			}
			printf("ip: %s port:%d \n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

			clients.push_back(conn);
			activate_nonblock(conn);

			event.data.fd = conn;
			event.events = EPOLLIN | EPOLLET;
			epoll_ctl(epollfd, EPOLL_CTL_ADD, conn, &event);
		}
		else if (events.at(i).events & EPOLLIN) {//如果不是listen套接字有内容输入就处理这个套接字
			conn = events.at(i).data.fd;//将这个套接字赋值给conn
			if (conn < 0)
				continue;
			int ret = do_svr(conn);
			if (ret == 0) {
				printf("client close\n");
				close(conn);
				event = events[i];
				epoll_ctl(epollfd, EPOLL_CTL_DEL, conn, &event);//将这个fd删除
				clients.erase(remove(clients.begin(), clients.end(), conn), clients.end());//将这个套接字从列表中删除
			}
		}
	}
}
