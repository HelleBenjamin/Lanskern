#include "../include/vga.h"
#include "../include/kernel.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

char *vmp = (char *)0xb8000;
unsigned int cursor_position = 0;
unsigned int current_fg = WHITE;
unsigned int current_bg = BLACK_BG;

void video_init(void) { // add more features later
    clear();
    return;
}

void newline(void) {
    cursor_position = (cursor_position / 160 + 1) * 160;
    update_cursor();
    return;
}

void clear(void) {
    unsigned int j = 0;
    while(j < 80 * 25 * 2) {
        vmp[j] = ' '; // blank character
        vmp[j+1] = current_bg |current_fg; //Fix color
        j = j + 2;
    }
    cursor_position = 0;
    update_cursor();
    return;
}

void prints(char *str) {
    unsigned int i = 0;
    while(str[i] != 0) {
        vmp[cursor_position] = str[i];
        vmp[cursor_position+1] = current_bg |current_fg;
        cursor_position = cursor_position + 2;
        i = i + 1;
        update_cursor();
    }
    return;
}

void printc(char c) {
    vmp[cursor_position] = c;
    vmp[cursor_position+1] = current_fg | current_bg;
    cursor_position = cursor_position + 2;
    update_cursor();
    return;
}

void printi(int i) {
    char buf[10];
    int j = 0;
    if (i < 0) {
        printc('-');
        i *= -1;
    }
    while (i > 0) {
        buf[j++] = i % 10 + '0';
        i /= 10;
    }
    while (j > 0) {
        printc(buf[--j]);
    }
}

void video_test(void) {
    unsigned int colors[8] = {RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE, BLACK};
    cursor_position = 0;
    unsigned int temp = 0;
    unsigned int j = 0;
    char str[9] = "Test";
    char *c;
    while (1) {
        for (int i = 0; i < 80 * 25 * 2; i++) {
            vmp[cursor_position] = str[j];
            j++;
            if (j == 9) {
                j = 0;
            }
            vmp[cursor_position+1] = colors[i%8] +1;
            cursor_position = cursor_position + 2;
        }
        cursor_position = temp;
        temp += 2;
        sysreade32(c, 1, 1);
        if (c = 'q') return;
        sleepMS(1000);
    }
    return;
}

void video_test2(void) {
    cursor_position = 0;
    unsigned int color = 0x10;
    for (int i = 0; i < 80 * 25 * 2; i++) {
        vmp[cursor_position] = 'A';
        vmp[cursor_position+1] = color;
        cursor_position = cursor_position + 2;
        color++;
    }
    return;
}

void ccolor(unsigned int color) {
    current_fg = color;
    return;
}

void put_pixel(int x, int y, unsigned int color) {
    vmp[(y * 80 * 2) + x] = ' ';
    vmp[(y * 80 * 2 + x) + 1] = color;
    return;
}

void move_cursor(unsigned int x, unsigned int y) {
    unsigned int pos = y * VGA_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    return;
}

void update_cursor() {
    unsigned int x = (cursor_position / 2) % VGA_WIDTH;
    unsigned int y = (cursor_position / 2) / VGA_WIDTH;
    move_cursor(x, y);
    return;
}

void backspace(void) {
    if (cursor_position == 0) {
        return;
    }
    unsigned int x = (cursor_position / 2) - 1;
    unsigned int y = cursor_position / (2 * VGA_WIDTH);
    move_cursor(x, y);
    vmp[cursor_position-2] = ' ';
    vmp[cursor_position-1] = WHITE;
    cursor_position = cursor_position - 2;
    update_cursor();
    return;
}

void background(unsigned int color) {
    for (int i = 0; i < 80 * 25 * 2; i++) {
        vmp[i] = vmp[i];
        vmp[i+1] = color | current_fg;
    }
    cursor_position = 0;
    update_cursor();
    return;
}