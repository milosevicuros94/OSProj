#include "../h/syscall_cpp.hpp"

void* operator new (size_t size) {
    return mem_alloc(size);
}

void* operator new[] (size_t size) {
    return mem_alloc(size);
}

void operator delete (void* ptr) noexcept {
    mem_free(ptr);
}

void operator delete[] (void* ptr) noexcept {
    mem_free(ptr);
}

/* ---- Thread ---- */
Thread::Thread(void (*body)(void*), void* arg) : myHandle(nullptr), body(body), arg(arg) {}

Thread::~Thread() = default;

int Thread::start() {
    using Body = void(*)(void*);

    void *tcArg = body ? arg : this;
    Body tcBody = body ? body : (Body) [](void* arg) {
        Thread* t = (Thread*) arg;
        t->run();
    };

    return thread_create(&myHandle, tcBody, tcArg);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

Thread::Thread() : myHandle(nullptr), body(nullptr), arg(nullptr) {}

/* ---- Semaphore ---- */
Semaphore::Semaphore(unsigned init) : myHandle(nullptr) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait(unsigned n) {
    if (n == 1) {
        return sem_wait(myHandle);
    } else {
        return sem_wait_n(myHandle, n);
    }
}

int Semaphore::signal(unsigned n) {
    if (n == 1) {
        return sem_signal(myHandle);
    } else {
        return sem_signal_n(myHandle, n);
    }
}

/* ---- Periodic Thread ---- */
PeriodicThread::PeriodicThread(time_t period) : Thread(), period(period) {}

void PeriodicThread::terminate() {
     period = (time_t)-1;
}

void PeriodicThread::run() {
    if (period == (time_t)-1) return;

    while (period != (time_t)-1) {
        periodicActivation();
        time_sleep(period);
    }
}

/* ---- Console ---- */
char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}
