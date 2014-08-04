
#include "ThreadPool.h"

#include <thread>
#include "../ServiceLocator.h"
#include "../DBConnection.h"

ThreadPool::ThreadPool() : 
count(0), runFunction([](int){}), isRunning(false)
{
    queueLock.signal();
}
    
void ThreadPool::setSize(unsigned int count) {
    this->count = count;
}

void ThreadPool::setSetupFunction(GenericFunction initFunction) {
    this->initFunction = initFunction;
}

void ThreadPool::setTeardownFunction(GenericFunction deinitFunction) {
    this->deinitFunction = deinitFunction;
}

void ThreadPool::setRunFunction(ClientHandler runFunction) {
    this->runFunction = runFunction;
}
    
void ThreadPool::run() {
    if(isRunning) return; //Threads are already running.
    
    isRunning = true;
    
    for(int i = 0; i < count; i++) {
        threads.push_back(std::thread([&] (void) -> void {
            
            DBConnection dbc;
            dbc.open(DB_FILE);
            
            ServiceLocator sl;
            sl.setAsServiceLocator();
            sl.setDBConnection(&dbc);
            
            initFunction();
            
            while(true) {
                
                poolLock.wait();
                if(!isRunning) break;
                
                int handle = this->popHandle();
                
                sl.setClientSocketHandle(handle);
                runFunction(handle);
            }
            
            deinitFunction();
            
            dbc.close();
        }));
    }
}

void ThreadPool::flush() {
    isRunning = false;
    
    for(int i = 0; i < count; i++) {
        poolLock.signal();
    }
    
    for(auto& thread : threads) {
        thread.join();
    }
}

void ThreadPool::acceptClient(int clientHandle) {
    this->pushHandle(clientHandle);
    poolLock.signal();
}

int ThreadPool::popHandle() {
    queueLock.wait();
    int handle = clientHandles.front();
    clientHandles.pop();
    queueLock.signal();
    
    return handle;
}

void ThreadPool::pushHandle(int handle) {
    queueLock.wait();
    clientHandles.push(handle);
    queueLock.signal();
}
