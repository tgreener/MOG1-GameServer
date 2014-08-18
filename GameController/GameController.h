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
#include "../Models/Location.h"
#include <atomic>
#include <vector>

class GameController {
public:
    unsigned int connectUser(std::string tag);
    bool disconnectUser(unsigned int userID);
    
    void getRandomPointOfInterest(std::function<void(PointOfInterest&)> randomPOIFunction);
    void getRoutesFromPointOfInterest(const PointOfInterest& poi, std::function<void(std::vector<Route>)> callback);
    void moveUser(User& user, Location& loc);
    
    Location getUserLocation(unsigned int userID);
    
    static GameController& getInstance();
    
    static ByteInterpreterFunction getConnectUserFunction();
    static ByteInterpreterFunction getDisconnectUserFunction();
    static ByteInterpreterFunction getUserLocationFunction();
    static ByteInterpreterFunction getRoutesFromPOIFunction();
    static ByteInterpreterFunction getMoveUserFunction();
    
private:
    GameController();
    
    static GameController instance;
    
    std::atomic<bool> singleConnectionTaken;
};

#endif	/* GAMECONTROLLER_H */

