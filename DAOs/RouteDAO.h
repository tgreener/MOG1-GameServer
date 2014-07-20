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

typedef std::function<void(RouteDAO*, int)> AllRouteDAOsCallback;

class RouteDAO : public AbstractDAO {
private:
    unsigned int id;
    int poiA;
    int poiB;
    
    bool needsWrite;
    
    bool checkValuesSet();
public:
    RouteDAO();
    ~RouteDAO();
    
    virtual bool retrieve(unsigned int id);
    virtual bool remove(unsigned int id);
    virtual int write();
    virtual int write(int id);
    
    unsigned int getID() const;
    unsigned int getPOIA() const;
    unsigned int getPOIB() const;
    
    void setPOIA(unsigned int a);
    void setPOIB(unsigned int b);
    
    static void allRouteDAOs(AllRouteDAOsCallback callback);
};

#endif	/* ROUTEDAO_H */

