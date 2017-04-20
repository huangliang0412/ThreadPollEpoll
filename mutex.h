#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include "errexit.h"

class Mutex{
public:
    Mutex();
    ~Mutex();
    
    int lock();
    int unlock();
    
    pthread_mutex_t* getMutexptr();
    bool getIs_lock();

private:
    pthread_mutex_t m_lock;
    volatile bool is_lock;
};

#endif // MUTEX_H
