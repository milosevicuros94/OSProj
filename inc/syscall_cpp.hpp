#ifndef _SYSCALL_CPP_HPP_
#define _SYSCALL_CPP_HPP_

#include "syscall_c.h"

void* operator new (size_t size);
void operator delete (void* ptr) noexcept;
void *operator new[](size_t);
void operator delete[](void*) noexcept;

/* ---- Thread ---- */
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
};

/* ---- Semaphore ---- */
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait (unsigned int n = 1);
    int signal (unsigned int n = 1);
private:
    sem_t myHandle;
};

/* ---- Periodic Thread ---- */
class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    void run() final;
    time_t period;
};

/* ---- Console ---- */
class Console {
public:
    static char getc ();
    static void putc (char);
};

#endif