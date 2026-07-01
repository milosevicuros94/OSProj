#ifndef _TESTS_HPP_
#define _TESTS_HPP_

#include "syscall_c.h"
#include "thread.hpp"

extern void testMemory(_thread* mainThread);
extern void testThreads();
extern void testSemClose();
extern void testOneSignal();
extern void testSignalBeforeWait();
extern void testSemSubset();
extern void testSemWaitOneSignalMany();
extern void testSleep();
extern void runAllTests(void* ptr);

#endif
