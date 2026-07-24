#include <memory.hpp>
#include <drivers/vga/vga.hpp>
BlockHeader* firstBlock = nullptr;


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
            // Can we split it?
            //
            // We only split if there is enough room left for:
            //  - another BlockHeader
            //  - at least 8 usable bytes
            if (current->size >= size + HEADER_SIZE + 8)
            {
                // The new current begins immediately after:
                //
                // [Header][Requested bytes]BlockHeader
                //
                // So:
                // newcurrent =
                // current
                // + sizeof(BlockHeader)
                // + requested size
                BlockHeader* newcurrent =
                    (BlockHeader*)
                    (
                        (uint8_t*)current
                        + HEADER_SIZE
                        + size
                    );

                // The remaining free space becomes the new current.
                newcurrent->size =
                    current->size
                    - size
                    - HEADER_SIZE;

                newcurrent->free = true;

                // Insert the new current into the linked list.
                newcurrent->next = current->next;
                current->next = newcurrent;

                // The current current now only represents
                // the requested allocation.
                current->size = size;
            }

            // Whether we split or not,
            // this current is now in use.
            current->free = false;

            // Return the address AFTER the header.
            //
            // User receives:
            //
            // [Header][User memory]
            //          ^
            //          returned pointer
            return
                (uint8_t*)current
                + HEADER_SIZE;
        }

        // This current wasn't suitable.
        // Try the next one.
        current = current->next;
    }

    // No current was large enough.
    // Later we'll grow the heap here.
    return nullptr;
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
