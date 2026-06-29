#include "../inc/semaphore.hpp"
#include "../inc/thread.hpp"

_sem::_sem(unsigned init) : resources(init) {
    closed = false;
    waitQueueHead = nullptr;
    waitQueueTail = nullptr;
    waitingAfterClose = 0;
}

int _sem::close() {
    if (closed) {
        return -1;
    }

    closed = true;
    while (waitQueueHead != nullptr) {
        waitingAfterClose++;

        thread_t t = waitQueueHead;
        waitQueueHead = waitQueueHead->getNext();

        t->wakeUp();
    }
    waitQueueTail = nullptr;

    if (waitingAfterClose == 0) {
        delete this;
    }

    return 0;
}

int _sem::signal(unsigned n) {
    if (closed) {
        return -1;
    }

    while (waitQueueHead != nullptr && waitQueueHead->getSemResources() <= n) {
        n -= waitQueueHead->getSemResources();

        thread_t t = waitQueueHead;
        waitQueueHead = waitQueueHead->getNext();

        t->wakeUp();
    }

    if (waitQueueHead != nullptr) {
        waitQueueHead->decrementSemResources(n);
    } else {
        resources += n;
        waitQueueTail = nullptr;
    }

    return 0;
}

int _sem::wait(unsigned n) {
    if (closed) {
        return -1;
    }

    if (resources >= n) {
        resources -= n;
        return 0;
    }

    _thread::running->setNext(nullptr);
    _thread::running->setState(_thread::SEM_WAIT);
    _thread::running->setSemResources(n - resources);
    resources = 0;

    if (waitQueueHead == nullptr) {
        waitQueueHead = _thread::running;
        waitQueueTail = _thread::running;
    } else {
        waitQueueTail->setNext(_thread::running);
        waitQueueTail = _thread::running;
    }

    thread_dispatch();

    if (closed) {
        waitingAfterClose--;
        if (waitingAfterClose == 0) {
            delete this;
        }

        return -2;
    }

    return 0;
}