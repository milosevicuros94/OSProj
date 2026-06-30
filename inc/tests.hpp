#include "../inc/syscall_c.h"

extern void testMemory(_thread* mainThread);
extern void testThreads();
extern void testSemClose();
extern void testOneSignal();
extern void testSignalBeforeWait();
extern void testSemSubset();
extern void testSemWaitOneSignalMany();
extern void testSleep();