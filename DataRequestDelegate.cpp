
#include "DataRequestDelegate.h"
#include "ServiceLocator.h"
#include "Models/PointOfInterest.h"
#include "Models/Route.h"
#include <cstdio>
#include <cstring>

void DataRequestDelegate::interpretCommand(char* bytes, int length) {
    if(length > 1) {
        int index = bytes[1];
        if(index < functions.size() && index > 0) functions[index](bytes, length);
        else functions[0](nullptr, 0);
    }
}

DataRequestDelegate::DataRequestDelegate() {
    functions.push_back([](const char* bytes, int length) -> void {
        ServiceLocator::getServiceLocator().sendMessageToClient("Unrecognized data command.");
    });
    
    functions.push_back(PointOfInterest::getFetchFunction());
    functions.push_back(PointOfInterest::getAddFunction());
    functions.push_back(PointOfInterest::getDeleteFunction());
    functions.push_back(PointOfInterest::getFetchAllFunction());
    
    functions.push_back(Route::getFetchFunction());
    functions.push_back(Route::getAddFunction());
    functions.push_back(Route::getDeleteFunction());
    functions.push_back(Route::getFetchAllFunction());
}
