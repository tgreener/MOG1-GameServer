
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

void PointOfInterest::getAllPOIs(AllModelsCallback callback) {
    PointsOfInterestDAO::allPOIDAOs([&](PointsOfInterestDAO* daos, int count) -> void {
        PointOfInterest* pois = new PointOfInterest[count];
        AbstractModel** pps = new AbstractModel*[count];
        
        for(int i = 0; i < count; i++) {
            pois[i].setDAO(daos[i]);
            pps[i] = &pois[i];
        }

        callback(pps, count);
        delete[] pps;
        delete[] pois;
    });
}

void PointOfInterest::getAllPOIs(PointOfInterestCallback callback) {
    PointsOfInterestDAO::allPOIDAOs([&](PointsOfInterestDAO* daos, int count) -> void {
        PointOfInterest* pois = new PointOfInterest[count];
        
        for(int i = 0; i < count; i++) {
            pois[i].setDAO(daos[i]);
        }

        callback(pois, count);
        delete[] pois;
    });
}

int PointOfInterest::createPointOfInterest(const char* bs, int length) {
    unsigned char* bytes = (unsigned char*)bs;
    if(bytes[length - 1] == 0xff && bytes[length - 2] == '\0') {
        POIAttrib attribs;
        attribs.soil = bytes[2];
        attribs.stone = bytes[3];
        attribs.wilderness = bytes[4];
        
        attribs.name = bytes + 5;
        
        PointOfInterest poi(attribs);
        poi.save();
        
        return poi.getID();
    }
    else {
        return -1;
    }
}

ByteInterpreterFunction PointOfInterest::getFetchFunction() {
    return [](const char* bytes, int length) -> void {
        if(length >= 3) {
            try {
                unsigned int id = bytes[2];
                PointOfInterest poi(id);
                AbstractModel::fetchModel(poi);
            }
            catch (const char* e) {
                ServiceLocator::getServiceLocator().sendMessageToClient(e);
            }
            return;
        }
        AbstractModel::insufficientDataMessage();
    };
}

ByteInterpreterFunction PointOfInterest::getDeleteFunction() {
    return [](const char* bytes, int length) -> void {
        if(length >= 3) {
            try {
                unsigned int id = bytes[2];
                PointOfInterest poi(id);
                AbstractModel::deleteModel(poi);
            }
            catch (const char* e) {
                ServiceLocator::getServiceLocator().sendMessageToClient(e);
            }
            return;
        }
        AbstractModel::insufficientDataMessage();
    };
}

ByteInterpreterFunction PointOfInterest::getAddFunction() {
    return [](const char* bytes, int length) -> void {
        AbstractModel::respondWithID(PointOfInterest::createPointOfInterest(bytes, length));
    };
}

ByteInterpreterFunction PointOfInterest::getFetchAllFunction() {
    return [](const char* bytes, int length) -> void {
        PointOfInterest::getAllPOIs(AbstractModel::getAllModelsCallback);
    };
}
