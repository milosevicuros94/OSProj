#include "../inc/thread.hpp"
#include "../inc/printing.hpp"
#include "../inc/riscv.hpp"

extern "C" void handlers();

// extern void userMain();
//
// void userMainWrapper(void*) {
//     userMain();
// }

int main() {
    // _thread* threads[5];
    //
    // threads[0] = _thread::createThread(nullptr);
    // _thread::running = threads[0];
    //
    // threads[1] = _thread::createThread(workerBodyA);
    // _printString("ThreadA created\n");
    // threads[2] = _thread::createThread(workerBodyB);
    // _printString("ThreadB created\n");
    // threads[3] = _thread::createThread(workerBodyC);
    // _printString("ThreadC created\n");
    // threads[4] = _thread::createThread(workerBodyD);
    // _printString("ThreadD created\n");
    //
    // OR 1 for vector mode
    Riscv::w_stvec((uint64)handlers | 1);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    _thread* mainThread = _thread::createThread(nullptr);
    _thread::running = mainThread;

    // _thread* userThread = _thread::createThread(userMainWrapper);

    // while (!(userThread->isFinished())) {
    //     _thread::yield();
    // }
    // for (auto &thread : threads) {
    //     delete thread;
    // }
    //
    // _printString("Finished\n");

    void* first = mem_alloc(20 * MEM_BLOCK_SIZE);
    void* second = mem_alloc(50 * MEM_BLOCK_SIZE);
    void* third = mem_alloc(10 * MEM_BLOCK_SIZE);

    mem_free(second);
    void* forth = mem_alloc(30 * MEM_BLOCK_SIZE);
    mem_free(third);
    mem_free(first);
    mem_free(forth);

    return 0;
}