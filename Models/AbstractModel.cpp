
#include "AbstractModel.h"
#include "../ServiceLocator.h"

AllModelsCallback AbstractModel::getAllModelsCallback = [](AbstractModel** models, int count) -> void {
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
};

void AbstractModel::deleteModel(AbstractModel& model) {
    bool deleted = model.remove();

    unsigned char buffer[2];
    buffer[0] = 0;
    buffer[1] = 0xff;

    if(deleted) {
        buffer[0] = 1;
    }

    ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, 2);
}

void AbstractModel::fetchModel(AbstractModel& model) {
    unsigned int bufferSize = 2 + model.serializedLength();
    unsigned char buffer[bufferSize];

    buffer[0] = 1;
    model.serialize(buffer + 1);
    buffer[bufferSize - 1] = 0xff;

    ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, bufferSize);
}

void AbstractModel::respondWithID(int id) {
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

void AbstractModel::insufficientDataMessage() {
    ServiceLocator::getServiceLocator().sendMessageToClient("Insufficient data.");
}

void AbstractModel::nullResponse() {
    unsigned char buffer[2];
    buffer[0] = 0;
    buffer[1] = 0xff;
    ServiceLocator::getServiceLocator().sendMessageToClient((char*)buffer, 2);
}
