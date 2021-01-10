#include <cstdio>
#include "mytcp.h"

int main()
{
	MyFtp myftp("127.0.0.1", 5188);

	while (true) {
		myftp.wait_connect();
		myftp.for_each_conn();
	}

    return 0;
}