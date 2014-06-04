#ifndef ISOCKETLISTENER_H
#define ISOCKETLISTENER_H

#include <stdio.h>

using namespace std;
class ISocketListener { 
	public:
		/// @brief Notification indicating the file descriptor has an event(s) to process
		/// @param fd[in] the file descriptor of interest
		/// @param revents[in] the returned events to process.  See "man poll".
		virtual int processEvents (int fd, short revents ) = 0;
		
	protected:
		virtual ~ISocketListener() {}
};

#endif //ISOCKETLISTENER_H