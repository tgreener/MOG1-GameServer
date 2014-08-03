/* 
 * File:   PointOfInterest.h
 * Author: toddgreener
 *
 * Created on May 28, 2014, 12:59 PM
 */

#ifndef POINTOFINTEREST_H
#define	POINTOFINTEREST_H

#include "Location.h"
#include "../DAOs/PointsOfInterestDAO.h"

typedef struct PointOfInterestAttributes {
    const unsigned char* name;
    int soil;
    int stone;
    int wilderness;
} POIAttrib;

class PointOfInterest;

typedef std::function<void(PointOfInterest*, unsigned int)> PointOfInterestCallback;

class PointOfInterest : public Location {
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
    ~PointOfInterest();
    
    unsigned int getID() const;
    const unsigned char* getName() const;
    
    unsigned int getAttributeSoil() const;
    unsigned int getAttributeStone() const;
    unsigned int getAttributeWilderness() const;
    
    void setAttributeSoil(int s);
    void setAttributeStone(int s);
    void setAttributeWilderness(int w);
    
    virtual void bark() const;
    virtual unsigned int serializedLength() const;
    virtual void serialize(unsigned char* buffer) const;
    virtual void save();
    virtual bool remove();
    
    static void getAllPOIs(PointOfInterestCallback callback);
    
    static ByteInterpreterFunction getFetchFunction();
    static ByteInterpreterFunction getAddFunction();
    static ByteInterpreterFunction getDeleteFunction();
    static ByteInterpreterFunction getFetchAllFunction();
};

#endif	/* POINTOFINTEREST_H */

