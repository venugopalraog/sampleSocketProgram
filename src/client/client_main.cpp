#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include "../ISocketListener.h"
#include "../EventManager.h"
#include "../DebugLogs.h"

using namespace std;

main (int argc, char *argv[])
{
	if (argc !=3) {
		cout<<"No IP Address and Port Number - Exit";
		exit(1);
	}
	
	EventManager evtMgr;
	evtMgr.createClientSocketHandler(atoi(argv[2]), argv[1]);
	return 1;
}