#ifndef SRAM_HPP 
#define SRAM_HPP 

#include "../../agb.h"
#include "../../macros.h"
#include "../gpu/gpu.hpp"
#include "../spu/sequencer.hpp"
#include "../../screens/tracker.hpp"
#include "../../screens/patedit.hpp"
#include "../../screens/instedit.hpp"
#include "../../data/data.hpp"

#define DEVICE_EEPROM		0x0000
#define DEVICE_SST			0xD4BF
#define DEVICE_MACRONIX		0x1CC2
#define DEVICE_PANASONIC	0x1B32
#define DEVICE_ATMEL		0x3D1F
#define DEVICE_SANYO_128	0x1362
#define DEVICE_MACRONIX_128	0x09C2


#define DATA_BASE_ADDRESS 	0x80
#define PATTERN_DATA_SIZE	0x2800	// 5 bytes * 16 cells * 128 SHARED patterns 
#define INSTRUMENT_DATA_SIZE 0x1F80 // should be 0x1DC0, but this allows to round the final size and reserve spare room


typedef struct sBitField {
	u8 *var;
	u8 position;
	u8 width;
}BitField;

class Sram {
	private:
		static u8 *sram;
		
		static u16  size[10];
		static u16  base[10];
		static bool verbosity;
		static u8 	stage;
		
	public:
		static void	prepare();
		static void	next();
		static int 	position;
		static u16 	waitstateBackup;
	
		static void	init				( );
		static void	erase				( );
		static void	seek				( int p );
		static void	forward				( int p );
		static u8  	read				( );
		static u16 	read16				( );
		static u32	read32				( );
		static void	write				( u8	);
		static void	write16				( u16 	);
		static void	write32				( u32 	);
	
		static void	songLoad			( bool verbose );
		static void	songSave			( bool verbose );
		static void	songDefaults		( bool verbose );
		static void	sharedDataLoad		( bool verbose );
		static void	sharedDataSave		( bool verbose );

		static bool checkSignature		( );
		
		static void	drawPosition		( u8 x, u8 y,u8 color );
};

extern void readByte	( u8 &byte );
extern void readNibbles	( u8 &nibble1	, u8 &nibble2	, u8 mask = 0xF );
extern void writeNibbles( u8  nibble1	, u8  nibble2 	, u8 mask = 0xF );
extern void readFields	( const BitField fields[ 8 ] );

// #define SRAM_SIZE 0x8000 // 64 KB SRAM
#define SRAM_SIZE 0x8000 // 64 KB SRAM. It was 0x8000 before. 

#endif
