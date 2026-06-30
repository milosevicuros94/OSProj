#include "../lib/hw.h"
#include "../inc/workers.hpp"
#include "../inc/printing.hpp"
#include "../inc/semaphore.hpp"

void workerBodyA(void* a) {
    time_sleep(5);

    for (uint64 i = 0; i < 10; i++) {
        _printString("A: i =");
        _printInteger(i);
        _printString("\n");

        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) {
                // busy wait
            }
            // thread_dispatch();
        }
    }
}

void workerBodyB(void* a) {
    time_sleep(6);

    for (uint64 i = 0; i < 16; i++) {
        _printString("B: i =");
        _printInteger(i);
        _printString("\n");

        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) {
                // busy wait
            }
            // thread_dispatch();
        }
    }
}

static uint64 fibonacci(uint64 n) {
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { thread_dispatch(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyC(void* a) {
    time_sleep(15);
    uint8 i = 0;
    for (; i < 3; i++) {
        _printString("C: i =");
        _printInteger(i);
        _printString("\n");
    }

    _printString("C: yield\n");
    __asm__ ("li t1, 7");
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r" (t1));

    _printString("C: t1=");
    _printInteger(t1);
    _printString("\n");

    uint64 result = fibonacci(12);
    _printString("C: fibonacci =");
    _printInteger(result);
    _printString("\n");

    for (; i < 6; i++) {
        _printString("C: i =");
        _printInteger(i);
        _printString("\n");
    }

    // _thread::yield();
}

void workerBodyD(void* a) {
    time_sleep(6);

    uint8 i = 10;
    for (; i < 13; i++) {
        _printString("D: i =");
        _printInteger(i);
        _printString("\n");
    }

    _printString("D: yield\n");
    __asm__ ("li t1, 5");
    thread_dispatch();

    uint64 result = fibonacci(12);
    _printString("D: fibonacci =");
    _printInteger(result);
    _printString("\n");

    for (; i < 16; i++) {
        _printString("D: i =");
        _printInteger(i);
        _printString("\n");
    }

    // _thread::yield();
}

void workerSemWaitCloseA(void* ptr) {
    _printString("A waiting\n");

    sem_t sem = (sem_t)ptr;
    int ret = sem_wait(sem);
    _printString("A unblocked. Wait ret: ");
    _printInteger(ret);
    _printString("\n");
}
void workerSemWaitCloseB(void* ptr) {
    _printString("B waiting\n");

    sem_t sem = (sem_t)ptr;
    int ret = sem_wait(sem);
    _printString("B unblocked. Wait ret: ");
    _printInteger(ret);
    _printString("\n");
}

void workerSemWaitCloseC(void* ptr)  {
    _printString("C waiting\n");

    sem_t sem = (sem_t)ptr;
    int ret = sem_wait(sem);
    _printString("C unblocked. Wait ret: ");
    _printInteger(ret);
    _printString("\n");
}

void workerSemWaitCloser(void* ptr) {
    _printString("Closer sleeping\n");
    time_sleep(100);

    _printString("Closing sem\n");

    sem_t sem = (sem_t)ptr;
    int ret = sem_close(sem);
    _printString("Closed sem. Close ret: ");
    _printInteger(ret);
    _printString("\n");

    ret = sem_close(sem);
    _printString("Second close. Close ret: ");
    _printInteger(ret);
    _printString("\n");
}
