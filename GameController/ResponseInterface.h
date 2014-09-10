/* 
 * File:   ResponseInterface.h
 * Author: toddgreener
 *
 * Created on August 6, 2014, 10:00 PM
 */

#ifndef RESPONSEINTERFACE_H
#define	RESPONSEINTERFACE_H

#include "../Models/AbstractModel.h"
#include "../Models/PointOfInterest.h"
#include "../Models/Route.h"
#include "../Models/User.h"

class ResponseInterface {
public:
    static void userConnectedResponse(unsigned int id);
    static void boolResponse(bool completed);
    static void userLocationResponse(const Location& loc);
    
    static void modelArrayResponse(const AbstractModel** pois, unsigned int count);
    static void poiArrayResponse(const PointOfInterest* pois, unsigned int count);
    
    static void routesAndEndpointsReponse(const Route* routes, const PointOfInterest* pois, unsigned int count);
private:
    
};

#endif	/* RESPONSEINTERFACE_H */

