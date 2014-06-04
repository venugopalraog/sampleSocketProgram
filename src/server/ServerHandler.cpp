//Header files
#include <iostream>

#include "../ISocketListener.h"
#include "../EventManager.h"
#include "../ServerHandler.h"
#include "../DebugLogs.h"

//Local Methods
/********************************************************************************
 * Function Name : convertStringToInteger
 * Description : This function will convert the String to Number
 * Param : str    [in]  - String which should be converted to Number
		   number [out] - Convert integer number
	   FilePath - Program path.
 * Return : No Return values.
 ********************************************************************************/
static int convertStringToInteger(char str[], int *number) { 
	char *temp = '\0';

	*number = (int) strtol(str, &temp, 0);	
	if(*temp != '\0' && *temp != ' ') {
		DEBUG_ERROR("Invalid String cannot convert to Integer");
		return FALSE;
	}
	return TRUE;
}

/********************************************************************************
 * Function Name : processDataReceived
 * Description : This function process the command received from client 
				 and replies.
 * Param : cmd   [in] - command received from client
		   reply [out] - Response to the command
 * Return : No Return
 ********************************************************************************/
static void processDataReceived(char cmd[], char reply[]) {
	int num;
	strlwr(cmd);
	if (!strcmp(cmd, "hello")) {
		strcpy(reply, "WELCOME");
	} else if(!strcmp(cmd, "bye")) {
		strcpy(reply, "BYE BYE");
	} else if(convertStringToInteger(cmd, &num)) {
		num *= num;
		sprintf(reply, "%d", num);
	}
	cout<<"\n**** Command Received: "<<cmd<<" ****\n";
	cout<<"\n**** Reply: "<<reply<<" ****\n";
}

//Class Methods
//Constructor
ServerHandler::ServerHandler(int port_num) {
	DEBUG("ServerHandler - port number: %d", port_num);
	mEventManager = new EventManager();
	mServerFd = mEventManager->createServerSocket(port_num);
	mEventManager->addListener(mServerFd, POLLIN, (ISocketListener *)this);
	mEventManager->ServerSocketListen(mServerFd);
}

//Destructor
ServerHandler::~ServerHandler() {
	DEBUG("");
	mEventManager->removeListener(mServerFd, (ISocketListener *)this);
	delete mEventManager;
}
/********************************************************************************
 * Function Name : processEvents
 * Description : Inherited method from ISocketListener which process the received
				 Command from client and replies.
 * Param : fd      [in] - file Descriptor on which the data is recieved
		   revents [in] - Type of Events (POLLIN or POLLOUT)
 * Return : Returns status of connection with Client.
 ********************************************************************************/
int ServerHandler::processEvents( int fd, short revents ) {
	int result = 0;
	char buffer[80];
	char reply[80] = "Invalid command";
	
	DEBUG("");

	do {
		//Receive data on the existing connection still recv fails.
		result = recv(fd, buffer, sizeof(buffer), 0);
		if (result < 0) {
			DEBUG_ERROR("Receive Data Failed");
			break;
		}
		if (result == 0) {
			DEBUG_ERROR(" Connection closed");
			break;
		}
		processDataReceived(buffer, reply);
		//Send Reply to Client Socket
		result = send(fd, reply, sizeof(reply), 0);
		if (result < 0) {
			DEBUG_ERROR(Send Data Failed);
			break;
		}
	} while(1);
	return TRUE;
}