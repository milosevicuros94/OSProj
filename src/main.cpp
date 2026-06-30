#include "../inc/thread.hpp"
#include "../inc/printing.hpp"
#include "../inc/riscv.hpp"
#include "../inc/tests.hpp"

extern void userMain();
//
// void userMainWrapper(void*) {
//     userMain();
// }

// struct IdleDone {
//     volatile bool done = false;
// };
//
// void idle(void* arg) {
//     IdleDone *done = (IdleDone *)arg;
//     while (!done->done) {
//         thread_dispatch();
//     }
//     _printString("Idle done\n");
// }

int main() {
    Riscv::set_stvec_handlers();

    _thread* mainThread = (_thread*)mem_alloc(sizeof(_thread));
    mainThread->initMainThread();

    _printString("---- Test Threads ----\n");
    testThreads();

    _printString("\n---- Test Sem Close ----\n");
    testSemClose();

    _printString("\n---- Test Sem One Signal ----\n");
    testOneSignal();

    _printString("\n---- Test Sem Signal Before Wait ----\n");
    testSignalBeforeWait();

    _printString("\n---- Test Sem Subset ----\n");
    testSemSubset();

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