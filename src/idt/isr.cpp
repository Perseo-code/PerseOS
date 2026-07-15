#include <drivers/vga/vga.hpp>
#include <io/io.hpp>
#include <string.hpp>

static const char* const exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved",
    "Reserved"
};

extern "C" void isr_handler(Registers* regs) {
    clean_screen();

    print("EXCEPTION: ");
    print(exception_messages[regs->int_no]);
    print("\n");
 
    if (regs->int_no < 32)
        print(exception_messages[regs->int_no]);
    else
        print("Unknown exception");

    while (true)
        asm volatile("hlt");
}