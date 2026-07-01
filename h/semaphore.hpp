#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "syscall_c.h"

class _sem {
public:
    _sem(unsigned init);

    int close();
    int signal(unsigned n = 1);
    int wait(unsigned n = 1);

private:
    ~_sem() = default;

    unsigned resources;
    bool closed;
    thread_t waitQueueHead;
    thread_t waitQueueTail;

    unsigned waitingAfterClose;
};

#endif