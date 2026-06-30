#include "../inc/thread.hpp"
#include "../inc/riscv.hpp"

extern void userMain();

void userMainWrapper(void*) {
    userMain();
}

struct IdleDone {
    volatile bool done = false;
};

void idle(void* arg) {
    IdleDone *done = (IdleDone *)arg;
    while (!done->done) {
        thread_dispatch();
    }
}

int main() {
    Riscv::set_stvec_handlers();

    _thread* mainThread = (_thread*)mem_alloc(sizeof(_thread));
    mainThread->initMainThread();

    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    thread_t userMain = nullptr;
    thread_create(&userMain, userMainWrapper, nullptr);

    while (userMain->getState() != _thread::FINISHED) {
        thread_dispatch();
    }

    done.done = true;
    while (idleThread->getState() != _thread::FINISHED) {
        thread_dispatch();
    }
    delete idleThread;
    delete mainThread;

    return 0;
}