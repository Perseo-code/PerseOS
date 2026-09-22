#include "kernel.hpp"
using namespace VIDEO;
void init() {
    clean_screen();
    idt_init();
    pic_remap();
    PIT::init(1193);
    outb(PIC1_DATA, 0xFC);
    outb(PIC2_DATA, 0xFF);
    paging_init();
    pmm_init();
    heap_init();
    asm volatile ("sti"); // Enable interruptions
    ramfs.init();
    is_ramfs = true;
    ATA* disk = (ATA*)kmalloc(sizeof(ATA));

    disk->init();
    persistent_filesystem.init(disk);
}

extern "C" void kernel_main() {
    init();
    print("Welcome to PerseOS \n");
    print("Write 'help' to see every command for now \n");
    shell_init();
    while (true) {}
}

extern "C" void __cxa_pure_virtual() {}

/*
Hello. You've made it here.
Congratulations.
*/

/*
R.I.P
Pinky
Summer, 2020 - Summer, 2026
*/