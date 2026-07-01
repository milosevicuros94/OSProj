#include "../inc/printing.hpp"
#include "../inc/syscall_c.h"

void _printString(const char *string) {
    while (*string != '\0')
    {
        putc(*string);
        string++;
    }
}

void _printInteger(int xx, int base, int sgn) {
    char digits[] = "0123456789ABCDEF";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if(sgn && xx < 0){
        neg = 1;
        x = -xx;
    } else {
        x = xx;
    }

    i = 0;
    do{
        buf[i++] = digits[x % base];
    }while((x /= base) != 0);
    if(neg)
        buf[i++] = '-';

    while(--i >= 0)
        putc(buf[i]);

}