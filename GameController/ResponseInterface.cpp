
#include "ResponseInterface.h"
#include "../ServiceLocator.h"

void ResponseInterface::userConnectedResponse(unsigned int id) {
    unsigned int responseLength = sizeof(unsigned int) + 2;
    char bytes[responseLength];
    bytes[responseLength - 1] = 0xff;
    
    if(id > 0) {
        bytes[0] = 1;
        memcpy(bytes + 1, &id, sizeof(unsigned int));
    }
    else {
        bytes[0] = 0;
    }
    
    ServiceLocator::getServiceLocator().sendMessageToClient(bytes, responseLength);
}

void ResponseInterface::userDisconnectedResponse(bool completed) {
    unsigned int responseLength = 2;
    char bytes[responseLength];
    
    bytes[0] = completed ? 1 : 0;
    bytes[1] = 0xff;
    
    ServiceLocator::getServiceLocator().sendMessageToClient(bytes, responseLength);
}

void ResponseInterface::userLocationResponse(const Location& loc) {
    char* bytes;
    unsigned int responseLength;
    
    if(loc.isRoute()) {
        Route route = loc.getRoute();
        responseLength = 3 + route.serializedLength();
        bytes = new char[responseLength];
        
        bytes[0] = 1;
        bytes[1] = 1;
        route.serialize((unsigned char*)bytes + 2);
        bytes[responseLength - 1] = 0xff;
    }
    else if(loc.isPOI()) {
        PointOfInterest poi = loc.getPOI();
        responseLength = 3 + poi.serializedLength();
        bytes = new char[responseLength];
        
        bytes[0] = 1;
        bytes[1] = 0;
        poi.serialize((unsigned char*)bytes + 2);
        bytes[responseLength - 1] = 0xff;
    }
    else {
        bytes = new char[2];
        
        bytes[0] = 0;
        bytes[1] = 0xff;
    }
    
    ServiceLocator::getServiceLocator().sendMessageToClient(bytes, responseLength);
    delete[] bytes;
}
