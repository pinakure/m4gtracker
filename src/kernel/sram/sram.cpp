#include "sram.hpp" 

Sram SRAM;

#include "../../screens/live.hpp"
#include "../../data/pattern.hpp"
#include "../../screens/instedit.hpp"
#include "../../screens/patedit.hpp"
#include "../../screens/songedit.hpp"
#include "../../screens/tracker.hpp"
#include "../key/key.hpp"
#include "../../debug.hpp"

u8* 	Sram::sram;
int 	Sram::position;
u16 	Sram::waitstateBackup;
bool 	Sram::verbosity;
u8 	 	Sram::stage;
u16  	Sram::size[10];
u16  	Sram::base[10];


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
	if(!ReallyDialog::result) SWI(26);
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
	for(i=0; i< SRAM_SIZE; i++){
		write(0x00);
	}	
}

void Sram::prepare(){
	stage=0;
	base[0] = position;
	size[0] = position;
}

void Sram::next(){
	stage++;
	base[stage]=position;
}

void Sram::drawPosition(u8 x, u8 y, u8 color) { 
	u8 s = (stage%10);
	size[ s ] = position;
	
	if( (stage%10) < 2)return;
	
	HEXADECIMAL_DOUBLE(x+1-3,	y-2,	COLOR_DARK_CYAN, position &0xFF);
	HEXADECIMAL_DOUBLE(x  -3,	y-2,	COLOR_DARK_CYAN, position >>8);
	
	HEXADECIMAL_DOUBLE(x+1	,	y-2,	color, (size[ s ] - base[ s ] )&0xFF);
	HEXADECIMAL_DOUBLE(x	,	y-2,	color, (size[ s ] - base[ s ] )>>8);
}

void Sram::write(u8 a){ 
	SRAM_WriteByte(position++, a);
	// Draw position (if saving on song screen)
	if(Sram::verbosity) drawPosition(28, 1+stage, 3);
	// Draw saving icon
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
	// Draw reading icon
	if((position & 0xFF)>0x80)Gpu::set( 2,29, 0, 0x100);
	else Gpu::set( 2,29, 0, (COLOR_GREEN << 12) | 0x3D);
	u8 r = SRAM_ReadByte(position++);
	// Draw position (if loading on song screen)
	if(Sram::verbosity) drawPosition(28, 1+stage, COLOR_DARK_GREEN);
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
	Sram::verbosity = verbose;
	Sram::prepare();
	// Write song details 
	VAR_SONG.write();	
	// Write Groove table data
	VAR_SONG.GROOVE.write();
	// Write live table 
	Performance::write();
	// Write chain data (order & transpose)
	Channel::writeAll();	
	// (Re)Write common data ( Pattern data & Instruments )
	sharedDataSave( verbose );
}

void Sram::songLoad( bool verbose ){
	Sram::verbosity = verbose;
	Sram::prepare();
	// Read song details 
	VAR_SONG.read();		
	// Read Groove table data
	VAR_SONG.GROOVE.read();
	// Read live table 
	Performance::read();
	// Read chain data (order & transpose)
	Channel::readAll();
	// (Re)Load common data (Pattern data & Instruments )
	sharedDataLoad( verbose );
	
	// Call redraw functions before returning ( TODO TO BE REVIEWED : GFX GLITCHES UPON THIS POINT )
	PatEdit::syncPosition( VAR_CFG.ORDERPOSITION );
	Tracker::syncPattern();
	// Update controls to show recently loaded data
	for(int i=0; i<CHANNEL_COUNT; i++){
		Tracker::syncChannel( VAR_CHANNEL + i );
	}
	// No need to redraw here
	PatEdit::sync(verbose);
}

void Sram::songDefaults( bool verbose ){
	Sram::verbosity = verbose;
	Sram::prepare();
	
	// Clear Song details
	VAR_SONG.clear();	
	// Pattern data clear
	PatternCell::clearAll();
	// Clear instrument pointer. I guess we should clear all Instruments instead...
	VAR_INSTRUMENT.clear();	
	// Live Perform table clear
	Performance::clear();
	// Clear all chain data (order & transpose
	Channel::clearAll();	
}

void Sram::sharedDataSave( bool verbose ){
	// Dump pattern data
	PatternCell::writeAll();
	//Dump instruments (these are shared along all songs)
	Instrument::writeAll();
}

void Sram::sharedDataLoad( bool verbose ){	
	// Load Pattern data
	PatternCell::readAll();
	//Load instruments (these are shared along all songs)
	Instrument::readAll();
	
	// Reload tempo
	Sequencer::setTempo( VAR_SONG.BPM );
}
