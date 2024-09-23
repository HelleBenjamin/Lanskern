# Lanskern
Lanskern is a minimal kernel written in C. It's a hobby project. Currently it's in very early development. More features will be added in the future. The kernel comes with a fork of ManuOS.

## Requirements
- gcc
- make
- NASM
- Linux based OS
- QEMU
- grub

## How to build
1. Type `make` in the terminal. This builds the kernel to an `Lanskern.iso` file and launches it in QEMU.

## System calls
- `syswrite32(char *buf, int len)`: Write a string to the screen
- `sysread32(char *buf, int len, int echo)`: Read a string from the screen
- `sysreade32(char *buf, int len, int echo)`: Read a string from the screen until enter is pressed
- `sysrestart32()`: Restart the kernel