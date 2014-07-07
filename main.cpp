/* 
 * File:   main.cpp
 * Author: toddgreener
 *
 * Created on May 25, 2014, 5:41 PM
 */

#include "Server.h"

#define SOCKET_PATH "./connection"

int main(int argc, char** argv) {
    
    Server s(SOCKET_PATH);
    s.init();
    s.serviceSocket();
    s.closeServer();
    
    return 0;
}

