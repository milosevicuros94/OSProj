#include "../inc/printing.hpp"
#include "../inc/riscv.hpp"
# include "../inc/syscall_c.h"

void _printString(const char *string) {
    while (*string != '\0')
    {
        putc(*string);
        string++;
    }
}

void _printInteger(uint64 integer) {
    static char digits[] = "0123456789";
    char buf[16];
    int i, neg;
    uint64 x;

    neg = 0;
    if (integer < 0) {
        neg = 1;
        x = -integer;
    } else {
        x = integer;
    }

    i = 0;
    do
    {
        buf[i++] = digits[x % 10];
    } while ((x /= 10) != 0);

    if (neg)
        buf[i++] = '-';

    while (--i >= 0) { putc(buf[i]); }
}