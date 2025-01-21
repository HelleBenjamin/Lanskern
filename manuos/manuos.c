#include "../include/kernel.h"
#include "manuos.h"
#include "../include/libstd.h"
#include "../include/vga.h"
#include "../include/disk.h"

unsigned int cRow = 0;
uint8_t taskbarColor;
char cProgram[25];
char username[32];

void WriteCharacter(unsigned char c, unsigned char forecolour, unsigned char backcolour, int x, int y){
    short attrib = (backcolour << 4) | (forecolour & 0x0F);
    volatile short * where;
    where = (volatile short *)0xB8000 + (y * 80 + x) ;
    *where = c | (attrib << 8);
    return;
}

void os_main() {
    taskbarColor = BLUE;
    strcpy(username, "ADMIN");
    terminal();
}

void nl() {
    newline();
    taskbar();
    return;
}

void terminal() {
    clt();
    char prompt[40];
    nl();
    while (1){
        strcpy(prompt, 0);
        printc('>');
        sysreade32(prompt, 40, 1);
        nl();
        if (strcmp(prompt, 0) == 0) {
        } else if (strcmp(prompt, "version") == 0) {
            prints("Copyright (C) 2024 - 2025 Benjamin Helle All rights reserved.");
            nl();
            prints("OS: " OS_VERSION);
            nl();
            prints("KERNEL: " KERNEL_VERSION);
            nl();
            printf("Username: %s\n", username);
        } else if (strcmp(prompt, "help") == 0) {
            prints(TERMINAL_HELP_MSG);
            nl();
        } else if (strcmp(prompt, "wpp") == 0) {
            wpp_interpreter();
        } else if (strcmp(prompt, "dices") == 0) {
            dices();
        } else if (strcmp(prompt, "clear") == 0) {
            clt();
        } else if (strcmp(prompt, "calculator") == 0 || strcmp(prompt, "calc") == 0) {
            calculator();
        } else if (strcmp(prompt, "taskbar") == 0) {
            prints("Change taskbar color: ");
            taskbarColor = getc();
            init_taskbar();
        } else if (strcmp(prompt, "color") == 0) {
            prints("Colors: 0 - Black, 1 - Blue 2 - Green, 3 - Cyan, 4 - Red, 5 - Magenta, 6 - Brown, 7 - Light Gray, 8 - Dark Gray, 9 - Light Blue, 10 - Light Green, 11 - Light Cyan, 12 - Light Red, 13 - Light Magenta, 14 - Yellow, 15 - White");
            nl();
        } else if (strcmp(prompt, "username") == 0) {
            prints("Current username: ");
            for (int i = 0; i < strlen(username); i++) {
                printc(username[i]);
            }
            nl();
            prints("Change current username: ");
            sysreade32(username, 32, 1);
            nl();
        } else if (startsWith(prompt, "echo ") == 0) {
            for (int i = 5; i < strlen(prompt); i++) {
                printc(prompt[i]);
            }
            nl();
        } else if (strcmp(prompt, "restart") == 0) {
            restart();
        } else if (strcmp(prompt, "fgcolor") == 0) {
            prints("Change foreground color: ");
            char c;
            sysreade32(c,1,1);
            ccolor(c);
        } else if (strcmp(prompt, "bgcolor") == 0) {
            prints("Change background color: ");
            char c;
            sysreade32(c,1,1);
            background(c);
        } else if (strcmp(prompt, "test") == 0) {
            char diskbuf[512];
            ide_read_sector(90, diskbuf);
            for (int i = 0; i < 512; i++) {
                printf("%x ", diskbuf[i]);
            }
        } else if (strcmp(prompt, "test2") == 0) {
            char diskbuf2[512];
            for (int i = 0; i < 512; i++) {
                diskbuf2[i] = i;
            }
            ide_write_sector(90, diskbuf2);
        } else if (strcmp(prompt, "exit") == 0) {
            break;
        }
    }
}

int diceroll() {
    return 2 + (2 % 6);
}
void dices(){
    strcpy(cProgram, "Dices");
    clrs();
    uint16_t dices[5] = {1};
    int16_t dicesToReRoll[5] = {0};
    for (int16_t i = 0; i < 5; i++) {
            dices[i] = diceroll();
    }
    while(1){
        dice_loop:
        prints("Your dices are: ");
        for (int i = 0; i < 5; i++) {
            printc('[');
            printi(dices[i]);
            printc(']');
            printc(' ');
        }
        nl();
        update_taskbar();
        prints("Select an action: ");
        nl();
        update_taskbar();
        prints("1 - reroll all");
        nl();
        update_taskbar();
        prints("2 - reroll selected");
        nl();
        update_taskbar();
        prints("3 - exit");
        update_taskbar();
        char c = getc();
        nl();
        switch (c){
            case '1':
                for (int i = 0; i < 5; i++) {
                    dices[i] = diceroll();
                }
                break;
            case '2':
                prints("Select dices to reroll: ");
                int i = 0;
                while (1) {
                    dicesToReRoll[i] = getc();
                    printc(dicesToReRoll[i]);
                    if (dicesToReRoll[i] == '\n') {
                        break;
                    }
                    i++;
                }
                for (int i = 0; i < 5; i++) {
                    switch (dicesToReRoll[i]) {
                        case '0':
                            break;
                        case '1':
                            dices[0] = diceroll();
                            break;
                        case '2':
                            dices[1] = diceroll();
                            break;
                        case '3':
                            dices[2] = diceroll();
                            break;
                        case '4':
                            dices[3] = diceroll();
                            break;
                        case '5':
                            dices[4] = diceroll();
                            break;
                    }
                }
                goto dice_loop;
            case '3':
                clt();
                return;
            default:
                goto dice_loop;
        }
    }
}

void wpp_interpreter() {
    char InterpretedProgram[0x200] = {0};
    int16_t i = 0;
    strcpy(cProgram, "Wuf++ Interpreter");
    clrs();
    prints("Press ESC to exit, enter to execute the code");
    nl();
    uint16_t halt, pc = 0;
    uint8_t stack[0xff] = {0};
    short sp, bx, cx, dx;
    loop:
    strcpy(InterpretedProgram, 0);
    prints("> ");
    sysreade32(InterpretedProgram, 0x200, 1);
    nl();
    interpret:
    bx = 0;
    dx = 0;
    cx = 0;
    sp = 0xff;
    pc = 0;
    halt = 0;
    while (pc < 0x200) {
        if (InterpretedProgram[pc] == 0) break;
        if (InterpretedProgram[pc] == 0x1b) {
            clt();
            return;
        }
        if(halt) break;
        switch (InterpretedProgram[pc]) {
            case '\n':
                break;
            case ' ':
                break;
            case 'i':
                break;
            case 'o':
                break;
            case '+':
                bx++;
                break;
            case '-':
                bx--;
                break;
            case '}':
                sp--;
                stack[sp] = bx & 0xFF;
                sp --;
                stack[sp] = (bx & 0xFF00) >> 8;
                break;
            case '{':
                bx = (stack[sp] << 8) | stack[sp+1];
                sp += 2;
                break;
            case '.':
                printc(bx);
                break;
            case ',':
                sysreade32(bx, 1, 1);
                break;
            case '&':
                pc = cx;
                break;
            case '[':
                pc = pc - cx;
                break;
            case ']':
                pc = pc + cx;
                break;
            case '!':
                bx = ~bx;
                break;
            case '>':
                cx++;
                break;
            case '<':
                cx--;
                break;
            case '$':
                printc(cx);
                break;
            case '#':
                bx = InterpretedProgram[pc+1];
                pc++;
                break;
            case '(': 
                dx = pc;
                cx--;
                break;
            case ')':
                if (cx != 0) {
                    cx--;
                    pc = dx;
                    break;
                } if (cx == 0) {
                    syswrite32('\n',1);
                    break;
                }
                break;
            case '"':
                dx = bx;
                bx = cx;
                cx = dx;
                break;
            case '%':
                pc++;
                if (bx == InterpretedProgram[pc]) {
                    pc = cx -1;
                    break;
                }
                break;
            case '=':
                halt = 1;
                break;
            case '/':
                bx = bx + cx;
                break;
            case '\\':
                bx = bx - cx;
                break;
            case '@':
                bx = 0;
                break;
            case '^':
                dx = bx;
                bx = (dx & 0xFF00);
                bx = (dx & 0xFF00) >> 8;
                break;
            default:
                prints("Error: Unknown command: '");
                printc(InterpretedProgram[pc]);
                prints("' at position: ");
                printi(pc);
                nl();
                break;
        }
        pc++;
    }
    nl();
    goto loop;
}

void calculator() {
    strcpy(cProgram, "Calculator");
    clrs();
    int a, b, c, sum;
    cloop:
    prints("Operations: 1.ADD 2.SUBTRACT 3.MULTIPLY 4.DIVIDE 5.EXIT ");
    update_taskbar();
    c = geti();
    if (c == 5){
        clt();
        return;
    }
    nl();
    update_taskbar();
    prints("Enter first number: ");
    a = geti();
    nl();
    update_taskbar();
    prints("Enter second number: ");
    b = geti();
    nl();
    update_taskbar();
    switch(c) {
        case 1:
            sum = a + b;
            break;
        case 2:
            sum = a - b;
            break;
        case 3:
            sum = a * b;
            break;
        case 4:
            sum = a / b;
            break;
        default:
            goto cloop;
    }
    prints("Result: ");
    printi(sum);
    nl();
    update_taskbar();
    goto cloop;
}

void clrs() { //clear screen and update taskbar
    clear();
    init_taskbar();
    return;
}

void clt(){ //clear screen for terminal, use for when exiting to terminal
    strcpy(cProgram, "Terminal");
    clrs();
    return;
}

void taskbar() { //simple taskbar, not required in custom programs
    char tskbr[80] = {0};
    strcpy(tskbr, username);
    strcat(tskbr, " | ");
    strcat(tskbr, cProgram);	
    strcat(tskbr, " | ");
    strcat(tskbr, OS_VERSION);
    for (int i = 0; i < 80; i++) {
        WriteCharacter(tskbr[i], 0xf, taskbarColor, i, 0);
    }
}

void update_taskbar() {
    cRow++;
    if (cRow > 23) {
        taskbar();
    }
    return;
}

void init_taskbar() {
    cRow = 0;
    taskbar();
    nl();
    return;
}