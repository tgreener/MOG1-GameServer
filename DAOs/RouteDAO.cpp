
#include <vector>

#include "RouteDAO.h"
#include "../DBStatement.h"
#include "../ServiceLocator.h"

RouteDAO::RouteDAO() : needsWrite(false), poiA(0), poiB(0), id(0), difficulty(0), bidirected(false), reverse(false), locationID(0) {
    
}

RouteDAO::~RouteDAO() {
    
}
    
bool RouteDAO::checkValuesSet() {
    return poiA != 0 && poiB != 0;
}

bool RouteDAO::retrieve(unsigned int id) {
    this->id = id;
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    const char* query = 
            "SELECT route.id, route.poi_a, route.poi_b, route.difficulty, route.bidirected, route.reverse, "
            "location.id, location.name " 
            "FROM route INNER JOIN location " 
            "ON location.id = route.location_id "
            "WHERE route.id = ? AND location.is_route <> 0";
    
    DBStatement statement = dbc.prepare(query, nullptr);
    statement.bindInt(1, this->id);
    
    if(statement.step()) {
        this->poiA = statement.getColumnInt(1);
        this->poiB = statement.getColumnInt(2);
        
        this->difficulty = statement.getColumnInt(3);
        this->bidirected = statement.getColumnInt(4) == 1;
        this->reverse = statement.getColumnInt(5) == 1;
        this->locationID = statement.getColumnInt(6);
        
        statement.finalize();
        return true;
    }
    
    statement.finalize();
    return false;
}
bool RouteDAO::remove(unsigned int id) {
    this->id = id;
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    
    const char* routeQuery = "SELECT route.location_id FROM route WHERE route.id = ?";
    DBStatement routeStatement = dbc.prepare(routeQuery, nullptr);
    routeStatement.bindInt(1, id);
    
    int locationID;
    if(routeStatement.step()) {
        locationID = routeStatement.getColumnInt(0);
    }
    else return false;
    
    const char* deleteQuery = "DELETE FROM location WHERE id=? AND is_route <> 0";
    
    DBStatement statement = dbc.prepare(deleteQuery, nullptr);
    statement.bindInt(1, locationID);
    
    dbc.obtainWriteLock();
    int result = statement.step();
    dbc.releaseWriteLock();
    
    return result != 0;
}

int RouteDAO::write() {
    const char* query = "INSERT INTO location (name, is_route) VALUES (?, 1)";
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    DBStatement statement1 = dbc.prepare(query, NULL);
    statement1.bindText(1, "A Route");
    
    dbc.obtainWriteLock();
    bool result = statement1.step();    
    dbc.releaseWriteLock();
    
    if(result) locationID = dbc.lastInsertRowId();
    else return -1;
    
    query = "INSERT INTO route (poi_a, poi_b, difficulty, bidirected, reverse, location_id) VALUES (?, ?, ?, ?, ?, ?)";
    DBStatement statement = dbc.prepare(query, nullptr);
    
    statement.bindInt(1, poiA);
    statement.bindInt(2, poiB);
    
    statement.bindInt(3, difficulty);
    statement.bindInt(4, (unsigned int)bidirected);
    statement.bindInt(5, (unsigned int)reverse);
    statement.bindInt(6, locationID);
    
    dbc.obtainWriteLock();
    if(statement.step()) id = dbc.lastInsertRowId();
    else id = -1;
    dbc.releaseWriteLock();
    
    return id;
}

int RouteDAO::write(int id) {
    const char* query = "UPDATE route SET poi_a = ?, poi_b = ?, difficulty = ?, bidirected = ?, reverse = ? WHERE id = ?";
    
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    DBStatement statement = dbc.prepare(query, nullptr);
    
    statement.bindInt(1, poiA);
    statement.bindInt(2, poiB);
    statement.bindInt(3, difficulty);
    statement.bindInt(4, (unsigned int)bidirected);
    statement.bindInt(5, (unsigned int)reverse);
    statement.bindInt(6, id);

    dbc.obtainWriteLock();
    statement.step();
    dbc.releaseWriteLock();
    
    return id;
}


unsigned int RouteDAO::getID() const {
    return this->id;
}

unsigned int RouteDAO::getLocationID() const {
    return this->locationID;
}

unsigned int RouteDAO::getPOIA() const {
    return this->poiA;
}

unsigned int RouteDAO::getPOIB() const {
    return this->poiB;
}

unsigned int RouteDAO::getDifficulty() const {
    return this->difficulty;
}

bool RouteDAO::isBidirectional() const {
    return this->bidirected;
}

bool RouteDAO::isReverse() const {
    return this->reverse;
}

void RouteDAO::setPOIA(unsigned int a) {
    this->poiA = a;
}

void RouteDAO::setPOIB(unsigned int b) {
    this->poiB = b;
}

void RouteDAO::setDifficulty(unsigned int dif) {
    this->difficulty = dif;
}

void RouteDAO::setBidirectional(bool bidir) {
    this->bidirected = bidir;
}

void RouteDAO::setReverse(bool rev) {
    this->reverse = rev;
}

void RouteDAO::allRouteDAOs(RouteDAOsCallback callback) {
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    
    const char* countQuery = "SELECT COUNT() FROM route";
    DBStatement countStatement = dbc.prepare(countQuery, NULL);
    
    countStatement.step();
    int count = countStatement.getColumnInt(0);
    countStatement.finalize();
    if(count <= 0) {
        callback(nullptr, 0);
        return;
    }
    
    RouteDAO* daos = new RouteDAO[count];
    
    const char* poisQuery = "SELECT route.id, route.poi_a, route.poi_b, route.difficulty, route.bidirected, route.reverse, "
            "location.id, location.name " 
            "FROM route INNER JOIN location " 
            "ON location.id = route.location_id "
            "WHERE location.is_route <> 0";
    DBStatement statement = dbc.prepare(poisQuery, NULL);
    
    for(int i = 0; i < count; i++) {
        statement.step();
        
        unsigned int id = statement.getColumnInt(0);
        unsigned int poiA = statement.getColumnInt(1);
        unsigned int poiB = statement.getColumnInt(2);
        unsigned int diff = statement.getColumnInt(3);
        bool bidirectional = statement.getColumnInt(4) != 0;
        bool reverse = statement.getColumnInt(5) != 0;
        unsigned int locID = statement.getColumnInt(6);
        
        printf("%d\n", locID);
        
        daos[i].id = id;
        daos[i].locationID = locID;
        daos[i].setPOIA(poiA);
        daos[i].setPOIB(poiB);
        daos[i].setDifficulty(diff);
        daos[i].setBidirectional(bidirectional);
        daos[i].setReverse(reverse);
    }
    statement.finalize();
    
    callback(daos, count);
    
    delete[] daos;
}

void RouteDAO::readRouteDAOResult(unsigned int poiID, const char* query, RouteDAOsCallback callback) {
    if(poiID == 0) {
        callback(nullptr, 0);
        return; 
    }
    
    DBConnection& dbc  = *(ServiceLocator::getServiceLocator().getDBConnection());

    DBStatement statement = dbc.prepare(query, nullptr);
    statement.bindInt(1, poiID);
    statement.step();
    
    std::vector<RouteDAO> routeDAOVector;
    
    while(statement.hasNextRow()) {
        unsigned int id = statement.getColumnInt(0);
        unsigned int poiA = statement.getColumnInt(1);
        unsigned int poiB = statement.getColumnInt(2);
        unsigned int diff = statement.getColumnInt(3);
        bool bidirectional = statement.getColumnInt(4) != 0;
        bool reverse = statement.getColumnInt(5) != 0;
        unsigned int loc_id = statement.getColumnInt(6);
        
        routeDAOVector.push_back(RouteDAO());
        
        routeDAOVector.back().id = id;
        routeDAOVector.back().locationID = loc_id;
        routeDAOVector.back().setPOIA(poiA);
        routeDAOVector.back().setPOIB(poiB);
        routeDAOVector.back().setDifficulty(diff);
        routeDAOVector.back().setBidirectional(bidirectional);
        routeDAOVector.back().setReverse(reverse);
        
        statement.step();
    }
    
    callback(&routeDAOVector[0], routeDAOVector.size());
}

void RouteDAO::outgoingRouteDAOs(unsigned int poiID, RouteDAOsCallback callback) {
    const char* query = "SELECT route.id, route.poi_a, route.poi_b, route.difficulty, route.bidirected, route.reverse, "
                        "location.id, location.name "
                        "FROM route INNER JOIN location "
                        "ON location.id = route.location_id "
                        "JOIN point_of_interest AS poi "
                        "ON route.poi_a = poi.id OR route.poi_b = poi.id "
                        "WHERE poi.id = ? "
                        "AND (route.bidirected = 1 OR (route.poi_a = poi.id AND route.reverse = 0) OR (route.poi_b = poi.id AND reverse = 1))";
    
    readRouteDAOResult(poiID, query, callback);
}

void RouteDAO::incomingRouteDAOs(unsigned int poiID, RouteDAOsCallback callback) {
    const char* query = "SELECT route.id, route.poi_a, route.poi_b, route.difficulty, route.bidirected, route.reverse, "
                        "location.id, location.name "
                        "FROM route INNER JOIN location "
                        "ON location.id = route.location_id "
                        "JOIN point_of_interest AS poi "
                        "ON route.poi_a = poi.id OR route.poi_b = poi.id "
                        "WHERE poi.id = ? "
                        "AND (route.bidirected = 1 OR (route.poi_a = poi.id AND route.reverse = 1) OR (route.poi_b = poi.id AND reverse = 0))";

    
    readRouteDAOResult(poiID, query, callback);
}
