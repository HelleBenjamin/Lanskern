#include "../include/interrupt.h"
#include "../include/kernel.h"

struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

// Load IDT
void idt_load(struct idt_ptr *idtptr) {
    __asm__ __volatile__("lidt %0" : : "m" (idtptr));
}

void exception_handler(void) {
    __asm__ volatile("cli\n hlt");
}

void set_idt_gate(int num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].offset_low = base & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
}

void idt_init(void) {
    idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < IDT_SIZE; i++) {
        set_idt_gate(i, 0, 0x08, 0x8E); // Fill with default entries
    }

    idt_load(&idtp);
    printf("IDT Loaded: Base = 0x%x, Limit = %d\n", idtp.base, idtp.limit);

}

// Set interrupt handler
void set_isr(int num, void *handler) {
    set_idt_gate(num, (uint32_t)handler, 0x08, 0x8E);
}

void pic_init(){
    pic_remap(0x20, 0x28);
}

void pic_remap(int offset0, int offset1) {
    uint8_t temp0, temp1;

    // Save current masks
    temp0 = inb(PIC1_DATA);
    temp1 = inb(PIC2_DATA);

    // Start the initialization sequence
    outb(PIC1_CMD, ICW1_INIT | ICW1_INTERVAL4);
    sleepMS(5);
    outb(PIC2_CMD, ICW1_INIT | ICW1_INTERVAL4);
    sleepMS(5);

    // Set offsets
    outb(PIC1_CMD, offset0);
    sleepMS(5);
    outb(PIC2_CMD, offset1);
    sleepMS(5);

    outb(PIC1_DATA, 4);
    sleepMS(5);
    outb(PIC2_DATA, 2);
    sleepMS(5);

    outb(PIC1_DATA, ICW4_8086);
    sleepMS(5);
    outb(PIC2_DATA, ICW4_8086);
    sleepMS(5);

    // Restore masks
    outb(PIC1_CMD, temp0);
    outb(PIC2_CMD, temp1);
}

void pic_send_eoi(uint8_t irq) {
    outb(PIC1_CMD, PIC_EOI);
    outb(PIC2_CMD, PIC_EOI);
}


void set_irq_mask(uint8_t irq) {
    uint16_t port;
    uint8_t val;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    val = inb(port | (1 << irq));
    outb(port, val);
}

void clear_irq_mask(uint8_t irq) {
    uint16_t port;
    uint8_t val;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    val = inb(port | (1 << irq));
    val &= ~(1 << irq);
    outb(port, val);
}