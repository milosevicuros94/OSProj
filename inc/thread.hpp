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
        FINISHED
    };

    ~_thread() { mem_free(stack); }

    void initMainThread();

    static _thread* running;

    inline ThreadState getState() { return state; }
    inline void setState(ThreadState newState) { state = newState; }

    inline _thread* getNext() { return next; }
    inline void setNext(_thread* t) { next = t; }

    inline time_t getSleepTime() { return sleepTime; }
    inline void setSleepTime(time_t newSleepTime) { sleepTime = newSleepTime; }
    inline void decrementSleepTime(time_t decrement = 1) { sleepTime -= decrement; }

private:
    struct Context {
        uint64 ra;
        uint64 sp;
    };

    _thread();
    _thread(Body startRoutine, void* arg, void* stack);

    static void threadWrapper();
    static void exit();
    static void dispatch();
    static void contextSwitch(Context* oldContext, Context* newContext);
    static bool timerTick();
    static void sleep(time_t time);

    static uint64 timeSliceCounter;

    Body startRoutine;
    void* arg;
    void* stack;
    Context context;
    ThreadState state;
    uint64 timeSlice;
    _thread* next;
    unsigned semResources;
    time_t sleepTime;

    friend class TrapHandler;

};

#endif