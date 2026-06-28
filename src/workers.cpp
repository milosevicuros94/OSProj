#include "../lib/hw.h"
#include "../inc/workers.hpp"
#include "../inc/printing.hpp"

void workerBodyA(void* a) {
    for (uint64 i = 0; i < 10; i++) {
        _printString("A: i =");
        _printInteger(i);
        _printString("\n");

        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) {
                // busy wait
            }
            // TCB::yield();
        }
    }
}

void workerBodyB(void* a) {
    for (uint64 i = 0; i < 16; i++) {
        _printString("B: i =");
        _printInteger(i);
        _printString("\n");

        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) {
                // busy wait
            }
            // TCB::yield();
        }
    }
}

static uint64 fibonacci(uint64 n) {
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { thread_dispatch(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyC(void* a) {
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
