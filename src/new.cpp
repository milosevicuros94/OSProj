#include "../inc/syscall_c.h"

void *operator new(uint64 n) {
    return mem_alloc(n);
}

void *operator new[](uint64 n) {
    return mem_alloc(n);
}

void operator delete(void *p) {
    mem_free(p);
}

void operator delete[](void *p) {
    mem_free(p);
}