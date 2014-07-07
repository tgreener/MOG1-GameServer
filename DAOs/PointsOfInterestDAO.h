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

class PointsOfInterestDAO : public AbstractDAO{
private:
    unsigned int id;
    unsigned int soil;
    unsigned int stone;
    unsigned int wilderness;
    
    const unsigned char* name;
    
    void freeName();
    bool checkValuesSet();
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
    const unsigned char* getName() const;
    
    void setName(const unsigned char* n);
    void setSoil(unsigned int sl);
    void setStone(unsigned int st);
    void setWilderness(unsigned int wld);
    
    static void allPOIDAOs(std::function<void(PointsOfInterestDAO*, int)>);
};

#endif	/* POINTSOFINTERESTDAO_H */

