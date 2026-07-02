ENTRY(_start) /*The entry point*/
SECTIONS {
    . = 0x100000;
    .multiboot : {
        *(.multiboot*) /*Where we save all multiboot headers*/
    }

    .text : ALIGN(4096) {
        *(.text*) /*Where we save all executable instructions*/
    }

    .data : ALIGN(4096){
        *(.data) /*Where all initialized variables reside*/
    }

    .bss : ALIGN(4096) {
        *(COMMON*) /*Where all the reserved memory will be in*/
        *(.bss)
    }

    .rodata : ALIGN(4096){
        *(.rodata*) /*Where constant data is stored*/
    }
}