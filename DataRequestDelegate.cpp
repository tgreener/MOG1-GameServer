
#include "DataRequestDelegate.h"
#include "ServiceLocator.h"
#include "Models/PointOfInterest.h"
#include "Models/Route.h"
#include <cstdio>
#include <cstring>

void DataRequestDelegate::insufficientDataMessage() {
    ServiceLocator::getServiceLocator().sendMessageToClient("Insufficient data.");
}

void DataRequestDelegate::nullResponse() {
    unsigned char buffer[2];
    buffer[0] = 0;
    buffer[1] = 0xff;
    ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, 2);
}

void DataRequestDelegate::respondWithID(int id) {
    unsigned int responseLength;
    unsigned char* response;

    if(id > 0) {
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

void DataRequestDelegate::deleteModel(AbstractModel& model) {
    bool deleted = model.remove();

    unsigned char buffer[2];
    buffer[0] = 0;
    buffer[1] = 0xff;

    if(deleted) {
        buffer[0] = 1;
    }

    ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, 2);
}

void DataRequestDelegate::fetchModel(AbstractModel& model) {
    unsigned int bufferSize = 2 + model.serializedLength();
    unsigned char buffer[bufferSize];

    buffer[0] = 1;
    model.serialize(buffer + 1);
    buffer[bufferSize - 1] = 0xff;

    ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, bufferSize);
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

void DataRequestDelegate::addPOI(char* bytes, int length) {
    respondWithID(createPointOfInterest(bytes, length));
}

void DataRequestDelegate::fetchPOI(unsigned int id) {
    try {
        PointOfInterest poi(id);
        fetchModel(poi);
    }
    catch (const char* e) {
        ServiceLocator::getServiceLocator().sendMessageToClient(e);
    }
}

void DataRequestDelegate::deletePOI(unsigned int id) {
    try {
        PointOfInterest poi(id);
        deleteModel(poi);
    }
    catch (const char* e) {
        ServiceLocator::getServiceLocator().sendMessageToClient(e);
    }
}

void DataRequestDelegate::fetchAllPOIs() {
    PointOfInterest::getAllPOIs(this->fetchAllModelsCallback);
}

int DataRequestDelegate::createRoute(char* bytes, int length) {
    if((unsigned char)bytes[length - 1] == 0xff) {
        RouteAttributes attribs = Route::extractAttributes(bytes + 2, length - 2);
        if(attribs.poiAID == 0) return -1;
        
        Route newRoute(attribs);
        newRoute.save();
        
        return newRoute.getID();
    }
    return -1;
}

void DataRequestDelegate::addRoute(char* bytes, int length) {
    respondWithID(createRoute(bytes, length));
}

void DataRequestDelegate::fetchRoute(unsigned int id) {
    try {
        Route route(id);
        fetchModel(route);
    }
    catch (const char* e) {
        ServiceLocator::getServiceLocator().sendMessageToClient(e);
    }
}

void DataRequestDelegate::deleteRoute(unsigned int id) {
    try {
        Route r(id);
        deleteModel(r);
    }
    catch (const char* e) {
        ServiceLocator::getServiceLocator().sendMessageToClient(e);
    }
}

void DataRequestDelegate::fetchAllRoutes() {
    Route::getAllRoutes(this->fetchAllModelsCallback);
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
            fetchRoute(bytes[2]);
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

DataRequestDelegate::DataRequestDelegate() {
//    this->fetchAllModelsCallback = 
}
