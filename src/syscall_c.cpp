#include "../inc/syscall_c.h"

template <typename T> T ecall(syscall syscall, ...) {
    T ret;
    __asm__ volatile ("ecall");
    __asm__ volatile("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

template <> void ecall(syscall syscall, ...) {
    __asm__ volatile ("ecall");
}

void* mem_alloc(size_t size) {
    size_t sizeInBlocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return ecall<void*>(MEM_ALLOC, sizeInBlocks);
}

int mem_free(void* ptr) {
    return ecall<int>(MEM_FREE, ptr);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    void* stack = mem_alloc(DEFAULT_STACK_SIZE);
    return ecall<int>(THREAD_CREATE, handle, start_routine, arg, stack);
}

int thread_exit() {
    return ecall<int>(THREAD_EXIT);
}

void thread_dispatch() {
    ecall<void>(THREAD_DISPATCH);
}


int time_sleep(time_t time) {
    return ecall<int>(TIME_SLEEP, time);
}
