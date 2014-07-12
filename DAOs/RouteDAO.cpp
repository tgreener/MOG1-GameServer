
#include "RouteDAO.h"

RouteDAO::RouteDAO() {
    
}

RouteDAO::~RouteDAO() {
    
}
    
bool RouteDAO::checkValuesSet() {
    return false;
}

bool RouteDAO::retrieve(unsigned int id) {
    return false;
}
bool RouteDAO::remove(unsigned int id) {
    return false;
}

int RouteDAO::write() {
    return -1;
}

int RouteDAO::write(unsigned int id) {
    return -1;
}


unsigned int RouteDAO::getID() {
    return 0;
}

unsigned int RouteDAO::getPOIA() {
    return 0;
}

unsigned int RouteDAO::getPOIB() {
    return 0;
}

void RouteDAO::setPOIA(unsigned int a) {
    
}

void RouteDAO::setPOIb(unsigned int b) {
    
}

void RouteDAO::allRouteDAOs(AllRoutesCallback callback) {
    callback(nullptr, 0);
}