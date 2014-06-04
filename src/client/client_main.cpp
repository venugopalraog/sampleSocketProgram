#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../ISocketListener.h"
#include "../EventManager.h"
#include "../DebugLogs.h"

main (int argc, char *argv[])
{
	if (argc !=3) {
		DEBUG("No input Port Number and IP Address");
		exit(1);
	}
	
	EventManager evtMgr;
	evtMgr.createClientSocketHandler(atoi(argv[2]), argv[1]);
	return 1;
}