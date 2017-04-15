#ifndef TASK_H
#define TASK_H

class Task {
public:
    Task(void (*fn_ptr)(void*), void* arg);
    ~Task;
    void run();
private:
    void (*m_fn_ptr)(void*);     //function
    void* m_arg;               // function arg
};

#endif // TASK_H
