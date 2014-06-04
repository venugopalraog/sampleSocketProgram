#ifndef DEBUGLOGS_H
#define DEBUGLOGS_H

#include <stdio.h>

//#define ENABLE_DEBUG

#define TRUE 	1
#define FALSE 	0 

#ifdef ENABLE_DEBUG
	#define DEBUG(a, args...) printf("DEBUG: %s:%d:"#a"\n", __FUNCTION__,__LINE__, ##args);
	#define DEBUG_ERROR(a, args...) printf("ERROR: %s:%d:"#a"\n", __FUNCTION__,__LINE__, ##args);
#else
	#define DEBUG(a, args...)
	#define DEBUG_ERROR(a, args...)
#endif

#endif //DEBUGLOGS_H