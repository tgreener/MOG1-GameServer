
#include "Location.h"

Location::Location() {}

Location::Location(unsigned int id) {
    if(!dao.retrieve(id)) {
        throw "Error loading location.";
    }
}

unsigned int Location::getID() const {
    return this->dao.getID();
}

const char* Location::getName() const {
    return this->dao.getName();
}

bool Location::isRoute() const {
    return this->dao.isRoute();
}

bool Location::isPOI() const {
    return this->dao.isPOI();
}

void Location::setName(const char* name) {
    dao.setName(name);
}

PointOfInterest Location::getPOI() {
    if(dao.isPOI()) {
        return dao.getPOI();
    }
    
    return PointOfInterest();        
}

Route Location::getRoute() {
    if(dao.isRoute()) {
        return dao.getRoute();
    }
    
    return Route();
}

void Location::save() {
    if(dao.getID()) {
        dao.write(dao.getID());
    }
    // The way things are right now, I don't want people making new
    // entries via this class. Needs refactor.
}

bool Location::remove() {
    return dao.remove(dao.getID());
}

unsigned int Location::serializedLength() const {
    return 0; // Sub types take care of this stuff for now, needs refactor
}

void Location::serialize(unsigned char* buffer) const {
    return; // Sub types take care of this stuff for now, needs refactor
}

