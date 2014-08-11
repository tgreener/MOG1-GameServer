/* 
 * File:   GameController.h
 * Author: toddgreener
 *
 * Created on August 6, 2014, 7:58 PM
 */

#ifndef GAMECONTROLLER_H
#define	GAMECONTROLLER_H

#include "../Definitions.h"
#include "../Models/PointOfInterest.h"
#include <atomic>

class GameController {
public:
    unsigned int connectUser(std::string tag);
    bool disconnectUser(unsigned int userID);
    void getRandomPointOfInterest(std::function<void(PointOfInterest&)> randomPOIFunction);
    
    static GameController& getInstance();
    static ByteInterpreterFunction getConnectUserFunction();
    static ByteInterpreterFunction getDisconnectUserFunction();
    
private:
    GameController();
    
    static GameController instance;
    
    std::atomic<bool> singleConnectionTaken;
};

#endif	/* GAMECONTROLLER_H */

