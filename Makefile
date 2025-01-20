ASSEMBLER = nasm
C_COMPILER = gcc
CXX_COMPILER = g++
LINKER = ld

BUILD_DIR = build/
DRIVER_DIR = drivers/
INCLUDE_DIR = include/
LIB_DIR = lib/
KERNEL_DIR = kernel/
PROGRAM_DIR = programs/
OS_DIR = manuos/

ASM_FLAGS = -f elf32
C_FLAGS = -m32 -std=gnu99 -ffreestanding -Wall -nostdlib -mno-red-zone
CXX_FLAGS = -m32 -std=gnu++17 -ffreestanding -Wall -nostdlib -mno-red-zone
LD_FLAGS = -m elf_i386 -nostdlib

ASM_S = $(KERNEL_DIR)boot.asm
KERNEL_S = $(KERNEL_DIR)kernel.c
VGA_S = $(DRIVER_DIR)vga.c
KEYBOARD_S = $(DRIVER_DIR)keyboard.c
STDLIB_S = $(LIB_DIR)libstd.c
MANUOS_S = $(OS_DIR)manuos.c
DISK_S = $(DRIVER_DIR)disk.c

LINK = link.ld

ASM_O = $(BUILD_DIR)boot.o
KERNEL_O = $(BUILD_DIR)kernel.o
VGA_O = $(BUILD_DIR)vga.o
KEYBOARD_O = $(BUILD_DIR)keyboard.o
STDLIB_O = $(BUILD_DIR)libstd.o
LANSKERN = $(BUILD_DIR)Lanskern.bin
MANUOS_O = $(BUILD_DIR)manuos.o
DISK_O = $(BUILD_DIR)disk.o

.PHONY: all clean

all:

	$(ASSEMBLER) $(ASM_FLAGS) -o $(ASM_O) $(ASM_S)
	$(C_COMPILER) -c $(KERNEL_S) -o $(KERNEL_O) $(C_FLAGS)
	$(C_COMPILER) -c $(VGA_S) -o $(VGA_O) $(C_FLAGS)
	$(C_COMPILER) -c $(KEYBOARD_S) -o $(KEYBOARD_O) $(C_FLAGS)
	$(C_COMPILER) -c $(STDLIB_S) -o $(STDLIB_O) $(C_FLAGS)
	$(C_COMPILER) -c $(MANUOS_S) -o $(MANUOS_O) $(C_FLAGS)
	$(C_COMPILER) -c $(DISK_S) -o $(DISK_O) $(C_FLAGS)
	$(LINKER) $(LD_FLAGS) -T $(LINK) -o $(LANSKERN) $(ASM_O) $(KERNEL_O) $(VGA_O) $(KEYBOARD_O) $(STDLIB_O) $(MANUOS_O) $(DISK_O)
	mkdir -p isodir/boot/grub
	cp build/Lanskern.bin isodir/boot/Lanskern.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o Lanskern.iso isodir
	qemu-system-i386 -hda Lanskern.iso

clean:
	rm -rf build