
#include "Route.h"
#include "../ServiceLocator.h"
#include <string>
#include <iostream>

Route::Route() {
    
}

Route::Route(unsigned int id) {
    if(!dao.retrieve(id)) {
        throw "Error loading Route.";
    }
}

Route::Route(RouteAttributes& attribs) {
    dao.setPOIA(attribs.poiAID);
    dao.setPOIB(attribs.poiBID);
    dao.setDifficulty(attribs.difficulty);
    dao.setBidirectional(attribs.bidirectional);
    dao.setReverse(attribs.reverse);
}

Route::~Route() {}

unsigned int Route::getID() const {
    return dao.getID();
}

unsigned int Route::getLocationID() const {
    return dao.getLocationID();
}

unsigned int Route::getEndpointAID() const {
    return dao.getPOIA();
}

unsigned int Route::getEndpointBID() const {
    return dao.getPOIB();
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
    return dao.isBidirectional();
}

bool Route::isReverse() const {
    return dao.isReverse();
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

void Route::setReverse(bool rev) {
    dao.setReverse(rev);
}

void Route::onUserEnter(const User& user) {
    if(user.getLocationID() == dao.getLocationID()) {
        
    }
    else {
        printf("Attempted Route::onUserEnter when user was not on Route.\n");
    }
}

void Route::onUserExit(const User& user) {
    if(user.getLocationID() == dao.getLocationID()) {
        
    }
    else {
        printf("Attempted Route::onUserExit when user was not on Route.\n");
    }
}

unsigned int Route::serializedLength() const {
    return sizeof(unsigned int) * 7;
}

void Route::serialize(unsigned char* buffer) const {
    
    unsigned int id = this->getID();
    unsigned int loc = dao.getLocationID();
    unsigned int poi_a = dao.getPOIA();
    unsigned int poi_b = dao.getPOIB();
    unsigned int diff = dao.getDifficulty();
    unsigned int bi = dao.isBidirectional();
    unsigned int rev = dao.isReverse();
    
    memcpy(buffer + (sizeof(unsigned int) * 0), &id, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 1), &loc, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 2), &poi_a, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 3), &poi_b, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 4), &diff, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 5), &bi, sizeof(unsigned int));
    memcpy(buffer + (sizeof(unsigned int) * 6), &rev, sizeof(unsigned int));
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
    str += "\n\tlocationID: " + std::to_string(dao.getLocationID());
    str += "\n\tpoiA: " + std::to_string(dao.getPOIA());
    str += "\n\tpoiB: " + std::to_string(dao.getPOIB());
    str += "\n\tdifficulty: " + std::to_string(dao.getDifficulty());
    str += "\n\tbidirectional: " + std::to_string(dao.isBidirectional());
    str += "\n\treverse: " + std::to_string(dao.isReverse());
    str += "\n}\n";
    
    std::cout << str << std::endl;
}

void Route::callbackFromDAOs(RouteDAO* daos, unsigned int count, RoutesCallback callback) {
    Route* routes = new Route[count];
        
    for(int i = 0; i < count; i++) {
        routes[i].dao = daos[i];
    }

    callback(routes, count);
    delete[] routes;
}

void Route::getAllRoutes(RoutesCallback callback) {
    RouteDAO::allRouteDAOs([&](RouteDAO* daos, int count) -> void {
        callbackFromDAOs(daos, count, callback);
    });
}

void Route::outgoingRoutes(unsigned int poiID, RoutesCallback callback) {
    RouteDAO::outgoingRouteDAOs(poiID, [&] (RouteDAO* daos, unsigned int count) {
        callbackFromDAOs(daos, count, callback);
    });
}

void Route::incomingRoutes(unsigned int poiID, RoutesCallback callback) {
    RouteDAO::incomingRouteDAOs(poiID, [&] (RouteDAO* daos, unsigned int count) {
        callbackFromDAOs(daos, count, callback);
    });
}

void Route::getAllRoutes(AllModelsCallback callback) {
    RouteDAO::allRouteDAOs([&](RouteDAO* daos, int count) -> void {
        Route* routes = new Route[count];
        AbstractModel** rps = new AbstractModel*[count];
        
        for(int i = 0; i < count; i++) {
            routes[i].dao = daos[i];
            rps[i] = &routes[i];
        }

        callback(rps, count);
        delete[] rps;
        delete[] routes;
    });
}

ByteInterpreterFunction Route::getFetchFunction() {
    return [](const char* bytes, int length) -> void {
        if(length >= 3) {
            try {
                unsigned int id = bytes[2];
                Route route(id);
                AbstractModel::fetchModel(route);
            }
            catch (const char* e) {
                ServiceLocator::getServiceLocator().sendMessageToClient(e);
            }
            
            return;
        }
        AbstractModel::insufficientDataMessage();
    };
}

ByteInterpreterFunction Route::getDeleteFunction() {
    return [](const char* bytes, int length) -> void {
        if(length >= 3) {
            try {
                unsigned int id = bytes[2];
                Route route(id);
                AbstractModel::deleteModel(route);
            }
            catch (const char* e) {
                ServiceLocator::getServiceLocator().sendMessageToClient(e);
            }
        }
        AbstractModel::insufficientDataMessage();
    };
}

ByteInterpreterFunction Route::getAddFunction() {
    return [](const char* bytes, int length) -> void {
        AbstractModel::respondWithID(Route::createRoute(bytes, length));
    };
}

ByteInterpreterFunction Route::getFetchAllFunction() {
    return [](const char* bytes, int length) -> void {
        Route::getAllRoutes(AbstractModel::getAllModelsCallback);
    };
}

RouteAttributes Route::extractAttributes(const char* bytes, int length) {
    RouteAttributes attribs;
    if(length >= 5) {
        attribs.poiAID = bytes[0];
        attribs.poiBID = bytes[1];
        attribs.difficulty = bytes[2];
        attribs.bidirectional = bytes[3] != 0;
        attribs.reverse = bytes[4] != 0;
    }
    
    return attribs;
}

int Route::createRoute(const char* bytes, int length) {
    if((unsigned char)bytes[length - 1] == 0xff) {
        RouteAttributes attribs = Route::extractAttributes(bytes + 2, length - 2);
        if(attribs.poiAID == 0) return -1;
        
        Route newRoute(attribs);
        newRoute.save();
        
        return newRoute.getID();
    }
    return -1;
}
