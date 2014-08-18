/* 
 * File:   RouteDAO.h
 * Author: toddgreener
 *
 * Created on July 12, 2014, 12:44 PM
 */

#ifndef ROUTEDAO_H
#define	ROUTEDAO_H

#include "AbstractDAO.h"
#include <functional>

class RouteDAO;

typedef std::function<void(RouteDAO*, int)> RouteDAOsCallback;

class RouteDAO : public AbstractDAO {
private:
    unsigned int id;
    unsigned int poiA;
    unsigned int poiB;
    unsigned int locationID;
    
    unsigned int difficulty;
    bool bidirected;
    bool reverse;
    
    bool needsWrite;
    
    bool checkValuesSet();
    static void readRouteDAOResult(unsigned int poiID, const char* query, RouteDAOsCallback callback);
    
public:
    RouteDAO();
    virtual ~RouteDAO();
    
    virtual bool retrieve(unsigned int id);
    virtual bool remove(unsigned int id);
    virtual int write();
    virtual int write(int id);
    
    unsigned int getID() const;
    unsigned int getLocationID() const;
    unsigned int getPOIA() const;
    unsigned int getPOIB() const;
    unsigned int getDifficulty() const;
    bool isBidrectional() const;
    bool isReverse() const;
    
    void setPOIA(unsigned int a);
    void setPOIB(unsigned int b);
    void setDifficulty(unsigned int dif);
    void setBidirectional(bool bidir);
    void setReverse(bool rev);
    
    static void allRouteDAOs(RouteDAOsCallback callback);
    static void outgoingRouteDAOs(unsigned int poiID, RouteDAOsCallback callbackWithRouteDAOs);
    static void incomingRouteDAOs(unsigned int poiID, RouteDAOsCallback callbackWithRouteDAOs);
    
};

#endif	/* ROUTEDAO_H */

