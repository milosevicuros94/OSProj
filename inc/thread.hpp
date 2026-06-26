#ifndef _THREAD_HPP
#define _THREAD_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"

class _thread {
public:
    ~_thread() { delete[] stack; }

    using Body = void (*)();

    static _thread* createThread(Body body);

    static void yield();

    static _thread* running;
    _thread* next = nullptr;

    bool isFinished() const { return finished; }
    void setFinished(bool f) { finished = f; }

    uint64 getTimeSlice() const { return timeSlice; }
    void setTimeSlice(uint64 ts) { timeSlice = ts; }

private:
    _thread(Body body, uint64 timeSlice) :
        body(body),
        stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
        context({(uint64)&threadWrapper, stack != nullptr ? (uint64)&stack[DEFAULT_STACK_SIZE] : 0}),
        finished(false),
        timeSlice(timeSlice)
    {
        if (body != nullptr) {
            Scheduler::getInstance().putReady(this);
        }
    }

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;
    bool finished;
    uint64 timeSlice;

    friend class Riscv;

    static void threadWrapper();

    static void dispatch();
    static void contextSwitch(Context* oldContext, Context* newContext);

    static uint64 timeSliceCounter;
};

#endif