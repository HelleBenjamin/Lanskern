#ifndef TINYASM_H
#define TINYASM_H

typedef void (*code_func_t)();

void execute(char* code);

#endif