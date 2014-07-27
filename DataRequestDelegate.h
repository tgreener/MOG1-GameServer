/* 
 * File:   DataRequestDelegate.h
 * Author: toddgreener
 *
 * Created on May 26, 2014, 1:27 PM
 */

#ifndef DATAREQUESTDELEGATE_H
#define	DATAREQUESTDELEGATE_H

#include "ByteCodeDelegate.h"
#include "Models/AbstractModel.h"
#include <vector>

typedef std::vector<ByteInterpreterFunction> InterpreterFunctions;

class DataRequestDelegate : public ByteCodeDelegate {
private:
    InterpreterFunctions functions;
    
public:
    void interpretCommand(char* bytes, int length);
    DataRequestDelegate();
};

#endif	/* DATAREQUESTDELEGATE_H */

