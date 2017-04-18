#include "mutex.h"

Mutex::Mutex() {
    if(pthread_mutex_init(&m_lock, NULL) != 0)
        errExit("pthread mutex create error");
    is_lock = false;
}

Mutex::~Mutex() {
    while(is_lock)
        unlock();
    pthread_mutex_destroy(&m_lock);
}

int Mutex::lock() {
    if(pthread_mutex_lock(&m_lock) != 0)
        errExit("pthread mutex lock error");
    is_lock = true;
    return 0;
}

int Mutex::unlock()
{
    if(is_lock)
    {
        if(pthread_mutex_unlock(&m_lock) != 0)
            errExit("pthread mutex unlock error");
        is_lock = false;
        return 0;
    }
    
}

pthread_mutex_t* Mutex::getMutexptr() {
    return &m_lock;
}
