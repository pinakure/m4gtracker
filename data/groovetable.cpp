#include "groovetable.hpp"
#include "../kernel/sram/sram.hpp"
#include "../screens/songedit.hpp"

static u8 iterator;

void GrooveTable::clear(){
}

void GrooveTable::seek(){
	//0x140
	Sram::seek		( DATA_BASE_ADDRESS 					);
	Sram::forward 	( SONG_DETAILS_SIZE * SONG_SLOT_COUNT 	);
	Sram::forward 	( GROOVE_TABLE_SIZE * Song::slot 		);
	Sram::next();	
}

void GrooveTable::write(){
	seek();
	// Write Groove Steps
	for(iterator=0; iterator < 0x10; iterator++){
		Sram::write( STEP[ iterator ] );
	}
	writeNibbles( length, ENABLE );
}

void GrooveTable::read(){
	seek();
	// Load groove table
	for( iterator=0; iterator < 0x10; iterator++){
		readByte( STEP[ iterator ] );
	}
	readNibbles	( length, ENABLE );
}
