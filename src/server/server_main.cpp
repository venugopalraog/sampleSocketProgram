#include <stdio.h>
#include <iostream>

#include "../ISocketListener.h"
#include "../EventManager.h"
#include "../ServerHandler.h"
#include "../DebugLogs.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 2) { 
		cout<<"No Port Number Provided - Exit";
		return -1;
	}

	//ServerHandler *server = new ServerHandler(atoi(argv[1]));
	ServerHandler server(atoi(argv[1]));
	
	return 1;
}