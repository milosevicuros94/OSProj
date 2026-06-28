#include "../inc/thread.hpp"
#include "../inc/riscv.hpp"
#include "../inc/scheduler.hpp"

_thread* _thread::running = nullptr;
uint64 _thread::timeSliceCounter = 0;

_thread::_thread() {}

_thread::_thread(Body startRoutine, void* arg, void* stack) :
    startRoutine(startRoutine),
    arg(arg),
    stack(stack),
    context({(uint64)&threadWrapper, (uint64)stack + DEFAULT_STACK_SIZE})
{
    state = READY;
    timeSlice = DEFAULT_TIME_SLICE;
    next = nullptr;
    joinQueue = nullptr;
    semResources = 0;
    sleepTime = 0;

    Scheduler::getInstance().putReady(this);
}

void _thread::yield() {
    __asm__ volatile ("ecall");
}

void _thread::dispatch() {
    _thread* oldRunning = running;
    if (oldRunning->getState() == READY) {
        Scheduler::getInstance().putReady(oldRunning);
    }

    running = Scheduler::getInstance().getReady();

    contextSwitch(&oldRunning->context, &running->context);
}

void _thread::threadWrapper() {
    Riscv::popSppSpie();
    running->startRoutine(running->arg);
    running->setState(FINISHED);
    yield();
}

void _thread::exit() {
    // TODO: This
}
