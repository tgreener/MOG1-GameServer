
#include "DataRequestDelegate.h"
#include "ServiceLocator.h"
#include "Models/PointOfInterest.h"
#include <cstdio>
#include <cstring>

void insufficientDataMessage() {
    ServiceLocator::getServiceLocator().sendMessageToClient("Insufficient data.");
}

int DataRequestDelegate::createPointOfInterest(char* bs, int length) {
    unsigned char* bytes = (unsigned char*)bs;
    if(bytes[length - 1] == 0xff && bytes[length - 2] == '\0') {
        POIAttrib attribs;
        attribs.soil = bytes[2];
        attribs.stone = bytes[3];
        attribs.wilderness = bytes[4];
        
        attribs.name = bytes + 5;
        
        PointOfInterest poi(attribs);
        poi.save();
        
        return poi.getID();
    }
    else {
        return -1;
    }
}

void DataRequestDelegate::fetchPOI(unsigned int id) {
    try {
        PointOfInterest poi(id);
        unsigned int bufferSize = 1 + poi.serializedLength();
        unsigned char buffer[bufferSize];

        buffer[0] = 1;
        poi.serialize(buffer + 1);

        ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, bufferSize);
    }
    catch (const char* e) {
        ServiceLocator::getServiceLocator().sendMessageToClient(e);
    }
}

void DataRequestDelegate::addPOI(char* bytes, int length) {
    int newRecordID = createPointOfInterest(bytes, length);
    int responseLength;
    unsigned char* response;

    if(newRecordID >= 0) {
        unsigned int id = newRecordID;
        responseLength = sizeof(unsigned int) + 2;

        response = new unsigned char[responseLength];
        memset(response, 0, responseLength);
        response[0] = 0x01;
        memcpy(response + 1, &id, sizeof(unsigned int));
        response[responseLength - 1] = 0xff;
    }
    else {
        responseLength = 2;
        response = new unsigned char[responseLength];
        response[0] = 0;
        response[1] = 0xff;
    }

    ServiceLocator::getServiceLocator().sendMessageToClient((char*)response, responseLength);
    delete[] response;
}

void DataRequestDelegate::deletePOI(unsigned int id) {
    bool deleted = false;
    
    try {
        PointOfInterest poi(id);
        deleted = poi.remove();
    }
    catch (const char* e) {
        ServiceLocator::getServiceLocator().sendMessageToClient(e);
    }

    unsigned char buffer[2];
    buffer[0] = 0;
    buffer[1] = 0xff;

    if(deleted) {
        buffer[0] = 1;
    }

    ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, 2);
}

void DataRequestDelegate::fetchAllPOIs() {
    PointOfInterest::getAllPOIs([] (PointOfInterest* pois, int count) -> void {
        try {
            unsigned int bufferHeaderLength = sizeof(unsigned int) + 2;
            unsigned int bufferLength = bufferHeaderLength + 1; //Bytes for response success code, final byte, and data length.
            for (int i = 0; i < count; i++) {
                bufferLength += pois[i].serializedLength();
            }
            
//            printf("%d\n", bufferLength);
            unsigned char buffer[bufferLength];
            
            buffer[0] = 0x01;
            buffer[1] = sizeof(unsigned int);
            buffer[bufferLength - 1] = 0xff;
            memcpy(buffer + 2, &bufferLength, sizeof(unsigned int));
            
            unsigned int bufferWriteHead = bufferHeaderLength;
            for(int i = 0; i < count; i++) {
                pois[i].serialize(buffer + bufferWriteHead);
                bufferWriteHead += pois[i].serializedLength();
            }
            
            ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, bufferLength);
        }
        catch (const char* e) {
            ServiceLocator::getServiceLocator().sendMessageToClient(e);
        }
    });
}

int DataRequestDelegate::createRoute(unsigned int rID, unsigned int poiAID, unsigned int poiBID) {
    return 0;
}

void DataRequestDelegate::fetchRoute(unsigned int id) {
    
}

void DataRequestDelegate::deleteRoute(unsigned int id) {
    
}

void DataRequestDelegate::addRoute(char* bytes, int length) {
    
}

void DataRequestDelegate::fetchAllRoutes() {
    
}

void DataRequestDelegate::interpretCommand(char* bytes, int length) {
    if(length <= 1) {
        return;
    }
    
    switch(bytes[1]) {
        case 0x01 : {
            if(length < 3) {
                insufficientDataMessage();
                break;
            }
            fetchPOI(bytes[2]);
            break;
        }
        case 0x02 : {
            addPOI(bytes, length);
            break;
        }
        case 0x03 : {
            if(length < 3) {
                insufficientDataMessage();
                break;
            }
            deletePOI(bytes[2]);
            break;
        }
        case 0x04 : {
            fetchAllPOIs();
            break;
        }
        case 0x05 : {
            if(length < 3) {
                insufficientDataMessage();
                break;
            }
            fetchPOI(bytes[2]);
            break;
        }
        case 0x06 : {
            addRoute(bytes, length);
            break;
        }
        case 0x07 : {
            if(length < 3) {
                insufficientDataMessage();
                break;
            }
            deleteRoute(bytes[2]);
            break;
        }
        case 0x08 : {
            fetchAllRoutes();
            break;
        }
        default : {
            ServiceLocator::getServiceLocator().sendMessageToClient("Unrecognized data command.");
            break;
        }
    }
}
