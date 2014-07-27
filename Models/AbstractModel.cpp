
#include "AbstractModel.h"

AbstractModel::AbstractModel() {
    this->amc = [](AbstractModel** models, int count) -> void {
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
}
