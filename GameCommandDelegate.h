/* 
 * File:   GameCommandDelegate.h
 * Author: toddgreener
 *
 * Created on May 26, 2014, 1:25 PM
 */

#ifndef GAMECOMMANDDELEGATE_H
#define	GAMECOMMANDDELEGATE_H

#include "ByteCodeDelegate.h"
#include "Definitions.h"

class GameCommandDelegate : public ByteCodeDelegate {
public:
    GameCommandDelegate();
    void interpretCommand(char* bytes, int length);
    
private:
    InterpreterFunctions functions;
};

#endif	/* GAMECOMMANDDELEGATE_H */

