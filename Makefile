SRC=src/
INCLUDE=src/
SOURCE_SERVER=$(SRC)server/server_main.cpp  $(SRC)EventManager.cpp  $(SRC)server/ServerHandler.cpp 

CLIENT_SERVER=$(SRC)client/client_main.cpp $(SRC)EventManager.cpp
			  
SERVER_BINARY=server_main
CLIENT_BINARY=client_main

SERVER_INCLUDES= $(INCLUDE)EventManager.h $(INCLUDE)ISocketListener.h $(INCLUDE)DebugLogs.h
CLIENT_INCLUDES= $(INCLUDE)EventManager.h $(INCLUDE)DebugLogs.h

MYLIBRARIES=
CC=g++

all: $(SERVER_BINARY) $(CLIENT_BINARY)

$(SERVER_BINARY): $(SOURCE_SERVER)

	$(CC) -I$(SERVER_INCLUDES) $(SOURCE_SERVER) -o $(SERVER_BINARY)
	
$(CLIENT_BINARY): $(CLIENT_SERVER)

	$(CC) -I$(CLIENT_INCLUDES) $(CLIENT_SERVER) -o $(CLIENT_BINARY)

clean:
	rm -f $(SERVER_BINARY) $(CLIENT_BINARY)