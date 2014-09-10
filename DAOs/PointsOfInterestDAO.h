/* 
 * File:   PointsOfInterestDAO.h
 * Author: toddgreener
 *
 * Created on May 28, 2014, 12:17 PM
 */

#ifndef POINTSOFINTERESTDAO_H
#define	POINTSOFINTERESTDAO_H

#include "AbstractDAO.h"
#include <stdbool.h>
#include <functional>

class PointsOfInterestDAO;

typedef std::function<void(PointsOfInterestDAO*, int)> PointOfInterestDAOsCallback;

class PointsOfInterestDAO : public AbstractDAO{
private:
    unsigned int id;
    unsigned int locationID;
    unsigned int soil;
    unsigned int stone;
    unsigned int wilderness;
    unsigned int population;
    
    const unsigned char* name;
    
    bool checkValuesSet();
    void freeName();
public:
    PointsOfInterestDAO();
    PointsOfInterestDAO(int id, const unsigned char* name, int soil, int stone, int wild);
    ~PointsOfInterestDAO();
    
    bool retrieve(unsigned int id);
    bool remove(unsigned int id);
    int write();
    int write(int id);
    
    unsigned int getID() const;
    unsigned int getSoil() const;
    unsigned int getStone() const;
    unsigned int getWilderness() const;
    unsigned int getLocationID() const;
    unsigned int getPopulation() const;
    const unsigned char* getName() const;
    
    void setName(const unsigned char* n);
    void setSoil(unsigned int sl);
    void setStone(unsigned int st);
    void setWilderness(unsigned int wld);
    void setLocation(unsigned int loc);
    void setPopulation(unsigned int pop);
    
    static void allPOIDAOs(PointOfInterestDAOsCallback callbackWithPOIDAOs);
};

#endif	/* POINTSOFINTERESTDAO_H */

