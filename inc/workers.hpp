#ifndef _WORKERS_HPP_
#define _WORKERS_HPP_

#include "../inc/thread.hpp"

// TODO: DELETE THIS

extern void workerBodyA(void*);
extern void workerBodyB(void*);
extern void workerBodyC(void*);
extern void workerBodyD(void*);

extern void workerSemWaitCloseA(void* ptr);
extern void workerSemWaitCloseB(void* ptr);
extern void workerSemWaitCloseC(void* ptr);
extern void workerSemWaitCloser(void* ptr);

extern void workerSemWaitOneSignal(void* ptr);
extern void workerSemOneSignal(void* ptr);

extern void workerSignalFirst(void* ptr);
extern void workerWaitAfterSignal(void* ptr);

extern void workersWaitSubset(void* ptr);
extern void workersSignalSubset(void* ptr);

extern void workersSleep50(void* ptr);
extern void workersSleep60(void* ptr);
extern void workersSleep150(void* ptr);
extern void workersSleep80(void* ptr);

#endif
