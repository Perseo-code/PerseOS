ENTRY(start) /* El nombre del símbolo en tu boot.asm */

SECTIONS {
    . = 1M; /* El kernel se suele cargar en el primer megabyte */

    .text : {
        *(.multiboot) /* Asegura que el header de arranque esté al principio */
        *(.text)
    }

    .rodata : { *(.rodata) }
    .data   : { *(.data) }
    
    /* Sección para constructores globales de C++ */
    .init_array : {
        PROVIDE(__init_array_start = .);
        *(.init_array)
        PROVIDE(__init_array_end = .);
    }

    .bss : { *(.common) *(.bss) }
}