#include "command.h"

void Command::waitCommand(std::string command,int sock1,int sock2) {
	printf("\n");
	char sendbuf[1024] = { 0 };
	printf("Current working directory:%s\n", path.c_str());
	if (command == std::string("LS")) {
		auto mapiter = filelist.find(path + std::string("\n"));
		std::list<std::string> flist = mapiter->second;
		for (auto it = flist.begin(); it != flist.end(); it++) {
			std::cout << *it;
		}
	}

	
	if (command == std::string("GETFILE")) {
		std::cout << "please chouse a file:" << std::endl;
		std::string filename;
		std::cin >> filename;
		char sendbuf[1024] = { 0 };
		char recvbuf[1024] = { 0 };

		sprintf(sendbuf, "socket1\nGET\n");
		writen(sock2, sendbuf, strlen(sendbuf));
		memset(sendbuf, 0, sizeof(sendbuf));
		sprintf(sendbuf, "%s\n", (path + filename).c_str());
		writen(sock2, sendbuf, strlen(sendbuf));

		int filesize = 0;
		int recvsize = 0;
		read(sock2, &filesize, sizeof(int));
		if (filesize <= 0) {
			printf("this file is Non-existent!!\n");
			return;
		}
		int fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
		int ret = 0;

		while (filesize != 0) {
			if (recvsize >= filesize) {
				printf("download file over\n");
				break;
			}
			usleep(100);
			int readsize = filesize - recvsize;
			if (readsize >= 1024)
				readsize = 1024;
			memset(recvbuf, 0, sizeof(recvbuf));
			ret = read(sock2, recvbuf, readsize);
			recvsize += ret;
			if (ret <= 0)
				break;
			ret = write(fd, recvbuf, sizeof(recvbuf));
		}
	}
	if (command == std::string("CHDIR")) {
		std::string dirname;
		std::cout << "please chouse a dirfile:" << std::endl;
		std::cin >> dirname;

		if (dirname == std::string("..")) {
			pathlist.pop();
			path = pathlist.top();
		}
		else
			path += dirname + std::string("/");
		pathlist.push(path);
		auto mapiter = filelist.find(path + std::string("\n"));
		std::list<std::string> flist = mapiter->second;
		for (auto it = flist.begin(); it != flist.end(); it++) {
			std::cout << *it;
		}
	}
	if (command == std::string("UPLOAD")) {
		std::string lopath;
		std::cout << "please input a local file path:" << std::endl;
		std::cin >> lopath;
		sprintf(sendbuf, "socket1\nLOAD\n");
		writen(sock2, sendbuf, strlen(sendbuf));
		memset(sendbuf, 0, sizeof(sendbuf));
		
		auto it1 = lopath.end();
		auto it2 = lopath.end();
		while (*it1 != '/')it1--;// it1移动到最后一个/
		it1++;
		std::string filename(it1,it2);//得到文件名

		sprintf(sendbuf, "%s\n", (path + filename).c_str());
		writen(sock2, sendbuf, strlen(sendbuf));
		memset(sendbuf, 0, sizeof(sendbuf));
		
		SendFile(lopath.c_str(), sock2);
	}
	if (command == std::string("CLOSE")) {
		sprintf(sendbuf, "socket0\nCLOSE\n");
		writen(sock2, sendbuf, strlen(sendbuf));
		memset(sendbuf, 0, sizeof(sendbuf));
		exit(0);
	}

}
void Command::SendFile(const char* path, int sock) {

	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		printf("this file is Non-existent!!\n");
		return;
	}
	struct stat fdstat;
	stat(path, &fdstat);
	int size = fdstat.st_size;
	write(sock, &size, sizeof(int));
	char buffer[1024] = { 0 };
	while (size != 0) {//文件大小是0就不发送
		memset(buffer, 0, sizeof(buffer));
		int ret = read(fd, buffer, sizeof(buffer));
		if (0 == ret)
			break;
		else if (ret < 0) {
			perror("read file error");
			exit(EXIT_FAILURE);
		}
		ret = write(sock, buffer, sizeof(buffer));
	}
	close(fd);
}