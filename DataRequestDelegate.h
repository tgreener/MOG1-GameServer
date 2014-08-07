/* 
 * File:   DataRequestDelegate.h
 * Author: toddgreener
 *
 * Created on May 26, 2014, 1:27 PM
 */

#ifndef DATAREQUESTDELEGATE_H
#define	DATAREQUESTDELEGATE_H

#include "ByteCodeDelegate.h"
#include "Definitions.h"

class DataRequestDelegate : public ByteCodeDelegate {
private:
    InterpreterFunctions functions;
    
public:
    void interpretCommand(char* bytes, int length);
    DataRequestDelegate();
};

#endif	/* DATAREQUESTDELEGATE_H */

