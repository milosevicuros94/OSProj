#include "../inc/trap_handler.hpp"
#include "../inc/riscv.hpp"
#include "../inc/thread.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"

void TrapHandler::handleInternal() {
    uint64 scause = Riscv::r_scause();
    uint64 sepc = Riscv::r_sepc();
    uint64 sstatus = Riscv::r_sstatus();

    if (scause == 8 || scause == 9) {
        sepc += 4;

        _thread::timeSliceCounter = 0;
        _thread::dispatch();
        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
    } else {
        // unexpected trap cause
        // print(scause)
        // print(sepc)
        // print(stval)
    }
}

void TrapHandler::handleTimer() {
    _thread::timeSliceCounter++;

    Riscv::mc_sip(Riscv::SIP_SSIP);

    if (_thread::timeSliceCounter >= _thread::running->timeSlice) {
        uint64 sepc = Riscv::r_sepc();
        uint64 sstatus = Riscv::r_sstatus();
        _thread::timeSliceCounter = 0;
        _thread::dispatch();
        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
    }
}

void TrapHandler::handleExternal() {
    console_handler();
}
