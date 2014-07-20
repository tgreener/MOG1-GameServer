
#include "RouteDAO.h"
#include "../DBStatement.h"
#include "../ServiceLocator.h"

RouteDAO::RouteDAO() : needsWrite(false), poiA(0), poiB(0), id(0) {
    
}

RouteDAO::~RouteDAO() {
    
}
    
bool RouteDAO::checkValuesSet() {
    return poiA != 0 || poiB != 0;
}

bool RouteDAO::retrieve(unsigned int id) {
    this->id = id;
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    const char* query = "SELECT * FROM route WHERE id=?";
    
    DBStatement statement = dbc.prepare(query, nullptr);
    statement.bindInt(1, this->id);
    
    if(statement.step()) {
        poiA = statement.getColumnInt(1);
        poiB = statement.getColumnInt(2);
        
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
    const char* query = "INSERT INTO route (poi_a, poi_b) VALUES (?, ?)";
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    DBStatement statement = dbc.prepare(query, nullptr);
    
    statement.bindInt(1, poiA);
    statement.bindInt(2, poiB);
    
    statement.step();
    statement.finalize();
    
    id = dbc.lastInsertRowId();
    
    return id;
}

int RouteDAO::write(int id) {
    const char* query = "UPDATE route SET poi_a = ?, poi_b = ? WHERE id = ?";
    
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    DBStatement statement = dbc.prepare(query, nullptr);
    
    statement.bindInt(1, poiA);
    statement.bindInt(2, poiB);
    statement.bindInt(3, id);
    
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

void RouteDAO::setPOIA(unsigned int a) {
    poiA = a;
}

void RouteDAO::setPOIB(unsigned int b) {
    poiB = b;
}

void RouteDAO::allRouteDAOs(AllRouteDAOsCallback callback) {
    callback(nullptr, 0);
}