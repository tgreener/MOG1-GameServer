/* 
 * File:   ResponseInterface.h
 * Author: toddgreener
 *
 * Created on August 6, 2014, 10:00 PM
 */

#ifndef RESPONSEINTERFACE_H
#define	RESPONSEINTERFACE_H

#include "../Models/User.h"

class ResponseInterface {
public:
    static void userConnectedResponse(User& connectedUser);
private:
    
};

#endif	/* RESPONSEINTERFACE_H */

