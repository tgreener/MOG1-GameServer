/* 
 * File:   UserDAO.h
 * Author: toddgreener
 *
 * Created on August 2, 2014, 11:25 AM
 */

#ifndef USERDAO_H
#define	USERDAO_H

#include "AbstractDAO.h"

class UserDAO : public AbstractDAO {
private:
    unsigned int id;
    unsigned int location;
    const char* tag;
    
public:
    virtual bool retrieve(unsigned int id);
    virtual bool remove(unsigned int id);
    virtual int write();
    virtual int write(int id);

    unsigned int getID() const;
    unsigned int getLocation() const;
    const char* getTag() const;
    
    void setLocation(unsigned int location);
    void setTag(const char* tag);
};

#endif	/* USERDAO_H */

