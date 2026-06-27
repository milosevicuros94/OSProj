#ifndef _RISCV_HPP
#define _RISCV_HPP

#include "../lib/hw.h"

class Riscv {
public:
    enum BitMaskSip {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9)
    };

    enum BitMaskSstatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8)
    };

    // static constexpr int A0 = 10;
    // static constexpr int A1 = 11;
    // static constexpr int A2 = 12;
    // static constexpr int A3 = 13;
    // static constexpr int A4 = 14;
    // static constexpr int A5 = 15;
    // static constexpr int A6 = 16;
    // static constexpr int A7 = 17;

    enum ArgRegs {
        A0 = 10,
        A1 = 11,
        A2 = 12,
        A3 = 13,
        A4 = 14,
        A5 = 15,
        A6 = 16,
        A7 = 17
    };

    // Has to be non-inline function
    static void popSppSpie();

    static uint64 r_scause();
    static void w_scause(uint64 scause);

    static uint64 r_sepc();
    static void w_sepc(uint64 sepc);

    static uint64 r_stvec();
    static void w_stvec(uint64 stvec);

    static uint64 r_stval();
    static void w_stval(uint64 stval);

    // Mask set sip
    static void ms_sip(uint64 mask);
    // Mask clear sip
    static void mc_sip(uint64 mask);
    static uint64 r_sip();
    static void w_sip(uint64 sip);

    // Mask set sstatus
    static void ms_sstatus(uint64 mask);
    // Mask clear sstatus
    static void mc_sstatus(uint64 mask);
    static uint64 r_sstatus();
    static void w_sstatus(uint64 sstatus);

    template <typename T> static T ld_a0();
    template <typename T> static T ld_a1();
    template <typename T> static T ld_a2();
    template <typename T> static T ld_a3();
    template <typename T> static T ld_a4();
    template <typename T> static T ld_a5();
    template <typename T> static T ld_a6();
    template <typename T> static T ld_a7();

    template <typename T> static void sd_a0(T t);
    template <typename T> static void sd_a1(T t);
    template <typename T> static void sd_a2(T t);
    template <typename T> static void sd_a3(T t);
    template <typename T> static void sd_a4(T t);
    template <typename T> static void sd_a5(T t);
    template <typename T> static void sd_a6(T t);
    template <typename T> static void sd_a7(T t);

};

inline uint64 Riscv::r_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void Riscv::w_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 Riscv::r_sepc() {
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void Riscv::w_sepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 Riscv::r_stvec() {
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void Riscv::w_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 Riscv::r_stval() {
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void Riscv::w_stval(uint64 stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void Riscv::ms_sip(uint64 mask) {
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sip(uint64 mask) {
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sip() {
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void Riscv::w_sip(uint64 sip) {
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void Riscv::ms_sstatus(uint64 mask) {
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask) {
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sstatus() {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void Riscv::w_sstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

template <typename T> inline T Riscv::ld_a0() {
    T ret;
    __asm__ volatile ("ld %[ret], %[offset]*8(s0)" : [ret] "=r"(ret) : [offset] "i"(A0));
    return ret;
}

template <typename T> inline T Riscv::ld_a1() {
    T ret;
    __asm__ volatile ("ld %[ret], %[offset]*8(s0)" : [ret] "=r"(ret) : [offset] "i"(A1));
    return ret;
}

template <typename T> inline T Riscv::ld_a2() {
    T ret;
    __asm__ volatile ("ld %[ret], %[offset]*8(s0)" : [ret] "=r"(ret) : [offset] "i"(A2));
    return ret;
}

template <typename T> inline T Riscv::ld_a3() {
    T ret;
    __asm__ volatile ("ld %[ret], %[offset]*8(s0)" : [ret] "=r"(ret) : [offset] "i"(A3));
    return ret;
}

template <typename T> inline T Riscv::ld_a4() {
    T ret;
    __asm__ volatile ("ld %[ret], %[offset]*8(s0)" : [ret] "=r"(ret) : [offset] "i"(A4));
    return ret;
}

template <typename T> inline T Riscv::ld_a5() {
    T ret;
    __asm__ volatile ("ld %[ret], %[offset]*8(s0)" : [ret] "=r"(ret) : [offset] "i"(A5));
    return ret;
}

template <typename T> inline T Riscv::ld_a6() {
    T ret;
    __asm__ volatile ("ld %[ret], %[offset]*8(s0)" : [ret] "=r"(ret) : [offset] "i"(A6));
    return ret;
}

template <typename T> inline T Riscv::ld_a7() {
    T ret;
    __asm__ volatile ("ld %[ret], %[offset]*8(s0)" : [ret] "=r"(ret) : [offset] "i"(A7));
    return ret;
}

template<typename T> inline void Riscv::sd_a0(T t) {
    __asm__ volatile ("sd %[t], %[offset]*8(s0)" : : [offset] "i"(A0), [t] "r"(t));
}

template<typename T> inline void Riscv::sd_a1(T t) {
    __asm__ volatile ("sd %[t], %[offset]*8(s0)" : : [offset] "i"(A1), [t] "r"(t));
}

template<typename T> inline void Riscv::sd_a2(T t) {
    __asm__ volatile ("sd %[t], %[offset]*8(s0)" : : [offset] "i"(A2), [t] "r"(t));
}

template<typename T> inline void Riscv::sd_a3(T t) {
    __asm__ volatile ("sd %[t], %[offset]*8(s0)" : : [offset] "i"(A3), [t] "r"(t));
}

template<typename T> inline void Riscv::sd_a4(T t) {
    __asm__ volatile ("sd %[t], %[offset]*8(s0)" : : [offset] "i"(A4), [t] "r"(t));
}

template<typename T> inline void Riscv::sd_a5(T t) {
    __asm__ volatile ("sd %[t], %[offset]*8(s0)" : : [offset] "i"(A5), [t] "r"(t));
}

template<typename T> inline void Riscv::sd_a6(T t) {
    __asm__ volatile ("sd %[t], %[offset]*8(s0)" : : [offset] "i"(A6), [t] "r"(t));
}

template<typename T> inline void Riscv::sd_a7(T t) {
    __asm__ volatile ("sd %[t], %[offset]*8(s0)" : : [offset] "i"(A7), [t] "r"(t));
}

#endif
