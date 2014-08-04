/* 
 * File:   ThreadPool.h
 * Author: toddgreener
 *
 * Created on August 3, 2014, 5:31 PM
 */

#ifndef THREADPOOL_H
#define	THREADPOOL_H

#include <functional>
#include <atomic>
#include <queue>
#include <vector>
#include <thread>

#include "Semaphore.h"

typedef std::function<void(int)> ClientHandler;
typedef std::function<void(void)> GenericFunction;

class ThreadPool {
private:
    Semaphore poolLock;

    GenericFunction initFunction;
    GenericFunction deinitFunction;
    ClientHandler runFunction;   
    
    std::atomic<bool> isRunning;
    std::queue<int> clientHandles;
    std::vector<std::thread> threads;
    
    unsigned int count;
    
public:
    ThreadPool();
    
    void setSize(unsigned int count);
    void setSetupFunction(GenericFunction initFunction);
    void setTeardownFunction(GenericFunction deinitFunction);
    void setRunFunction(ClientHandler runFunction);
    
    void run();
    void flush();
    
    void acceptClient(int clientHandle);
};

#endif	/* THREADPOOL_H */

