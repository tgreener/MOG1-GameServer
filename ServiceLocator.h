/* 
 * File:   ServiceLocator.h
 * Author: toddgreener
 *
 * Created on May 27, 2014, 7:35 PM
 */

#ifndef SERVICELOCATOR_H
#define	SERVICELOCATOR_H

#include "DBConnection.h"
#include "ImmutableServiceLocator.h"
#include <stdlib.h>

class Server;

class ServiceLocator :  public ImmutableServiceLocator {
private:
    DBConnection* connection;
    Server* server;
    int clientHandle;
    static __thread ServiceLocator* locator;
    
public:
    ServiceLocator();
    ~ServiceLocator();
    
    static ImmutableServiceLocator& getServiceLocator();
    
    DBConnection* getDBConnection();
    void sendMessageToClient(const char* message);
    void sendMessageToClient(const char* message, int length);
    void stopServer();
    
    void setDBConnection(DBConnection* dbc);
    void setClientSocketHandle(int handle);
    void setAsServiceLocator();
    void setServer(Server* s);
};

#endif	/* SERVICELOCATOR_H */

