#include "../inc/thread.hpp"
#include "../inc/riscv.hpp"
#include "../inc/scheduler.hpp"

_thread* _thread::running = nullptr;
uint64 _thread::timeSliceCounter = 0;

void _thread::createMainThread() {
    startRoutine = nullptr;
    arg = nullptr;
    stack = nullptr;
    context = { 0, 0 };
    state = READY;
    timeSlice = DEFAULT_TIME_SLICE;
    next = nullptr;
    semResources = 0;
    sleepTime = 0;

    running = this;
}

_thread::_thread(Body startRoutine, void* arg, void* stack) :
    startRoutine(startRoutine),
    arg(arg),
    stack(stack),
    context({(uint64)&threadWrapper, (uint64)stack + DEFAULT_STACK_SIZE})
{
    state = READY;
    timeSlice = DEFAULT_TIME_SLICE;
    next = nullptr;
    semResources = 0;
    sleepTime = 0;

    Scheduler::getInstance().putReady(this);
}

void _thread::exit() {
    running->state = FINISHED;
    dispatch();
}

void _thread::dispatch() {
    _thread* oldRunning = running;
    if (oldRunning->getState() == READY) {
        Scheduler::getInstance().putReady(oldRunning);
    }
    running = Scheduler::getInstance().getReady();

    if (oldRunning != running) {
        contextSwitch(&oldRunning->context, &running->context);
    }
}

// TRUE if thread time slice expired. FALSE otherwise
bool _thread::timerTick() {
    timeSliceCounter++;

    if (timeSliceCounter >= running->timeSlice) {
        timeSliceCounter = 0;
        return true;
    }

    return false;
}

void _thread::sleep(time_t time) {
    if (time == 0) return;

    running->sleepTime = time;
    running->state = SLEEPING;

    Scheduler::getInstance().insertSleeping(running);
    dispatch();
}

void _thread::threadWrapper() {
    Riscv::popSppSpie();
    running->startRoutine(running->arg);
    exit();
}
