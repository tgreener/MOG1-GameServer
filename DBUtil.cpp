
#include "DBUtil.h"
#include "ServiceLocator.h"
#include "DBConnection.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>

using namespace std;

void DBUtil::runSQLFromFile(const char* filename) {
    ifstream stream(filename);
    
    if(stream) {
        stream.seekg(0, stream.end);
        int length = stream.tellg();
        stream.seekg(0, stream.beg);
        
        char* buffer = new char[length];
        memset(buffer, 0, length);
        
        stream.read(buffer, length);
        stream.close();
        
//        cout.write(buffer, length);
//        cout.flush();
        
        const char* currentRequest = buffer;
        DBConnection* dbc = ServiceLocator::getServiceLocator().getDBConnection();
        
        while(currentRequest != (buffer + length)) {
            DBStatement statement = dbc->prepare(currentRequest, &currentRequest);
            
            int stepResult = statement.step();
            
            if(!stepResult) {
                printf("Error loading schema: %s\n", statement.sql());
                statement.finalize();
                break;
            }
            
            statement.finalize();
        }
        
        delete[] buffer;
    }
}
