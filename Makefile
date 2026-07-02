# Config
CPPC          := x86_64-elf-g++
CC			:= x86_64-elf-gcc
LD          := x86_64-elf-ld
ASM         := nasm

CPPFLAGS      := -ffreestanding -g -O0 -fno-exceptions -fno-rtti
LDFLAGS     := -T linker/linker.ld
ASMFLAGS    := -f elf64

BUILD_DIR   := build
ISO_DIR     := iso
KERNEL_ELF  := $(BUILD_DIR)/kernel.elf
ISO_IMAGE   := $(ISO_DIR)/PerseOS.iso

# ===== Fuentes =====
CPP_SOURCES   := $(shell find kernel -name "*.cpp")
ASM_SOURCES := $(shell find boot -name "*.asm")

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
	qemu-system-x86 -cdrom $(ISO_IMAGE) -no-reboot -d int


# ----- Clean -----
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)/kernel.elf

.PHONY: all run clean
