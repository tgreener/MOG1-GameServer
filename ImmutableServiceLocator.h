/* 
 * File:   ImmutableServiceLocator.h
 * Author: toddgreener
 *
 * Created on May 28, 2014, 2:36 AM
 */

#include "DBConnection.h"

#ifndef IMMUTABLESERVICELOCATOR_H
#define	IMMUTABLESERVICELOCATOR_H

class ImmutableServiceLocator {
public:
    virtual DBConnection* getDBConnection() = 0;
    virtual void sendMessageToClient(const char* message) = 0;
    virtual void sendMessageToClient(const char* message, int length) = 0;
    virtual void stopServer() = 0;
};

#endif	/* IMMUTABLESERVICELOCATOR_H */

