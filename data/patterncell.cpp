#include "patterncell.hpp"
#include "../debug.hpp"
#include "../modules/sram/sram.hpp"
#include "../screens/songedit.hpp"
#include "../screens/live/performance.hpp"

void PatternCell::init(){
	PatternCell *end = VAR_DATA + CELL_COUNT;
	u8 i=0;
	for( PatternCell *cell = VAR_DATA; cell < end; cell++){
		cell->clear();
		cell->index = i;
		i++;
	}
}

void PatternCell::seek(){
	// seek(0x2700);// SHOULD BE 0x4880!!!!
	Sram::seek	  ( DATA_BASE_ADDRESS 						);	// Address =  0x080 go past CFG area
	Sram::forward ( SONG_DETAILS_SIZE * SONG_SLOT_COUNT 	);	// Address += 0x020 * 6 = 0x00C0 + 0x080 = 0x0140 ( 320 )
	Sram::forward ( GROOVE_TABLE_SIZE * SONG_SLOT_COUNT		);	// Address += 0x010 * 6 = 0x0060 + 0x140 = 0x01A0 ( 416 )
	Sram::forward ( LIVE_TABLE_SIZE	  * SONG_SLOT_COUNT		);	// Adresss += 0x060 * 6 = 0x0240 + 0x1A0 = 0x03E0 ( 992 )
	Sram::forward ( ORDER_DATA_SIZE   * SONG_SLOT_COUNT 	);	// Adresss += 0xC00 * 6 = 0x4800 + 0x080 : 0x4880 = end of songs data
	Sram::next();	
}

void PatternCell::readAll(){
	seek();
	for( PatternCell *cell = VAR_DATA, *end = cell + CELL_COUNT; cell < end; cell++ ){
		cell->read();
	}	
}

void PatternCell::writeAll(){
	seek();
	for( PatternCell *cell = VAR_DATA, *end = cell + CELL_COUNT; cell < end; cell++ ){
		cell->write();
	}
}

void PatternCell::clearAll(){
	seek();
	for( PatternCell *cell = VAR_DATA, *end = cell + CELL_COUNT; cell < end; cell++ ){
		cell->clear();
	}
}

void PatternCell::clear(){
	static u8* p_key; 
	static u8* p_ins; 
	static u8* p_vol; 
	static u8* p_cmd; 
	static u8* p_val;
	static u8* p_end;
	
	p_key = KEY;
	p_end = p_key + PATTERN_LENGTH;
	p_ins = INS;
	p_vol = VOL;
	p_cmd = CMD;
	p_val = VAL;
	
	while(p_key < p_end){
		*p_key = 0x00;;
		*p_ins = 0x00;;
		*p_vol = 0x00;;
		*p_cmd = 0x00;;
		*p_val = 0x00;;	
		p_key++;
		p_ins++;
		p_vol++;
		p_cmd++;
		p_val++;
	}
}

void PatternCell::read(){
	static u8* p_key; 
	static u8* p_ins; 
	static u8* p_vol; 
	static u8* p_cmd; 
	static u8* p_val;
	static u8* p_end;
	
	p_key = KEY;
	p_end = p_key + PATTERN_LENGTH;
	p_ins = INS;
	p_vol = VOL;
	p_cmd = CMD;
	p_val = VAL;
	
	while(p_key < p_end){
		*p_key = Sram::read();
		*p_ins = Sram::read();
		*p_vol = Sram::read();
		*p_cmd = Sram::read();
		*p_val = Sram::read();	
		p_key++;
		p_ins++;
		p_vol++;
		p_cmd++;
		p_val++;
	}	
}

void PatternCell::write(){
	static u8* p_key; 
	static u8* p_ins; 
	static u8* p_vol; 
	static u8* p_cmd; 
	static u8* p_val;
	static u8* p_end;
	
	p_key = KEY;
	p_end = p_key + PATTERN_LENGTH;
	p_ins = INS;
	p_vol = VOL;
	p_cmd = CMD;
	p_val = VAL;
	
	while(p_key < p_end){
		Sram::write( *p_key );
		Sram::write( *p_ins );
		Sram::write( *p_vol );
		Sram::write( *p_cmd );
		Sram::write( *p_val );
		p_key++;
		p_ins++;
		p_vol++;
		p_cmd++;
		p_val++;
	}
}
