#!/bin/bash
wget -O appimage-builder-x86_64.AppImage https://github.com/AppImageCrafters/appimage-builder/releases/download/v1.1.0/appimage-builder-1.1.0-x86_64.AppImage
chmod +x appimage-builder-x86_64.AppImage
sudo mv appimage-builder-x86_64.AppImage /usr/local/bin/appimage-builder

cd build/linux
mkdir -p AppDir/lib
cp main AppDir/

appimage-builder --generate

# find and copy raylib wherever it is
RAYLIB=$(find /usr -name "libraylib.so*" 2>/dev/null | head -1)
cp "$RAYLIB" AppDir/lib/

appimage-builder --recipe AppImageBuilder.yml