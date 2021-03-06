/* 
 * File:   Model.h
 * Author: toddgreener
 *
 * Created on May 28, 2014, 1:00 PM
 */

#ifndef ABSTRACTMODEL_H
#define	ABSTRACTMODEL_H

#include <functional>
#include "../Definitions.h"

class AbstractModel;
class PointOfInterest;
class Route;
class Location;
class User;

typedef std::function<void(AbstractModel**, int)> AllModelsCallback;
typedef std::function<void(PointOfInterest*, unsigned int)> PointOfInterestCallback;
typedef std::function<void(Route*, unsigned int count)> RoutesCallback;
typedef std::function<void(User*, unsigned int)> UsersCallback;

class AbstractModel {
public:
//    virtual void bark() const = 0;
    virtual void save() = 0;
    virtual unsigned int serializedLength() const = 0;
    virtual void serialize(unsigned char* buffer) const = 0;
    virtual bool remove() = 0;
    
protected:
    static AllModelsCallback getAllModelsCallback;
    static void deleteModel(AbstractModel& model);
    static void fetchModel(AbstractModel& model);
    static void respondWithID(int id);
    static void insufficientDataMessage();
    static void nullResponse();
};

#endif	/* MODEL_H */

