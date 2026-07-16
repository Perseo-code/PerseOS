BITS 32

global paging_enable

paging_enable:
    ; [esp+4] → page directory addresses
    mov eax, [esp + 4]

    ; Load page directory
    mov cr3, eax

    ; Enable paging (set PG bit in CR0)
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ret