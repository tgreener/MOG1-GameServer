
#include "PointsOfInterestDAO.h"
#include "../ServiceLocator.h"
#include "../StringUtil.h"

#include <stdio.h>

PointsOfInterestDAO::PointsOfInterestDAO() : name(NULL), soil(0), stone(0), wilderness(0) {
}

PointsOfInterestDAO::PointsOfInterestDAO(int id, const unsigned char* name, int soil, int stone, int wild) : 
id(id), name(NULL), soil(soil), stone(stone), wilderness(wild) {
    this->name = heapifyStringUnsigned(name);
}

PointsOfInterestDAO::~PointsOfInterestDAO() {
    freeName();
}

void PointsOfInterestDAO::freeName() {
    if(name != NULL) {
        delete[] name;
    }
}

bool PointsOfInterestDAO::checkValuesSet() {
    return name != NULL && soil > 0 && stone > 0 && wilderness > 0 &&
            soil <= 10 && stone <= 10 && wilderness <= 10;
}

bool PointsOfInterestDAO::retrieve(unsigned int id) {
    this->id = id;
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    const char* query;
    
    query = "SELECT location.id, location.name, point_of_interest.soil, point_of_interest.stone, point_of_interest.wilderness "
            "FROM location INNER JOIN point_of_interest "
            "ON location.id = point_of_interest.poi " 
            "WHERE location.id = ? AND location.is_point_of_interest <> 0";
    DBStatement statement = dbc->prepare(query, NULL);
    statement.bindInt(1, id);
    
    if(statement.step() && statement.getColumnText(1) != NULL) {
        name = heapifyStringUnsigned(statement.getColumnText(1));
        soil = statement.getColumnInt(2);
        stone = statement.getColumnInt(3);
        wilderness = statement.getColumnInt(4);
        
        statement.finalize();
        return true;
    }
    
    statement.finalize();
    return false;
}

bool PointsOfInterestDAO::remove(unsigned int id) {
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    const char* query = "DELETE FROM location WHERE id = ? AND is_point_of_interest <> 0";
    
    DBStatement statement = dbc->prepare(query, NULL);
    statement.bindInt(1, id);
    
    int result = statement.step();
    
    return result != 0;
}

int PointsOfInterestDAO::write() {
    const char* query = "INSERT INTO location (name, is_point_of_interest) VALUES (?, 1)";
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    DBStatement statement1 = dbc->prepare(query, NULL);
    statement1.bindText(1, (const char*)name);
    
    bool result = statement1.step();
    statement1.finalize();
    
    if(result) {
        id = dbc->lastInsertRowId();
    }
    else {
        return -1;
    }
    
    query = "insert into point_of_interest (poi, soil, stone, wilderness) values ("
            "?, ?, ?, ?);";
    DBStatement statement3 = dbc->prepare(query, NULL);
    
    statement3.bindInt(1, id);
    statement3.bindInt(2, soil);
    statement3.bindInt(3, stone);
    statement3.bindInt(4, wilderness);
    
    queryResult = statement3.step();
    statement3.finalize();
    
    if(!queryResult) {
        return -1;
    }
    
    return id;
}

int PointsOfInterestDAO::write(int id) {
    this->id = id;
    
    const char* query = "UPDATE point_of_interest "
    "SET soil = ?, stone = ?, wilderness = ? "
    "WHERE poi = ?;";
    
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    DBStatement statement = dbc->prepare(query, NULL);
    
    statement.bindInt(1, soil);
    statement.bindInt(2, stone);
    statement.bindInt(3, wilderness);
    statement.bindInt(4, id);
    
    int result = statement.step();
    if(!result) {
        return -1;
    }
    
    return id;
}

unsigned int PointsOfInterestDAO::getSoil() const {
    return soil;
}

unsigned int PointsOfInterestDAO::getStone() const {
    return stone;
}

unsigned int PointsOfInterestDAO::getWilderness() const {
    return wilderness;
}

const unsigned char* PointsOfInterestDAO::getName() const {
    return name;
}

unsigned int PointsOfInterestDAO::getID() const {
    return id;
}

void PointsOfInterestDAO::setName(const unsigned char* n) {
    freeName();
    name = heapifyStringUnsigned(n);
}

void PointsOfInterestDAO::setSoil(unsigned int sl) {
    soil = sl;
}

void PointsOfInterestDAO::setStone(unsigned int st) {
    stone = st;
}

void PointsOfInterestDAO::setWilderness(unsigned int wld) {
    wilderness = wld;
}

void PointsOfInterestDAO::allPOIDAOs(std::function<void(PointsOfInterestDAO*, int)> callback) {
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    
    const char* countQuery = "SELECT COUNT() FROM point_of_interest";
    DBStatement countStatement = dbc->prepare(countQuery, NULL);
    
    countStatement.step();
    int count = countStatement.getColumnInt(0);
    if(count <= 0) {
        throw "No POIs to load";
        return;
    }
    countStatement.finalize();
    
    PointsOfInterestDAO* daos = new PointsOfInterestDAO[count];
    
    const char* poisQuery = "SELECT location.id, location.name, point_of_interest.soil, point_of_interest.stone, point_of_interest.wilderness "
            "FROM location INNER JOIN point_of_interest "
            "ON location.id = point_of_interest.poi " 
            "WHERE location.is_point_of_interest <> 0";
    DBStatement statement = dbc->prepare(poisQuery, NULL);
    
    for(int i = 0; i < count; i++) {
        statement.step();
        
        int id = statement.getColumnInt(0);
        const unsigned char* name = statement.getColumnText(1);
        int soil = statement.getColumnInt(2);
        int stone = statement.getColumnInt(3);
        int wilderness = statement.getColumnInt(4);
        
        daos[i].id = id;
        daos[i].setName(name);
        daos[i].setSoil(soil);
        daos[i].setStone(stone);
        daos[i].setWilderness(wilderness);
    }
    statement.finalize();

    callback(daos, count);

    delete[] daos;
}
