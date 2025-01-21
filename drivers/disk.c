#include "../include/disk.h"

void ide_wait(void) {
    while (inb(ATA_PRIMARY_IO + 7) & 0x80);
    while (!(inb(ATA_PRIMARY_IO + 7) & 0x08));
}

void ide_read_sector(uint32_t lba, uint8_t *buffer) {
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F));

    // Send the sector count (1 sector), add more later
    outb(ATA_PRIMARY_IO + 2, 1);
    
    // Send the LBA
    outb(ATA_PRIMARY_IO + 3, (uint8_t)lba);
    outb(ATA_PRIMARY_IO + 4, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO + 5, (uint8_t)(lba >> 16));
    
    // Send the command
    outb(ATA_PRIMARY_IO + 7, 0x20);

    ide_wait(); // Wait for the disk to be ready

    for (int i = 0; i < 256; i++) { // Copy the data
        ((uint16_t*)buffer)[i] = inw(ATA_PRIMARY_IO);
    }
}

void ide_write_sector(uint32_t lba, const uint8_t *buffer) {
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F));

    // Send the sector count (1 sector), add more later
    outb(ATA_PRIMARY_IO + 2, 1);
    
    // Send the LBA
    outb(ATA_PRIMARY_IO + 3, (uint8_t)lba);
    outb(ATA_PRIMARY_IO + 4, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO + 5, (uint8_t)(lba >> 16));

    // Send the command
    outb(ATA_PRIMARY_IO + 7, 0x30);

    ide_wait(); // Wait for the disk to be ready

    for (int i = 0; i < 256; i++) { // Copy the data
        outw(ATA_PRIMARY_IO, ((uint16_t*)buffer)[i]);
    }

    // Flush, enable on real hardware
    //outb(ATA_PRIMARY_IO + 7, 0xE7);
    //ide_wait();
}