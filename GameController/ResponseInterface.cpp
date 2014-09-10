
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

void ResponseInterface::boolResponse(bool completed) {
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

void ResponseInterface::modelArrayResponse(const AbstractModel** models, unsigned int count) {
    try {
        unsigned int bufferHeaderLength = sizeof(unsigned int) + 2;
        unsigned int bufferLength = bufferHeaderLength + 1; //Bytes for response success code, final byte, and data length.
        for (int i = 0; i < count; i++) {
            bufferLength += models[i]->serializedLength();
        }

        unsigned char buffer[bufferLength];

        buffer[0] = 0x01;
        buffer[1] = sizeof(unsigned int);
        buffer[bufferLength - 1] = 0xff;
        memcpy(buffer + 2, &bufferLength, sizeof(unsigned int));

        unsigned int bufferWriteHead = bufferHeaderLength;
        for(int i = 0; i < count; i++) {
            models[i]->serialize(buffer + bufferWriteHead);
            bufferWriteHead += models[i]->serializedLength();
        }

        ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, bufferLength);
    }
    catch (const char* e) {
        ServiceLocator::getServiceLocator().sendMessageToClient(e);
    }
}

void ResponseInterface::poiArrayResponse(const PointOfInterest* pois, unsigned int count) {
    const AbstractModel** poiPointers = new const AbstractModel*[count];
    
    for(int i = 0; i < count; i++) {
        poiPointers[i] = &pois[i];
    }
    
    ResponseInterface::modelArrayResponse(poiPointers, count);
    
    delete[] poiPointers;
}

void ResponseInterface::routesAndEndpointsReponse(const Route* routes, const PointOfInterest* pois, unsigned int count) {
    unsigned int bufferHeaderSize = sizeof(unsigned int) + 2;
    unsigned int bufferDataSize = bufferHeaderSize + sizeof(unsigned int) + 1;
    for(int i = 0; i < count; i++) {
        bufferDataSize += routes[i].serializedLength();
        bufferDataSize += pois[i].serializedLength();
    }
    
    unsigned char buffer[bufferDataSize];
    
    buffer[0] = 0x01;
    buffer[1] = sizeof(unsigned int);
    memcpy(buffer + 2, &bufferDataSize, sizeof(unsigned int));
    buffer[bufferDataSize - 1] = 0xff;
    
    unsigned int bufferWriteHead = bufferHeaderSize;
    for(int i = 0; i < count; i++) {
        routes[i].serialize(buffer + bufferWriteHead);
        bufferWriteHead += routes[i].serializedLength();
    }
    
    memset(buffer + bufferWriteHead, 0, sizeof(unsigned int));
    bufferWriteHead += sizeof(unsigned int);
    
    for(int i = 0; i < count; i++) {
        pois[i].serialize(buffer + bufferWriteHead);
        bufferWriteHead += pois[i].serializedLength();
    }
    
    ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, bufferDataSize);
}
