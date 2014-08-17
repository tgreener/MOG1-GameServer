
#include <string>
#include <cstring>

#include "User.h"
#include "../ServiceLocator.h"
#include "../StringUtil.h"

User::User() {}

User::User(UserAttributes& attribs) {
    dao.setLocation(attribs.location);
    dao.setTag(attribs.tag);
}

User::User(unsigned int id) {
    if(!dao.retrieve(id)) {
        throw "Error loading User.";
    }
}

unsigned int User::getID() const {
    return dao.getID();
}

unsigned int User::getLocationID() const {
    return dao.getLocation();
}

Location User::getLocation() const {
    return Location(dao.getLocation());
}

const char* User::getTag() const {
    return dao.getTag();
}

void User::setLocation(unsigned int loc) {
    dao.setLocation(loc);
}

void User::setLocation(const Location& loc) {
    dao.setLocation(loc.getID());
}

void User::setTag(const char* tag) {
    dao.setTag(tag);
}

void User::bark() const {
    std::string str = "User: {\n\t";
    str += "id: " + std::to_string(dao.getID());
    str += "\n\tcurrent_location: " + std::to_string(dao.getLocation());
    str += "\n\ttag: ";
    str += dao.getTag();
    str += "\n}";
    
    ServiceLocator::getServiceLocator().sendMessageToClient(str.c_str());
}

void User::save() {
    if(dao.getID()) {
        dao.write(dao.getID());
    }
    else {
        dao.write();
    }
}

unsigned int User::serializedLength() const {
    int idSize = sizeof(unsigned int);
    int locationIDSize = sizeof(unsigned int);
    int tagSize = strlen(dao.getTag()) + 1; // Plus null character    
    return idSize + locationIDSize + tagSize; 
}

void User::serialize(unsigned char* buffer) const {
    memset(buffer, 0, serializedLength());
    
    buffer[0] = 1;
    
    unsigned int id = dao.getID();
    unsigned int loc = dao.getLocation();
    
    unsigned int intSize = sizeof(unsigned int);
    
    memcpy(buffer + (intSize * 0), &id, intSize);
    memcpy(buffer + (intSize * 1), &loc, intSize);
    memcpy(buffer + (intSize * 2), dao.getTag(), strlen(dao.getTag()));
    
    buffer[serializedLength() - 1] = 0xff;
}

bool User::remove() {
    return dao.remove(dao.getID());
}

void User::getAllUsers(AllUsersCallback callback) {
    UserDAO::allUserDAOs([&](UserDAO* daos, int count) -> void {
        User* users = new User[count];
        
        for(int i = 0; i < count; i++) {
            users[i].dao = daos[i];
        }
        
        callback(users, count);
        delete[] users;
    });
}

void User::getAllUsers(AllModelsCallback callback) {
    UserDAO::allUserDAOs([&](UserDAO* daos, int count) -> void {
        User* users = new User[count];
        AbstractModel** ups = new AbstractModel*[count];
        
        for(int i = 0; i < count; i++) {
            users[i].dao = daos[i];
            ups[i] = &users[i];
        }
        
        callback(ups, count);
        delete[] ups;
        delete[] users;
    });
}

ByteInterpreterFunction User::getFetchFunction() {
    return [](const char* bytes, int length) -> void {
        if(length >= 3) {
            try {
                unsigned int id = *((unsigned int*)(bytes + 2));
                User user(id);
                AbstractModel::fetchModel(user);
            }
            catch (const char* e) {
                ServiceLocator::getServiceLocator().sendMessageToClient(e);
            }
            
            return;
        }
        AbstractModel::insufficientDataMessage();
    };
}

ByteInterpreterFunction User::getAddFunction() {
    return [](const char* bytes, int length) -> void {
        AbstractModel::respondWithID(User::createUser(bytes, length));
    };
}

ByteInterpreterFunction User::getDeleteFunction() {
    return [](const char* bytes, int length) -> void {
        if(length >= 3) {
            try {
                unsigned int id = bytes[2];
                User user(id);
                AbstractModel::deleteModel(user);
            }
            catch (const char* e) {
                ServiceLocator::getServiceLocator().sendMessageToClient(e);
            }
        }
        AbstractModel::insufficientDataMessage();
    };
}

ByteInterpreterFunction User::getFetchAllFunction() {
    return [](const char* bytes, int length) -> void {
        User::getAllUsers(AbstractModel::getAllModelsCallback);
    };
}

UserAttributes User::extractAttributes(const char* bytes, int length) {
    UserAttributes attribs;
    if(length > sizeof(unsigned int)) {
        attribs.location = *((unsigned int*)(bytes));
        attribs.tag = bytes + sizeof(unsigned int);
    }
    
    return attribs;
}

int User::createUser(const char* bytes, int length) {
    if((unsigned char)bytes[length - 1] == 0xff) {
        UserAttributes attribs = User::extractAttributes(bytes + 2, length - 2);
        if(attribs.location == 0) return -1;
        
        User newUser(attribs);
        newUser.save();
        
        return newUser.getID();
    }
    return -1;
}
