
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
        attribs.location = poi.getLocationID();
    });
    
    User newUser(attribs);
    newUser.save();
    newUser.getLocation().onUserEnter(newUser);
    
    return newUser.getID();
}

bool GameController::disconnectUser(unsigned int userID) {
    try {
        User disconnectingUser(userID);
        disconnectingUser.getLocation().onUserExit(disconnectingUser);
        
        return disconnectingUser.remove();
    }
    catch(const char* e) {
        printf("%s\n", e);
        return false;
    }
}

Location GameController::getUserLocation(unsigned int userID) {
    User user(userID);
    
    return user.getLocation();
}

bool GameController::moveUser(User& user, Location& loc) {
    Location currentLocation = user.getLocation();
    
    if(currentLocation.getID() > 0) {
        currentLocation.onUserExit(user);
    }
    user.setLocation(loc);
    loc.onUserEnter(user);
    user.save();
    
    return true;
}

void GameController::getRandomPointOfInterest(std::function<void(PointOfInterest&)> randomPOIFunction) {
    PointOfInterest::getAllPOIs([&](PointOfInterest* pois, unsigned int length) -> void {
        if(length > 0) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<unsigned int> dis(0, length - 1);
            
            unsigned int randomPOIIndex = dis(gen);
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
        ResponseInterface::boolResponse(instance.disconnectUser(id));
        instance.singleConnectionTaken = false;
    };
}

ByteInterpreterFunction GameController::getUserLocationFunction() {
    return [] (const char* bytes, int length) {
        // Mocking this again. Faking is fun.
        // Parse bytes
        // Get ID
        unsigned int id = 1;
        ResponseInterface::userLocationResponse(instance.getUserLocation(id));
    };
}

ByteInterpreterFunction GameController::getRoutesFromUserFunction() {
    return [] (const char* bytes, int length) {
        try {
            // First, parse user id.
            unsigned int id = 1; // Mock
            User user(id);
            Location loc = user.getLocation();
            PointOfInterest poi = loc.isPOI() ? loc.getPOI() : PointOfInterest();

            poi.getOutgoingRoutes([&](Route* routes, unsigned int count) {
                PointOfInterest* endPoints = new PointOfInterest[count];

                for(unsigned int i = 0; i < count; i++) {
                    if(routes[i].getEndpointAID() == poi.getID()) {
                        endPoints[i] =  routes[i].getEndpointB();
                    }
                    else {
                        endPoints[i] =  routes[i].getEndpointA();
                    }
                }

                ResponseInterface::routesAndEndpointsReponse(routes, endPoints, count);

                delete[] endPoints;

            });
        }
        catch(const char* e) {
            printf("%s\n", e);
        }
    };
}

ByteInterpreterFunction GameController::getMoveUserFunction() {
    return [] (const char* bytes, int length) {
        try {
            if (length >= 11 && (unsigned char)bytes[length - 1] == 0xff) {
                // User id stored in bytes 2 - 5, but I'll just mock for now
                unsigned int id = 1;
                unsigned int locationID = *((unsigned int*)(bytes + 6));

                User user(id);
                Location loc(locationID);

                ResponseInterface::boolResponse(instance.moveUser(user, loc));
            }
        }
        catch(const char* e) {
            printf("%s\n", e);
             ResponseInterface::boolResponse(false);
        }
    };
}

