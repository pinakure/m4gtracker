#ifndef SRAM_HPP 
#define SRAM_HPP 

#include "../../agb.h"
#include "../../macros.h"
#include "../../data/helpers.hpp"
#include "../gpu/gpu.hpp"
#include "../spu/sequencer.hpp"
#include "../../callbacks/trk.hpp"
#include "../../callbacks/pat.hpp"
#include "../../callbacks/ins.hpp"
#include "../../data/data.hpp"

#define DEVICE_EEPROM		0x0000
#define DEVICE_SST			0xD4BF
#define DEVICE_MACRONIX		0x1CC2
#define DEVICE_PANASONIC	0x1B32
#define DEVICE_ATMEL		0x3D1F
#define DEVICE_SANYO_128	0x1362
#define DEVICE_MACRONIX_128	0x09C2


#define DATA_BASE_ADDRESS 	0x80
#define SONG_DETAILS_SIZE	0x20
#define GROOVE_TABLE_SIZE	0x10
#define PATTERN_DATA_SIZE	(1536+256)


typedef struct sBitField {
	u8 *var;
	u8 position;
	u8 width;
}BitField;

class Sram {
	private:
		u8 *sram;
		
	public:
		int position;
		u16 waitstateBackup;
	
		Sram(){};
		void Init();
		void erase();
		void seek(int p);
		void forward(int p);
		u8   read();
		u16  read16();
		u32  read32();
		void write(u8);
		void write16(u16);
		void write32(u32);
		
		void songLoad		( bool verbose );
		void songSave		( bool verbose );
		void songDefaults	( bool verbose );
		void sharedDataLoad	( bool verbose );
		void sharedDataSave	( bool verbose );
		
		void drawPosition(u8 x, u8 y,u8 color);
};

extern Sram SRAM;

extern void readByte	( u8 &byte );
extern void readNibbles	( u8 &nibble1	, u8 &nibble2	, u8 mask = 0xF );
extern void writeNibbles( u8  nibble1 	, u8  nibble2 	, u8 mask = 0xF );
extern void readFields	( const BitField fields[ 8 ] );

#define SRAM_SIZE 0x8000

#endif
