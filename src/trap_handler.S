.extern _ZN11TrapHandler14handleInternalEv
.extern _ZN11TrapHandler11handleTimerEv
.extern _ZN11TrapHandler14handleExternalEv

.global handlers

.align 4
handlers:
    j internal
    j timer
    .skip 28
    j external

.macro pushRegisters
    addi sp, sp, -256
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        sd x\index, \index * 8(sp)
    .endr
.endm

.macro popRegisters
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256
.endm

internal:
    pushRegisters
    call _ZN11TrapHandler14handleInternalEv
    popRegisters
    sret

timer:
    pushRegisters
    call _ZN11TrapHandler11handleTimerEv
    popRegisters
    sret

external:
    pushRegisters
    call _ZN11TrapHandler14handleExternalEv
    popRegisters
    sret
