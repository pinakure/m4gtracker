:build
make
echo C:/HAM/gcc-arm/bin/arm-thumb-elf-g++  -I C:/HAM/gcc-arm/include -I C:/HAM/gcc-arm/arm-thumb-elf/include -I C:/HAM/include -I C:/HAM/system  -c  -mthumb-interwork -mlong-calls -Wall -save-temps -fverbose-asm -nostartfiles -O2 -g -mthumb m4g.cpp modules/gpu/gpu.cpp
if errorlevel 1 goto build