#ifndef _MEMORY_ALLOCATOR_HPP
#define _MEMORY_ALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
public:
    static MemoryAllocator& getInstance();
    void* alloc(size_t sizeInBlocks);
    int free(void* address);

private:
    MemoryAllocator();
    ~MemoryAllocator() = default;

    struct Descriptor {
        size_t sizeInBlocks;
        Descriptor* next;
    };

    static size_t ceilToBlock(size_t size) {
        return (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    }
    static size_t floorToBlock(size_t size) {
        return size / MEM_BLOCK_SIZE;
    }
    static size_t blocksToBytes(size_t blocks) {
        return blocks * MEM_BLOCK_SIZE;
    }

    static void tryToJoin(Descriptor* first, Descriptor* second);

    Descriptor* freeHead;
};

#endif