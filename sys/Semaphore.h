/* 
 * File:   Sempahore.h
 * Author: toddgreener
 *
 * Created on August 3, 2014, 5:31 PM
 */

#ifndef SEMPAHORE_H
#define	SEMPAHORE_H

#include <mutex>
#include <condition_variable>

class Semaphore{
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;

public:
    Semaphore();
    void signal();
    void wait();
};

#endif	/* SEMPAHORE_H */

