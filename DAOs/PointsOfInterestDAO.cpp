
#include "PointsOfInterestDAO.h"
#include "../ServiceLocator.h"
#include "../StringUtil.h"

#include <stdio.h>
#include <vector>

PointsOfInterestDAO::PointsOfInterestDAO() : name(nullptr), soil(0), stone(0), wilderness(0) {
}

PointsOfInterestDAO::PointsOfInterestDAO(int id, const unsigned char* name, int soil, int stone, int wild) : 
id(id), name(nullptr), soil(soil), stone(stone), wilderness(wild) {
    this->name = heapifyStringUnsigned(name);
}

PointsOfInterestDAO::~PointsOfInterestDAO() {
    freeName();
}

void PointsOfInterestDAO::freeName() {
    if(name != nullptr) {
        delete[] name;
    }
}

bool PointsOfInterestDAO::checkValuesSet() {
    return name != nullptr && soil > 0 && stone > 0 && wilderness > 0 &&
            soil <= 10 && stone <= 10 && wilderness <= 10;
}

bool PointsOfInterestDAO::retrieve(unsigned int id) {
    this->id = id;
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    const char* query;
    
    query = "SELECT point_of_interest.id, location.name, "
            "point_of_interest.soil, point_of_interest.stone, point_of_interest.wilderness, location.id, point_of_interest.population "
            "FROM location INNER JOIN point_of_interest "
            "ON location.id = point_of_interest.location_id " 
            "WHERE point_of_interest.id = ? AND location.is_point_of_interest <> 0";
    DBStatement statement = dbc->prepare(query, nullptr);
    statement.bindInt(1, id);
    
    if(statement.step() && statement.getColumnText(1) != nullptr) {
        name = heapifyStringUnsigned(statement.getColumnText(1));
        soil = statement.getColumnInt(2);
        stone = statement.getColumnInt(3);
        wilderness = statement.getColumnInt(4);
        locationID = statement.getColumnInt(5);
        population = statement.getColumnInt(6);
        
        statement.finalize();
        return true;
    }
    
    statement.finalize();
    return false;
}

bool PointsOfInterestDAO::remove(unsigned int id) {
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    
    const char* poiQuery = "SELECT location_id FROM point_of_interest WHERE id = ?";
    DBStatement poiStatement = dbc->prepare(poiQuery, nullptr);
    poiStatement.bindInt(1, id);
    
    int locationID;
    if(poiStatement.step()) {
        locationID = poiStatement.getColumnInt(0);
    }
    else return false;
    
    const char* query = "DELETE FROM location WHERE id = ? AND is_point_of_interest <> 0";
    
    DBStatement statement = dbc->prepare(query, nullptr);
    statement.bindInt(1, locationID);
    
    int result = statement.step();
    
    return result != 0;
}

int PointsOfInterestDAO::write() {
    const char* query = "INSERT INTO location (name, is_point_of_interest) VALUES (?, 1)";
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    DBStatement statement1 = dbc->prepare(query, nullptr);
    statement1.bindText(1, (const char*)name);
    
    bool result = statement1.step();
    statement1.finalize();
    
    int locationID;
    if(result) {
        locationID = dbc->lastInsertRowId();
    }
    else {
        return -1;
    }
    
    query = "INSERT INTO point_of_interest (location_id, soil, stone, wilderness) VALUES ("
            "?, ?, ?, ?);";
    DBStatement statement3 = dbc->prepare(query, nullptr);
    
    statement3.bindInt(1, locationID);
    statement3.bindInt(2, soil);
    statement3.bindInt(3, stone);
    statement3.bindInt(4, wilderness);
    
    result = statement3.step();
    statement3.finalize();
    
    if(!result) {
        return -1;
    }
    
    id = dbc->lastInsertRowId();
    
    return id;
}

int PointsOfInterestDAO::write(int id) {
    this->id = id;
    
    const char* query = "UPDATE point_of_interest "
    "SET soil = ?, stone = ?, wilderness = ?, population = ? "
    "WHERE id = ?;";
    
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    DBStatement statement = dbc->prepare(query, nullptr);
    
    statement.bindInt(1, soil);
    statement.bindInt(2, stone);
    statement.bindInt(3, wilderness);
    statement.bindInt(4, population);
    statement.bindInt(5, id);
    
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

unsigned int PointsOfInterestDAO::getLocationID() const {
    return locationID;
}

unsigned int PointsOfInterestDAO::getPopulation() const {
    return population;
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

void PointsOfInterestDAO::setLocation(unsigned int loc) {
    locationID = loc;
}

void PointsOfInterestDAO::setPopulation(unsigned int pop) {
    population = pop;
}

void PointsOfInterestDAO::allPOIDAOs(std::function<void(PointsOfInterestDAO*, int)> callback) {
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    
    const char* countQuery = "SELECT COUNT() FROM point_of_interest";
    DBStatement countStatement = dbc->prepare(countQuery, nullptr);
    
    countStatement.step();
    int count = countStatement.getColumnInt(0);
    countStatement.finalize();
    if(count <= 0) {
        callback(nullptr, 0);
        return;
    }
    
    PointsOfInterestDAO* daos = new PointsOfInterestDAO[count];
    
    const char* poisQuery = 
            "SELECT point_of_interest.id, location.name, "
            "point_of_interest.soil, point_of_interest.stone, point_of_interest.wilderness, location.id, point_of_interest.population "
            "FROM location INNER JOIN point_of_interest "
            "ON location.id = point_of_interest.location_id " 
            "WHERE location.is_point_of_interest <> 0";
    DBStatement statement = dbc->prepare(poisQuery, nullptr);
    
    for(int i = 0; i < count; i++) {
        statement.step();
        
        int id = statement.getColumnInt(0);
        const unsigned char* name = statement.getColumnText(1);
        int soil = statement.getColumnInt(2);
        int stone = statement.getColumnInt(3);
        int wilderness = statement.getColumnInt(4);
        int location = statement.getColumnInt(5);
        int population = statement.getColumnInt(6);
        
        daos[i].id = id;
        daos[i].setName(name);
        daos[i].setSoil(soil);
        daos[i].setStone(stone);
        daos[i].setWilderness(wilderness);
        daos[i].setLocation(location);
        daos[i].setPopulation(population);
    }
    statement.finalize();
    
    callback(daos, count);

    delete[] daos;
}
