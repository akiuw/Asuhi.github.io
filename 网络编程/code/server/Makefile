CC=g++
CFLAGS=-Wall -g
BIN=TCP_com mytcp FileList main 
OBJS1=TCP_com.o main.o mytcp.o FileList.o 
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -std=c++11
ftpsvr:${OBJS1}
	$(CC) $^ -o $@
clean:
	rm -f *.o
