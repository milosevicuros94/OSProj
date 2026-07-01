#include "../h/scheduler.hpp"
#include "../h/thread.hpp"

Scheduler& Scheduler::getInstance() {
    static Scheduler instance;
    return instance;
}

void Scheduler::putReady(thread_t thread) {
    if (readyQueueHead == nullptr) {
        readyQueueHead = thread;
        readyQueueTail = thread;
    } else {
        readyQueueTail->setNext(thread);
        readyQueueTail = thread;
    }

    readyQueueTail->setNext(nullptr);
}

thread_t Scheduler::getReady() {
    thread_t readyThread = readyQueueHead;

    if (readyQueueHead != nullptr) {
        readyQueueHead = readyQueueHead->getNext();
        if (readyQueueHead == nullptr) {
            readyQueueTail = nullptr;
        }

        readyThread->setNext(nullptr);
    }

    return readyThread;
}

void Scheduler::insertSleeping(thread_t thread) {
    unsigned sleepTime = thread->getSleepTime();

    thread_t cur = sleepQueueHead;
    thread_t prev = nullptr;
    while (cur != nullptr && cur->getSleepTime() <= sleepTime) {
        sleepTime -= cur->getSleepTime();
        prev = cur;
        cur = cur->getNext();
    }

    if (prev == nullptr) {
        if (cur == nullptr) {
            sleepQueueHead = thread;
        } else {
            sleepQueueHead->decrementSleepTime(sleepTime);
            thread->setNext(sleepQueueHead);
            sleepQueueHead = thread;
        }
    } else if (cur == nullptr) {
        thread->setSleepTime(sleepTime);
        thread->setNext(nullptr);
        prev->setNext(thread);
    } else {
        cur->decrementSleepTime(sleepTime);
        thread->setSleepTime(sleepTime);
        thread->setNext(cur);
        prev->setNext(thread);
    }

    thread->setState(_thread::SLEEPING);
}

void Scheduler::timerTick() {
    if (sleepQueueHead != nullptr) {
        sleepQueueHead->decrementSleepTime();

        while (sleepQueueHead != nullptr && sleepQueueHead->getSleepTime() == 0) {
            thread_t t = sleepQueueHead;
            sleepQueueHead = sleepQueueHead->getNext();

            t->wakeUp();
        }
    }
}
