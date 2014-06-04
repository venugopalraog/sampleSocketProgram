#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include "DebugLogs.h"
#include "ISocketListener.h"
#include "EventManager.h"

#define POLL_TIMEOUT (3 * 60 * 1000)

EventManager::EventManager() { 
	DEBUG("");
}

EventManager::~EventManager() { 
	DEBUG("");
}

/********************************************************************************
 * Function Name : createServerSocket
 * Description : This function creates Socket, sets port number and binds it. 
 * Param : port_numb  [in]  - Port Number required for initializing socket
 * Return : Returns the FD created.
 ********************************************************************************/
int	EventManager::createServerSocket(int port_num) {
	int result = 0, serverFd;
	int optional_value;
	struct sockaddr_in addr;
	
	DEBUG("");

	//Create socket
	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0) {
		DEBUG_ERROR("socket() API failed");
		exit(-1);
	}
	//Set socket options
	result = setsockopt(serverFd, SOL_SOCKET,  SO_REUSEADDR,
					(char *)&optional_value, sizeof(optional_value));
	if (result < 0) {
		DEBUG_ERROR("setsockopt() API failed");
		close(serverFd);
		exit(-1);
	}

	result = ioctl(serverFd, FIONBIO, (char *)&optional_value);
	if (result < 0) {
		DEBUG_ERROR("ioctl() API failed");
		close(serverFd);
		exit(-1);
	}

	//Bind the socket to port and address
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port        = htons(port_num);

	result = bind(serverFd, (struct sockaddr *)&addr, sizeof(addr));
	if (result < 0) {
		DEBUG_ERROR("bind() API failed");
		close(serverFd);
		exit(-1);
	}
	return serverFd;
}

/********************************************************************************
 * Function Name : ServerSocketListen
 * Description : This function does poll and listens to clients incoming 
				connection Request to process data.
 * Param : server_fd  [in]  - Socket FD created.
 * Return : No return Value
 ********************************************************************************/
void EventManager::ServerSocketListen(int server_fd) {
	struct pollfd poll_fds[200];
	int result = 0, new_sd = -1, no_of_fds = 1, current_size, conn_close, len;
	char buffer[80];

	DEBUG("");

	//Set the listen back log
	result = listen(server_fd, 32);
	if (result < 0) {
		DEBUG_ERROR("listen() API failed");
		close(server_fd);
		exit(-1);
	}

	memset(poll_fds, 0 , sizeof(poll_fds));	
	poll_fds[0].fd = server_fd;
	poll_fds[0].events = POLLIN;

	while (1) {
		/* Call poll() and wait 3 minutes for it to complete. */
		DEBUG("Waiting on poll()");
		result = poll(poll_fds, no_of_fds, POLL_TIMEOUT);
		
		if (result < 0) {
			DEBUG_ERROR("poll() failed");
			break;
		}
		if (result == 0) { 
			DEBUG_ERROR("poll() Timeout");
			continue;
		}

		DEBUG("Data received Read the data: %d", no_of_fds);
		current_size = no_of_fds;

		//Run through the fds and find the descriptors which returned 
		//POLLIN
		for (int i = 0; i < current_size; i++) {
			if(poll_fds[i].revents == 0)  continue;

			if(poll_fds[i].revents != POLLIN) {
				DEBUG_ERROR("revents = %d", poll_fds[i].revents);
				break;
			}
			if (poll_fds[i].fd == server_fd) {
				DEBUG("This fd - %d status is POLLIN Accept it", i);
				do {
					new_sd = accept(server_fd, NULL, NULL);
					if (new_sd < 0) {
						DEBUG_ERROR("accept() API failed");
						break;
					}
					//New incoming connection add to pollfd.
					DEBUG("New incoming connection - %d", new_sd);
					poll_fds[no_of_fds].fd = new_sd;
					poll_fds[no_of_fds].events = POLLIN;
					no_of_fds++;
				} while (new_sd != -1);
			}else {
				DEBUG("Call procesEvents() to receive and send the result");
				conn_close = FALSE;
				conn_close = listener->processEvents(poll_fds[i].fd, POLLIN);
				#if 0
				for(int i = 0; i < Registered_Listeners.size(); i++) {
					if (Registered_Listeners[i].fd == poll_fds[i].fd) { 
						cout<<"Listener fd already exists Return";
						Registered_Listeners[i].listener->processEvents(poll_fds[i].fd, POLLIN); 
					}
				}
				#endif
				if (conn_close) {
					DEBUG("close the fd here");
					close(poll_fds[i].fd);
					poll_fds[i].fd = -1;
				}
			}
		}
	}
	//Cleanup the fds structure.
	for (int i = 0; i < no_of_fds; i++) {
		if(poll_fds[i].fd >= 0)
			close(poll_fds[i].fd);
	}
}

/********************************************************************************
 * Function Name : createClientSocketHandler
 * Description : This function creates client socket and sends the command entered
				 by user to Server.
 * Param : port_num  [in]  - Port number.
		   ipAddress [in]  - Ip Address
 * Return : No return Value
 ********************************************************************************/
void EventManager::createClientSocketHandler(int port_num, char ipAddress[]) {
	int    result = 0, client_fd = -1;
	char   command[256];
	char   buffer[32 * 1024];
	struct sockaddr_in   addr;
	struct hostent      *host_ent;

	//Initialize the socket address
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(port_num);

	//Add IP address
	addr.sin_addr.s_addr = inet_addr(ipAddress);

	while(1) {
		cout<<"\n**** Enter a Command: ****\n";
		gets(command);
		//Create socket
		client_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (client_fd < 0) {
			DEBUG_ERROR("socket() API failed");
			exit(-1);
		}

		//connect to Server
		result = connect(client_fd, (struct sockaddr *)&addr,
						 sizeof(struct sockaddr_in));
		if (result < 0) {
			DEBUG_ERROR("connect() failed");
			close(client_fd);
			exit(-1);
		}

		//Send request to Server
		result = send(client_fd, command, strlen(command) + 1, 0);
		if (result < 0) {
			DEBUG_ERROR("send() API failed");
			close(client_fd);
			exit(-1);
		}
		DEBUG("Request for %s sent", command);

		/*************************************************/
		/* Receive the file from the server              */
		/*************************************************/
		do {
			result = recv(client_fd, buffer, sizeof(buffer), 0);
			if (result < 0) {
				DEBUG_ERROR("recv() API failed");
				close(client_fd);
				exit(-1);
			} else if (result == 0) {
				DEBUG("Data receive Ended");
				break;
			}
			cout<<"\n**** Reply: "<<buffer<<" ****";
		} while (result > 0);
		//close the socket
		close(client_fd);
	}
}

/********************************************************************************
 * Function Name : addListener
 * Description : Registers a socket listener
 * Param : fd[in] the file descriptor
		   events[in] the poll events of interest.  See "man poll".
		   listener[in] the listener to call when an event happens.
 * Return : result
 ********************************************************************************/
bool EventManager::addListener( int fd, short events, ISocketListener *socketListener) {
	DEBUG("");
	listener = socketListener;
#if 0
	int len = Registered_Listeners.size();
	Listener temp;
	for(int i = 0; i < len; i++) {
		if (Registered_Listeners[i].fd == fd) { 
			cout<<"Listener fd already exists Return";
			return FALSE;
		}
	}

	Registered_Listeners[len].fd = fd;
	Registered_Listeners[len].listener = listener;
#endif
	return TRUE;
}

bool EventManager::removeListener( int fd, ISocketListener *listener ) { 
	listener = NULL;
	DEBUG("");
#if 0
	int len = Registered_Listeners.size();
	for(int i = 0; i < len; i++) {
		if (Registered_Listeners[i].fd == fd) { 
			cout<<"Lister with fd is removed";
			Registered_Listeners.erase(i);
			return TRUE;
		}
	}
#endif
	return TRUE;
}