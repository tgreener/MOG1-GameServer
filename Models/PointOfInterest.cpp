
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
    dao.setLocation(otherDao.getLocationID());
    dao.setPopulation(otherDao.getPopulation());
    
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

const char* PointOfInterest::getName() const {
    return (const char*)(dao.getName());
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

unsigned int PointOfInterest::getPopulation() const {
    return dao.getPopulation();
}
    
unsigned int PointOfInterest::getLocationID() const {
    return dao.getLocationID();
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

void PointOfInterest::setPopulation(unsigned int pop) {
    needsUpdate = true;
    dao.setPopulation(pop);
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
    unsigned int length = 6 * sizeof(unsigned int);
    length += strlen((const char*)getName()) + 1;
    
    return length;
}

void PointOfInterest::serialize(unsigned char* buffer) const {
    unsigned int loc = this->getLocationID();
    unsigned int soil = this->getAttributeSoil();
    unsigned int stn = this->getAttributeStone();
    unsigned int wld = this->getAttributeWilderness();
    unsigned int pop = this->getPopulation();
    
//    printf("%d, %d, %d, %d, %d\n", loc, soil, stn, wld, pop);
    
    memcpy(buffer + (sizeof(unsigned int) * 0), &id, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 1), &loc, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 2), &soil, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 3), &stn, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 4), &wld, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 5), &pop, sizeof(unsigned int));
    strcpy((char*)buffer + (sizeof(unsigned int) * 6), (char*)this->getName());
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
