; The GDT is a table that describes memory segments.
; In 32-bit mode, the gdt defines lots of things, like:
; - Base address - Limit (Size) - Access Rights
; In 64-bit long mode, the segmentation is almost entirely disabled
; So, why do we need one? To jump from the secure mode (32-bit arch) to the long mode (64-bit arch)

; We only need 3 entries for now:
; - Null descriptor - 64-bit code descriptor - Data descriptor
; Each is 8 bytes, so 8*3 = 24
; Intel requires the selector 0 to be invalid (That's why the null descriptor is the first one)

; We'll use selectors instead of indices.

; Base address: In 64-bit, we always start in 0.
; Limit: The largest offset allowed in a segment. 0xFFFFFF is exactly 1MiB, but an OS doesn't fit there, so we use the Granularity
; variable, which is 'G'. If G=0 then, the limit is measured in bytes, but if G=1, the limit is measured in 4KiB pages, so it becomes aprox. 4 GiB
; Access byte: It determines if the segment is a code or data segment, if it is executable, if it can be written to, and if it's present on memory.

; Access byte is composed by:

; 1st bit (bit 7) is = Present. Determines if the segment is present or not. 1 = present, 0 = not present

; Bits 6-5: DPL (descriptor privilege level) = The 2 bits represent the privilege level required to use the segment.
; There are 4 priveleges:
; | DPL | Ring   | Typical use   |  Bits  |
; | --- | ------ | ------------- | ------ |
; | 0   | Ring 0 | Kernel        |   00   |
; | 1   | Ring 1 | Rarely used   |   01   |
; | 2   | Ring 2 | Rarely used   |   10   |
; | 3   | Ring 3 | User programs |   11   |
; Since we're only running the kernel, we'll need the highest privilege (Ring 0, Kernel level)

; Bit 4: S (Descriptor type)
; This bit is the one that describes wether the segment is code/data or system descriptor type
; 0 = System descriptor (TSS, LDT, call gate, interrupt gate)
; 1 = Normal code or data segment

; Bit 3: E (Executable)
; This determines wether the segment is executable or not.
; 0 = Data segment (Not executable)
; 1 = Code segment (Executable)

; Bit 2: DC (Direction / Conforming)
; This bit has two meanings, depending on wether the descriptor is a code segment or a data segment:
; If it is a data segments (E = 0):
; It's called the direction bit:
; - 0 = Addresses grow upward (Normal)
; - 1 = Addresses grow downward (Used for special stacks segments)
; And if it is a code segment (E = 1):
; It's called a conforming bit:
; - 0 = Only code with the appropiate privilege level can execute this segment
; - 1 = Lower-privilege code is allowed to execute without changing privileges.
; For a kernel, we don't want the user-mode (ring 3) to execute it.

; Bit 1 - RW (Read/Write)
; This bit changes meaning depending on wether it's code or data 
; For a code segment:
; 0 = Execute only
; 1 = Execute & Read
; Notice there's no write? Code segments are never writable

; Bit 0 - A (Accessed)
; The CPU automatically sets it to 1
; OSs usually set this bit to 0, because the processor will update if it wants to. 

; Flag:

; Bit 7: G (granularity): It'll increase the size of the segment, going from 1 MB to almost 4 GiB. Instead of measuring with bytes, it measures with 4KB pages
; G = 1: It'll measure it with 4KB pages
; G = 0: It'll measure it with bytes

; Bit 6:  D/B (Default operand size):
; In 32-bit code segments:
; - 0 = 16-bit instructions by default
; - 1 = 32-bit instructions by default
; This one is tricky. There's an intel rule that says "If the L bit is 1, then D/B must be 0"
; The CPU doesn't allow both to be 1
; For a data segment, this is almost always 1

; Bit 5: L (Long):
; - 0 = Not a 64-bit code segment
; - 1 = A 64-bit code segment
; This bit has to be 0 when working with a data segment

; Bit 4: AVL (Available to software)
; This bit means nothing. You can do whatever you want with it

; Now we build them:
; 63                                              0
; +--------+----+------+--------+--------------+----------+
; |Base31:24|Flags|Limit| Access | Base23:0     |Limit15:0 |
; +--------+----+------+--------+--------------+----------+
; 64-bit code descriptor
; Base31:24      = 00
; Flags          = A
; Limit19:16     = F
; Access         = 9A
; Base23:0       = 000000
; Limit15:0      = FFFF
; And now, concatenate them:
; 00 AF 9A 00 00 00 FF FF -> You've got the 64-bit code descriptor
; Data descriptor:
; Base = 00
; Flags = C
; Limit = F
; Access = 92
; Base = 000000
; Limit = FFFF
; 00 CF 92 00 00 0F FF F0
; Since this version doesn't support 64-bit, both flags must be 0xC
; The code descriptor must end up like this 0x00CF9A000000FFFF
; The data descriptor doesn't change
section .rodata
%include "gdt.inc"
global gdt
global gdt_descriptor
gdt:
    dq 0 ; Null descriptor

    ; 32 bit code descriptor
    dq 0x00CF9A000000FFFF

    ; Data descriptor
    dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt - 1
    dd gdt