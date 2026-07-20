#pragma once

#include <stdint.hpp>

#define PAGE_SIZE 4096

extern uint32_t page_directory[1024];
extern uint32_t first_page_table[1024];
extern uint8_t bitmap[2048];

void paging_init();
extern "C" void paging_enable(uint32_t* page_directory);
void pmm_init();
uint32_t pmm_alloc_page();
void pmm_free_page(uint32_t addr);

static inline void setBit(uint32_t bit)
{
    bitmap[bit / 8] |= 1 << (bit % 8);
}

static inline void clearBit(uint32_t bit)
{
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static inline bool testBit(uint32_t bit)
{
    return bitmap[bit / 8] & (1 << (bit % 8));
}