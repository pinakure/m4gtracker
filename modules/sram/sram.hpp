#ifndef _MODULES_SRAM
#define _MODULES_SRAM

#include "../../agb.h"
#include "../../data/variables.hpp"

#define DEVICE_EEPROM		0x0000
#define DEVICE_SST			0xD4BF
#define DEVICE_MACRONIX		0x1CC2
#define DEVICE_PANASONIC	0x1B32
#define DEVICE_ATMEL		0x3D1F
#define DEVICE_SANYO_128	0x1362
#define DEVICE_MACRONIX_128	0x09C2

class SRAM {
	private:
		static u8 *sram;
		
	public:
		static int position;
		static u16 waitstateBackup;
	
		static void init();
		static void erase();
		static void seek(int p);
		static void forward(int p);
		static u8   read();
		static u16  read16();
		static u32  read32();
		static void write(u8);
		static void write16(u16);
		static void write32(u32);
		
		static void songLoad();
		static void songSave();
		static void songDefaults();
		static void sharedDataLoad();
		static void sharedDataSave();
		static void dataRevert();
		static void dataBackup();
		
		static void drawPosition(u8 x, u8 y,u8 color);
};

void instrumentUnpack(Instrument *i);
void instrumentPack(Instrument *i);
void instcopy(Instrument *s, Instrument *d);

#endif
