@echo off
if not exist build\win mkdir build\win
set CC=C:\raylib\w64devkit\bin\gcc.exe
set RAYLIB_PATH=C:\raylib\raylib\src
%CC% src/main.c -o build/win/main.exe ^
    -I%RAYLIB_PATH% ^
    -L%RAYLIB_PATH% ^
    -lraylib -lopengl32 -lgdi32 -lwinmm -lm -lpthread ^
    -std=c99 -Wall -Wextra -O2
echo Windows build done: build/win/main.exe