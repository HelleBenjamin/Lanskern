cmake_minimum_required(VERSION 3.10)
project(Lanskern)

set(CMAKE_ASM_NASM_COMPILER nasm)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_C_FLAGS "-m32 -std=gnu99 -ffreestanding -Wall -nostdlib -mno-red-zone")
set(CMAKE_CXX_FLAGS "-m32 -std=gnu++17 -ffreestanding -Wall -nostdlib -mno-red-zone")
set(CMAKE_EXE_LINKER_FLAGS "-f elf_i386")

set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)
file(MAKE_DIRECTORY ${BUILD_DIR})

set(KERNEL_S kernel/kernel.c)
set(VGA_S drivers/vga.c)
set(KEYBOARD_S drivers/keyboard.c)
set(STDLIB_S lib/libstd.c)
set(MANUOS_S manuos/manuos.c)
set(DISK_S drivers/disk.c)
set(ASM_S kernel/boot.asm)

set(SOURCES ${KERNEL_S} ${VGA_S} ${KEYBOARD_S} ${STDLIB_S} ${MANUOS_S} ${DISK_S})

enable_language(ASM_NASM)
add_executable(Lanskern.bin ${ASM_S} ${SOURCES})

set_target_properties(Lanskern.bin PROPERTIES LINK_DEPENDS link.ld)
target_link_options(Lanskern.bin PRIVATE -T link.ld)

target_include_directories(Lanskern.bin PRIVATE include)

target_compile_options(Lanskern.bin PRIVATE ${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS})

target_link_options(Lanskern.bin PRIVATE ${CMAKE_EXE_LINKER_FLAGS})

add_custom_target(iso
    COMMAND mkdir -p isodir/boot/grub
    COMMAND cp ${BUILD_DIR}/Lanskern.bin isodir/boot/Lanskern.bin
    COMMAND cp grub.cfg isodir/boot/grub/grub.cfg
    COMMAND grub-mkrescue -o Lanskern.iso isodir
    COMMAND qemu-system-i386 -hda Lanskern.iso
    DEPENDS Lanskern.bin
)
