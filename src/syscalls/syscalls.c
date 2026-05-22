#include <syscalls/syscalls.h>

uint32_t c_syscall_handler(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx) {
    switch (eax) {
        case SYS_WRITE:
            print((const char*)ebx); // Gets the ebx register and changes it to string type
            return 0;

        case SYS_READ:
            input((char*)ebx, ecx); // Here, ebx is the buffer (Where you save the text), and ecx is the maximum of characters to write
            return 0;
        
        case SYS_CLEAR:
            clear(); // Simply calls the clear function
            return 0;
        
        case SYS_EXIT:
            while (1) { __asm__ volatile ("hlt"); } // Does nothing until the next interruption
            return 0;
        default:
            return -1; // Unknown syscall
    }
}