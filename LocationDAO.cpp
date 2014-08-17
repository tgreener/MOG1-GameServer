
#include "LocationDAO.h"
#include "../ServiceLocator.h"
#include "../StringUtil.h"

LocationDAO::LocationDAO() : id(0), route(false), poi(false), name(nullptr) {
    
}

LocationDAO::~LocationDAO() {
    freeName();
}

void LocationDAO::freeName() {
    if(name != nullptr) {
        delete[] name;
    }
}

unsigned int LocationDAO::getID() const {
    return this->id;
}

const char* LocationDAO::getName() const {
    return this->name;
}

bool LocationDAO::isRoute() const {
    return this->route;
}

bool LocationDAO::isPOI() const {
    return this->poi;
}

void LocationDAO::setName(const char* name) {
    freeName();
    this->name = heapifyString(name);
}

void LocationDAO::setIsRoute(bool isRoute) {
    route = isRoute;
    poi = !route;
}

void LocationDAO::setIsPOI(bool isPOI) {
    poi = isPOI;
    route = !poi;
}

bool LocationDAO::retrieve(unsigned int id) {
    this->id = id;
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    
    const char* query = "SELECT * FROM location WHERE id = ?";
    DBStatement statement = dbc->prepare(query, nullptr);
    statement.bindInt(1, this->id);
    
    if(statement.step()) {
        setName((const char*)(statement.getColumnText(1)));
        poi = statement.getColumnInt(2) != 0;
        route = statement.getColumnInt(3) != 0;
        
        return true;
    }
    
    return false;
}

PointOfInterest LocationDAO::getPOI() const {
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    const char* query = "SELECT COUNT() FROM point_of_interest WHERE location_id = ?";
    DBStatement countStatement = dbc->prepare(query, nullptr);
    countStatement.bindInt(1, id);
    
    if(countStatement.step() && countStatement.getColumnInt(0) > 0) {
        query = "SELECT id FROM point_of_interest WHERE location_id = ?";
        DBStatement statement = dbc->prepare(query, nullptr);
        statement.bindInt(1, id);

        if(statement.step()) {
            unsigned int poiID = statement.getColumnInt(0);
            return PointOfInterest(poiID);
        }
        else {
            return PointOfInterest();
        }
    }
    else {
        return PointOfInterest();
    }
    
    
}

Route LocationDAO::getRoute() const {
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    const char* query = "SELECT COUNT() FROM route WHERE location_id = ?";
    DBStatement countStatement = dbc->prepare(query, nullptr);
    countStatement.bindInt(1, id);
    
    if(countStatement.step() && countStatement.getColumnInt(0) > 0) {
        query = "SELECT id FROM route WHERE location_id = ?";
        DBStatement statement = dbc->prepare(query, nullptr);
        statement.bindInt(1, id);

        if(statement.step()) {
            unsigned int routeID = statement.getColumnInt(0);
            return Route(routeID);
        }
        else {
            return Route();
        }
    }
    else {
        return Route();
    }
}

bool LocationDAO::remove(unsigned int id) {
    return false;
}

int LocationDAO::write() {
    return -1;
}

int LocationDAO::write(int id) {
    return -1;
}
