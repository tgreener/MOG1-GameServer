
#include "Server.h"
#include "ServiceLocator.h"

Server::Server(const char* filename) : filename(filename), isServicingConnections(true) {
}

void Server::init() {
    struct sockaddr_un serverSocketAddress;
    serverSocketHandle = socket(PF_LOCAL, SOCK_STREAM, 0);
    
    if(serverSocketHandle < 0) {
        printf("Could not create socket.\n");
    }
    
    if(setAddress(&serverSocketAddress) < 0) {
        printf("Could not set address.\n");
    }
    
    sockaddr* ssaPointer = (sockaddr*)&serverSocketAddress;
    socklen_t ssaLength = (strlen(serverSocketAddress.sun_path) + 1 )+
                          sizeof(serverSocketAddress.sun_family);
    
    if(::bind(serverSocketHandle, ssaPointer, ssaLength) < 0) {
        printf("Could not bind socket.\n");
    }
    
    if(listen(serverSocketHandle, QUEUE_SIZE) < 0) {
        printf("Could not force the socket to listen.\n");
    }
    
    sl.setAsServiceLocator();
    sl.setServer(this);
    
    interpreterVector.push_back(&sysDel);
    interpreterVector.push_back(&gameDel);
    interpreterVector.push_back(&dataDel);
    
    threadPool.setSize(NUM_THREADS);
    
    threadPool.setSetupFunction([this] (void) -> void {
        ((ServiceLocator&)(ServiceLocator::getServiceLocator())).setServer(this);
    });
    
    threadPool.setRunFunction([this] (int clientHandle) -> void {
        char buffer[256];
        int byteCount = readMessageFromClient(clientHandle, buffer, 256);
        
        interpretByteMessage(buffer, byteCount);
        
        if(close(clientHandle) < 0) {
            printf("Error closing client socket.\n");
        }
    });
    
    threadPool.setTeardownFunction([] (void) -> void {});
    
    threadPool.run();
}

void Server::serviceSocket() {
    struct sockaddr_un clientSocketAddress;
    
    while(isServicingConnections) {
        socklen_t csaLength = sizeof(clientSocketAddress);
        int clientSocketHandle = accept(serverSocketHandle, (sockaddr*)&clientSocketAddress, &csaLength);
        threadPool.acceptClient(clientSocketHandle);
    }
}

void Server::interpretByteMessage(char* bytes, int length) {
    if(length <= 0) return;
    
    if(bytes[0] >= 1 && interpreterVector.size()) {
        unsigned int i = bytes[0] - 1;
        interpreterVector[i]->interpretCommand(bytes, length);
    }
    else sl.sendMessageToClient("Unrecognized command.\n");
}

void Server::stopServer() {
    isServicingConnections = false;
}

void Server::closeServer() {
    threadPool.flush();
    close(serverSocketHandle);
    unlink(filename);
}

int Server::setAddress(struct sockaddr_un *address) {
    unlink(filename);
    
    memset(address, 0, sizeof(*address));
    
    address->sun_family = PF_LOCAL;
    strcpy(address->sun_path, filename);
    
    return 1;
}

int Server::readMessageFromClient(int clientSocketHandle, char* buffer, int bufferSize) {
    memset(buffer, 0, 256);
    int recvResult = 0;
    
    recvResult = recv(clientSocketHandle, buffer, bufferSize, 0);
    return recvResult;
}
