#include "../include/kernel.h"
#include "../include/libstd.h"
#include "../include/vga.h"
#include "../include/keyboard.h"
#include "../include/tinyasm.h"
#include "../manuos/manuos.h"

void sleepMS(int ms) {
    for (int i = 0; i < ms; i++) {
        for (int j = 0; j < 100000; j++) {
            __asm__ volatile("nop");
        }
    }
}

int geti() {
    char c = 0;
    int i = 0;
    while (1) {
        c = getc();
        if (c == '\b') {
            if (i > 0) {
                i--;
                backspace();
            }
            continue;
        } else if (c == '\n') {
            break;
        }
        printc(c);
        if (c >= '0' && c <= '9') {
            i = i * 10 + (c - '0');
        } else if (c == '-') {
            i *= -1;
        } else {
            break;
        }
    }
    return i;
}

void minimal_terminal() {
    clear();
    prints("Kernel Mode");
    newline();
    char buf[20] = {0};
    while(1){
        syswrite32(">", 1);
        strcpy(buf, 0);
        sysreade32(buf, 20, 1);
        if (strcmp(buf, 0) == 0){
            // nop
        } else if (strcmp(buf,"test1") == 0){
            video_test();
        } else if (strcmp(buf,"test2") == 0){
            video_test2();
        } else if (strcmp(buf, "ver") == 0){
            syswrite32("\nLanskern Kernel ", 17);
            syswrite32(KERNEL_VERSION_SHORT, 6);
        } else if (strcmp(buf, "clear") == 0){
            clear();
        } else if (strcmp(buf, "restart") == 0){
            restart();
        } else if (strcmp(buf, "os") == 0){
            os_main();    
        }
        newline();
    }
}

void restart() {
    __asm__ __volatile__ (
        "cli\n\t"
        "movw $0x1234, %ax\n\t"
        "movw %ax, %ds \n\t"
        "movw %ax, %es \n\t"
        "movw %ax, %fs \n\t"
        "movw %ax, %gs \n\t"
        "movw %ax, %ss \n\t"
        "ljmp $0xF000, $0xE05B \n\t"
    );
}

void kernel_main(void) {
    video_init();
    os_main();
    minimal_terminal();
}

// system write
int syswrite32(char *buf, int len){
    for(int i = 0; i < len; i++){
        if(buf[i] == '\n'){
            newline();
        } else {
            printc(buf[i]);
        }
    }
    return 0;
}

// system read
int sysread32(char *buf, int len, int echo){
    for (int i = 0; i < len; i++) {
        buf[i] = getc();
        if (buf[i] == '\b'){
            if (i > 0) {
                buf[i-1] = 0;
                i = i - 1;
                backspace();
            }
        }
        if (echo) printc(buf[i]);
    }
    return 0;
}

// system read until enter
int sysreade32(char *buf, int len, int echo){
    for (int i = 0; i < len; i++) {
        buf[i] = getc();
        if (buf[i] == '\b'){
            if (i > 0) {
                i--;
                buf[i] = '\0';
                i--;
                backspace();
            }
            continue;
        }
        if (buf[i] == '\n') {
            buf[i] = 0;
            break;
        }
        if (echo && buf[i] != '\t' && buf[i] != '\n') printc(buf[i]);
    }
    return 0;
}

int sysrestart32(){
    restart();
    return 0;
}