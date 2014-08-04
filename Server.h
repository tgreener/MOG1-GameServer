/* 
 * File:   Server.h
 * Author: toddgreener
 *
 * Created on May 26, 2014, 1:34 PM
 */

#ifndef SERVER_H
#define	SERVER_H

#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdbool.h>

#include "StringUtil.h"
#include "SystemCommandDelegate.h"
#include "DataRequestDelegate.h"
#include "GameCommandDelegate.h"
#include "ServiceLocator.h"
#include "sys/ThreadPool.h"

using namespace std;

#define QUEUE_SIZE 10
#define NUM_THREADS 10

typedef vector<ByteCodeDelegate*> InterpreterVector;

class Server {
private:
    int serverSocketHandle;
    
    const char* filename;
    bool isServicingConnections;
    
    int setAddress(struct sockaddr_un *address);
    int readMessageFromClient(int clientSocketHandle, char* buffer, int bufferSize);
    void interpretByteMessage(char* bytes, int length);
    
    SystemCommandDelegate sysDel;
    GameCommandDelegate gameDel;
    DataRequestDelegate dataDel;
    InterpreterVector interpreterVector;
    
    ServiceLocator sl;
    ThreadPool threadPool;
    
public:
    Server(const char* filename);
    
    void init();
    void serviceSocket();
    
    void stopServer();
    void closeServer();
};

#endif	/* SERVER_H */

