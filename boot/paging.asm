; Here, we'll enable paging.

;section .paging
%if 0
global pml4, pdpt, pd, pt

pml4: resq 512
pdpt: resq 512
pd:   resq 512
pt:   resq 512

section .text
global setup_paging
setup_paging:
    ; 1. Bulletproof Initialization: Zero out all 16KB of page tables
    mov edi, pml4      ; Point EDI to the start of our tables
    xor eax, eax       ; We want to write zeros
    mov ecx, 4096      ; 4096 DWORDs = 16,384 bytes (16KB)
    rep stosd          ; Write EAX to [EDI] ECX times

    ; 2. Link the paging hierarchy
    ; Because we zeroed everything, we only need to write the lower 32 bits!
    mov eax, pdpt
    or eax, 3
    mov dword [pml4], eax

    mov eax, pd
    or eax, 3
    mov dword [pdpt], eax

    mov eax, pt
    or eax, 3
    mov dword [pd], eax

    ; 3. Fill PT to identity map the first 2 Megabytes
    xor ebx, ebx

.loop:
    mov eax, ebx
    shl eax, 12        ; Multiply by 4096 to get the physical address
    or eax, 3          ; Set Present (bit 0) and R/W (bit 1)
    mov dword [pt + ebx*8], eax ; Write to the lower 32 bits of the entry
    
    inc ebx
    cmp ebx, 512
    jne .loop

    ret
%endif