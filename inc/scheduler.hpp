#ifndef _SCHEDULER_HPP_
#define _SCHEDULER_HPP_

#include "../inc/syscall_c.h"

class Scheduler {
public:
    static Scheduler& getInstance();

    void putReady(thread_t thread);
    thread_t getReady();

private:
    Scheduler() = default;
    ~Scheduler() = default;

    thread_t readyQueueHead = nullptr;
    thread_t readyQueueTail = nullptr;

    thread_t sleepQueueHead = nullptr;
};

#endif