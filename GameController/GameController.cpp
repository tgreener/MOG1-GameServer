
#include "GameController.h"
#include "../Models/User.h"
#include "ResponseInterface.h"
#include <random>

GameController GameController::instance;

GameController& GameController::getInstance() {
    return instance;
}

GameController::GameController() : singleConnectionTaken(false) {}

unsigned int GameController::connectUser(std::string tag) {
    UserAttributes attribs;
    attribs.tag = tag.c_str();
    
    getRandomPointOfInterest([&](PointOfInterest& poi) {
        printf("Random poi location id: %d\n", poi.getLocationID());
        attribs.location = poi.getLocationID();
    });
    
    User newUser(attribs);
    newUser.save();
    
    return newUser.getID();
}

bool GameController::disconnectUser(unsigned int userID) {
    try {
        User disconnectingUser(userID);
        return disconnectingUser.remove();
    }
    catch(const char* e) {
        printf("%s\n", e);
        return false;
    }
}

void GameController::getRandomPointOfInterest(std::function<void(PointOfInterest&)> randomPOIFunction) {
    PointOfInterest::getAllPOIs([&](PointOfInterest* pois, unsigned int length) -> void {
        if(length > 0) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<unsigned int> dis(0, length - 1);
            
            unsigned int randomPOIIndex = dis(gen);
            
            printf("Random poi index: %d length: %d\n", randomPOIIndex, length);
            
            randomPOIFunction(pois[randomPOIIndex]);
        }
    });
}

ByteInterpreterFunction GameController::getConnectUserFunction() {
    return [] (const char* bytes, int length) {
        // Mocking out user connection behavior
        // Parse bytes
        // Get Tag
        if(instance.singleConnectionTaken) return;
        instance.singleConnectionTaken = true;
        
        std::string tag = "A User";
        ResponseInterface::userConnectedResponse(instance.connectUser(tag));
    };
}

ByteInterpreterFunction GameController::getDisconnectUserFunction() {
    return [] (const char* bytes, int length) {
        // Mocking out user disconnect behavior
        // Parse bytes
        // Get ID
        unsigned int id = 1;
        ResponseInterface::userDisconnectedResponse(instance.disconnectUser(id));
        instance.singleConnectionTaken = false;
    };
}

