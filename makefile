PROGNAME=m4g
#rm C:/agb/BATTERY/*.SAV
# NOTE: REMOVE DEBUG.O FROM OFILES WHEN RELEASING TO STRIP 5 KB OF USELESS DATA
OFILES = asm/crt0.o asm/sram.o asm/clock.o asm/sys.o \
	modules/gpu/virtualscreen.o \
	modules/mem/mem.o \
	modules/sram/sram.o \
	modules/int/int.o \
	modules/tim/tim.o \
	modules/gpu/gpu.o \
	modules/key/key.o \
	modules/spu/spu.o \
	modules/regionhandler/regionhandler.o \
	modules/sys/sys.o \
	data/variables.o \
	data/palette.o \
	data/controls.o \
	data/displays.o \
	data/regions.o \
	data/viewports.o \
	data/help.o \
	data/caches.o \
	data/tables.o \
	data/layers.o \
	data/tileset.o \
	callbacks/callbacks.o \
	m4g.o 

HAM_VERSION_MAJOR = 2
HAM_VERSION_MINOR = 8
HAM_VERSION_BINUTILS = 2.14
HAM_VERSION_GCC = 3.3.2
HAM_VERSION_INSIGHT = 6.0
HAM_VERSION_NEWLIB = 1.11.0

MAGTOOL=C:/codigo/m4gtool/m4gtool.exe
ROOT = C:/codigo/m4gtool/data/m4g
HAMDIR = C:/HAM
GCCARM = $(HAMDIR)/gcc-arm
GCC_BINDIR = $(GCCARM)/bin
PFX = arm-thumb-elf-

HAM_LIBGCC_STYLE = interwork
EXEC_POSTFIX = .exe

LDLIBS = -lm -lstdc++ -lsupc++ -lgcc -lc -lgcc
INCDIR      = -I $(GCCARM)/include -I $(GCCARM)/arm-thumb-elf/include -I $(HAMDIR)/include -I $(HAMDIR)/system 
LIBDIR      = -L $(GCCARM)/lib/gcc-lib/arm-thumb-elf/$(HAM_VERSION_GCC)/$(HAM_LIBGCC_STYLE) \
              -L $(GCCARM)/lib/gcc-lib/arm-thumb-elf/$(HAM_VERSION_GCC) \
              -L $(GCCARM)/arm-thumb-elf/lib/$(HAM_LIBGCC_STYLE) \
              -L $(GCCARM)/arm-thumb-elf/lib \
              -L $(GCCARM)/lib 
CFLAGS      = $(INCDIR) -c $(HAM_CFLAGS) -mthumb-interwork -mlong-calls -Wall -save-temps -fverbose-asm -nostartfiles -O2 -g -mthumb
LDFLAGS     = $(LIBDIR) -T link.ld -Map $(PROGNAME).Map
ASFLAGS     = -mthumb-interwork 
PATH       +=;$(GCCARM) 

STRIP       = $(GCC_BINDIR)/$(PFX)strip
CC          = $(GCC_BINDIR)/$(PFX)g++
AS          = $(GCC_BINDIR)/$(PFX)as
LD          = $(GCC_BINDIR)/$(PFX)ld
OBJCOPY     = $(GCC_BINDIR)/$(PFX)objcopy
SHELL       = $(HAMDIR)/tools/win32/sh$(EXEC_POSTFIX) 
PATH       := $(GCCARM)/$(PREFIX):$(PATH)


$(PROGNAME).gba : $(PROGNAME).elf 
	$(OBJCOPY) -v -O binary $(PROGNAME).elf $(PROGNAME).gba

%.o : %.c
	$(CC) -o $@ $(CFLAGS) $<

%.o : %.cpp
	$(CC) -o $@ $(CFLAGS) $<

%.o : %.s
	$(AS) -o $@ $(ASFLAGS) $<

$(PROGNAME).elf : $(OFILES)
	$(LD) $(LDFLAGS) -o $(PROGNAME).elf $(OFILES) $(LDLIBS)

index.m4h: index.hml
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f *.o *.i *.ii *.m4h	
	$(ROOT)/tools/helpcode/helpcode.exe index.hml index.m4h /c help /v -l -t
#----------------------------------------------------------------------------------------#
all : $(PROGNAME).gba
	$(GCC_BINDIR)/$(PFX)nm $(PROGNAME).elf > syms.txt

help: index.m4h

vba : all fixheader
	$(HAMDIR)/tools/win32/vbawin.exe $(PROGNAME).gba

gba : clean vba

fixheader:
	$(HAMDIR)/tools/win32/gbafix$(EXEC_POSTFIX) $(PROGNAME).gba -t$(PROGNAME)

clean:
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f *.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f data/*.o 
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/gpu/*.o 
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/sram/*.o 
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/int/*.o 
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/tim/*.o 
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/mem/*.o 
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/key/*.o 
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/spu/*.o 
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/sys/*.o 
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/regionhandler/*.o 
	
