#include "../include/libstd.h"
#include "../include/kernel.h"
#include "../include/vga.h"

short strcmp(const char *str1, const char *str2) { // string compare
    int i = 0;
    while (str1[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}

void strcpy(char *dest, char *src) {
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

void strcat(char *dest, char *src) {
    while (*dest) {
        dest++;
    }
    strcpy(dest, src);
}

short strlen(char *str) {
    unsigned int i = 0;
    while (str[i]) {
        i++;
    }
    return i;
}

short startsWith(char *str1, char *str2) {
    while (*str2 != '\0') {
        if (*str1 != *str2) {
            return 1;
        }
        str1++;
        str2++;
    }
    return 0;
}

char *strncpy(char *dest, const char *src, int n) {
    int i = 0;
    while (src[i] && i < n) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

void* memset(void* s, int c, size_t n) {
    unsigned char* p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

void* memcpy(void* dest, const void* src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((char*)dest)[i] = ((char*)src)[i];
    }
    return dest;
}

int strncmp(const char *str1, const char *str2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return 1;
        }
    }
    return 0;
}

void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    for (const char *ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) != '\0') {
            ptr++;
            switch (*ptr) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    printc(c);
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char*);
                    prints(str);
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    printi(num);
                    break;
                }
                case 'u': {
                    unsigned int num = va_arg(args, unsigned int);
                    printi(num);
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args, unsigned int);
                    printi(num);
                    break;
                }
                default:
                    printc('%');
                    printc(*ptr);
                    break;
            }
        } else if (*ptr == '\n') {
            newline();
        } else {
            printc(*ptr);
        }
    }
    va_end(args);
}
