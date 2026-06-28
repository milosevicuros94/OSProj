#ifndef _THREAD_HPP_
#define _THREAD_HPP_

#include "../lib/hw.h"
#include "../inc/syscall_c.h"

class _thread {
public:
    using Body = void (*)(void*);
    enum ThreadState {
        READY,
        SLEEPING,
        SEM_WAIT,
        THREAD_WAIT,
        FINISHED
    };

    _thread();
    _thread(Body startRoutine, void* arg, void* stack);
    ~_thread() { mem_free(stack); }

    static void yield();
    static _thread* running;

    void exit();

    ThreadState getState() { return state; }
    void setState(ThreadState newState) { state = newState; }

    _thread* getNext() { return next; }
    void setNext(_thread* t) { next = t; }

    unsigned getSleepTime() { return sleepTime; }
    void setSleepTime(unsigned newSleepTime) { sleepTime = newSleepTime; }
    void decrementSleepTime(unsigned decrement = 1) { sleepTime -= decrement; }

private:
    struct Context {
        uint64 ra;
        uint64 sp;
    };

    static void threadWrapper();
    static void dispatch();
    static void contextSwitch(Context* oldContext, Context* newContext);

    static uint64 timeSliceCounter;

    Body startRoutine;
    void* arg;
    void* stack;
    Context context;
    ThreadState state;
    uint64 timeSlice;
    _thread* next;
    _thread* joinQueue;
    unsigned semResources;
    unsigned sleepTime;

    friend class TrapHandler;

};

#endif