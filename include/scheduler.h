#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "libstd.h"

/* The scheduler is in very early development */

enum State {
    RUNNING,
    READY,
    TERMINATED,
    WAINTING,
};

// General purpose registers
struct GPR { 
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
};

// Special purpose registers
struct SGR {
    uint32_t eip;
    uint32_t eflags;
    uint16_t cs;
    uint16_t ss;
    uint32_t esp;
    uint16_t ds;
    uint16_t es;
    uint16_t fs;
    uint16_t gs;
};

struct ProcessControlBlock {
    uint16_t pid;
    State state;
    GPR gpr;
    SGR sgr;
    uint32_t time;
};


#endif