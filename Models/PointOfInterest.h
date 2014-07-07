/* 
 * File:   PointOfInterest.h
 * Author: toddgreener
 *
 * Created on May 28, 2014, 12:59 PM
 */

#ifndef POINTOFINTEREST_H
#define	POINTOFINTEREST_H

#include <functional>
#include "AbstractModel.h"
#include "../DAOs/PointsOfInterestDAO.h"

typedef struct PointOfInterestAttributes {
    const unsigned char* name;
    int soil;
    int stone;
    int wilderness;
} POIAttrib;

class PointOfInterest : public AbstractModel{
private:
    unsigned int id;
    bool needsUpdate;
    
    PointsOfInterestDAO dao;
    
    void load();
    
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
    
    void bark() const;
    unsigned int serializedLength() const;
    void serialize(unsigned char* buffer) const;
    
    void setDAO(const PointsOfInterestDAO& dao);
    void save();
    bool remove();
    
    static void getAllPOIs(std::function<void(PointOfInterest*, int)> callback);
};

#endif	/* POINTOFINTEREST_H */

