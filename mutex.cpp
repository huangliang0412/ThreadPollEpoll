#include "mutex.h"
#include <iostream>
using namespace std;
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

 //   cout << "is_lock: " << is_lock <<" line: " << __LINE__ << endl;
    //std::cout << pthread_mutex_lock(&m_lock) << " " << __LINE__ << std::endl;
    //if(!is_lock) {
        if (pthread_mutex_lock(&m_lock) != 0)
            errExit("pthread mutex lock error");
        is_lock = true;
    //}

    //cout << "is_lock: " << is_lock <<" line: " << __LINE__ << endl;
    return 0;
}

int Mutex::unlock()
{

    //cout << "is_lock: " << is_lock <<" line: " << __LINE__ << endl;
    //if(is_lock)
    //{
        if(pthread_mutex_unlock(&m_lock) != 0)
            errExit("pthread mutex unlock error");
        is_lock = false;
        return 0;
    //}
    
}

pthread_mutex_t* Mutex::getMutexptr() {
    return &m_lock;
}

bool Mutex::getIs_lock() {
    return is_lock;
}