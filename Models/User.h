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

typedef struct UserAtr {
    unsigned int location;
    const char* tag;
} UserAttributes;

class User : public AbstractModel{
private:
    
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
};

#endif	/* USER_H */

