/* 
 * File:   AbstractDAO.h
 * Author: toddgreener
 *
 * Created on May 28, 2014, 12:17 PM
 */

#ifndef ABSTRACTDAO_H
#define	ABSTRACTDAO_H

class AbstractDAO {
public:
    virtual bool retrieve(unsigned int id) = 0;
    virtual bool remove(unsigned int id) = 0;
    virtual int write() = 0;
    virtual int write(int id) = 0;
};

#endif	/* ABSTRACTDAO_H */

