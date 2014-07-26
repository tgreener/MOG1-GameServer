/* 
 * File:   Route.h
 * Author: toddgreener
 *
 * Created on July 12, 2014, 12:13 PM
 */

#ifndef ROUTE_H
#define	ROUTE_H

#include "AbstractModel.h"
#include "../DAOs/RouteDAO.h"
#include "PointOfInterest.h"

class Route;

typedef std::function<void(Route*, int)> AllRoutesCallback;

class Route : public AbstractModel {
private:
    RouteDAO dao;
    
public:
    Route();
    Route(unsigned int id);
    ~Route();
    
    unsigned int getID() const;
    PointOfInterest getEndpointA() const;
    PointOfInterest getEndpointB() const;
    unsigned int getDifficulty() const;
    bool isBidirectional() const;
    
    void setEndpointA(unsigned int poiID);
    void setEndpointB(unsigned int poiID);
    void setDifficulty(unsigned int dif);
    void setBidirectional(bool dir);
    
    virtual void bark() const;
    virtual void save();
    virtual unsigned int serializedLength() const;
    virtual void serialize(unsigned char* buffer) const;
    virtual bool remove() ;
    
    static void getAllRoutes(AllRoutesCallback callback);
};

#endif	/* ROUTE_H */

