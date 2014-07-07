/* 
 * File:   DataRequestDelegate.h
 * Author: toddgreener
 *
 * Created on May 26, 2014, 1:27 PM
 */

#ifndef DATAREQUESTDELEGATE_H
#define	DATAREQUESTDELEGATE_H

#include "ByteCodeDelegate.h"

class DataRequestDelegate : public ByteCodeDelegate {
    int createPointOfInterest(char* bytes, int length);
    void fetchPOI(unsigned int id);
    void deletePOI(unsigned int id);
    void addPOI(char* bytes, int length);
    
    void fetchAllPOIs();
public:
    void interpretCommand(char* bytes, int length);
};

#endif	/* DATAREQUESTDELEGATE_H */

