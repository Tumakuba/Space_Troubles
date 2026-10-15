#!/bin/bash
mkdir -p build/linux
gcc src/main.c -o build/linux/main \
    -lraylib -lm -lpthread -ldl \
    -std=c99 -Wall -Wextra -g
echo "Linux build done → build/linux/main"