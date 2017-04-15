#ifndef CONDVAR_H
#define CONDVAR_H

#include "errexit.h"
#include <pthread.h>

class CondVar{
public:
    CondVar();
    ~CondVar();
    
    void waitCond(pthread_mutex_t* mutex);

    void signal();
    void broadcast();

    
private:
    pthread_cond_t m_condvar;
    
};

#endif // CONDVAR_H
