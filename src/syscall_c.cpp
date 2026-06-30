#include "../inc/syscall_c.h"

template <typename T> T ecall(syscall syscall, ...) {
    T volatile ret;
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
    if (handle != nullptr) {
        void* stack = mem_alloc(DEFAULT_STACK_SIZE);
        return ecall<int>(THREAD_CREATE, handle, start_routine, arg, stack);
    }

    return -10;
}

int thread_exit() {
    return ecall<int>(THREAD_EXIT);
}

void thread_dispatch() {
    ecall<void>(THREAD_DISPATCH);
}

int sem_open(sem_t* handle, unsigned init) {
    return ecall<int>(SEM_OPEN, handle, init);
}

int sem_close(sem_t handle) {
    return ecall<int>(SEM_CLOSE, handle);
}

int sem_wait(sem_t id) {
    return ecall<int>(SEM_WAIT, id);
}

int sem_signal(sem_t id) {
    return ecall<int>(SEM_SIGNAL, id);
}

int sem_wait_n(sem_t id, unsigned n) {
    return ecall<int>(SEM_WAIT_N, id, n);
}

int sem_signal_n(sem_t id, unsigned n) {
    return ecall<int>(SEM_SIGNAL_N, id, n);
}

int time_sleep(time_t time) {
    return ecall<int>(TIME_SLEEP, time);
}

char getc() {
    return ecall<char>(GETC);
}
void putc(char c) {
    ecall<void>(PUTC, c);
}