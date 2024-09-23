#ifndef MANUOS_H
#define MANUOS_H
#pragma once
#define OS_VERSION "ManuOS 0.0.4c-release-dev 32-bit" 
#define TERMINAL_HELP_MSG "Commands: version, help, wpp, dices, clear, calculator, echo, taskbar, username, restart, color, fgcolor, bgcolor"

void nl();
void os_main();
void terminal();
void wpp_interpreter();
void dices();
void calculator();
void bouncing_ball();
void taskbar();
void update_taskbar();
void init_taskbar();
void clrs(); // clear screen, should be used in os environment
void clt(); // clear screen and initialize for terminal

#endif