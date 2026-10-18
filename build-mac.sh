#!/bin/bash
mkdir -p build/mac
gcc src/main.c -o build/mac/main \
    -lraylib -lm -lpthread \
    -framework OpenGL -framework Cocoa -framework IOKit \
    -std=c99 -Wall -Wextra -g
cp src/Rocket.png build/mac/
echo "Mac build done → build/mac/main"