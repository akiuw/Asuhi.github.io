#include "head.h"
#include "tcpconn.h"
#include "command.h"
using namespace std;
int main(int argc, char** argv) {
	TcpFtp ftpcli;
	
	if (ftpcli.ConnToSer("127.0.0.1", 5188)) {
		ftpcli.GetDirInfo(ftpcli.sock[0]);
	}
	else {
		perror("¡¨Ω” ß∞‹ ConnToSer");
		return 0;
	}
	Command commd(ftpcli.filelist);
	cout << "LS\t:show file list" << endl;
	cout << "CHDIR\t:change working dirfile" << endl;
	cout << "GETFILE\t:download a file" << endl;
	while (true) {
		
		printf("\nplease input your command:");
		string comd;
		cin >> comd;
		commd.waitCommand(comd, ftpcli.sock[0],ftpcli.sock[1]);
	}
	return 0;
}
