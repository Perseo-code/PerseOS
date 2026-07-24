#pragma once
#include <stdint.hpp>
#include <paging/paging.hpp>
//#include <drivers/vga/vga.hpp>
struct BlockHeader {
    uint32_t size;
    bool free;
    BlockHeader* next;
};

constexpr uintptr_t HEADER_SIZE =
    (sizeof(BlockHeader) + 7) & ~7;
static uint8_t* heap_start;
extern BlockHeader* firstBlock;
static uint8_t* heap_end;

inline void heap_init() {
    firstBlock = (BlockHeader*)pmm_alloc_page();

    firstBlock->size =
        4096 - HEADER_SIZE;

    firstBlock->free = true;
    firstBlock->next = nullptr;
}
void* kmalloc(uintptr_t size);
void kfree(void*);
void* krealloc(void* ptr, uintptr_t size);