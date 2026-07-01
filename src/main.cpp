#include "../inc/thread.hpp"
#include "../inc/riscv.hpp"
#include "../inc/tests.hpp"

extern void userMain();

void userMainWrapper(void* ptr) {
    sem_t sem = (sem_t)ptr;

    userMain();

    sem_signal(sem);
}

struct IdleDone {
    volatile bool done = false;
};

void idle(void* arg) {
    IdleDone* done = (IdleDone*)arg;
    while (!done->done) {
        thread_dispatch();
    }
}

int main() {
    Riscv::set_stvec_handlers();

    thread_t mainThread = (thread_t)mem_alloc(sizeof(_thread));
    mainThread->initMainThread();

    // sem_t mainSem = nullptr;
    // sem_open(&mainSem, 0);

    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    // thread_t userMain = nullptr;
    // thread_create(&userMain, userMainWrapper, mainSem);

    // thread_t userMain = nullptr;
    // thread_create(&userMain, runAllTests, mainThread);

    runAllTests(mainThread);

    // sem_wait(mainSem);
    // while (userMain->getState() != _thread::FINISHED) {
    //     thread_dispatch();
    // }
    // delete userMain;
    // sem_close(mainSem);

    done.done = true;
    while (idleThread->getState() != _thread::FINISHED) {
        thread_dispatch();
    }
    delete idleThread;

    return 0;
}