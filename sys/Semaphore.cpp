
#include "Semaphore.h"

using namespace std;

Semaphore::Semaphore() : count(0){}

void Semaphore::signal()
{
    unique_lock<mutex> lck(mtx);
    ++count;
    cv.notify_one();
}

void Semaphore::wait()
{
    unique_lock<mutex> lck(mtx);

    while(count == 0){
        cv.wait(lck);
    }
    count--;
}
