
#include "ResponseInterface.h"
#include "../ServiceLocator.h"

void ResponseInterface::userConnectedResponse(User& connectedUser) {
    unsigned int responseLength = 2 + (sizeof(unsigned int) * 2);
    char bytes[2 + sizeof(unsigned int)];
    bytes[responseLength - 1] = 0xff;
    
    if(connectedUser.getID() > 0) {
        unsigned int id = connectedUser.getID();
        Location loc = connectedUser.getLocation();
        unsigned int locID = loc.getID();
        
        bytes[0] = 1;
        memcpy(bytes + 1, &id, sizeof(unsigned int));
        memcpy(bytes + (1 + sizeof(unsigned int)), &locID, sizeof(unsigned int));
    }
    else {
        bytes[0] = 0;
    }
    
    ServiceLocator::getServiceLocator().sendMessageToClient(bytes, responseLength);
}