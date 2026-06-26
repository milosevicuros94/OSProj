#include "../inc/scheduler.hpp"
#include "../inc/thread.hpp"

Scheduler& Scheduler::getInstance() {
    static Scheduler instance;
    return instance;
}

void Scheduler::putReady(thread_t thread) {
    if (readyQueueHead == nullptr) {
        readyQueueHead = thread;
        readyQueueTail = thread;
    } else {
        readyQueueTail->next = thread;
        readyQueueTail = thread;
    }

    readyQueueTail->next = nullptr;
}

thread_t Scheduler::getReady() {
    thread_t readyThread = readyQueueHead;

    if (readyQueueHead != nullptr) {
        readyQueueHead = readyQueueHead->next;
        if (readyQueueHead == nullptr) {
            readyQueueTail = nullptr;
        }

        readyThread->next = nullptr;
    }

    return readyThread;
}