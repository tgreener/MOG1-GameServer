
#include "PointOfInterest.h"
#include "../ServiceLocator.h"
#include <cstdlib>
#include <string>


PointOfInterest::PointOfInterest() : id(0), needsUpdate(false) {}

PointOfInterest::PointOfInterest(unsigned int id) : id(id), needsUpdate(false) {
    load();
}

PointOfInterest::PointOfInterest(POIAttrib& attribs) : id(0), needsUpdate(true) {
    dao.setName(attribs.name);
    dao.setSoil(attribs.soil);
    dao.setStone(attribs.stone);
    dao.setWilderness(attribs.wilderness);
}

PointOfInterest::~PointOfInterest() {
}

void PointOfInterest::load() {
    if(!dao.retrieve(id)) {
        throw "Error loading POI.";
    }
}

void PointOfInterest::setDAO(const PointsOfInterestDAO& otherDao) {
    id = otherDao.getID();
    
    dao.setName(otherDao.getName());
    dao.setSoil(otherDao.getSoil());
    dao.setStone(otherDao.getStone());
    dao.setWilderness(otherDao.getWilderness());
    
    needsUpdate = true;
}

void PointOfInterest::save() {
    if(!needsUpdate) {
        return;
    }
    
    int result;
    if(id) {
        result = dao.write(id);
    }
    else  {
        result = dao.write();
        id = result;
    }
    
    if(result < 0) {
        throw "Something went wrong writing POI.\n";
    }
}

unsigned int PointOfInterest::getID() const {
    return id;
}

const unsigned char* PointOfInterest::getName() const {
    return dao.getName();
}

unsigned int PointOfInterest::getAttributeStone() const {
    return dao.getStone();
}
unsigned int PointOfInterest::getAttributeSoil() const {
    return dao.getSoil();
}

unsigned int PointOfInterest::getAttributeWilderness() const {
    return dao.getWilderness();
}

void PointOfInterest::setAttributeSoil(int s) {
    needsUpdate = true;
    dao.setSoil(s);
}

void PointOfInterest::setAttributeStone(int s) {
    needsUpdate = true;
    dao.setStone(s);
}

void PointOfInterest::setAttributeWilderness(int w) {
    needsUpdate = true;
    dao.setWilderness(w);
}

void PointOfInterest::bark() const {
    std::string str = "Point of Interest {";
    str += "\n\tid: " + std::to_string(id);
    str += "\n\tname: ";
    str += (const char*)getName(); 
    str += "\n\tsoil: " + std::to_string(getAttributeSoil());
    str += "\n\tstone: " + std::to_string(getAttributeStone());
    str += "\n\twilderness: " + std::to_string(getAttributeWilderness());
    str += "\n}\n";
    
    ServiceLocator::getServiceLocator().sendMessageToClient(str.c_str());
}

bool PointOfInterest::remove() {
    return dao.remove(id);
}

unsigned int PointOfInterest::serializedLength() const {
    unsigned int length = 4;
    length += strlen((const char*)getName()) + 1;
    
    return length;
}

void PointOfInterest::serialize(unsigned char* buffer) const {
    buffer[0] = id;
    buffer[1] = this->getAttributeSoil();
    buffer[2] = this->getAttributeStone();
    buffer[3] = this->getAttributeWilderness();
    strcpy((char*)buffer + 4, (char*)this->getName());
}

void PointOfInterest::getAllPOIs(std::function<void(PointOfInterest*,int)> callback) {
    PointsOfInterestDAO::allPOIDAOs([&](PointsOfInterestDAO* daos, int count) -> void {
        PointOfInterest* pois = new PointOfInterest[count];
        
        for(int i = 0; i < count; i++) {
            pois[i].setDAO(daos[i]);
        }

        callback(pois, count);
        delete[] pois;
    });
}
