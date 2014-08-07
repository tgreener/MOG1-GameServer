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

class GameController {
public:
    static GameController& getInstance();
    static ByteInterpreterFunction getConnectUserFunction();
    
    unsigned int connectUser(std::string tag);
    void getRandomPointOfInterest(std::function<void(PointOfInterest&)> randomPOIFunction);
    
private:
    GameController();
    
    static GameController instance;
};

#endif	/* GAMECONTROLLER_H */

