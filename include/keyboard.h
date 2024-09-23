#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_PORT 0x60

void keyboard_init(void);
char keyboard_read_char(void);
char getc(void);

#endif