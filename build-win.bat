@echo off
if not exist build\win mkdir build\win
x86_64-w64-mingw32-gcc src/main.c -o build/win/main.exe ^
    -lraylib -lm -lpthread ^
    -std=c99 -Wall -Wextra
echo Windows build done: build/win/main.exe