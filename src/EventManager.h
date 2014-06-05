#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <stdio.h>
#include <vector>
#include <map>

typedef struct SocketListener {
	int fd;
	ISocketListener *listener;
} Listener;


/// Class to handle file events and notify listeners
/// This class needs to be fully implemented -- it is just a partial skeleton at the moment.
class EventManager {
	//vector<Listener> Registered_Listeners;
	map<int, ISocketListener *> Registered_Listeners;

	int isFdRegistered(int fd, ISocketListener **listener);
	public:
		EventManager();
		~EventManager();
		///
		/// @brief Registers a socket listener
		///
		/// @param fd[in] the file descriptor
		/// @param events[in] the poll events of interest.  See "man poll".
		/// @param listener[in] the listener to call when an event happens.
		///
		bool addListener(int fd, short events, ISocketListener *listener);

		///
		/// @brief Removes a socket listener
		///
		/// @param fd[in] the file descriptor
		/// @param listener[in] the listener to call when an event happens.
		///
		bool removeListener( int fd, ISocketListener *listener );
		
		///
		/// @brief creates and configures Server Socket
		///
		/// @param fd[in] port number
		///
		int	createServerSocket(int port_num);
		/// @brief creates and configures Server Socket
		///
		/// @param fd[in] port number
		///
		void ServerSocketListen(int server_fd);

		/// @brief creates client socket and sends command to Server
		///
		/// @param fd[in] port number
		///
		void createClientSocketHandler(int port_num, char ipAddress[]);
};

#endif //EVENTMANAGER_H