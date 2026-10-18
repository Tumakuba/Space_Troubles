@echo off
if not exist build\win mkdir build\win
xxd -i src\Rocket.png > src\assets.h
gcc src/main.c -o build/win/SpaceTroubles.exe ^
    libraylib.a ^
    -lwinmm -lgdi32 -lopengl32 -lcomdlg32 ^
    -std=c99 -static
echo Windows build done: build/win/SpaceTroubles.exe