#ifndef _SCHEDULER_HPP
#define _SCHEDULER_HPP

#include "../inc/syscall_c.h"

class Scheduler {
private:
    Scheduler() = default;
    ~Scheduler() = default;

    thread_t readyQueueHead = nullptr;
    thread_t readyQueueTail = nullptr;

    thread_t sleepQueueHead = nullptr;

public:
    static Scheduler& getInstance();

    void putReady(thread_t thread);
    thread_t getReady();

};

#endif