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
#include "User.h"

class AbstractModel;

typedef std::function<void(AbstractModel**, int)> AllModelsCallback;

class AbstractModel {
public:
//    virtual void bark() const = 0;
    virtual void save() = 0;
    virtual unsigned int serializedLength() const = 0;
    virtual void serialize(unsigned char* buffer) const = 0;
    virtual bool remove() = 0;
    
    virtual void onUserEnter(const User& user) = 0;
    virtual void onUserExit(const User& user) = 0;
    
protected:
    static AllModelsCallback getAllModelsCallback;
    static void deleteModel(AbstractModel& model);
    static void fetchModel(AbstractModel& model);
    static void respondWithID(int id);
    static void insufficientDataMessage();
    static void nullResponse();
};

#endif	/* MODEL_H */

