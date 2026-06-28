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

int main() {
    // OR 1 for vector mode
    Riscv::w_stvec((uint64)handlers | 1);

    _thread* mainThread = (_thread*) mem_alloc(sizeof(_thread));
    mainThread->createMainThread();

    // thread_t handle = nullptr;
    // thread_create(&handle, workerBodyA, nullptr);

    thread_t handle0 = nullptr;
    // thread_t handle1 = nullptr;
    // thread_t handle2 = nullptr;
    // thread_t handle3 = nullptr;

    thread_create(&handle0, workerBodyA, mainThread);
    _printString("ThreadA created\n");
    // thread_create(&handle1, workerBodyB, mainThread);
    // _printString("ThreadB created\n");
    // thread_create(&handle2, workerBodyC, mainThread);
    // _printString("ThreadC created\n");
    // thread_create(&handle3, workerBodyD, mainThread);
    // _printString("ThreadD created\n");
    //
    // Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    //
    // while (!(handle0->getState() == _thread::FINISHED &&
    //     handle1->getState() == _thread::FINISHED &&
    //     handle2->getState() == _thread::FINISHED &&
    //     handle3->getState() == _thread::FINISHED)) {
    //     thread_dispatch();
    // }

    // for (auto &thread : threads) {
    //     delete thread;
    // }

    // void* first = mem_alloc(20 * MEM_BLOCK_SIZE);
    // void* second = mem_alloc(50 * MEM_BLOCK_SIZE);
    // void* third = mem_alloc(10 * MEM_BLOCK_SIZE);
    //
    // mem_free(second);
    // void* forth = mem_alloc(30 * MEM_BLOCK_SIZE);
    // mem_free(third);
    // mem_free(first);
    // mem_free(forth);

    _printString("Main done\n");

    return 0;
}