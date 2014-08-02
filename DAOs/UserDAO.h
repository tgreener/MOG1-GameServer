/* 
 * File:   UserDAO.h
 * Author: toddgreener
 *
 * Created on August 2, 2014, 11:25 AM
 */

#ifndef USERDAO_H
#define	USERDAO_H

#include "AbstractDAO.h"
#include <functional>

class UserDAO;

typedef std::function<void(UserDAO*, int)> AllUserDAOsCallback;

class UserDAO : public AbstractDAO {
private:
    unsigned int id;
    unsigned int location;
    const char* tag;
    
    bool checkValuesSet();
    
public:
    UserDAO();
    virtual ~UserDAO() {}
    
    virtual bool retrieve(unsigned int id);
    virtual bool remove(unsigned int id);
    virtual int write();
    virtual int write(int id);

    unsigned int getID() const;
    unsigned int getLocation() const;
    const char* getTag() const;
    
    void setLocation(unsigned int location);
    void setTag(const char* tag);
    
    static void allUserDAOs(AllUserDAOsCallback callback);
};

#endif	/* USERDAO_H */

