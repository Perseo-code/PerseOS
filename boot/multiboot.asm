section .multiboot
align 8 ; We have to align the multiboot section exactly 8 bytes

; Create every constant
%define MAGIC 0xE85250D6
%define HEADER_LENGTH 0x18
%define ARCH 0
%define CHECKSUM -(MAGIC + ARCH + HEADER_LENGTH)

; Write them inside the sector
dd MAGIC
dd ARCH
dd HEADER_LENGTH
dd CHECKSUM
; Note: Each tag is 16 bytes wide, and remember that the final one is only bytes.
; That influences in the header length, which right now is hardcored with the number 24 (0x18 in hex)
; Remember to align
; Make the end tag so grub knows that the sector ends here
dw 0
dw 0
dd 8