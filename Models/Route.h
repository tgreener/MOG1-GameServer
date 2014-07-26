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

typedef struct RouteAttribs {
    unsigned int poiAID = 0;
    unsigned int poiBID = 0;
    unsigned int difficulty = 0;
    bool bidirectional = false;
    bool reverse = false;
} RouteAttributes;

typedef std::function<void(Route*, int)> AllRoutesCallback;

class Route : public AbstractModel {
private:
    RouteDAO dao;
    
public:
    Route();
    Route(RouteAttributes& attribs);
    Route(unsigned int id);
    ~Route();
    
    unsigned int getID() const;
    PointOfInterest getEndpointA() const;
    PointOfInterest getEndpointB() const;
    unsigned int getDifficulty() const;
    bool isBidirectional() const;
    bool isReverse() const;
    
    void setEndpointA(unsigned int poiID);
    void setEndpointB(unsigned int poiID);
    void setDifficulty(unsigned int dif);
    void setBidirectional(bool dir);
    void setReverse(bool rev);
    
    virtual void bark() const;
    virtual void save();
    virtual unsigned int serializedLength() const;
    virtual void serialize(unsigned char* buffer) const;
    virtual bool remove() ;
    
    static void getAllRoutes(AllRoutesCallback callback);
    static RouteAttributes extractAttributes(char* bytes, int length);
};

#endif	/* ROUTE_H */

