#include "../inc/riscv.hpp"

void Riscv::popSppSpie() {
    __asm__ volatile ("csrc sstatus , %[mask]" : : [mask] "r"(SSTATUS_SPP));
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}
