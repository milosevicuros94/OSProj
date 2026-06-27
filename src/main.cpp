#include "../inc/thread.hpp"
#include "../inc/printing.hpp"
#include "../inc/workers.hpp"
#include "../inc/riscv.hpp"

extern "C" void handlers();

int main() {
    _thread* threads[5];

    threads[0] = _thread::createThread(nullptr);
    _thread::running = threads[0];

    threads[1] = _thread::createThread(workerBodyA);
    _printString("ThreadA created\n");
    threads[2] = _thread::createThread(workerBodyB);
    _printString("ThreadB created\n");
    threads[3] = _thread::createThread(workerBodyC);
    _printString("ThreadC created\n");
    threads[4] = _thread::createThread(workerBodyD);
    _printString("ThreadD created\n");

    // OR 1 for vector mode
    Riscv::w_stvec((uint64)handlers | 1);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!(threads[1]->isFinished() &&
        threads[2]->isFinished() &&
        threads[3]->isFinished() &&
        threads[4]->isFinished())) {
        _thread::yield();
    }

    for (auto &thread : threads) {
        delete thread;
    }

    _printString("Finished\n");

    return 0;
}