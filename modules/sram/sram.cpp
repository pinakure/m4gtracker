#include "sram.hpp" 

Sram SRAM;

#include "../../callbacks/ins.hpp"
#include "../../callbacks/pat.hpp"
#include "../../callbacks/trk.hpp"
#include "../../callbacks/debug.hpp"

extern "C" {
	u8 	 SRAM_ReadByte(u16 position);
	void SRAM_WriteByte(u16 position, u8 byte);
};

void readByte( u8 &byte ){
	byte = SRAM.read();
}

void readNibbles(u8 &nibble1, u8 &nibble2, u8 mask){
	u8 h = SRAM.read();
	nibble1 = (h >> 4) & mask;
	nibble2 = h & mask;
}
void writeNibbles(u8 nibble1, u8 nibble2, u8 mask){
	SRAM.write(((nibble1 & mask) << 4) | (nibble2 & mask));
}

void readFields( const BitField fields[8] ) {
	u8 byte = SRAM.read();
	for( int i=0; i<8; i++){
		if( !fields[ i ].var ) continue;
		u8 mask = ( 2 << (fields[ i ].width-1) ) - 1;
		*fields[ i ].var = (byte >> fields[ i ].position) & mask;
	}	
}

void Sram::Init(void){
	// Set WAITCNT (Waitstate controller) to 8 wait cycle mode	
	
	waitstateBackup = PORT(0x4000204);
	// TODO: Read first 8 bytes
	// If header does not match m4geek signature, erase SRAM, then save settings
	
	// Set SRAM Write cursor to 0.
	seek(0);
	
	// Restore old waitstate value
}

void Sram::seek(int p){ 
	position = p;	
}

void Sram::forward(int p){ 
	position += p;	
}

void Sram::erase(void){
	int i;
	seek(0);
	for(i=0; i<0x8000; i++){
		write(0x00);
	}	
}

void Sram::drawPosition(u8 x, u8 y, u8 color) { 
	HEXADECIMAL_DOUBLE(x+1,	y,	color, position&0xFF);
	HEXADECIMAL_DOUBLE(x,	y,	color, position>>8);
}

void Sram::write(u8 a){ 
	SRAM_WriteByte(position++, a);
	if((position & 0xFF)>0x80) gpu.set( 2,29, 0, 0x100);
	else gpu.set( 2,29, 0, (COLOR_RED << 12) | 0x3D);
}

void Sram::write16(u16 a) { 
	write(a&0x00FF); 
	write((a&0xFF00) >> 8);
}

void Sram::write32(u32 a) {  
	write( a & 0x000000FF); 
	write((a & 0x0000FF00) >> 8); 
	write((a & 0x00FF0000) >> 16); 
	write((a & 0xFF000000) >> 24); 
}

u8 Sram::read(){
	if((position & 0xFF)>0x80)gpu.set( 2,29, 0, 0x100);
	else gpu.set( 2,29, 0, (COLOR_GREEN << 12) | 0x3D);
	u8 r = SRAM_ReadByte(position++);
	return r;
}

u16 Sram::read16() { 
	u8 a, b;
	a = read();
	b = read();
	return (b<<8) | a;
}

u32 Sram::read32() { 
	u8 a, b, c, d;
	a = read();
	b = read();
	c = read();
	d = read();
	return (d<<24) | (c<<16) | (b<<8) | a;
}

void Sram::songSave( bool verbose ){
	int i;
	
	seek	( DATA_BASE_ADDRESS );
	forward	( SONG_DETAILS_SIZE * VAR_CFG.SLOT );

	// Write Song details
	/*1*/ write		( VAR_SONG.TRANSPOSE);
	/*2*/ write		( VAR_SONG.BPM);
	/*3*/ writeNibbles( VAR_SONG.PATTERNLENGTH, VAR_SONG.GROOVE.LENGTH);
	/*4*/ write		( (VAR_SONG.GROOVE.ENABLE <<2) | VAR_SONG.NOTEMPTY);//6 bit left unused!!
		
	// Write Artist and title
	for(i=0; i<14;i++){
		/*18*/write	( VAR_SONG.TITLE	[ i ] );
		/*32*/write	( VAR_SONG.ARTIST	[ i ] );
	} 

	//0x140
	seek	( DATA_BASE_ADDRESS );
	forward ( SONG_DETAILS_SIZE * 6 );
	forward ( GROOVE_TABLE_SIZE * VAR_CFG.SLOT);
	if( verbose ) drawPosition(27, 2, 2);	
	
	// Write Groove Steps
	for(i=0; i<16; i++){
		write( VAR_SONG.GROOVE.STEP[ i ] );
	}
	
	//0x1A0
	seek(DATA_BASE_ADDRESS + (SONG_DETAILS_SIZE * 6) + ( GROOVE_TABLE_SIZE * 6 ) );
	forward( PATTERN_DATA_SIZE * VAR_CFG.SLOT );
	if( verbose ) drawPosition(27, 3, 2);	
	//seek(0x200);
		
	for(i=0; i<256; i++){
		write(VAR_SONG.PATTERNS[0].ORDER[i]);
		write(VAR_SONG.PATTERNS[1].ORDER[i]);
		write(VAR_SONG.PATTERNS[2].ORDER[i]);
		write(VAR_SONG.PATTERNS[3].ORDER[i]);
		write(VAR_SONG.PATTERNS[4].ORDER[i]);
		write(VAR_SONG.PATTERNS[5].ORDER[i]);
		#ifndef NSONGTRANSPOSE
		write(VAR_SONG.PATTERNS[0].TRANSPOSE[i]);
		write(VAR_SONG.PATTERNS[1].TRANSPOSE[i]);
		write(VAR_SONG.PATTERNS[2].TRANSPOSE[i]);
		write(VAR_SONG.PATTERNS[3].TRANSPOSE[i]);
		write(VAR_SONG.PATTERNS[4].TRANSPOSE[i]);
		write(VAR_SONG.PATTERNS[5].TRANSPOSE[i]);
		#endif
	}
	
	if( verbose ) drawPosition(27, 4, 2);	
	
	sharedDataSave( verbose );
}



void Sram::songLoad( bool verbose ){
	int i;
	u8 h;
	
	
	seek		( DATA_BASE_ADDRESS );
	forward	( SONG_DETAILS_SIZE * VAR_CFG.SLOT);
	
	// Song details
	readByte( VAR_SONG.TRANSPOSE );
	readByte( VAR_SONG.BPM 	  );
	readNibbles( VAR_SONG.PATTERNLENGTH, VAR_SONG.GROOVE.LENGTH);
	
	h = read();
	VAR_SONG.GROOVE.ENABLE = EXTRACT( h, 2, 0x1);
	VAR_SONG.NOTEMPTY = h & 0x1;
		
	// Artist and title
	for(i=0; i<14;i++){
		VAR_SONG.TITLE[i]  = read();
		VAR_SONG.ARTIST[i] = read();
	} 

		seek	( DATA_BASE_ADDRESS );
	forward ( SONG_DETAILS_SIZE * 6 );
	forward ( GROOVE_TABLE_SIZE * VAR_CFG.SLOT);
	if( verbose ) drawPosition(27, 2, 6);	
	
	for(i=0; i<16; i++){
		readByte( VAR_SONG.GROOVE.STEP[i] );
	}
		
	//1A0
	seek(DATA_BASE_ADDRESS + (SONG_DETAILS_SIZE * 6) + ( GROOVE_TABLE_SIZE * 6 ) );
	forward( PATTERN_DATA_SIZE * VAR_CFG.SLOT );
	if( verbose ) drawPosition(27, 3, 6);	
	//seek(0x200);
	
	for(i=0; i<256; i++){
		VAR_SONG.PATTERNS[0].ORDER[i] = read();
		VAR_SONG.PATTERNS[1].ORDER[i] = read();
		VAR_SONG.PATTERNS[2].ORDER[i] = read();
		VAR_SONG.PATTERNS[3].ORDER[i] = read();
		VAR_SONG.PATTERNS[4].ORDER[i] = read();
		VAR_SONG.PATTERNS[5].ORDER[i] = read();
		#ifndef NSONGTRANSPOSE
		VAR_SONG.PATTERNS[0].TRANSPOSE[i] = read();
		VAR_SONG.PATTERNS[1].TRANSPOSE[i] = read();
		VAR_SONG.PATTERNS[2].TRANSPOSE[i] = read();
		VAR_SONG.PATTERNS[3].TRANSPOSE[i] = read();
		VAR_SONG.PATTERNS[4].TRANSPOSE[i] = read();
		VAR_SONG.PATTERNS[5].TRANSPOSE[i] = read();
		#endif
	}
	

	PatEdit::syncPosition( VAR_CFG.ORDERPOSITION );
	Tracker::syncPattern();
	
	if( verbose ) drawPosition(27, 4, 6);	
	
	sharedDataLoad( verbose );
	// Update controls to show recently loaded data
	for(int i=0; i<6; i++){
		Tracker::syncChannel(i);
	}

	// No need to redraw here
	PatEdit::sync(verbose);
}

void Sram::songDefaults( bool verbose ){
	int i,di;
	
	for(i=0; i<14; i++){
		VAR_SONG.TITLE[i]  = 0xFF;
		VAR_SONG.ARTIST[i] = 0xFF;
	}
	Sequencer::setTempo(0x80);
	VAR_SONG.BPM = 0x80;
	VAR_SONG.PATTERNLENGTH = 0xF;
	
	for(i=0; i<6; i++){
		VAR_INSTRUMENT.NAME[i] = 0xFF;
	}
	
	// Pattern data clear
	for(i=0; i<128; i++){
		for(di=0; di<16; di++){
			VAR_DATA[i].KEY[di] = 0;
			VAR_DATA[i].INS[di] = 0;
			VAR_DATA[i].VOL[di] = 0;
			VAR_DATA[i].CMD[di] = 0;
			VAR_DATA[i].VAL[di] = 0;
		}
	}

	// Live Perform table clear
	for(i=0; i<8; i++){
		VAR_LIVE.PERFORM.LEFT.KEY[i] = 0;
		VAR_LIVE.PERFORM.LEFT.INS[i] = 0;
		VAR_LIVE.PERFORM.LEFT.CHAN[i] = 0;
		VAR_LIVE.PERFORM.LEFT.VOL[i] = 0;
		VAR_LIVE.PERFORM.LEFT.CMD[i] = 0;
		VAR_LIVE.PERFORM.LEFT.VAL[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.KEY[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.INS[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.CHAN[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.VOL[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.CMD[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.VAL[i] = 0;
	}

	// Empty Order chains
	for(i=0; i<6; i++){
		for(di=0; di<256; di++){
			VAR_SONG.PATTERNS[i].ORDER[di] = 0;
			#ifndef NSONGTRANSPOSE
			VAR_SONG.PATTERNS[i].TRANSPOSE[di] = 0;
			#endif
		}
		VAR_SONG.PATTERNS[i].POSITION = 0;
	}	
}

void Sram::sharedDataSave( bool verbose ){
	int i, di;
	// Dump pattern data
	seek(0x2700);
	for(i=0;i<128;i++){
		for(di=0;di<16;di++){
			write(VAR_DATA[i].KEY[di]);
			write(VAR_DATA[i].INS[di]);
			write(VAR_DATA[i].VOL[di]);
			write(VAR_DATA[i].CMD[di]);
			write(VAR_DATA[i].VAL[di]);
		}
	}	
	
	//0x4E00
	if( verbose ) drawPosition(27, 5, 2);	
	seek(0x5100);
	
	//Dump instruments (these are shared along all songs)
	InstEdit::copy(&VAR_INSTRUMENT, &VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT]);

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

	if( verbose ) drawPosition(27, 5, 2);	
}

void Sram::sharedDataLoad( bool verbose ){
	int i, di;

	seek(0x2700);

	// Pattern data
	for(i=0;i<128;i++){
		for(di=0;di<16;di++){
			VAR_DATA[i].KEY[di] = read();
			VAR_DATA[i].INS[di] = read();
			VAR_DATA[i].VOL[di] = read();
			VAR_DATA[i].CMD[di] = read();
			VAR_DATA[i].VAL[di] = read();
		}
	}	
	
	//0x4E00
	if( verbose ) drawPosition(27, 5, 6);	
	seek(0x5100);
	
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

	InstEdit::copy(&VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT], &VAR_INSTRUMENT);
	
	Sequencer::setTempo( VAR_SONG.BPM );

	if( verbose ) drawPosition(27, 5, 6);
}
