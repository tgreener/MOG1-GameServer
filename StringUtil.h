/* 
 * File:   StringUtil.h
 * Author: toddgreener
 *
 * Created on May 25, 2014, 11:46 PM
 */

#ifndef STRINGUTIL_H
#define	STRINGUTIL_H

void strToLower(char* str);
int trim(char* str);
void printOctals(char* str, int length);

const unsigned char* heapifyStringUnsigned(const unsigned char* n);
const char* heapifyString(const char* n);

#endif	/* STRINGUTIL_H */

