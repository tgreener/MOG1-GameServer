/* 
 * File:   Route.h
 * Author: toddgreener
 *
 * Created on July 12, 2014, 12:13 PM
 */

#ifndef ROUTE_H
#define	ROUTE_H

#include "Location.h"
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

typedef std::function<void(Route*, unsigned int count)> AllRoutesCallback;

class Route : public Location {
private:
    RouteDAO dao;
    
    static int createRoute(const char* bytes, int length);
    static void getAllRoutes(AllModelsCallback callback);
    static RouteAttributes extractAttributes(const char* bytes, int length);
    
public:
    Route();
    Route(RouteAttributes& attribs);
    Route(unsigned int id);
    virtual ~Route();
    
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
    virtual bool remove();
    
    static void getAllRoutes(AllRoutesCallback callback);
    
    static ByteInterpreterFunction getFetchFunction();
    static ByteInterpreterFunction getAddFunction();
    static ByteInterpreterFunction getDeleteFunction();
    static ByteInterpreterFunction getFetchAllFunction();
};

#endif	/* ROUTE_H */

