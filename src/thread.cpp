#include "../inc/thread.hpp"
#include "../inc/riscv.hpp"

_thread* _thread::running = nullptr;
uint64 _thread::timeSliceCounter = 0;

_thread* _thread::createThread(Body body) {
    return new _thread(body, DEFAULT_TIME_SLICE);
}

void _thread::yield() {
    __asm__ volatile ("ecall");
}

void _thread::dispatch() {
    _thread* oldRunning = running;
    if (!oldRunning->isFinished()) {
        Scheduler::getInstance().putReady(oldRunning);
    }
    running = Scheduler::getInstance().getReady();

    contextSwitch(&oldRunning->context, &running->context);
}

void _thread::threadWrapper() {
    Riscv::popSppSpie();
    running->body();
    running->setFinished(true);
    yield();
}
