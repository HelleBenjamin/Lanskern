#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "libstd.h"

#define IDT_SIZE 256

struct idt_entry {
    uint16_t offset_low; // Lower addr
    uint16_t selector;
    uint8_t zero; // Reserved, must be 0
    uint8_t type_attr;
    uint16_t offset_high; // Higher addr
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


void idt_load(struct idt_ptr *idtptr);
void set_idt_gate(int num, uint32_t base, uint16_t selector, uint8_t flags);
void idt_init(void);
void set_isr(int num, void *handler);

// PIC related

#define PIC1	0x20
#define PIC2    0xA0
#define PIC1_CMD	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_CMD	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI 0x20


#define ICW1_IWC4    0x01
#define ICW1_INIT    0x10
#define ICW1_SINGLE  0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL   0x08

#define ICW4_8086    0x01
#define ICW4_AUTO   0x02
#define ICW4_BUF_SLAVE  0x08
#define ICW4_BUF_MASTER 0x1C
#define ICW4_SFNM    0x10

void pic_init();

void pic_send_eoi(uint8_t irq);
void pic_remap(int offset0, int offset1);

void set_irq_mask(uint8_t irq);
void clear_irq_mask(uint8_t irqline);

void exception_handler(void);

#endif