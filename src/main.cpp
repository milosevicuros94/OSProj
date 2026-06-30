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
    uint64 endAddress = ((uint64)HEAP_END_ADDR) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;
    uint64 blockCnt = (endAddress - startAddress) / MEM_BLOCK_SIZE;
    uint64 address = startAddress;
    if ((uint64)mainThread != startAddress + MEM_BLOCK_SIZE) {
        _printString("!!! Main thread bad alloc\n");
    } else {
        _printString("Main thread alloc correct\n");
    }
    address += 3 * MEM_BLOCK_SIZE + MEM_BLOCK_SIZE;

    size_t firstSize = 20 * MEM_BLOCK_SIZE;
    void* first = mem_alloc(firstSize);
    if ((uint64)first != address) {
        _printString("!!! First bad alloc\n");
    } else {
        _printString("First alloc correct\n");
    }
    address += firstSize + MEM_BLOCK_SIZE;

    size_t secondSize = 50 * MEM_BLOCK_SIZE;
    void* second = mem_alloc(secondSize);
    if ((uint64)second != address) {
        _printString("!!! Second bad alloc\n");
    } else {
        _printString("Second alloc correct\n");
    }
    address += secondSize + MEM_BLOCK_SIZE;

    size_t thirdSize = 10 * MEM_BLOCK_SIZE;
    void* third = mem_alloc(thirdSize);
    if ((uint64)third != address) {
        _printString("!!! Third bad alloc\n");
    } else {
        _printString("Third alloc correct\n");
    }
    address += thirdSize;

    void* badSize = mem_alloc(blockCnt * MEM_BLOCK_SIZE);
    if (badSize != nullptr) {
        _printString("!!! Bad size allocated\n");
    } else {
        _printString("Bad size correct\n");
    }

    int secondFree = mem_free(second);
    if (secondFree != 0) {
        _printString("!!! Second free failed\n");
    } else {
        _printString("Second free success\n");
    }

    int doubleFree = mem_free(second);
    if (doubleFree == 0) {
        _printString("!!! Double free success\n");
    } else {
        _printString("Double free failed\n");
    }

    int closeFree = mem_free((void*)((uint64)second + 20 * MEM_BLOCK_SIZE));
    if (closeFree == 0) {
        _printString("!!! Close free success\n");
    } else {
        _printString("Close free failed\n");
    }

    size_t forthSize = 30 * MEM_BLOCK_SIZE;
    void* forth = mem_alloc(forthSize);
    if ((uint64)second != (uint64)forth) {
        _printString("!!! Forth bad alloc\n");
    } else {
        _printString("Forth alloc correct\n");
    }

    mem_free(third);
    mem_free(first);
    mem_free(forth);

    int badFree = mem_free((void*)(endAddress - 15));
    if (badFree == 0) {
        _printString("!!! Bad free went through!\n");
    } else {
        _printString("Bad free errored out\n");
    }
}

void testThreads() {
    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    int noHandle = thread_create(nullptr, workerBodyA, nullptr);
    if (noHandle == 0) {
        _printString("!!! Thread with null handle created\n");
    } else {
        _printString("Null handle thread ignored\n");
    }

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

void testSemClose() {
    sem_t sem = nullptr;
    sem_open(&sem, 0);

    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    thread_t handle0 = nullptr;
    thread_create(&handle0, workerSemWaitCloseA, sem);
    _printString("ThreadA created\n");

    thread_t handle1 = nullptr;
    thread_create(&handle1, workerSemWaitCloseB, sem);
    _printString("ThreadB created\n");

    thread_t handle2 = nullptr;
    thread_create(&handle2, workerSemWaitCloseC, sem);
    _printString("ThreadC created\n");

    thread_t handle3 = nullptr;
    thread_create(&handle3, workerSemWaitCloser, sem);
    _printString("ThreadD created\n");

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

void testOneSignal() {
    sem_t sem = nullptr;
    sem_open(&sem, 0);

    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    thread_t handle0 = nullptr;
    thread_create(&handle0, workerSemWaitOneSignal, sem);
    _printString("ThreadA created\n");

    thread_t handle1 = nullptr;
    thread_create(&handle1, workerSemWaitOneSignal, sem);
    _printString("ThreadB created\n");

    thread_t handle2 = nullptr;
    thread_create(&handle2, workerSemWaitOneSignal, sem);
    _printString("ThreadC created\n");

    thread_t handle3 = nullptr;
    thread_create(&handle3, workerSemOneSignal, sem);
    _printString("ThreadD created\n");

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

    int ret = sem_close(sem);
    _printString("Sem close result: ");
    _printInteger(ret);
    _printString("\n");

    while (idleThread->getState() != _thread::FINISHED) {
        thread_dispatch();
    }

    delete idleThread;
}

void testSignalBeforeWait() {
    sem_t sem = nullptr;
    sem_open(&sem, 2);

    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    thread_t handle0 = nullptr;
    thread_create(&handle0, workerSignalFirst, sem);
    _printString("ThreadA created\n");

    thread_t handle1 = nullptr;
    thread_create(&handle1, workerWaitAfterSignal, sem);
    _printString("ThreadB created\n");

    while (!(handle0->getState() == _thread::FINISHED &&
        handle1->getState() == _thread::FINISHED)) {
        thread_dispatch();
        }

    _printString("Threads done\n");
    done.done = true;

    delete handle0;
    delete handle1;

    int ret = sem_close(sem);
    _printString("Sem close result: ");
    _printInteger(ret);
    _printString("\n");

    while (idleThread->getState() != _thread::FINISHED) {
        thread_dispatch();
    }

    delete idleThread;
}

void testSemSubset() {
    sem_t sem = nullptr;
    sem_open(&sem, 0);

    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    thread_t handle0 = nullptr;
    thread_create(&handle0, workersWaitSubset, sem);
    _printString("ThreadA created\n");

    thread_t handle1 = nullptr;
    thread_create(&handle1, workersWaitSubset, sem);
    _printString("ThreadB created\n");

    thread_t handle2 = nullptr;
    thread_create(&handle2, workersWaitSubset, sem);
    _printString("ThreadC created\n");

    thread_t handle3 = nullptr;
    thread_create(&handle3, workersSignalSubset, sem);
    _printString("ThreadD created\n");

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

    int ret = sem_close(sem);
    _printString("Sem close result: ");
    _printInteger(ret);
    _printString("\n");

    while (idleThread->getState() != _thread::FINISHED) {
        thread_dispatch();
    }

    delete idleThread;
}

void testSemWaitOneSignalMany() {
    sem_t sem = nullptr;
    sem_open(&sem, 0);

    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    thread_t handle0 = nullptr;
    thread_create(&handle0, workersWaitOne, sem);
    _printString("ThreadA created\n");

    thread_t handle1 = nullptr;
    thread_create(&handle1, workersWaitOneSmall, sem);
    _printString("ThreadB created\n");

    thread_t handle2 = nullptr;
    thread_create(&handle2, workersSignalMany, sem);
    _printString("ThreadC created\n");

    while (!(handle0->getState() == _thread::FINISHED &&
        handle1->getState() == _thread::FINISHED)) {
        thread_dispatch();
        }

    _printString("Threads done\n");
    done.done = true;

    delete handle0;
    delete handle1;

    int ret = sem_close(sem);
    _printString("Sem close result: ");
    _printInteger(ret);
    _printString("\n");

    while (idleThread->getState() != _thread::FINISHED) {
        thread_dispatch();
    }

    delete idleThread;
}

void testSleep() {
    thread_t idleThread = nullptr;
    IdleDone done;
    thread_create(&idleThread, idle, &done);

    thread_t handle0 = nullptr;
    thread_create(&handle0, workersSleep50, nullptr);
    _printString("ThreadA created\n");

    thread_t handle1 = nullptr;
    thread_create(&handle1, workersSleep60, nullptr);
    _printString("ThreadB created\n");

    thread_t handle2 = nullptr;
    thread_create(&handle2, workersSleep150, nullptr);
    _printString("ThreadC created\n");

    thread_t handle3 = nullptr;
    thread_create(&handle3, workersSleep80, nullptr);
    _printString("ThreadD created\n");

    thread_t handle4 = nullptr;
    thread_create(&handle4, workersSleep0, nullptr);
    _printString("ThreadE created\n");

    while (!(handle0->getState() == _thread::FINISHED &&
        handle1->getState() == _thread::FINISHED &&
        handle2->getState() == _thread::FINISHED &&
        handle3->getState() == _thread::FINISHED &&
        handle4->getState() == _thread::FINISHED)) {
        thread_dispatch();
    }

    _printString("Threads done\n");
    done.done = true;

    delete handle0;
    delete handle1;
    delete handle2;
    delete handle3;
    delete handle4;

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

    // _printString("---- Test Threads ----\n");
    // testThreads();
    //
    // _printString("\n---- Test Sem Close ----\n");
    // testSemClose();
    //
    // _printString("\n---- Test Sem One Signal ----\n");
    // testOneSignal();
    //
    // _printString("\n---- Test Sem Signal Before Wait ----\n");
    // testSignalBeforeWait();
    //
    // _printString("\n---- Test Sem Subset ----\n");
    // testSemSubset();

    _printString("\n---- Test Sem One Wait Many Signal ----\n");
    testSemWaitOneSignalMany();

    _printString("\n---- Test Sleep ----\n");
    testSleep();

    _printString("\n---- Test Memory ----\n");
    testMemory(mainThread);

    int ret = mem_free(mainThread);
    _printString("Main done. Cleanup: ");
    _printInteger(ret);
    _printString("\n");

    return 0;
}