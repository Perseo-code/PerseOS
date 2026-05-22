# ===== Configuración =====
ARCH        := i386
CC          := clang
LD          := ld.lld
ASM         := nasm

CFLAGS      := -target i386-pc-none-elf -m32 -ffreestanding -fno-stack-protector \
               -fno-pie -fno-builtin -nostdlib -nostdinc -Wall -Wextra \
               -Iinclude -Isrc -Idrivers -Inet
LDFLAGS     := -m elf_i386 -T linker/linker.ld
ASMFLAGS    := -f elf32

BUILD_DIR   := build
ISO_DIR     := iso
KERNEL_ELF  := $(BUILD_DIR)/kernel.elf
ISO_IMAGE   := $(BUILD_DIR)/os.iso

# ===== Fuentes =====
C_SOURCES   := $(shell find kernel src include drivers -name "*.c")
ASM_SOURCES := $(shell find boot include -name "*.asm")

OBJECTS := \
  $(C_SOURCES:%.c=$(BUILD_DIR)/%.o) \
  $(ASM_SOURCES:%.asm=$(BUILD_DIR)/%.o)

# ===== Targets =====
all: $(ISO_IMAGE)

# ----- Compile C -----
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ----- Compile ASM -----
$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

# ----- Linkear kernel -----
BOOT_OBJ := $(BUILD_DIR)/boot/multiboot.o

$(KERNEL_ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) $(BOOT_OBJ) $(filter-out $(BOOT_OBJ), $(OBJECTS)) -o $@

# ----- Create ISO -----
$(ISO_IMAGE): $(KERNEL_ELF)
	mkdir -p $(ISO_DIR)/boot
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/
	grub-mkrescue -o $@ $(ISO_DIR)

# ----- Execute -----
run: $(ISO_IMAGE)
	qemu-system-i386 -cdrom $(ISO_IMAGE) -no-reboot -d int


# ----- Clean -----
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)/kernel.elf

.PHONY: all run clean
