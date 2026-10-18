#!/bin/bash
echo "Building static raylib..."
rm -rf /tmp/raylib
git clone --depth 1 --branch 6.0 https://github.com/raysan5/raylib.git /tmp/raylib
cd /tmp/raylib/src
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC
cp libraylib.a /home/tumakuba/Dokumenty/c++/Space_Troubles/
echo "Done! Now run build-linux.sh"