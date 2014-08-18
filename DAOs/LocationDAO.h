/* 
 * File:   LocationDAO.h
 * Author: toddgreener
 *
 * Created on August 10, 2014, 12:53 AM
 */

#ifndef LOCATIONDAO_H
#define	LOCATIONDAO_H

#include "AbstractDAO.h"

class LocationDAO : public AbstractDAO {
private:
    unsigned int id;
    bool route;
    bool poi;
    const char* name;
    
protected:
    void freeName();
    
public:
    LocationDAO();
    virtual ~LocationDAO();
    
    unsigned int getID() const;
    const char* getName() const;
    bool isRoute() const;
    bool isPOI() const;  
    
    void setName(const char* name);
    void setIsRoute(bool isRoute);
    void setIsPOI(bool isPOI);
    
    unsigned int getPOIID() const;
    unsigned int getRouteID() const;
    
    virtual bool retrieve(unsigned int id);
    virtual bool remove(unsigned int id);
    virtual int write();
    virtual int write(int id);
};

#endif	/* LOCATIONDAO_H */

