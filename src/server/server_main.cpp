#include <stdio.h>

#include "../ISocketListener.h"
#include "../EventManager.h"
#include "../ServerHandler.h"
#include "../DebugLogs.h"

int main(int argc, char *argv[])
{
	if (argc != 2) { 
		DEBUG("No Port Number Provided: Exit");
		return -1;
	}

	//ServerHandler *server = new ServerHandler(atoi(argv[1]));
	ServerHandler server(atoi(argv[1]));
	
	return 1;
}