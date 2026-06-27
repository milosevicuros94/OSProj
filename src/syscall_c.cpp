#include "../inc/syscall_c.h"

template <typename T> T envcall(syscall syscall, ...) {
    T ret;
    __asm__ volatile ("ecall");
    __asm__ volatile("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

void* mem_alloc(size_t size) {
    size_t sizeInBlocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return envcall<void*>(MEM_ALLOC, sizeInBlocks);
}

int mem_free(void* ptr) {
    return envcall<int>(MEM_FREE, ptr);
}