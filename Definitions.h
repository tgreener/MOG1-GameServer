/* 
 * File:   Definitions.h
 * Author: toddgreener
 *
 * Created on August 6, 2014, 7:45 PM
 */

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#include <functional>
#include <vector>

typedef std::function<void(const char* bytes, int length)> ByteInterpreterFunction;
typedef std::vector<ByteInterpreterFunction> InterpreterFunctions;

#endif	/* DEFINITIONS_H */

