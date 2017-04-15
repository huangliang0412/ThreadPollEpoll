#include "task.h"

Task::Task(void (*fn_ptr)(void *), void *arg)
        : m_fn_ptr(fn_ptr), m_arg(arg) {}


Task::~Task() {}

void Task::run() {
    (*m_fn_ptr)(arg);
}
