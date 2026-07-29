#include <memory.hpp>
#include <drivers/vga/vga.hpp>
BlockHeader* firstBlock = nullptr;


void* allocateFromBlock(BlockHeader* block, uintptr_t size) {
    if (block->size >= size + HEADER_SIZE + 8)
    {
        BlockHeader* newBlock =
            (BlockHeader*)
            (
                (uint8_t*)block
                + HEADER_SIZE
                + size
            );

        newBlock->size =
            block->size
            - size
            - HEADER_SIZE;

        newBlock->free = true;
        newBlock->next = block->next;

        block->next = newBlock;
        block->size = size;
    }

    block->free = false;

    return (uint8_t*)block + HEADER_SIZE;
}


void* kmalloc(uintptr_t size)
{
    // Align every allocation to 8 bytes.
    // Example:
    // 1  -> 8
    // 9  -> 16
    // 13 -> 16
    size = (size + 7) & ~7;

    // Start looking from the beginning of the heap.
    BlockHeader* current = firstBlock;
    BlockHeader* last = nullptr;
    /*print("firstBlock = ");
    print(hexToString((uint32_t)firstBlock));
    print("\n");

    if (firstBlock != nullptr) {
        print("size = ");ough?
        if (current->free && current->size >= size)
        {
        print(intToString(firstBlock->size));
        print("\n");

        print("free = ");
        print(firstBlock->free ? "true\n" : "false\n");
    }*/
    // Walk through every current.
    while (current != nullptr)
    {
        // Is this current free and large enough?
        if (current->free && current->size >= size)
        {
            // Allocate the block
            return allocateFromBlock(current, size);
        }

        // This current wasn't suitable.
        // Try the next one.
        last = current;
        current = current->next;
    }

    // No current was large enough.
    uint32_t page = pmm_alloc_page();

    if (page == 0) {
        // Out of physical memory
        return nullptr;
    }
    
    BlockHeader* newBlock = (BlockHeader*)page;

    newBlock->size = PAGE_SIZE - HEADER_SIZE;
    newBlock->free = true;
    newBlock->next = nullptr;

    // Add it to the linked list
    if (last->free)
    {
        last->size += HEADER_SIZE + PAGE_SIZE;
    }
    else
    {
        last->next = newBlock;
    }

    current = newBlock;
    return allocateFromBlock(newBlock, size);
}

void kfree(void* ptr)
{
    // Freeing nullptr does nothing.
    if (ptr == nullptr)
        return;

    // The user pointer points AFTER the header.
    //
    // Move backwards to recover it.
    BlockHeader* current =
        (BlockHeader*)
        (
            (uint8_t*)ptr
            - HEADER_SIZE
        );

    // Mark the current as free.
    // A future kmalloc() can reuse it.
    current->free = true;
    
    // Coalescing (Merge adjancent free currents)
    current = firstBlock;

    while (current && current->next)
    {
        if (current->free && current->next->free)
        {
            current->size +=
                HEADER_SIZE
                + current->next->size;

            current->next = current->next->next;

            continue;
        }

        current = current->next;
    }
}

void* krealloc(void* ptr, size_t newSize) {
    // Reallocate the selection.
    if (ptr == nullptr) {
        return kmalloc(newSize);
    }

    if (newSize == 0) {
        kfree(ptr);
        return nullptr;
    }

    BlockHeader* current = (BlockHeader*)((uint8_t*)ptr - HEADER_SIZE);
    
    size_t alignedNewSize = (newSize + 7) & ~7;
    if (alignedNewSize <= current->size) {
        

        if (current->size >= alignedNewSize + HEADER_SIZE + 8) {
            
            char* current_payload_start = (char*)current + HEADER_SIZE;
            BlockHeader* next_free_current = (BlockHeader*)(current_payload_start + alignedNewSize);

            next_free_current->size = current->size - alignedNewSize - HEADER_SIZE;
            next_free_current->free = true;
            next_free_current->next = current->next;

            current->size = alignedNewSize;
            current->next = next_free_current;
        }
        
        return ptr; 
    }

    if (current->next != nullptr && current->next->free && (current->size + HEADER_SIZE + current->next->size) >= newSize) {
        current->size += HEADER_SIZE + current->next->size;
        current->next = current->next->next;

        return ptr;
    }

    void* new_ptr = kmalloc(newSize);
    if (new_ptr == nullptr) return nullptr;

    memcpy(new_ptr, ptr, current->size);

    kfree(ptr);
    return new_ptr;
}
