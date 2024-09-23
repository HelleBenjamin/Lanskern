#ifndef VGA_H
#define VGA_H

#define BLACK 0x00
#define BLUE 0x01
#define GREEN 0x02
#define CYAN 0x03
#define RED 0x04
#define MAGENTA 0x05
#define YELLOW 0x06
#define WHITE 0x07

#define BLACK_BG 0x100
#define BLUE_BG 0x10
#define GREEN_BG 0x20
#define CYAN_BG 0x30
#define RED_BG 0x40
#define MAGENTA_BG 0x50
#define YELLOW_BG 0x60
#define WHITE_BG 0x70

void video_init();
void clear(void);
void printc(char c);
void prints(char *str);
void printi(int i);
void video_test();
void video_test2();
void ccolor(unsigned int color);
void put_pixel(int x, int y, unsigned int color);
unsigned int get_pixel(int x, int y);
void newline(void);
void update_cursor();
void backspace(void);
void background(unsigned int color);


#endif