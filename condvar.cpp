#include "condvar.h"

CondVar::CondVar() {
    if(pthread_cond_init(&m_condvar, NULL) != 0)
        errExit("condition variable init error");
}

CondVar::~CondVar() {
    if(pthread_cond_destroy(&m_condvar) != 0)
        errExit("condition variable destroy error");
}

void CondVar::waitCond(pthread_mutex_t* mutex) {
    if(pthread_cond_wait(&m_condvar, mutex) != 0)
        errExit("conditon variable destroy error");

}

void CondVar::signal() {
    if(pthread_cond_signal(&m_condvar) != 0)
        errExit("condition variable signal error");

}

void CondVar::broadcast()  {
    if(pthread_cond_broadcast(&m_condvar) != 0)
        errExit("conditon variable broadcast error");
}
