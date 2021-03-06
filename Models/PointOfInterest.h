/* 
 * File:   PointOfInterest.h
 * Author: toddgreener
 *
 * Created on May 28, 2014, 12:59 PM
 */

#ifndef POINTOFINTEREST_H
#define	POINTOFINTEREST_H

#include "AbstractModel.h"
#include "../DAOs/PointsOfInterestDAO.h"
#include "Route.h"
#include "User.h"

typedef struct PointOfInterestAttributes {
    const unsigned char* name;
    int soil;
    int stone;
    int wilderness;
} POIAttrib;

class PointOfInterest : public AbstractModel {
private:
    unsigned int id;
    bool needsUpdate;
    
    PointsOfInterestDAO dao;
    
    void load();
    void setDAO(const PointsOfInterestDAO& dao);
    
    static int createPointOfInterest(const char* bytes, int length);
    static void getAllPOIs(AllModelsCallback callback);
public:
    PointOfInterest();
    PointOfInterest(unsigned int id);
    PointOfInterest(POIAttrib& attribs);
    PointOfInterest(const PointOfInterest& that);
    PointOfInterest(PointOfInterest&& that);
    ~PointOfInterest();
    
    PointOfInterest& operator=(PointOfInterest&& that);
    
    virtual unsigned int getID() const;
    virtual const char* getName() const;
    
    unsigned int getAttributeSoil() const;
    unsigned int getAttributeStone() const;
    unsigned int getAttributeWilderness() const;
    unsigned int getPopulation() const;
    
    unsigned int getLocationID() const;
    
    void setAttributeSoil(int s);
    void setAttributeStone(int s);
    void setAttributeWilderness(int w);
    void setPopulation(unsigned int pop);
    
    virtual void bark() const;
    virtual unsigned int serializedLength() const;
    virtual void serialize(unsigned char* buffer) const;
    virtual void save();
    virtual bool remove();
    
    virtual void onUserEnter(const User& user);
    virtual void onUserExit(const User& user);

    void getOutgoingRoutes(RoutesCallback callback);
    void getIncomingRoutes(RoutesCallback callback);
    
    static void getAllPOIs(PointOfInterestCallback callback);
    
    static ByteInterpreterFunction getFetchFunction();
    static ByteInterpreterFunction getAddFunction();
    static ByteInterpreterFunction getDeleteFunction();
    static ByteInterpreterFunction getFetchAllFunction();
};

#endif	/* POINTOFINTEREST_H */

