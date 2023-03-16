#include "sram.hpp" 

Sram SRAM;

#include "../../screens/live.hpp"
#include "../../data/pattern.hpp"
#include "../../screens/instedit.hpp"
#include "../../screens/patedit.hpp"
#include "../../screens/songedit.hpp"
#include "../../screens/tracker.hpp"
#include "../../debug.hpp"

u8* Sram::sram;
int Sram::position;
u16 Sram::waitstateBackup;

extern "C" {
	u8 	 SRAM_ReadByte(u16 position);
	void SRAM_WriteByte(u16 position, u8 byte);
};

void readByte( u8 &byte ){
	byte = Sram::read();
}

void readNibbles(u8 &nibble1, u8 &nibble2, u8 mask){
	u8 h = Sram::read();
	nibble1 = (h >> 4) & mask;
	nibble2 = h & mask;
}
void writeNibbles(u8 nibble1, u8 nibble2, u8 mask){
	Sram::write(((nibble1 & mask) << 4) | (nibble2 & mask));
}

void readFields( const BitField fields[8] ) {
	u8 byte = Sram::read();
	for( int i=0; i<8; i++){
		if( !fields[ i ].var ) continue;
		u8 mask = ( 2 << (fields[ i ].width-1) ) - 1;
		*fields[ i ].var = (byte >> fields[ i ].position) & mask;
	}	
}
#include "../key/key.hpp"

bool Sram::checkSignature(){
	// If header does not match m4geek signature, erase SRAM, then save settings
	seek(0);
	
	if( read32() == M4GEEK_SIGNATURE)
		if( read  () == M4G_VERSION		) return true;

	Gpu::drawDialog(0,0,30,20, " Bad SRAM Signature ");
	Gpu::ascii(3, 2,"Invalid SRAM data read.", COLOR_CYAN);
	Gpu::ascii(3, 4,"Format of the data stored in this gamepak backup RAM");
	Gpu::ascii(3, 5,"does not match the expected structure for this version.");
	Gpu::ascii(3, 7,"If you had data from a previous version in this SRAM,");
	Gpu::ascii(3, 8,"please turn off the unit and try to extract the data ");
	Gpu::ascii(3, 9,"using a external SRAM manager, upload it to the tool");
	Gpu::ascii(3,10,"              www.m4gtracker.com. ", COLOR_CYAN);
	Gpu::ascii(3,10,"           at ");
	Gpu::ascii(3,10,"m4gpatcher", COLOR_CYAN);
	Gpu::ascii(3,12,"You may then download a updated version of your data,");
	Gpu::ascii(3,13,"which will be usable with this version, as long as the");
	Gpu::ascii(3,14,"original SRAM file contains valid M4GTracker save data.");
	Gpu::ascii(3,17,"Press a button to ignore this and overwrite SRAM data.", COLOR_YELLOW);
	while(!KEYACTIVITY()){ KEYUPDATE(); }
	// Gpu::clear(0x3);
	Gpu::drawDialog(1,1,28,12," Initialize SRAM");
	Gpu::ascii(5, 3,"                the contents in the SRAM", COLOR_YELLOW);
	Gpu::ascii(5, 3,"          ERASE", COLOR_RED);
	Gpu::ascii(5, 3,"This will", COLOR_YELLOW);
	Gpu::ascii(5, 4,"This action cannot be undone", COLOR_ORANGE);
	Key::forceNoInput();
	ReallyDialog::enable();
	if(!ReallyDialog::result)asm("swi 00");
	Gpu::clear(0x0);
	return false;
}

void Sram::init(){
	// Set WAITCNT (Waitstate controller) to 8 wait cycle mode	
	
	waitstateBackup = PORT(0x4000204);
	
	
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

void Sram::erase(){
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
	if((position & 0xFF)>0x80) Gpu::set( 2,29, 0, 0x100);
	else Gpu::set( 2,29, 0, (COLOR_RED << 12) | 0x3D);
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
	if((position & 0xFF)>0x80)Gpu::set( 2,29, 0, 0x100);
	else Gpu::set( 2,29, 0, (COLOR_GREEN << 12) | 0x3D);
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
	/*1*/ write			( VAR_SONG.TRANSPOSE );
	/*2*/ write			( VAR_SONG.BPM );
	/*3*/ writeNibbles	( VAR_SONG.PATTERNLENGTH, VAR_SONG.GROOVE.LENGTH );
	/*4*/ write			( ( VAR_SONG.GROOVE.ENABLE << 2 ) | VAR_SONG.NOTEMPTY );//6 bit left unused!!
		
	// Write Artist and title
	for( i = 0 ; i < 14 ; i++ ){
		/*18*/write	( VAR_SONG.TITLE	[ i ] );
		/*32*/write	( VAR_SONG.ARTIST	[ i ] );
	} 

	//0x140
	seek	( DATA_BASE_ADDRESS );
	forward ( SONG_DETAILS_SIZE * SONG_SLOT_COUNT );
	forward ( GROOVE_TABLE_SIZE * VAR_CFG.SLOT);
	if( verbose ) drawPosition(27, 2, 2);	
	
	// Write Groove Steps
	for(i=0; i<0x10; i++){
		write( VAR_SONG.GROOVE.STEP[ i ] );
	}
	
	//0x1A0
	seek(DATA_BASE_ADDRESS + (SONG_DETAILS_SIZE * SONG_SLOT_COUNT) + ( GROOVE_TABLE_SIZE * SONG_SLOT_COUNT ) );
	forward( PATTERN_DATA_SIZE * VAR_CFG.SLOT );
	if( verbose ) drawPosition(27, 3, 2);	
	//seek(0x200);
		
	for(i=0; i<ORDER_COUNT; i++){
		for(int c=0; c<CHANNEL_COUNT; c++){
			write(VAR_SONG.PATTERNS[ c ].ORDER		[ i ] );
			write(VAR_SONG.PATTERNS[ c ].TRANSPOSE	[ i ] );
		}
	}

	if( verbose ) drawPosition(27, 4, 2);	
	sharedDataSave( verbose );
}

#include "../key/key.hpp"

void Sram::songLoad( bool verbose ){
	int i;
	u8 h;
		
	seek	( DATA_BASE_ADDRESS );
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
	forward ( SONG_DETAILS_SIZE * SONG_SLOT_COUNT );
	forward ( GROOVE_TABLE_SIZE * VAR_CFG.SLOT);
	if( verbose ) drawPosition(27, 2, 6);	
	
	for(i=0; i < 0x10; i++){
		readByte( VAR_SONG.GROOVE.STEP[i] );
	}
		
	//1A0
	seek(DATA_BASE_ADDRESS + (SONG_DETAILS_SIZE * SONG_SLOT_COUNT) + ( GROOVE_TABLE_SIZE * SONG_SLOT_COUNT ) );
	forward( PATTERN_DATA_SIZE * VAR_CFG.SLOT );
	if( verbose ) drawPosition(27, 3, 6);	
	//seek(0x200);
	
	for(i=0; i < ORDER_COUNT; i++ ){
		for( int c=0; c < CHANNEL_COUNT; c++ ){
			VAR_SONG.PATTERNS[ c ].ORDER	[ i ] = read();
			VAR_SONG.PATTERNS[ c ].TRANSPOSE[ i ] = read();
		}
	}

	PatEdit::syncPosition( VAR_CFG.ORDERPOSITION );
	Tracker::syncPattern();
	
	if( verbose ) drawPosition(27, 4, 6);	
	
	sharedDataLoad( verbose );
	// Update controls to show recently loaded data
	for(int i=0; i<CHANNEL_COUNT; i++){
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
	VAR_SONG.BPM 			= 0x80;
	VAR_SONG.PATTERNLENGTH 	= 0xF;
	VAR_SONG.TRANSPOSE 		= 0x00;
	
	for(i=0; i < CHANNEL_COUNT ; i++){
		VAR_INSTRUMENT.NAME[i] = 0xFF;
	}
	
	// Pattern data clear
	for(i=0; i<PATTERN_COUNT; i++){
		for(di=0; di<0x10; di++){
			VAR_DATA[ i ].KEY[ di ] = 0;
			VAR_DATA[ i ].INS[ di ] = 0;
			VAR_DATA[ i ].VOL[ di ] = 0;
			VAR_DATA[ i ].CMD[ di ] = 0;
			VAR_DATA[ i ].VAL[ di ] = 0;
		}
	}

	// Live Perform table clear
	for(i=0; i<8; i++){
		Live::PERFORM.LEFT.KEY	[ i ] = 0;
		Live::PERFORM.LEFT.INS	[ i ] = 0;
		Live::PERFORM.LEFT.CHAN	[ i ] = 0;
		Live::PERFORM.LEFT.VOL	[ i ] = 0;
		Live::PERFORM.LEFT.CMD	[ i ] = 0;
		Live::PERFORM.LEFT.VAL	[ i ] = 0;
		Live::PERFORM.RIGHT.KEY	[ i ] = 0;
		Live::PERFORM.RIGHT.INS	[ i ] = 0;
		Live::PERFORM.RIGHT.CHAN	[ i ] = 0;
		Live::PERFORM.RIGHT.VOL	[ i ] = 0;
		Live::PERFORM.RIGHT.CMD	[ i ] = 0;
		Live::PERFORM.RIGHT.VAL	[ i ] = 0;
	}

	// Empty Order chains
	for(i=0; i<CHANNEL_COUNT; i++){
		for(di=0; di<PATTERN_COUNT; di++){
			VAR_SONG.PATTERNS[i].ORDER[di] = 0;
			VAR_SONG.PATTERNS[i].TRANSPOSE[di] = 0;
		}
		VAR_SONG.PATTERNS[i].POSITION = 0;
	}	
}

void Sram::sharedDataSave( bool verbose ){
	int i, di;
	// Dump pattern data
	seek(0x2700);
	for( i=0 ; i < PATTERN_COUNT; i++ ){
		for(di=0;di<0x10;di++){
			write( VAR_DATA[ i ].KEY[ di ]);
			write( VAR_DATA[ i ].INS[ di ]);
			write( VAR_DATA[ i ].VOL[ di ]);
			write( VAR_DATA[ i ].CMD[ di ]);
			write( VAR_DATA[ i ].VAL[ di ]);
		}
	}	
	
	//0x4E00
	if( verbose ) drawPosition(27, 5, 2);	
	seek(0x5100);
	
	//Dump instruments (these are shared along all songs)
	InstEdit::copy(&VAR_INSTRUMENT, &VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT]);

	Instrument *instrument = VAR_INSTRUMENTS;
	for( i=0; i < INSTRUMENT_COUNT; i++, instrument++ ){
		write( instrument->TYPE);
		for( di=0; di < CHANNEL_COUNT; di++ ){
			write( instrument->NAME[ di ] );
		}
		for( di=0; di < SETTINGS_SIZE; di++ ){
			write( instrument->SETTINGS[ di ] );
		}
		for( di=0; di < 0x10; di++ ){
			write( instrument->TABLE.TRANSPOSE	  [ di ] );
			write( instrument->TABLE.VOLUME		  [ di ] );
			write( instrument->TABLE.COMMAND [ 0 ][ di ] );
			write( instrument->TABLE.COMMAND [ 1 ][ di ] );
			write( instrument->TABLE.VALUE   [ 0 ][ di ] );
			write( instrument->TABLE.VALUE   [ 1 ][ di ] );
		}
	}

	if( verbose ) drawPosition(27, 5, 2);	
}

void Sram::sharedDataLoad( bool verbose ){
	int i, di;

	seek(0x2700);

	// Pattern data
	for( i=0 ;i < PATTERN_COUNT; i++ ){
		for( di=0; di<0x10; di++ ){
			VAR_DATA[ i ].KEY[ di ] = read();
			VAR_DATA[ i ].INS[ di ] = read();
			VAR_DATA[ i ].VOL[ di ] = read();
			VAR_DATA[ i ].CMD[ di ] = read();
			VAR_DATA[ i ].VAL[ di ] = read();
		}
	}	
	
	//0x4E00
	if( verbose ) drawPosition(27, 5, 6);	
	seek(0x5100);
	
	//Load instruments (these are shared along all songs)
	Instrument *instrument = VAR_INSTRUMENTS;
	for( i=0; i < INSTRUMENT_COUNT; i++, instrument++ ){
		instrument->TYPE = read();
		for( di=0; di < CHANNEL_COUNT; di++ ){
			instrument->NAME[di] = read();
		}
		// Load settings bytes (also shared among different types on same instrument)
		for( di=0; di < SETTINGS_SIZE; di++ ){
			instrument->SETTINGS[di] = read();
		}
		
		// Dump instrument table
		for(di=0; di < 0x10; di++){
			/*!OPTIMIZABLE!*/
			instrument->TABLE.TRANSPOSE	[di] 	= read();
			instrument->TABLE.VOLUME	[di] 	= read();
			instrument->TABLE.COMMAND	[0][di] = read();
			instrument->TABLE.COMMAND	[1][di] = read();
			instrument->TABLE.VALUE		[0][di] = read();
			instrument->TABLE.VALUE		[1][di] = read();
		}
	}

	InstEdit::copy(&VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT], &VAR_INSTRUMENT);
	
	Sequencer::setTempo( VAR_SONG.BPM );

	if( verbose ) drawPosition(27, 5, 6);
}
