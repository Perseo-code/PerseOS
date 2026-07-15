# Config
CPPC        := clang++
CC			:= clang
LD          := ld.lld
ASM         := nasm

CPPFLAGS      := --target=i686-unknown-elf -ffreestanding -fno-exceptions -fno-rtti -Iinclude
LDFLAGS     := -T linker/linker.ld
ASMFLAGS    := -Iboot/ -f elf32

BUILD_DIR   := build
ISO_DIR     := iso
DIST_DIR	:= dist
KERNEL_ELF  := $(BUILD_DIR)/kernel.elf
ISO_IMAGE   := $(DIST_DIR)/PerseOS.iso

# ===== Fuentes =====
CPP_SOURCES   := $(shell find kernel src shell -name "*.cpp")
ASM_SOURCES := $(shell find boot src -name "*.asm")

OBJECTS := \
  $(CPP_SOURCES:%.cpp=$(BUILD_DIR)/%.o) \
  $(ASM_SOURCES:%.asm=$(BUILD_DIR)/%.o)

# ===== Targets =====
all: $(ISO_IMAGE)

# ----- Compile C++ -----
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CPPC) $(CPPFLAGS) -c $< -o $@

# ----- Compile ASM -----
$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

# ----- Link kernel -----
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
	qemu-system-x86_64 -cdrom $(ISO_IMAGE) -no-reboot -d int


# ----- Clean -----
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)/kernel.elf
	rm -rf $(ISO_IMAGE)

.PHONY: all run clean
