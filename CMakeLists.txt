cmake_minimum_required(VERSION 3.10)

project(Lanskern)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


include_directories(include)

set(SOURCE_FILES
    src/kernel.c
    src/vga.c
    src/keyboard.c
)

set(LIBRARY_FILES
    lib/libstd.c

)