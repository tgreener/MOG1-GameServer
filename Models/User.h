/* 
 * File:   User.h
 * Author: toddgreener
 *
 * Created on August 2, 2014, 11:24 AM
 */

#ifndef USER_H
#define	USER_H

#include "AbstractModel.h"
#include "Location.h"
#include "../DAOs/UserDAO.h"

typedef struct UserAtr {
    unsigned int location = 0;
    const char* tag = nullptr;
} UserAttributes;

class User : public AbstractModel{
private:
    UserDAO dao;
    
    static int createUser(const char* bytes, int length);
    static void getAllUsers(AllModelsCallback callback);
    static UserAttributes extractAttributes(const char* bytes, int length);
public:
    User();
    User(UserAttributes& attribs);
    User(unsigned int id);
    virtual ~User() {}
    
    unsigned int getID() const;
    unsigned int getLocationID() const;
    Location getLocation() const;
    const char* getTag() const;
    
    void setLocation(unsigned int loc);
    void setLocation(const Location& loc);
    void setTag(const char* tag);
    
    virtual void bark() const;
    virtual void save();
    virtual unsigned int serializedLength() const;
    virtual void serialize(unsigned char* buffer) const;
    virtual bool remove();
    
    static void getAllUsers(UsersCallback callback);
    
    static ByteInterpreterFunction getFetchFunction();
    static ByteInterpreterFunction getAddFunction();
    static ByteInterpreterFunction getDeleteFunction();
    static ByteInterpreterFunction getFetchAllFunction();
};

#endif	/* USER_H */

