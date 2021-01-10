#include "mytcp.h"

int MyFtp::do_svr(int conn) {
	char recvbuf[1024] = { 0 }; // 接收缓存1kb

	readline(conn, recvbuf, 1024);
	//处理指令套接字
	if (!strncmp(recvbuf, "socket0", 7)) {
		readline(conn, recvbuf, 1024);
		if (!strncmp(recvbuf, "LS", 2)) {
			filelist.GetDirInfo();
			SendDirInfo(conn);
		}
		if (!strncmp(recvbuf, "DEL", 3)) {
			readline(conn, recvbuf, 1024);
		}
		if (!strncmp(recvbuf, "CLOSE", 5)) {
			return 0;
		}
	}
	//发送文件套接字
	if (!strncmp(recvbuf, "socket1", 7)) {
		usleep(10000);
		readline(conn, recvbuf, 1024);
		if(!strncmp(recvbuf, "GET", 3)){
			readline(conn, recvbuf, 1024);
			recvbuf[strlen(recvbuf) - 1] = 0; //由于readline最后一项是'\n'所一要吧这个'\n'去掉才能作为路径
			SendFile(recvbuf, conn);
		}

		if (!strncmp(recvbuf, "LOAD", 4)) {
			usleep(1000);
			readline(conn, recvbuf, 1024);
			recvbuf[strlen(recvbuf) - 1] = 0;
			int filesize = 0;
			int recvsize = 0;
			read(conn, &filesize, sizeof(int));

			int fd = open(recvbuf, O_RDWR | O_CREAT, 0666);
			while (filesize != 0) {//文件大小是0就不发送
				if (recvsize >= filesize) {
					printf("download file over\n");
					break;
				}
				usleep(100);
				int readsize = filesize - recvsize;
				if (readsize >= 1024)
					readsize = 1024;
				memset(recvbuf, 0, sizeof(recvbuf));
				int ret = read(conn, recvbuf, readsize);
				recvsize += ret;
				if (0 == ret)
					break;
				else if (ret < 0) {
					perror("read file error");
					exit(EXIT_FAILURE);
				}
				ret = write(fd, recvbuf, sizeof(recvbuf));
			}
			close(fd);
		}
	}
	return true;
}


void MyFtp::SendFile(const char* path,int sock) {
	int size = 0;
	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		size = -1;
		write(sock, &size, sizeof(int));
		return;
	}
	else {
		struct stat fdstat;
		stat(path, &fdstat);
		size = fdstat.st_size;
		write(sock, &size, sizeof(int));
	}
	char buffer[1024] = { 0 };
	while (size != 0) {//文件大小是0就不发送
		usleep(100);
		memset(buffer, 0, sizeof(buffer));
		int ret = read(fd, buffer, sizeof(buffer));
		if (0 == ret)
			break;
		else if (ret < 0){
			perror("read file error");
			exit(EXIT_FAILURE);
		}
		ret = write(sock, buffer, sizeof(buffer));
	}
}

void MyFtp::SendDirInfo(int conn) {
	std::string sendbuf;
	for (auto iter = filelist.dirlist.begin(); iter != filelist.dirlist.end(); iter++) {
		sendbuf += std::string("key#\n");
		writen(conn, sendbuf.c_str(), sendbuf.size());
		sendbuf.clear();
		sendbuf += std::string(iter->first);
		sendbuf += std::string("\n");
		writen(conn, sendbuf.c_str(), sendbuf.size());
		sendbuf.clear();
		for (auto lit = iter->second.begin(); lit != iter->second.end(); lit++) {
			sendbuf = std::string(*lit);
			sendbuf += std::string("\n");
			writen(conn, sendbuf.c_str(), sendbuf.size());
			sendbuf.clear();
		}
		sendbuf = std::string("end\n");
		writen(conn, sendbuf.c_str(), sendbuf.size());
		sendbuf.clear();
	}
	sendbuf = std::string("over\n");
	writen(conn, sendbuf.c_str(), sendbuf.size());
	sendbuf.clear();
}

void MyFtp::RecvFile(const char* path, int sock) {

}
