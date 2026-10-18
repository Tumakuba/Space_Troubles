#!/bin/bash
mkdir -p build/linux
xxd -i src/Rocket.png > src/assets.h
gcc src/main.c libraylib.a \
-Wl,-Bstatic -lm -lpthread \
-Wl,-Bdynamic -lGL -ldl \
-lX11 -lXrandr -lXinerama -lXi -lXcursor \
-std=c99 \
-o build/linux/SpaceTroubles
chmod +x build/linux/SpaceTroubles
echo Linux build done → build/linux/SpaceTroubles