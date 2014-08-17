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

typedef std::function<void(Route*, unsigned int count)> RoutesCallback;

class Route : public AbstractModel{
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
    
    virtual unsigned int getID() const;
    unsigned int getLocationID() const;
    unsigned int getEndpointAID() const;
    unsigned int getEndpointBID() const;
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
    
    virtual void onUserEnter(const User& user) override;
    virtual void onUserExit(const User& user) override;
    
    static void getAllRoutes(RoutesCallback callback);
    
    static ByteInterpreterFunction getFetchFunction();
    static ByteInterpreterFunction getAddFunction();
    static ByteInterpreterFunction getDeleteFunction();
    static ByteInterpreterFunction getFetchAllFunction();
};

#endif	/* ROUTE_H */

