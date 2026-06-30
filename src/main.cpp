#include "../inc/thread.hpp"
#include "../inc/printing.hpp"
#include "../inc/riscv.hpp"
#include "../inc/tests.hpp"

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
    _printString("Idle done\n");
}

int main() {
    Riscv::set_stvec_handlers();

    _thread* mainThread = (_thread*)mem_alloc(sizeof(_thread));
    mainThread->initMainThread();

    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    runAllTests(mainThread);

    done.done = true;
    while (idleThread->getState() != _thread::FINISHED) {
        thread_dispatch();
    }
    delete idleThread;

    mem_free(mainThread);
    _printString("Main done.");

    return 0;
}