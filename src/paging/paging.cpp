#include <paging/paging.hpp>

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
void paging_init() {
    for (uint32_t i = 0; i < 1024; i++)
    {
        first_page_table[i] = (i * 0x1000) | 3;
    }

    for (int i = 0; i < 1024; i++)
        page_directory[i] = 0;
    
    page_directory[0] =
        ((uint32_t)first_page_table) | 3;
    
    paging_enable(page_directory);
}

void pmm_init()
{
    for (int i = 0; i < sizeof(bitmap); i++)
        bitmap[i] = 0xFF;
    for (uint32_t i = 0; i < 16384; i++)
        clearBit(i);
    for (uint32_t i = 0; i < 256; i++)
        setBit(i);
}

uint32_t pmm_alloc_page()
{
    for (uint32_t i = 0; i < 16384; i++)
    {
        if (!testBit(i))
        {
            setBit(i);
            return i * PAGE_SIZE;
        }
    }

    return 0;
}

void pmm_free_page(uint32_t addr)
{
    clearBit(addr / PAGE_SIZE);
}