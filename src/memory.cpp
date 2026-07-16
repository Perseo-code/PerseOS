#include <memory.hpp>

void* kmalloc(uint32_t size)
{
    // Align every allocation to 8 bytes.
    // Example:
    // 1  -> 8
    // 9  -> 16
    // 13 -> 16
    size = (size + 7) & ~7;

    // Start looking from the beginning of the heap.
    BlockHeader* current = firstBlock;

    // Walk through every block.
    while (current != nullptr)
    {
        // Is this block free and large enough?
        if (current->free && current->size >= size)
        {
            // Can we split it?
            //
            // We only split if there is enough room left for:
            //  - another BlockHeader
            //  - at least 8 usable bytes
            if (current->size >= size + sizeof(BlockHeader) + 8)
            {
                // The new block begins immediately after:
                //
                // [Header][Requested bytes]
                //
                // So:
                // newBlock =
                // current
                // + sizeof(BlockHeader)
                // + requested size
                BlockHeader* newBlock =
                    (BlockHeader*)
                    (
                        (uint8_t*)current
                        + sizeof(BlockHeader)
                        + size
                    );

                // The remaining free space becomes the new block.
                newBlock->size =
                    current->size
                    - size
                    - sizeof(BlockHeader);

                newBlock->free = true;

                // Insert the new block into the linked list.
                newBlock->next = current->next;
                current->next = newBlock;

                // The current block now only represents
                // the requested allocation.
                current->size = size;
            }

            // Whether we split or not,
            // this block is now in use.
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
                + sizeof(BlockHeader);
        }

        // This block wasn't suitable.
        // Try the next one.
        current = current->next;
    }

    // No block was large enough.
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
    BlockHeader* block =
        (BlockHeader*)
        (
            (uint8_t*)ptr
            - sizeof(BlockHeader)
        );

    // Mark the block as free.
    // A future kmalloc() can reuse it.
    block->free = true;

    // Later we'll merge neighbouring free blocks here.
}