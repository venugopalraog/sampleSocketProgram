#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <string.h>

class ServerHandler : public ISocketListener{ 
	EventManager *mEventManager;
	int mServerFd;
	public: 
		ServerHandler(int port_num);
		~ServerHandler();
		int processEvents(int fd, short revents);
//		int processDataReceived(char cmd[], int len, char reply[]);
};

#endif //SERVERHANDLER_H