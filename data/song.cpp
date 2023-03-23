#include "song.hpp"
#include "../kernel/sram/sram.hpp"
#include "../screens/songedit.hpp"

MEM_IN_EWRAM u8 Song::slot;

static u8 iterator;

void Song::seek(){
	Sram::seek		( DATA_BASE_ADDRESS 				);
	Sram::forward	( SONG_DETAILS_SIZE * Song::slot	);	
	Sram::next();
}

void Song::write(){
	seek();
	// Write Song details
	Sram::write		( TRANSPOSE );
	Sram::write		( BPM );
	
	// Write Artist and title
	for( iterator = 0 ; iterator < SONG_STRING_SIZE ; iterator++ ){
		Sram::write	( TITLE	[ iterator ] );
		Sram::write	( ARTIST[ iterator ] );
	} 
}

void Song::read(){
	seek();
	// Read Sond details
	readByte	( TRANSPOSE );					
	readByte	( BPM 	  	);
		
	// Read Artist and title
	for( iterator=0; iterator<SONG_STRING_SIZE; iterator++ ){
		TITLE	[ iterator ] = Sram::read();
		ARTIST	[ iterator ] = Sram::read();
	} 
}

void Song::clear(){
	
	TRANSPOSE 			= 0x00;
	BPM 				= 0x80;
	Sequencer::setTempo	(0x80);
		
	for( iterator=0; iterator<SONG_STRING_SIZE; iterator++ ){
		TITLE [ iterator ] = 0xFF;
		ARTIST[ iterator ] = 0xFF;
	}
}
