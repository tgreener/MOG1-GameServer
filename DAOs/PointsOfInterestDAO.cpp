
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
    
    query = "SELECT points_of_interest.*, poi_attributes.soil, poi_attributes.stone, poi_attributes.wilderness "
            "FROM points_of_interest INNER JOIN poi_attributes "
            "ON points_of_interest.id = poi_attributes.poi WHERE id = ?";
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
    const char* query;
    
    query = "DELETE FROM points_of_interest WHERE id = ?";
    DBStatement statement = dbc->prepare(query, NULL);
    statement.bindInt(1, id);
    
    int result = statement.step();
    
    return result != 0;
}

int PointsOfInterestDAO::write() {
    const char* query = "INSERT INTO points_of_interest (name) VALUES (?)";
    DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
    DBStatement statement1 = dbc->prepare(query, NULL);
    statement1.bindText(1, (const char*)name);
    
    bool queryResult = statement1.step();
    statement1.finalize();
    
    if(!queryResult) {
        return -1;
    }
    
    int id;
    query = "SELECT id from points_of_interest WHERE id NOT IN (SELECT poi FROM poi_attributes);";
    DBStatement statement2 = dbc->prepare(query, NULL);
    queryResult = statement2.step();
    if(!queryResult) {
        return -1;
    }
    id = statement2.getColumnInt(0);
    statement2.finalize();
    
    query = "insert into poi_attributes (poi, soil, stone, wilderness) values ("
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
    
    const char* query = "UPDATE poi_attributes "
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
    
    const char* countQuery = "SELECT COUNT() FROM points_of_interest";
    DBStatement countStatement = dbc->prepare(countQuery, NULL);
    
    countStatement.step();
    int count = countStatement.getColumnInt(0);
    if(count <= 0) {
        throw "No POIs to load";
        return;
    }
    countStatement.finalize();
    
    PointsOfInterestDAO* daos = new PointsOfInterestDAO[count];
    
    const char* poisQuery = "SELECT points_of_interest.*, poi_attributes.soil, poi_attributes.stone, poi_attributes.wilderness "
            "FROM points_of_interest INNER JOIN poi_attributes "
            "ON points_of_interest.id = poi_attributes.poi";
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
