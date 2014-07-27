/* 
 * File:   Model.h
 * Author: toddgreener
 *
 * Created on May 28, 2014, 1:00 PM
 */

#ifndef ABSTRACTMODEL_H
#define	ABSTRACTMODEL_H

#include <functional>

class AbstractModel {
public:
    virtual void bark() const = 0;
    virtual void save() = 0;
    virtual unsigned int serializedLength() const = 0;
    virtual void serialize(unsigned char* buffer) const = 0;
    
    virtual bool remove() = 0;
};

typedef std::function<void(AbstractModel**, int)> AllModelsCallback;

#endif	/* MODEL_H */

