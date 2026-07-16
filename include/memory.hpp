#pragma once
#include <stdint.hpp>
#include <paging/paging.hpp>

static uint8_t* heap_start;
static BlockHeader* firstBlock = nullptr;
static uint8_t* heap_end;

struct BlockHeader {
    uint32_t size;
    bool free;
    BlockHeader* next;
};

inline void heap_init() {
    firstBlock = (BlockHeader*)pmm_alloc_page();

    firstBlock->size =
        4096 - sizeof(BlockHeader);

    firstBlock->free = true;
    firstBlock->next = nullptr;
}
void* kmalloc(uint32_t size);
void kfree(void*);
