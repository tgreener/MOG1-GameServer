/* 
 * File:   ByteCodeDelegate.h
 * Author: toddgreener
 *
 * Created on May 26, 2014, 1:19 PM
 */

#ifndef BYTECODEDELEGATE_H
#define	BYTECODEDELEGATE_H

class ByteCodeDelegate {
protected:
public:
    virtual void interpretCommand(char* bytes, int length) = 0;
};

#endif	/* BYTECODEDELEGATE_H */

