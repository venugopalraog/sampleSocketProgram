/*************************************************/
/* Client example requests file data from server */
/*************************************************/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <iostream>

#include "../ISocketListener.h"
#include "../EventManager.h"
#include "../DebugLogs.h"

using namespace std;

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
#if 0
   int    result = 0, client_fd = -1;

   char   command[256];
   char   buffer[32 * 1024];

   struct sockaddr_in   addr;
   struct hostent      *host_ent;

   //Initialize the socket address
   memset(&addr, 0, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_port   = htons(SERVER_PORT);

   //Add IP address
   addr.sin_addr.s_addr = htonl(INADDR_ANY);

   while(1) {
	cout<<"Enter a Command:\n";
	gets(command);

   /*************************************************/
   /* Create an AF_INET stream socket               */
   /*************************************************/
   client_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (client_fd < 0) {
      perror("socket() failed");
      exit(-1);
   }
   printf("Socket completed.\n");

   /*************************************************/
   /* Connect to the server                         */
   /*************************************************/
   result = connect(client_fd,
                (struct sockaddr *)&addr,
                sizeof(struct sockaddr_in));
   if (result < 0) {
      perror("connect() failed");
      close(client_fd);
      exit(-1);
   }
   printf("Connect completed.\n");

   
   /*************************************************/
   /* Send the request over to the server           */
   /*************************************************/
   result = send(client_fd, command, strlen(command) + 1, 0);
   if (result < 0) {
      perror("send() failed");
      close(client_fd);
      exit(-1);
   }
   printf("Request for %s sent\n", command);

   /*************************************************/
   /* Receive the file from the server              */
   /*************************************************/
   do
   {
      result = recv(client_fd, buffer, sizeof(buffer), 0);
      if (result < 0) {
         perror("recv() failed");
         close(client_fd);
         exit(-1);
      } else if (result == 0) {
         printf("End of file\n");
         break;
      }
      cout<<"\nbytes received"<<result<<"buffer: "<<buffer;

   } while (result > 0);
   
      /*************************************************/
   /* Close the socket                              */
   /*************************************************/
   close(client_fd);
	}
}
#endif