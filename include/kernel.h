// include/kernel.h
#ifndef KERNEL_H
#define KERNEL_H

#include "../include/libstd.h"

#define KERNEL_VERSION "Lanskern 0.0.1"
#define KERNEL_VERSION_SHORT "0.0.1"

void kernel_main(void);
void sleepMS(int ms);
void restart();
int geti();

// System calls
int syswrite32(char *buf, int len);
int sysread32(char *buf, int len, int echo);
int sysreade32(char *buf, int len, int echo);
int sysrestart32();


static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "dN"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t value;
    __asm__ volatile ("inw %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

static inline void outw(uint16_t port, uint16_t value) {
    __asm__ volatile ("outw %0, %1" : : "a"(value), "dN"(port));
}

#endif // KERNEL_H
