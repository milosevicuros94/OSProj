#include "../inc/trap_handler.hpp"
#include "../inc/MemoryAllocator.hpp"
#include "../inc/printing.hpp"
#include "../inc/riscv.hpp"
#include "../inc/scheduler.hpp"
#include "../inc/semaphore.hpp"
#include "../inc/thread.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"

void TrapHandler::handleInternal() {
    uint64 scause = Riscv::r_scause();
    volatile uint64 sepc = Riscv::r_sepc();
    volatile uint64 sstatus = Riscv::r_sstatus();

    if (scause == 8 || scause == 9) {
        sepc += 4;

        uint64 call = Riscv::ld_a0<uint64>();
        switch (call) {
            case MEM_ALLOC: {
                size_t sizeInBlocks = Riscv::ld_a1<size_t>();
                void* adr = MemoryAllocator::getInstance().alloc(sizeInBlocks);
                Riscv::sd_a0(adr);

                break;
            }
            case MEM_FREE: {
                void* adr = Riscv::ld_a1<void*>();
                int ret = MemoryAllocator::getInstance().free(adr);
                Riscv::sd_a0(ret);

                break;
            }
            case THREAD_CREATE: {
                thread_t* handle = Riscv::ld_a1<thread_t*>();
                _thread::Body startRoutine = Riscv::ld_a2<_thread::Body>();
                void* arg = Riscv::ld_a3<void*>();
                void* stack = Riscv::ld_a4<void*>();

                if (handle == nullptr || startRoutine == nullptr || stack == nullptr) {
                    Riscv::sd_a0(-10);
                } else {
                    thread_t t = new _thread(startRoutine, arg, stack);
                    *handle = t;
                    Riscv::sd_a0(0);
                }

                break;
            }
            case THREAD_DISPATCH: {
                _thread::dispatch();

                break;
            }
            case THREAD_EXIT: {
                _thread::exit();
                Riscv::sd_a0(-10);

                break;
            }
            case SEM_OPEN: {
                sem_t* handle = Riscv::ld_a1<sem_t*>();
                unsigned init = Riscv::ld_a2<unsigned>();

                if (handle == nullptr) {
                    Riscv::sd_a0(-10);
                } else {
                    sem_t sem = new _sem(init);
                    *handle = sem;
                    Riscv::sd_a0(0);
                }

                break;
            }
            case SEM_CLOSE: {
                sem_t sem = Riscv::ld_a1<sem_t>();

                if (sem == nullptr) {
                    Riscv::sd_a0(-10);
                } else {
                    int ret = sem->close();
                    Riscv::sd_a0(ret);
                }

                break;
            }
            case SEM_WAIT: {
                sem_t sem = Riscv::ld_a1<sem_t>();

                if (sem == nullptr) {
                    Riscv::sd_a0(-10);
                } else {
                    int ret = sem->wait();
                    Riscv::sd_a0(ret);
                }

                break;
            }
            case SEM_SIGNAL: {
                sem_t sem = Riscv::ld_a1<sem_t>();

                if (sem == nullptr) {
                    Riscv::sd_a0(-10);
                } else {
                    int ret = sem->signal();
                    Riscv::sd_a0(ret);
                }

                break;
            }
            case SEM_WAIT_N: {
                sem_t sem = Riscv::ld_a1<sem_t>();
                unsigned n = Riscv::ld_a2<unsigned>();

                if (sem == nullptr) {
                    Riscv::sd_a0(-10);
                } else {
                    int ret = sem->wait(n);
                    Riscv::sd_a0(ret);
                }

                break;
            }
            case SEM_SIGNAL_N: {
                sem_t sem = Riscv::ld_a1<sem_t>();
                unsigned n = Riscv::ld_a2<unsigned>();

                if (sem == nullptr) {
                    Riscv::sd_a0(-10);
                } else {
                    int ret = sem->signal(n);
                    Riscv::sd_a0(ret);
                }

                break;
            }
            case TIME_SLEEP: {
                time_t time = Riscv::ld_a1<time_t>();
                _thread::sleep(time);
                Riscv::sd_a0(0);

                break;
            }
            case GETC: {
                char c = __getc();
                Riscv::sd_a0(c);

                break;
            }
            case PUTC: {
                char c = Riscv::ld_a1<char>();
                __putc(c);

                break;
            }
            default:
                _printString("Unexpected call type: ");
                _printInteger(call, 16);
                _printString("\n");
                return;
        }

        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
    } else {
        _printString("Unexpected trap cause\n");
        _printString("scause: ");
        _printInteger(scause, 16);
        _printString("; sepc: ");
        _printInteger(sepc, 16);
        _printString("; stval: ");
        _printInteger(Riscv::r_stval(), 16);
        _printString("\n");

        Riscv::kill();
    }
}

void TrapHandler::handleTimer() {
    Riscv::mc_sip(Riscv::SIP_SSIP);

    Scheduler::getInstance().timerTick();
    bool timeSliceExpired = _thread::timerTick();

    if (timeSliceExpired) {
        volatile uint64 sepc = Riscv::r_sepc();
        volatile uint64 sstatus = Riscv::r_sstatus();
        _thread::dispatch();
        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
    }
}

void TrapHandler::handleExternal() {
    console_handler();
}
