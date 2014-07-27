/* 
 * File:   Model.h
 * Author: toddgreener
 *
 * Created on May 28, 2014, 1:00 PM
 */

#ifndef ABSTRACTMODEL_H
#define	ABSTRACTMODEL_H

#include <functional>

class AbstractModel;

typedef std::function<void(AbstractModel**, int)> AllModelsCallback;

class AbstractModel {
public:
    virtual AbstractModel();
    
    virtual void bark() const = 0;
    virtual void save() = 0;
    virtual unsigned int serializedLength() const = 0;
    virtual void serialize(unsigned char* buffer) const = 0;
    
    virtual bool remove() = 0;
    
protected:
    AllModelsCallback amc;
};

typedef std::function<void(const char* bytes, unsigned int length)> ByteInterpreterFunction;

#endif	/* MODEL_H */

