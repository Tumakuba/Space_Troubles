#!/bin/bash
mkdir -p build/linux
xxd -i src/Rocket.png > src/assets.h
gcc src/main.c -o build/linux/main \
-lraylib -lm -lpthread -ldl \
-std=c99
chmod +x build/linux/main
echo Linux build done → build/linux/main