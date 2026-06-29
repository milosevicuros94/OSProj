#include "../inc/MemoryAllocator.hpp"
#include "../inc/thread.hpp"
#include "../inc/printing.hpp"
#include "../inc/riscv.hpp"
#include "../inc/workers.hpp"

extern "C" void handlers();

// extern void userMain();
//
// void userMainWrapper(void*) {
//     userMain();
// }

struct IdleDone {
    volatile bool done = false;
};

void idle(void* arg) {
    IdleDone *done = (IdleDone *) arg;
    while (!done->done) {
        thread_dispatch();
    }
    _printString("Idle done\n");
}

void testMemory(_thread* mainThread) {
    uint64 startAddress = ((uint64)HEAP_START_ADDR + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;
    uint64 address = startAddress;
    if ((uint64)mainThread != startAddress + MEM_BLOCK_SIZE) {
        _printString("Main thread bad alloc\n");
    }
    address += 3 * MEM_BLOCK_SIZE + MEM_BLOCK_SIZE;

    size_t firstSize = 20 * MEM_BLOCK_SIZE;
    void* first = mem_alloc(firstSize);
    if ((uint64)first != address) {
        _printString("First bad alloc\n");
    }
    address += firstSize + MEM_BLOCK_SIZE;

    size_t secondSize = 50 * MEM_BLOCK_SIZE;
    void* second = mem_alloc(secondSize);
    if ((uint64)second != address) {
        _printString("Second bad alloc\n");
    }
    address += secondSize + MEM_BLOCK_SIZE;

    size_t thirdSize = 10 * MEM_BLOCK_SIZE;
    void* third = mem_alloc(thirdSize);
    if ((uint64)third != address) {
        _printString("Third bad alloc\n");
    }
    address += thirdSize;

    mem_free(second);

    size_t forthSize = 30 * MEM_BLOCK_SIZE;
    void* forth = mem_alloc(forthSize);
    if ((uint64)second != (uint64)forth) {
        _printString("Forth bad alloc\n");
    }

    mem_free(third);
    mem_free(first);
    mem_free(forth);
}

void testThreads() {
    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    thread_t handle0 = nullptr;
    thread_create(&handle0, workerBodyA, nullptr);
    _printString("ThreadA created\n");

    thread_t handle1 = nullptr;
    thread_create(&handle1, workerBodyB, nullptr);
    _printString("ThreadB created\n");

    thread_t handle2 = nullptr;
    thread_create(&handle2, workerBodyC, nullptr);
    _printString("ThreadC created\n");

    thread_t handle3 = nullptr;
    thread_create(&handle3, workerBodyD, nullptr);
    _printString("ThreadD created\n");

    // while (handle0->getState() != _thread::FINISHED) {
    //     thread_dispatch();
    // }

    while (!(handle0->getState() == _thread::FINISHED &&
        handle1->getState() == _thread::FINISHED &&
        handle2->getState() == _thread::FINISHED &&
        handle3->getState() == _thread::FINISHED)) {
        thread_dispatch();
        }

    _printString("Threads done\n");
    done.done = true;

    delete handle0;
    delete handle1;
    delete handle2;
    delete handle3;

    while (idleThread->getState() != _thread::FINISHED) {
        thread_dispatch();
    }

    delete idleThread;
}

int main() {
    // OR 1 for vector mode
    Riscv::w_stvec((uint64)handlers | 1);

    _thread* mainThread = (_thread*) mem_alloc(sizeof(_thread));
    mainThread->initMainThread();

    // testThreads();
    testMemory(mainThread);

    _printString("Main done\n");

    return 0;
}