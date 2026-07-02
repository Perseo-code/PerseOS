ENTRY(_start) /*Donde debe comenzar el archivo*/
SECTIONS {
    . = 0x100000;
    .text : ALIGN(4096) {
        *(.text*) /*Alojamiento para todas las instrucciones ejecutables*/
    }

    .data : ALIGN(4096){
        *(.data) /*Alojamiento para todas las variables inicializadas*/
    }

    .bss : ALIGN(4096) {
        *(COMMON*) /*Alojamiento para bytes reservados*/
        *(.bss)
    }

    .rodata : ALIGN(4096){
        *(.rodata*) /*Alojamiento de variables constantes*/
    }
}