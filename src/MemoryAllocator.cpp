#include "../inc/MemoryAllocator.hpp"

MemoryAllocator &MemoryAllocator::getInstance() {
    static MemoryAllocator instance;
    return instance;
}

MemoryAllocator::MemoryAllocator() {
    startBlock = ceilToBlock((size_t)HEAP_START_ADDR);
    endBlock = floorToBlock((size_t)HEAP_END_ADDR);

    Descriptor* initial = (Descriptor*)blocksToBytes(startBlock);
    initial->next = nullptr;
    initial->sizeInBlocks = endBlock - startBlock;

    freeHead = initial;
}

void* MemoryAllocator::alloc(size_t sizeInBlocks) {
    if (sizeInBlocks == 0) {
        return nullptr;
    }

    // +1 for the Descriptor
    sizeInBlocks += 1;

    Descriptor* cur = freeHead;
    Descriptor* prev = nullptr;

    // First fit
    while (cur != nullptr && cur->sizeInBlocks < sizeInBlocks) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == nullptr) {
        return nullptr;
    }

    if (cur->sizeInBlocks - sizeInBlocks > 0) {
        Descriptor* newDescriptor = (Descriptor*)((size_t)cur + blocksToBytes(sizeInBlocks));
        newDescriptor->sizeInBlocks = cur->sizeInBlocks - sizeInBlocks;
        newDescriptor->next = cur->next;

        // For easier update of prev->next
        cur->next = newDescriptor;
    }

    if (prev != nullptr) {
        prev->next = cur->next;
    } else {
        freeHead = cur->next;
    }

    cur->sizeInBlocks = sizeInBlocks;
    cur->next = nullptr;

    return (void*)((size_t)cur + blocksToBytes(1));
}

int MemoryAllocator::free(void *address) {
    if (address == nullptr) {
        return -1;
    }

    if ((size_t)address % MEM_BLOCK_SIZE != 0) {
        return -2;
    }

    Descriptor* allocated = (Descriptor*)((size_t)address - blocksToBytes(1));

    if ((size_t)allocated < blocksToBytes(startBlock) ||
        (size_t)allocated >= blocksToBytes(endBlock - 1)) {
        return -3;
    }

    Descriptor* cur = freeHead;
    Descriptor* prev = nullptr;
    while (cur != nullptr && (size_t)cur < (size_t)allocated) {
        prev = cur;
        cur = cur->next;
    }

    if (cur != nullptr) {
        if ((size_t)cur == (size_t)allocated) {
            return -4;
        }
        if ((size_t)cur < (size_t)allocated + blocksToBytes(allocated->sizeInBlocks)) {
            return -5;
        }
    }

    if (prev != nullptr) {
        if ((size_t)prev + blocksToBytes(prev->sizeInBlocks) > (size_t)allocated) {
            return -6;
        }
    }

    allocated->next = cur;

    if (prev == nullptr) {
        freeHead = allocated;
    } else {
        prev->next = allocated;
    }

    tryToJoin(allocated, cur);
    tryToJoin(prev, allocated);

    return 0;
}

void MemoryAllocator::tryToJoin(Descriptor *first, Descriptor *second) {
    if (first != nullptr && (size_t)first + blocksToBytes(first->sizeInBlocks) == (size_t)second) {
        first->sizeInBlocks += second->sizeInBlocks;
        first->next = second->next;
    }
}
