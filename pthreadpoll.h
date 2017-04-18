#ifndef PTHREADPOLL_H
#define PTHREADPOLL_H

#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <deque>
#include <iostream>

#include "condvar.h"
#include "mutex.h"
#include "task.h"
#include "errexit.h"

class ThreadPoll {
public:
    ThreadPoll();
    ThreadPoll(int pool_size);
    ~ThreadPoll();

    int initialize_threadpoll();
    int destroy_threadpoll();
    void* excute_thread();
    int add_task(Task* task);


private:
    int m_pool_size;
    Mutex m_task_mutex;
    CondVar m_task_convar;
    std::vector<pthread_t> m_thread;     //store pthread_id
    std::deque<Task*> m_task;
    volatile int m_pool_state;
};

#endif // PTHREADPOLL_H
