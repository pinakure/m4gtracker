@echo off
cls
SET C_FILES=data/tables.cpp data/tileset.cpp modules/gpu/gpu.cpp modules/gpu/virtualscreen.cpp m4g.cpp
SET O_FILES=tables.o tileset.o GPU::o virtualscreen.o asm/crt0.o asm/sram.o asm/clock.o asm/sys.o m4g.o
SET LD_LIBS=-L C:/HAM/gcc-arm/lib/gcc-lib/arm-thumb-elf/3.3.2/interwork -L C:/HAM/gcc-arm/lib/gcc-lib/arm-thumb-elf/3.3.2 -L C:/HAM/gcc-arm/arm-thumb-elf/lib/interwork -L C:/HAM/gcc-arm/arm-thumb-elf/lib -L C:/HAM/gcc-arm/lib 
echo COMPILING-----------------------------------------------------------
C:\HAM\gcc-arm\bin\arm-thumb-elf-g++ -mthumb -mthumb-interwork -g -mlong-calls -save-temps -fverbose-asm -nostartfiles -c %C_FILES%
echo LINKING-----------------------------------------------------------
REM C:\HAM\gcc-arm\bin\arm-thumb-elf-ld -T link.ld %LD_LIBS% -Map m4g.Map -o m4g.elf %O_FILES%
C:\HAM\gcc-arm\bin\arm-thumb-elf-ld %LD_LIBS% -o m4g.elf %O_FILES%
