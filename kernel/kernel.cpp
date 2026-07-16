#include "kernel.hpp"

void init() {
    clean_screen();
    idt_init();
    pic_remap();
    outb(PIC1_DATA, 0xFC);
    outb(PIC2_DATA, 0xFF);
    paging_init();
    pmm_init();
    asm volatile ("sti"); // Enable interruptions
}

extern "C" void kernel_main() {
    init();
    print("Welcome to PerseOS \n");
    print("Write 'help' to see every command for now \n");
    shell_init();
    while (true) {}
}