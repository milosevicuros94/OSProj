.global _ZN5Riscv4killEv

_ZN5Riscv4killEv:
    la t0, 0x100000
    la t1, 0x5555
    sw t1, (t0)