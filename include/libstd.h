#ifndef LIBSTD_H
#define LIBSTD_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;
typedef __SIZE_TYPE__ size_t;
typedef enum { false, true } bool;
#define NULL 0
#define true 1
#define false 0

short strcmp(const char *str1, const char *str2);
void strcpy(char *dest, char *src);
void strcat(char *dest, char *src);
short strlen(char *str);
short startsWith(char *str1, char *str2);
char *strncpy(char *dest, const char *src, int n);
void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
int strncmp(const char *str1, const char *str2, size_t n);

typedef __builtin_va_list va_list;

#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_copy(dest, src) __builtin_va_copy(dest, src)

#endif