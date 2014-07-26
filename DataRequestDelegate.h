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
#include <functional>

class DataRequestDelegate : public ByteCodeDelegate {
private:
    std::function<void(AbstractModel**, int)> fetchAllModelsCallback;
    
    void insufficientDataMessage();
    void nullResponse();
    void respondWithID(int id);
    void deleteModel(AbstractModel& model);
    void fetchModel(AbstractModel& model);
    
    int createPointOfInterest(char* bytes, int length);
    void fetchPOI(unsigned int id);
    void deletePOI(unsigned int id);
    void addPOI(char* bytes, int length);
    
    int createRoute(char* bytes, int length);
    void fetchRoute(unsigned int id);
    void deleteRoute(unsigned int id);
    void addRoute(char* bytes, int length);
    
    void fetchAllPOIs();
    void fetchAllRoutes();
public:
    void interpretCommand(char* bytes, int length);
    DataRequestDelegate();
};

#endif	/* DATAREQUESTDELEGATE_H */

