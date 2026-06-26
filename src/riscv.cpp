#include "../inc/riscv.hpp"
#include "../lib/console.h"
#include "../inc/thread.hpp"

void Riscv::popSppSpie() {
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap() {
    uint64 scause = r_scause();

    if (scause == 0x0000000000000009UL) {
        // interrupt: NO, cause code: environment call from S-mode (9)
        uint64 sepc = r_sepc() + 4;
        uint64 sstatus = r_sstatus();
        _thread::timeSliceCounter = 0;
        _thread::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
    } else if (scause == 0x8000000000000001UL) {
        // interrupt: yes, cause code: supervisor software interrupt (timer)
        // TODO: Timer interrupt is here
        _thread::timeSliceCounter++;
        if (_thread::timeSliceCounter >= _thread::running->getTimeSlice()) {
            uint64 sepc = r_sepc();
            uint64 sstatus = r_sstatus();
            _thread::timeSliceCounter = 0;
            _thread::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        mc_sip(SIP_SSIP);
    } else if (scause == 0x8000000000000009UL) {
        // interrupt: yes, cause code: supervisor external interrupt (console)
        // TODO: Console interrupt is here
        console_handler();
    } else {
        // unexpected trap cause
        // print(scause)
        // print(sepc)
        // print(stval)
    }
}
