
#include "UserDAO.h"
#include "../ServiceLocator.h"

UserDAO::UserDAO() : id(0), location(0), tag(nullptr) {}

UserDAO::~UserDAO() {
    freeTag();
}

void UserDAO::freeTag() {
    if(tag != nullptr) {
        delete[] tag;
    }
}

bool UserDAO::checkValuesSet() {
    return location > 0 && tag != nullptr;
}

bool UserDAO::retrieve(unsigned int id) {
    this->id = id;
    
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    const char* query = "SELECT user.id, user.current_location, user.tag FROM user WHERE user.id = ?";
    DBStatement statement = dbc.prepare(query, nullptr);
    statement.bindInt(1, this->id);
    
    if(statement.step()) {
        this->location = statement.getColumnInt(1);
        this->tag = (const char*)(statement.getColumnText(2));
        
        statement.finalize();
        return true;
    }
    
    statement.finalize();
    return false;
}

bool UserDAO::remove(unsigned int id) {
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    const char* query = "DELETE FROM user WHERE id = ?";
    DBStatement statement = dbc.prepare(query, nullptr);
    statement.bindInt(1, this->id);
    
    int result = statement.step();
    return result != 0;
}

int UserDAO::write() {
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    const char* query = "INSERT INTO user (tag) VALUES (?)";
    DBStatement statement = dbc.prepare(query, nullptr);
    statement.bindText(1, tag);
    
    if(statement.step()) id = dbc.lastInsertRowId();
    else id = -1;
    
    statement.finalize();
    
    return id;
}

int UserDAO::write(int id) {
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    const char* query = "UPDATE user SET current_location = ?, tag = ? WHERE id = ?";
    DBStatement statement = dbc.prepare(query, nullptr);
    statement.bindInt(1, this->location);
    statement.bindText(2, this->tag);
    
    statement.step();
    statement.finalize();
    
    return id;
}

unsigned int UserDAO::getID() const {
    return this->id;
}

unsigned int UserDAO::getLocation() const {
    return this->location;
}

const char* UserDAO::getTag() const {
    return this->tag;
}

void UserDAO::setLocation(unsigned int location) {
    this->location = location;
}

void UserDAO::setTag(const char* tag) {
    freeTag();
    this->tag = tag;
}

void UserDAO::allUserDAOs(AllUserDAOsCallback callback) {
    DBConnection& dbc = *(ServiceLocator::getServiceLocator().getDBConnection());
    const char* query = "SELECT COUNT() FROM user";
    DBStatement countStatement = dbc.prepare(query, nullptr);
    
    countStatement.step();
    int count = countStatement.getColumnInt(0);
    countStatement.finalize();
    if(count <= 0) {
        callback(nullptr, 0);
        return;
    }

    UserDAO* daos = new UserDAO[count];
    
    query = "SELECT user.id, user.current_location, user.tag FROM user";
    DBStatement statement = dbc.prepare(query, nullptr);
    
    for(int i = 0; i < count; i++) {
        statement.step();
        
        unsigned int id = statement.getColumnInt(0);
        unsigned int currentLocation = statement.getColumnInt(1);
        const char* tag = (const char*)(statement.getColumnText(2));
        
        daos[i].id = id;
        daos[i].setLocation(currentLocation);
        daos[i].setTag(tag);
    }
    statement.finalize();
    
    callback(daos, count);
    
    delete[] daos;
}
