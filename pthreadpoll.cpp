#include <cstdio>
#include "pthreadpoll.h"

using namespace std;

const int DEFAULT_POOL_SIZE = 10;
const int START = 0;
const int STOP = 1;

ThreadPoll* ThreadPoll::instance = NULL;

ThreadPoll* ThreadPoll::getThreadPoll() {
    if(instance == NULL)
        instance = new ThreadPoll();
    return instance;
}

ThreadPoll* ThreadPoll::getThreadPoll(int pool_size) {
    if(instance == NULL)
        instance = new ThreadPoll(pool_size);
    return instance;
}

ThreadPoll::ThreadPoll() : m_pool_size(DEFAULT_POOL_SIZE) {}

ThreadPoll::ThreadPoll(int pool_size) : m_pool_size(pool_size) {}

ThreadPoll::~ThreadPoll() {
    destroy_threadpoll();
    delete this;
    instance = NULL;
}

extern "C"
void* start_thread(void* arg) {
    ThreadPoll* tp = (ThreadPoll*) arg;
    cout << "start" << endl;
    fflush(stdout);
    tp->excute_thread();
    return NULL;
}

int ThreadPoll::initialize_threadpoll() {
    m_pool_state = START;
    int ret = -1;
    for(int i = 0; i < m_pool_size; ++i) {
        pthread_t tid;
        ret = pthread_create(&tid, NULL, start_thread, (void*) this);
        if(ret != 0){
            errExit("pthreads create error");
            return -1;
        }
        m_thread.push_back(tid);

    }
    return 0;
}

int ThreadPoll::destroy_threadpoll() {
    m_task_mutex.unlock();

    m_pool_state = STOP;
    m_task_convar.broadcast();   //notify all thread wake up
    int ret = -1;
    for(int i = 0; i < m_pool_size; ++i) {
        ret = pthread_join(m_thread[i], NULL);
        if(ret != 0) {
            errExit("pthread_join error");
            return -1;
        }
        m_task_convar.broadcast();   // try to wake threads up
    }
    return 0;
}

void* ThreadPoll::excute_thread() {
    Task* task = NULL;
    while(true) {
        m_task_mutex.lock();
        while((m_pool_state != STOP) && m_task.empty()) {
            //  m_task_convar.waitCond(m_task_mutex.getMutexptr());
            m_task_convar.waitCond(m_task_mutex.getMutexptr());
        }

        if(m_pool_state == STOP) {
            //m_task_mutex.unlock();
            m_task_mutex.unlock();
            pthread_exit(NULL);
        }
        task = m_task.front();
        m_task.pop_front();
        //m_task_mutex.unlock();
        m_task_mutex.unlock();
        task->run();
        delete task;
    }
    return NULL;
}

int ThreadPoll::add_task(Task *task) {
    m_task_mutex.lock();
    m_task.push_back(task);

    m_task_mutex.unlock();
    m_task_convar.signal();    //wake up one thread
    //m_task_convar.broadcast();
    //m_task_mutex.unlock();
}

void ThreadPoll::print_pid() {
    for(int i = 0; i < m_thread.size(); ++i)
        cout << m_thread[i] << endl;
}

void ThreadPoll::pthreads_join() {
    for(int i = 0; i < m_thread.size(); ++i)
        pthread_join(m_thread[i], NULL);
}
