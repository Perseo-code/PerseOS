ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .multiboot : {
        *(.multiboot*)
    }

    .text ALIGN(4096) : {
        *(.text*)
    }

    .rodata ALIGN(4096) : {
        *(.rodata*)
    }

    .data ALIGN(4096) : {
        *(.data*)
    }

    /*.paging 0x5000 : {
        *(.paging*)
    }*/
    
    .bss ALIGN(4096) : {
        *(COMMON)
        *(.bss*)
    }
}