PROGNAME=m4g
#rm C:/agb/BATTERY/*.SAV
# NOTE: REMOVE DEBUG.O FROM OFILES WHEN RELEASING TO STRIP 5 KB OF USELESS DATA
OFILES = asm/crt0.o asm/sram.o asm/clock.o asm/sys.o \
		modules/int/int.o \
		modules/tim/tim.o \
		modules/sys/sys.o \
		modules/sram/sram.o \
		modules/clip/clip.o \
		modules/key/key.o \
		modules/mem/mem.o \
		modules/gpu/gpu.o \
		modules/net/net.o \
		modules/spu/mixer.o \
		modules/spu/sequencer.o \
		modules/spu/synth.o \
		modules/spu/adsr.o \
		data/tables.o \
		data/layers.o \
		data/tileset.o \
		data/palette.o \
		data/caches.o \
		data/regions.o \
		data/controls.o \
		data/displays.o \
		data/viewports.o \
		callbacks/sng.o \
		callbacks/liv1.o \
		callbacks/liv2.o \
		callbacks/pat.o \
		callbacks/console.o \
		callbacks/debug.o \
		callbacks/hlp.o \
		callbacks/snk.o \
		callbacks/ins.o \
		callbacks/trk.o \
		callbacks/cfg.o \
		callbacks/linkmode.o \
		callbacks/looknfeel.o \
		callbacks/behavior.o \
		callbacks/coloreditor.o \
		dialogs/really.o \
		dialogs/alpha.o \
		dialogs/progress.o \
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
	@echo -------------------------------------------------------------------------------
	@echo Linking
	@echo -------------------------------------------------------------------------------
	$(LD) $(LDFLAGS) -o $(PROGNAME).elf $(OFILES) $(LDLIBS)

index.m4h: index.hml
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f *.o *.i *.ii *.m4h	
	$(ROOT)/tools/helpcode/helpcode.exe index.hml index.m4h /c help /v -l -t
#----------------------------------------------------------------------------------------#
all : $(PROGNAME).gba
	$(GCC_BINDIR)/$(PFX)nm $(PROGNAME).elf > syms.txt

help: index.m4h

vba : all fixheader
	@echo -------------------------------------------------------------------------------
	@echo Patching ROM
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/vbawin.exe $(PROGNAME).gba

gba : clean vba

fixheader:
	$(HAMDIR)/tools/win32/gbafix$(EXEC_POSTFIX) $(PROGNAME).gba -t$(PROGNAME)

clean-modules: 
	@echo -------------------------------------------------------------------------------
	@echo Cleaning up modules
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/int/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/tim/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/sys/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/clip/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/key/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/gpu/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/net/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/mem/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/spu/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/sram/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/key/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f modules/clip/*.o *.i *.ii *.m4h	

clean-callbacks: clean-modules
	@echo -------------------------------------------------------------------------------
	@echo Cleaning callbacks
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f callbacks/*.o *.i *.ii *.m4h	

clean-data: clean-callbacks
	@echo -------------------------------------------------------------------------------
	@echo Cleaning data
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f data/*.o *.i *.ii *.m4h	

clean-dialogs: clean-data
	@echo -------------------------------------------------------------------------------
	@echo Cleaning dialogs
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f dialogs/*.o *.i *.ii *.m4h	

clean: clean-dialogs
	@echo -------------------------------------------------------------------------------
	@echo Cleaning program code
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f *.o *.i *.ii *.m4h	
	@echo --------------------------------------------------------------------------------------------------------------------------------------------------------------

