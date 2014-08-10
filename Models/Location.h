/* 
 * File:   Location.h
 * Author: toddgreener
 *
 * Created on July 27, 2014, 3:52 PM
 */

#ifndef LOCATION_H
#define	LOCATION_H

#include "AbstractModel.h"
#include "../DAOs/LocationDAO.h"
#include "PointOfInterest.h"
#include "Route.h"

class Location : public AbstractModel {
private:
    LocationDAO dao;
protected:
    
public:
    Location();
    Location(unsigned int id);
    
    virtual unsigned int getID() const;
    virtual const char* getName() const;
    virtual bool isRoute() const;
    virtual bool isPOI() const;  

    virtual void setName(const char* name);
    
    PointOfInterest getPOI();
    Route getRoute();
    
    virtual void save();
    virtual bool remove();
    virtual unsigned int serializedLength() const;
    virtual void serialize(unsigned char* buffer) const;
};

#endif	/* LOCATION_H */

