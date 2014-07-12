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

typedef std::function<void(RouteDAO*, int)> AllRoutesCallback;

class RouteDAO : public AbstractDAO {
private:
    int id;
    int poiA;
    int poiB;
    
    bool checkValuesSet();
public:
    RouteDAO();
    ~RouteDAO();
    
    virtual bool retrieve(unsigned int id);
    virtual bool remove(unsigned int id);
    virtual int write();
    virtual int write(unsigned int id);
    
    unsigned int getID();
    unsigned int getPOIA();
    unsigned int getPOIB();
    
    void setPOIA(unsigned int a);
    void setPOIb(unsigned int b);
    
    static void allRouteDAOs(AllRoutesCallback callback);
};

#endif	/* ROUTEDAO_H */

