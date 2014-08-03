/* 
 * File:   Location.h
 * Author: toddgreener
 *
 * Created on July 27, 2014, 3:52 PM
 */

#ifndef LOCATION_H
#define	LOCATION_H

#include "AbstractModel.h"

class Location : public AbstractModel {
protected:
public:
    virtual unsigned int getID() const = 0;
};

#endif	/* LOCATION_H */

