#include "sram.hpp"

#include "../spu/spu.hpp"
#include "../../data/variables.hpp"
#include "../../macros.hpp"

extern void HEXADECIMAL_DOUBLE(u8 x, u8 y, u16 color, u16 value);
extern void patternSync(u8 position);
extern void cellSync();

u8* SRAM::sram;
int SRAM::position;
u16 SRAM::waitstateBackup;

// This declares both functions defined @ asm/sram.s
extern "C" {
	void SRAM_WriteByte(u16 position, u8 byte);
	u8 SRAM_ReadByte(u16 position);
}

void SRAM::init(void){
	// Set WAITCNT (Waitstate controller) to 8 wait cycle mode	
	
	waitstateBackup = PORT(0x4000204);
	// TODO: Read first 8 bytes
	// If header does not match m4geek signature, erase SRAM, then save settings
	
	// Set SRAM Write cursor to 0.
	seek(0);
	
	// Restore old waitstate value
}

void SRAM::seek(int p){ 
	position = p;	
}

void SRAM::forward(int p){ 
	position += p;	
}

void SRAM::erase(void){
	int i;
	seek(0);
	for(i=0; i<0x8000; i++){
		write(0x00);
	}	
}

void SRAM::drawPosition(u8 x, u8 y, u8 color) { 
	HEXADECIMAL_DOUBLE(x+1,	y,	color, position&0xFF);
	HEXADECIMAL_DOUBLE(x,	y,	color, position>>8);
}

void SRAM::write(u8 a){ 
	SRAM_WriteByte(position++, a);
}

void SRAM::write16(u16 a) { 
	write(a&0x00FF); 
	write((a&0xFF00) >> 8);
}

void SRAM::write32(u32 a) {  
	write( a & 0x000000FF); 
	write((a & 0x0000FF00) >> 8); 
	write((a & 0x00FF0000) >> 16); 
	write((a & 0xFF000000) >> 24); 
}

u8 SRAM::read(){
	u8 r = SRAM_ReadByte(position++);
	return r;
}

u16 SRAM::read16() { 
	u8 a, b;
	a = read();
	b = read();
	return (b<<8) | a;
}

u32 SRAM::read32() { 
	u8 a, b, c, d;
	a = read();
	b = read();
	c = read();
	d = read();
	return (d<<24) | (c<<16) | (b<<8) | a;
}

void SRAM::songSave(){
	int i;
	
	seek(0x80);
	forward(32 * CFG::slot);

	// Write Song details
	write( song.transpose	);
	write( song.bpm 		);
	write( ( song.pattern_length << 4 ) | ( song.groove.length ) );
	write( ( song.groove.enable  << 2 ) | song.not_empty );//6 bit left unused!!
		
	// Write Artist and title
	for(i=0; i<14;i++){
		write( song.title[ i ] );
		write( song.artist[ i ] );
	} 

	drawPosition(27, 2, 2);	
	//0x140
	
	// Write Groove Steps
	forward( 16 * CFG::slot );
	for( i=0; i < 16; i++ ){
		write( song.groove.step[ i ] );
	}
	
	//0x1A0
	drawPosition( 27, 3, 2 );	
	seek( 0x200 );
		
	forward( 1536 * CFG::slot );	
	for( i=0; i < 256; i++ ){
		write( song.patterns[ 0 ].order[ i ] );
		write( song.patterns[ 1 ].order[ i ] );
		write( song.patterns[ 2 ].order[ i ] );
		write( song.patterns[ 3 ].order[ i ] );
		write( song.patterns[ 4 ].order[ i ] );
		write( song.patterns[ 5 ].order[ i ] );
	}
	
	drawPosition(27, 4, 2);	
	
	sharedDataSave();
}

void SRAM::songLoad(){
	int i;
	u8  h;
	
	seek( 0x80 );
	forward( 32 * CFG::slot );
	
	// Song details
	song.transpose 		= read();
	song.bpm 			= read();
		
	h = read();
	song.pattern_length = EXTRACT( h, 4, 0xF);
	song.groove.length	= h & 0xF;
		
	h = read();
	song.groove.enable	= EXTRACT( h, 2, 0x1);
	song.not_empty		= h & 0x1;
		
	// Artist and title
	for( i=0; i < 14; i++ ){
		song.title[ i ]  = read();
		song.artist[ i ] = read();
	} 

	drawPosition( 27, 2, 6 );	

	forward( 16 * CFG::slot );
	for( i=0; i < 16; i++ ){
		song.groove.step[ i ] = read();
	}
		
	//1A0
	drawPosition( 27, 3, 6 );	
	seek( 0x200 );
	
	forward( 1536 * CFG::slot );
	for( i=0; i < 256; i++ ){
		song.patterns[ 0 ].order[ i ] = read();
		song.patterns[ 1 ].order[ i ] = read();
		song.patterns[ 2 ].order[ i ] = read();
		song.patterns[ 3 ].order[ i ] = read();
		song.patterns[ 4 ].order[ i ] = read();
		song.patterns[ 5 ].order[ i ] = read();
	}
	
	patternSync( CFG::order_position );
	cellSync();
	
	drawPosition( 27, 4, 6 );
	
	sharedDataLoad();
}

void SRAM::songDefaults(){
	int i,di;
	
	for( i=0; i < 14; i++ ){
		song.title[i]  	= 0xFF;
		song.artist[i] 	= 0xFF;
	}
	//SPU::setTempo(0x80);
	song.bpm 			= 0x80;
	song.pattern_length = 0xF;
	
	for( i=0; i < 6; i++ ){
		VAR_INSTRUMENT.NAME[i] = 0xFF;
	}
	
	// Pattern data clear
	for(i=0; i<128; i++){
		for(di=0; di<16; di++){
			VAR_DATA[i].key[di] = 0;
			VAR_DATA[i].ins[di] = 0;
			VAR_DATA[i].vol[di] = 0;
			VAR_DATA[i].cmd[di] = 0;
			VAR_DATA[i].val[di] = 0;
		}
	}

	// Live Perform table clear
	for(i=0; i<8; i++){
		live.perform.left.key[i] = 0;
		live.perform.left.ins[i] = 0;
		live.perform.left.chan[i] = 0;
		live.perform.left.vol[i] = 0;
		live.perform.left.cmd[i] = 0;
		live.perform.left.val[i] = 0;
		live.perform.right.key[i] = 0;
		live.perform.right.ins[i] = 0;
		live.perform.right.chan[i] = 0;
		live.perform.right.vol[i] = 0;
		live.perform.right.cmd[i] = 0;
		live.perform.right.val[i] = 0;
	}

	// Empty Order chains
	for( i=0; i < 6; i++ ){
		for( di=0; di < 256; di++ ){
			song.patterns[ i ].order[ di ] = 0;
		}
		song.patterns[ i ].position = 0;
	}	
}




void SRAM::sharedDataSave(void){
	int i, di;
	// Dump pattern data
	seek(0x2600);
	for(i=0;i<128;i++){
		for(di=0;di<16;di++){
			write(VAR_DATA[i].key[di]);
			write(VAR_DATA[i].ins[di]);
			write(VAR_DATA[i].vol[di]);
			write(VAR_DATA[i].cmd[di]);
			write(VAR_DATA[i].val[di]);
		}
	}	
	
	//0x4E00
	drawPosition(27, 5, 2);	
	seek(0x5000);
	
	//Dump instruments (these are shared along all songs)
	instcopy(&VAR_INSTRUMENT, &VAR_INSTRUMENTS[ CFG::current_instrument ]);

	for(i=0; i<64; i++){
		write(VAR_INSTRUMENTS[i].TYPE);
		for(di=0; di<6; di++){
			write(VAR_INSTRUMENTS[i].NAME[di]);
		}
		for(di=0; di<32; di++){
			write(VAR_INSTRUMENTS[i].SETTINGS[di]);
		}
		for(di=0; di<16; di++){
			write(VAR_INSTRUMENTS[i].TABLE.TRANSPOSE[di]);
			write(VAR_INSTRUMENTS[i].TABLE.VOLUME[di]);
			write(VAR_INSTRUMENTS[i].TABLE.COMMAND[0][di]);
			write(VAR_INSTRUMENTS[i].TABLE.COMMAND[1][di]);
			write(VAR_INSTRUMENTS[i].TABLE.VALUE[0][di]);
			write(VAR_INSTRUMENTS[i].TABLE.VALUE[1][di]);
		}
	}

	drawPosition(27, 5, 2);	
}

void SRAM::sharedDataLoad(void){
	int i, di;

	seek(0x2600);

	// Pattern data
	for(i=0;i<128;i++){
		for(di=0;di<16;di++){
			VAR_DATA[i].key[di] = read();
			VAR_DATA[i].ins[di] = read();
			VAR_DATA[i].vol[di] = read();
			VAR_DATA[i].cmd[di] = read();
			VAR_DATA[i].val[di] = read();
		}
	}	
	
	//0x4E00
	drawPosition(27, 5, 6);	
	seek(0x5000);
	
	//Load instruments (these are shared along all songs)
	for(i=0; i<64; i++){
		VAR_INSTRUMENTS[i].TYPE = read();
		for(di=0; di<6; di++){
			VAR_INSTRUMENTS[i].NAME[di] = read();
		}
		for(di=0; di<32; di++){
			VAR_INSTRUMENTS[i].SETTINGS[di] = read();
		}
		// Dump instrument table
		for(di=0; di<16; di++){
			VAR_INSTRUMENTS[i].TABLE.TRANSPOSE[di] 	= read();
			VAR_INSTRUMENTS[i].TABLE.VOLUME[di] 	= read();
			VAR_INSTRUMENTS[i].TABLE.COMMAND[0][di] = read();
			VAR_INSTRUMENTS[i].TABLE.COMMAND[1][di] = read();
			VAR_INSTRUMENTS[i].TABLE.VALUE[0][di] 	= read();
			VAR_INSTRUMENTS[i].TABLE.VALUE[1][di] 	= read();
		}
	}

	instcopy(&VAR_INSTRUMENTS[CFG::current_instrument], &VAR_INSTRUMENT);
	
	SPU::setTempo( song.bpm );

	drawPosition( 27, 5, 6 );
}

void SRAM::dataBackup(void){
	int i, di;
	
	// Dump 6 Song Headers (32 bytes each one, 192 total)
	seek(0x80);
	for( i=0; i < 6; i++ ){
		// Song details
		write( songs[ i ].transpose		);
		write( songs[ i ].bpm			);
		write( ( songs[ i ].pattern_length  << 4 ) | ( songs[ i ].groove.length ) );
		write( ( songs[ i ].groove.enable	<< 2 ) |   songs[ i ].not_empty );//6 bit left unused!!
		
		// Artist and title
		for( di=0; di < 14; di++ ){
			write( songs[ i ].title[ di ] );
			write( songs[ i ].artist[ di ] );
		} 
	} 

	//0x140
	
	// Groove Steps
	for( i=0; i < 6; i++ ){
		for( di=0; di < 16; di++ ){
			write( songs[ i ].groove.step[ di ] );
		}
	} 
	
	//0x1A0
	
	// Dump pattern orders (256 * 6)
	seek( 0x200 );
	for( i=0; i < 6; i++ ){
		for( di=0; di < 256; di++ ){
			write( songs[ i ].patterns[ 0 ].order[ di ] );
			write( songs[ i ].patterns[ 1 ].order[ di ] );
			write( songs[ i ].patterns[ 2 ].order[ di ] );
			write( songs[ i ].patterns[ 3 ].order[ di ] );
			write( songs[ i ].patterns[ 4 ].order[ di ] );
			write( songs[ i ].patterns[ 5 ].order[ di ] );
		}
	}
	
	sharedDataSave();
	
	drawPosition( 27, 1, 5 );
}

void SRAM::dataRevert(void){
	int i, di;
	u8  h;
	
	// Load 6 Song Headers (32 bytes each one, 192 total)
	seek( 0x80 );
	for(i=0; i<6; i++){
		// Song details
		songs[ i ].transpose	 	= read();
		songs[ i ].bpm 				= read();
		
		h = read();
		songs[ i ].pattern_length	= EXTRACT( h, 4, 0xF );
		songs[ i ].groove.length	= h & 0xF;
		
		h = read();
		songs[ i ].groove.enable	= EXTRACT( h, 2, 0x1 );
		songs[ i ].not_empty		= h & 0x1;
		
		// Artist and title
		for( di=0; di < 14;di++ ){
			songs[ i ].title[ di ] 	= read();
			songs[ i ].artist[ di ] = read();
		} 
	} 

	// Groove Steps (96 bytes)
	for( i=0; i < 6; i++ ){
		for( di=0; di < 16; di++ ){
			songs[ i ].groove.step[ di ] = read();
		}
	} 
	
	
	// Dump pattern orders (256 * 6)
	seek( 0x200 );
	for( i=0; i < 6; i++ ){
		for( di=0; di < 256; di++ ){
			songs[ i ].patterns[ 0 ].order[ di ] = read();
			songs[ i ].patterns[ 1 ].order[ di ] = read();
			songs[ i ].patterns[ 2 ].order[ di ] = read();
			songs[ i ].patterns[ 3 ].order[ di ] = read();
			songs[ i ].patterns[ 4 ].order[ di ] = read();
			songs[ i ].patterns[ 5 ].order[ di ] = read();
		}
	}
	
	sharedDataLoad();
	
	drawPosition( 27, 1, 7 );
}
