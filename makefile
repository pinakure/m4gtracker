PROGNAME=m4g
#rm C:/agb/BATTERY/*.SAV
# NOTE: REMOVE DEBUG.O FROM OFILES WHEN RELEASING TO STRIP 5 KB OF USELESS DATA
OFILES = asm/crt0.o asm/sram.o asm/clock.o asm/sys.o \
		kernel/int/int.o \
		kernel/tim/tim.o \
		kernel/sys/sys.o \
		kernel/sram/sram.o \
		kernel/clip/clip.o \
		kernel/key/key.o \
		kernel/regionhandler/cursor.o \
		kernel/regionhandler/regionhandler.o \
		kernel/gpu/virtualscreen.o \
		kernel/gpu/gpu.o \
		kernel/net/net.o \
		kernel/spu/mixer.o \
		kernel/spu/sequencer.o \
		kernel/spu/synth.o \
		kernel/spu/adsr.o \
		data/tables.o \
		data/layers.o \
		data/tileset.o \
		data/palette.o \
		data/caches.o \
		data/regions.o \
		data/groovetable.o \
		data/tablecell.o \
		data/patterncell.o \
		data/control.o \
		data/instrument.o \
		data/bitshifter.o \
		data/hudcursor.o \
		data/song.o \
		data/channel.o \
		data/viewports.o \
		screens/config.o \
		screens/config/coloreditor.o \
		screens/config/looknfeel.o \
		screens/config/linkmode.o \
		screens/config/behavior.o \
		screens/config/memory.o \
		screens/console.o \
		screens/snake.o \
		screens/tracker.o \
		screens/patedit.o \
		screens/instedit.o \
		screens/songedit.o \
		screens/live.o \
		screens/live/piano.o \
		screens/live/performance.o \
		screens/help.o \
		dialogs/really.o \
		dialogs/alpha.o \
		dialogs/progress.o \
		debug.o \
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
	@$(CC) -o $@ $(CFLAGS) $<
	@echo "  C -> $@"
%.o : %.cpp
	@$(CC) -o $@ $(CFLAGS) $<
	@echo "CPP -> $@"
	
%.o : %.s
	$(AS) -o $@ $(ASFLAGS) $<
	@echo "ASM -> $@"
	
$(PROGNAME).elf : $(OFILES)
	@echo OK!
	@echo -------------------------------------------------------------------------------
	@echo Linking
	@echo -------------------------------------------------------------------------------
	@$(LD) $(LDFLAGS) -o $(PROGNAME).elf $(OFILES) $(LDLIBS)

index.m4h: index.hml
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f *.o *.i *.ii *.m4h	
	$(ROOT)/tools/helpcode/helpcode.exe index.hml index.m4h /c help /v -l -t
#----------------------------------------------------------------------------------------#
all : $(PROGNAME).gba
	$(GCC_BINDIR)/$(PFX)nm $(PROGNAME).elf > syms.txt

help: index.m4h

vba : all fixheader
	
gba : clean vba

fixheader:
	@echo -------------------------------------------------------------------------------
	@echo Patching ROM
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/gbafix$(EXEC_POSTFIX) $(PROGNAME).gba -t$(PROGNAME)
	call patch.bat
	
clean-kernel: 
	@echo -------------------------------------------------------------------------------
	@echo Cleaning up kernel kernel
	@echo -------------------------------------------------------------------------------
	 -f kernel/int/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/regionhandler/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/tim/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/sys/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/clip/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/key/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/gpu/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/net/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/mem/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/spu/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/sram/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/key/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f kernel/clip/*.o *.i *.ii *.m4h	

clean-data: clean-kernel
	@echo -------------------------------------------------------------------------------
	@echo Cleaning data
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f data/*.o *.i *.ii *.m4h	

clean-dialogs: clean-data
	@echo -------------------------------------------------------------------------------
	@echo Cleaning dialogs
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f dialogs/*.o *.i *.ii *.m4h	

clean-screens: clean-dialogs
	@echo -------------------------------------------------------------------------------
	@echo Cleaning screens
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f screens/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f screens/config/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f screens/tracker/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f screens/songedit/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f screens/live/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f screens/instedit/*.o *.i *.ii *.m4h	
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f screens/patedit/*.o *.i *.ii *.m4h	
	
clean-samples: 
	@echo -------------------------------------------------------------------------------
	@echo Encoding Samples
	@tools/gbfs$(EXEC_POSTFIX) data.gbfs st-01/* >NUL
	@echo Done.
	@echo Generating Name Objects under /st-01
	cd st-01
	@echo|set /p=PROGRESS: 0%...
	@echo|set /p=Alien > st-01\Alien.snm
	@echo|set /p=Aligator > st-01\Aligator.snm
	@echo|set /p=AnalogString > st-01\AnalogString.snm
	@echo|set /p=Asia > st-01\Asia.snm
	@echo|set /p=BassDrum1 > st-01\BassDrum1.snm
	@echo|set /p=BassDrum2 > st-01\BassDrum2.snm
	@echo|set /p=BassDrum3 > st-01\BassDrum3.snm
	@echo|set /p=BassDrum4 > st-01\BassDrum4.snm
	@echo|set /p=BigBow > st-01\BigBow.snm
	@echo|set /p=Blast > st-01\Blast.snm
	@echo|set /p=Blubzing > st-01\Blubzing.snm
	@echo|set /p=Breath > st-01\Breath.snm
	@echo|set /p=Call > st-01\Call.snm
	@echo|set /p=Celeste > st-01\Celeste.snm
	@echo|set /p=Chink > st-01\Chink.snm
	@echo|set /p=Cinema > st-01\Cinema.snm
	@echo|set /p=Claps1 > st-01\Claps1.snm
	@echo|set /p=Claps2 > st-01\Claps2.snm
	@echo|set /p=Claves > st-01\Claves.snm
	@echo|set /p=CloseHiHat > st-01\CloseHiHat.snm
	@echo|set /p=Conga > st-01\Conga.snm
	@echo|set /p=CowBell > st-01\CowBell.snm
	@echo|set /p=Dangerous > st-01\Dangerous.snm
	@echo|set /p=DeepBass > st-01\DeepBass.snm
	@echo|set /p=Detune > st-01\Detune.snm
	@echo|set /p=DigDug > st-01\DigDug.snm
	@echo|set /p=DigiHarp > st-01\DigiHarp.snm
	@echo|set /p=DreamBells > st-01\DreamBells.snm
	@echo|set /p=DXBass > st-01\DXBass.snm
	@echo|set /p=DxTom > st-01\DxTom.snm
	@echo|set /p=25%...
	@echo|set /p=ElecTom > st-01\ElecTom.snm
	@echo|set /p=EPiano > st-01\EPiano.snm
	@echo|set /p=ExBells > st-01\ExBells.snm
	@echo|set /p=FaeryTale > st-01\FaeryTale.snm
	@echo|set /p=FilterBass > st-01\FilterBass.snm
	@echo|set /p=FunBass > st-01\FunBass.snm
	@echo|set /p=FunkBass > st-01\FunkBass.snm
	@echo|set /p=Gato > st-01\Gato.snm
	@echo|set /p=Great > st-01\Great.snm
	@echo|set /p=HallBrass > st-01\HallBrass.snm
	@echo|set /p=Heaven > st-01\Heaven.snm
	@echo|set /p=HeavySynth > st-01\HeavySynth.snm
	@echo|set /p=Heifer > st-01\Heifer.snm
	@echo|set /p=HiHat1 > st-01\HiHat1.snm
	@echo|set /p=HiHat2 > st-01\HiHat2.snm
	@echo|set /p=Hooman > st-01\Hooman.snm
	@echo|set /p=Horns > st-01\Horns.snm
	@echo|set /p=JahrMarkt1 > st-01\JahrMarkt1.snm
	@echo|set /p=JahrMarkt2 > st-01\JahrMarkt2.snm
	@echo|set /p=Jetes > st-01\Jetes.snm
	@echo|set /p=Klickorgan > st-01\Klickorgan.snm
	@echo|set /p=KorgBass > st-01\KorgBass.snm
	@echo|set /p=KorgBeau > st-01\KorgBeau.snm
	@echo|set /p=KorgBow > st-01\KorgBow.snm
	@echo|set /p=KorgFilter > st-01\KorgFilter.snm
	@echo|set /p=KorgString > st-01\KorgString.snm
	@echo|set /p=Koto > st-01\Koto.snm
	@echo|set /p=Leader > st-01\Leader.snm
	@echo|set /p=Licks > st-01\Licks.snm
	@echo|set /p=Magic > st-01\Magic.snm
	@echo|set /p=Marimba > st-01\Marimba.snm
	@echo|set /p=50%...
	@echo|set /p=Mechanic1 > st-01\Mechanic1.snm
	@echo|set /p=Mechanic2 > st-01\Mechanic2.snm
	@echo|set /p=MetalKeys > st-01\MetalKeys.snm
	@echo|set /p=MonoBass > st-01\MonoBass.snm
	@echo|set /p=MonsterBass > st-01\MonsterBass.snm
	@echo|set /p=MuteClav > st-01\MuteClav.snm
	@echo|set /p=Nice > st-01\Nice.snm
	@echo|set /p=NightMare > st-01\NightMare.snm
	@echo|set /p=NoteMan > st-01\NoteMan.snm
	@echo|set /p=Organ > st-01\Organ.snm
	@echo|set /p=Outlaw > st-01\Outlaw.snm
	@echo|set /p=PanFlute > st-01\PanFlute.snm
	@echo|set /p=Perco > st-01\Perco.snm
	@echo|set /p=PingBells > st-01\PingBells.snm
	@echo|set /p=Pizza > st-01\Pizza.snm
	@echo|set /p=PolySynth > st-01\PolySynth.snm
	@echo|set /p=PopBass > st-01\PopBass.snm
	@echo|set /p=PopSnare1 > st-01\PopSnare1.snm
	@echo|set /p=PopSnare2 > st-01\PopSnare2.snm
	@echo|set /p=PopSnare3 > st-01\PopSnare3.snm
	@echo|set /p=Pulse > st-01\Pulse.snm
	@echo|set /p=RichString > st-01\RichString.snm
	@echo|set /p=RingPiano > st-01\RingPiano.snm
	@echo|set /p=RoomBrass > st-01\RoomBrass.snm
	@echo|set /p=RubberBass > st-01\RubberBass.snm
	@echo|set /p=Shaker > st-01\Shaker.snm
	@echo|set /p=Shamus > st-01\Shamus.snm
	@echo|set /p=SineCZ > st-01\SineCZ.snm
	@echo|set /p=SixTease > st-01\SixTease.snm
	@echo|set /p=SlapBass > st-01\SlapBass.snm
	@echo|set /p=Smash1 > st-01\Smash1.snm
	@echo|set /p=Smash2 > st-01\Smash2.snm
	@echo|set /p=75%...
	@echo|set /p=Snare1 > st-01\Snare1.snm
	@echo|set /p=Snare2 > st-01\Snare2.snm
	@echo|set /p=Snare3 > st-01\Snare3.snm
	@echo|set /p=Snare4 > st-01\Snare4.snm
	@echo|set /p=Snare5 > st-01\Snare5.snm
	@echo|set /p=SoftBass > st-01\SoftBass.snm
	@echo|set /p=Soundtrack > st-01\Soundtrack.snm
	@echo|set /p=Squares > st-01\Squares.snm
	@echo|set /p=Stabs > st-01\Stabs.snm
	@echo|set /p=Steinway > st-01\Steinway.snm
	@echo|set /p=Strange > st-01\Strange.snm
	@echo|set /p=Strings1 > st-01\Strings1.snm
	@echo|set /p=Strings2 > st-01\Strings2.snm
	@echo|set /p=Strings3 > st-01\Strings3.snm
	@echo|set /p=Strings4 > st-01\Strings4.snm
	@echo|set /p=Strings5 > st-01\Strings5.snm
	@echo|set /p=strings6 > st-01\strings6.snm
	@echo|set /p=Strings7 > st-01\Strings7.snm
	@echo|set /p=Strings8 > st-01\Strings8.snm
	@echo|set /p=Sweep > st-01\Sweep.snm
	@echo|set /p=SynBrass > st-01\SynBrass.snm
	@echo|set /p=SynClaves > st-01\SynClaves.snm
	@echo|set /p=SyntheBass > st-01\SyntheBass.snm
	@echo|set /p=SynthPiano > st-01\SynthPiano.snm
	@echo|set /p=TechBass > st-01\TechBass.snm
	@echo|set /p=TheEgg > st-01\TheEgg.snm
	@echo|set /p=TineWave > st-01\TineWave.snm
	@echo|set /p=Touch > st-01\Touch.snm
	@echo|set /p=TuneBass > st-01\TuneBass.snm
	@echo|set /p=Voices > st-01\Voices.snm
	@echo|set /p=WabberString > st-01\WabberString.snm
	@echo|set /p=WoodBlock > st-01\WoodBlock.snm
	@echo|set /p=WowBass > st-01\WowBass.snm
	cd ..
clean: clean-screens
	@echo -------------------------------------------------------------------------------
	@echo Cleaning program code
	@echo -------------------------------------------------------------------------------
	$(HAMDIR)/tools/win32/rm$(EXEC_POSTFIX) -f *.o *.i *.ii *.m4h	
	@echo -------------------------------------------------------------------------------

