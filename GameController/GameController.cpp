
#include "GameController.h"
#include "../Models/User.h"
#include <random>

GameController GameController::instance;

GameController& GameController::getInstance() {
    return instance;
}

GameController::GameController() {}

unsigned int GameController::connectUser(std::string tag) {
    UserAttributes attribs;
    attribs.tag = tag.c_str();
    
    getRandomPointOfInterest([&](PointOfInterest& poi) {
        attribs.location = poi.getLocationID();
    });
    
    User newUser(attribs);
    newUser.save();
    
    return newUser.getID();
}

void GameController::getRandomPointOfInterest(std::function<void(PointOfInterest&)> randomPOIFunction) {
    PointOfInterest::getAllPOIs([&](PointOfInterest* pois, unsigned int length) -> void {
        if(length > 0) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<unsigned int> dis(0, length);
            
            unsigned int randomPOIIndex = dis(gen);
            
            randomPOIFunction(pois[randomPOIIndex]);
        }
    });
}

ByteInterpreterFunction GameController::getConnectUserFunction() {
    return [] (const char* bytes, int length) {
        
    };
}

