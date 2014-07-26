
#include "RouteDAO.h"
#include "../DBStatement.h"
#include "../ServiceLocator.h"

RouteDAO::RouteDAO() : needsWrite(false), poiA(0), poiB(0), id(0), difficulty(0), bidirected(false), reverse(false) {
    
}

RouteDAO::~RouteDAO() {
    
}
    
bool RouteDAO::checkValuesSet() {
    return poiA != 0 && poiB != 0;
}

bool RouteDAO::retrieve(unsigned int id) {
    this->id = id;
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    const char* query = "SELECT * FROM route WHERE id=?";
    
    DBStatement statement = dbc.prepare(query, nullptr);
    statement.bindInt(1, this->id);
    
    if(statement.step()) {
        this->poiA = statement.getColumnInt(1);
        this->poiB = statement.getColumnInt(2);
        
        this->difficulty = statement.getColumnInt(3);
        this->bidirected = statement.getColumnInt(4) == 1;
        this->reverse = statement.getColumnInt(5) == 1;
        
        statement.finalize();
        return true;
    }
    
    statement.finalize();
    return false;
}
bool RouteDAO::remove(unsigned int id) {
    this->id = id;
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    const char* query = "DELETE FROM route WHERE id=?";
    
    DBStatement statement = dbc.prepare(query, nullptr);
    statement.bindInt(1, id);
    
    int result = statement.step();
    
    return result != 0;
}

int RouteDAO::write() {
    const char* query = "INSERT INTO route (poi_a, poi_b, difficulty, bidirected, reverse) VALUES (?, ?, ?, ?, ?)";
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    DBStatement statement = dbc.prepare(query, nullptr);
    
    statement.bindInt(1, poiA);
    statement.bindInt(2, poiB);
    
    statement.bindInt(3, difficulty);
    statement.bindInt(4, (unsigned int)bidirected);
    statement.bindInt(5, (unsigned int)reverse);
    
    if(statement.step()) {
        id = dbc.lastInsertRowId();
    }
    statement.finalize();
    
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
    
    statement.step();
    statement.finalize();
    
    return id;
}


unsigned int RouteDAO::getID() const {
    return this->id;
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

bool RouteDAO::isBidrectional() const {
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

void RouteDAO::allRouteDAOs(AllRouteDAOsCallback callback) {
    callback(nullptr, 0);
}