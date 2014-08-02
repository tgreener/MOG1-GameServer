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
#include "DAOs/UserDAO.h"

class User;

typedef struct UserAtr {
    unsigned int location;
    const char* tag;
} UserAttributes;

typedef std::function<void(User*, unsigned int)> AllUsersCallback;

class User : public AbstractModel{
private:
    UserDAO dao;
    
    static void getAllUsers(AllModelsCallback calback);
public:
    User();
    User(UserAttributes& attribs);
    User(unsigned int id);
    virtual ~User() {}
    
    unsigned int getID() const;
    unsigned int getLocation() const;
    const char* getTag() const;
    
    void setLocation(unsigned int loc);
    void setLocation(const Location& loc);
    void setTag(const char* tag);
    
    virtual void bark() const;
    virtual void save();
    virtual unsigned int serializedLength() const;
    virtual void serialize(unsigned char* buffer) const;
    virtual bool remove();
    
    static void getAllUsers(AllUsersCallback callback);
    
    static ByteInterpreterFunction getFetchFunction();
    static ByteInterpreterFunction getAddFunction();
    static ByteInterpreterFunction getDeleteFunction();
    static ByteInterpreterFunction getFetchAllFunction();
};

#endif	/* USER_H */

