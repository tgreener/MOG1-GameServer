
#include "Route.h"

#include "../ServiceLocator.h"
#include <string>

Route::Route() {
    
}

Route::Route(unsigned int id) {
    if(!dao.retrieve(id)) {
        throw "Error loading Route.";
    }
}

Route::~Route() {}

unsigned int Route::getID() const {
    return dao.getID();
}

PointOfInterest Route::getEndpointA() const {
    return PointOfInterest(dao.getPOIA());
}

PointOfInterest Route::getEndpointB() const {
    return PointOfInterest(dao.getPOIB());
}

unsigned int Route::getDifficulty() const {
    return dao.getDifficulty();
}

bool Route::isBidirectional() const {
    return dao.isBidrectional();
}

void Route::setEndpointA(unsigned int poiID) {
    dao.setPOIA(poiID);
}

void Route::setEndpointB(unsigned int poiID) {
    dao.setPOIB(poiID);
}

void Route::setDifficulty(unsigned int dif) {
    dao.setDifficulty(dif);
}

void Route::setBidirectional(bool dir) {
    dao.setBidirectional(dir);
}

unsigned int Route::serializedLength() const {
    return 3;
}

void Route::serialize(unsigned char* buffer) const {
    buffer[0] = (unsigned char)this->getID();
    buffer[1] = (unsigned char)dao.getPOIA();
    buffer[2] = (unsigned char)dao.getPOIB();
}

bool Route::remove() {
    return dao.remove(dao.getID());
}

void Route::save() {
    if(dao.getID()) {
        dao.write(dao.getID());
    }
    else {
        dao.write();
    }
}

void Route::bark() const {
    std::string str = "Route: {";
    str += "\n\tid: " + std::to_string(dao.getID());
    str += "\n\tpoiA: " + std::to_string(dao.getPOIA());
    str += "\n\tpoiB: " + std::to_string(dao.getPOIB());
    str += "\n}\n";
    
    ServiceLocator::getServiceLocator().sendMessageToClient(str.c_str());
}

void Route::getAllRoutes(AllRoutesCallback callback) {
    callback(nullptr, 0);
}
