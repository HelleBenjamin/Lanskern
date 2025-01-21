#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_DATA_PORT 0x60
#define KBD_COMMAND_PORT 0x64
#define KBD_PORT KBD_DATA_PORT

void keyboard_init(void);
char keyboard_read_char(void);
char getc(void);

#endif