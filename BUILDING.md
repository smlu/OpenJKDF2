## Building OpenJKDF2

Building is currently only tested on Arch Linux, Ubuntu, and MacOS. Windows builds require MinGW.

Arch Linux Dependencies:
```
# All
pacman -S base-devel make python python-pip bison imagemagick
pip3 install cogapp

# Win32/MinGW
pacman -S mingw-w64

# Linux 32-bit
pacman -S multilib-devel lib32-sdl2 lib32-sdl2_mixer lib32-glew lib32-openal lib32-freealut

# Linux 64-bit
pacman -S clang sdl2 sdl2_mixer glew openal freealut

# WebAssembly
pacaur -S emscripten
```

WIP Ubuntu Dependencies:
```
# All
apt install build-essential make python3 python3-pip bison imagemagick
pip3 install cogapp

# Win32/MinGW
apt install mingw-w64

# Linux 32-bit
#multilib-devel lib32-sdl2 lib32-glew lib32-openal lib32-freealut

# Linux 64-bit
apt install clang libsdl2-dev libsdl2-mixer-dev libopenal-dev libglew-dev libalut-dev

# WebAssembly
#emscripten
```

Add the following to the end of ~/.bashrc:
```
export PATH=$PATH:~/.local/bin
```

MacOS:
Before starting, install Xcode. This is required for OpenGL headers, among other things.
```
# All
brew install make python3 imagemagick
pip3 install cogapp generate-iconset

# After installing cogapp, make sure the following is in your ~/.zshrc:
# export PATH=$PATH:$HOME/Library/Python/3.8/bin

# Win32/MinGW building
brew install mingw-w64

# MacOS 64-bit
brew install openal-soft freealut sdl2 sdl2_mixer glew
```

### x86 Win32/MinGW DLL
`make`

### x86 Linux/SDL2, mmap blobs
`OPENJKDF2_USE_BLOBS=1 make -f Makefile.linux`

### x86 Linux/SDL2, blobless
`make -f Makefile.linux`

### 64-bit Linux/SDL2
`make -f Makefile.linux64`

### 64-bit Windows/SDL2
`make -f Makefile.win64`

### MacOS SDL2
```
make -f Makefile.macos
codesign -s - openjkdf2-64
```

### Emscripten/WebAssembly
`make -f Makefile.emcc`