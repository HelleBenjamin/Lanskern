#ifndef DISK_H
#define DISK_H

#include "libstd.h"
#include "kernel.h"

#define ATA_PRIMARY_IO  0x1F0
#define ATA_CONTROL_IO  0x3F6

void ide_wait(void);
void ide_read_sector(uint32_t lba, uint8_t *buffer);
void ide_write_sector(uint32_t lba, const uint8_t *buffer);

#endif