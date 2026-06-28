#include "../inc/trap_handler.hpp"
#include "../inc/MemoryAllocator.hpp"
#include "../inc/printing.hpp"
#include "../inc/riscv.hpp"
#include "../inc/scheduler.hpp"
#include "../inc/thread.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"

void TrapHandler::handleInternal() {
    uint64 scause = Riscv::r_scause();
    uint64 sepc = Riscv::r_sepc();
    uint64 sstatus = Riscv::r_sstatus();

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
                    Riscv::sd_a0(-1);
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
                _printString("Thread exit failed");
                Riscv::sd_a0(-1);
                break;
            }
            case TIME_SLEEP: {
                time_t time = Riscv::ld_a1<time_t>();
                _thread::sleep(time);
                Riscv::sd_a0(0);
                break;
            }
            default:
                _printString("Unexpected");
        }
    } else {
        _printString("Unexpected trap cause\n");
        // _printInteger(scause);
        // _printString("\n");
        // _printInteger(sepc);
        // _printString("\n");
        // _printInteger(Riscv::r_stval());
        // _printString("\n");
    }

    Riscv::w_sstatus(sstatus);
    Riscv::w_sepc(sepc);
}

void TrapHandler::handleTimer() {
    Riscv::mc_sip(Riscv::SIP_SSIP);

    Scheduler::getInstance().timerTick();
    bool shouldSwitch = _thread::timerTick();

    if (shouldSwitch) {
        uint64 sepc = Riscv::r_sepc();
        uint64 sstatus = Riscv::r_sstatus();
        _thread::dispatch();
        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
    }
}

void TrapHandler::handleExternal() {
    console_handler();
}
