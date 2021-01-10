CC=g++
CFLAGS=-Wall -g
BIN=command tcpconn main 
OBJS1=tcpconn.o command.o main.o  
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -std=c++11
ftpcli:${OBJS1}
	$(CC) $^ -o $@
clean:
	rm -f *.o
