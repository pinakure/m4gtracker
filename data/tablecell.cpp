#include "tablecell.hpp"
#include "../modules/sram/sram.hpp"

static u8 iterator;

void TableCell::write(){
	// Part of instrument, do not need seek call here
	for( iterator = 0; iterator < PATTERN_LENGTH; iterator++ ){
		Sram::write( TRANSPOSE	    [ iterator ] );
		Sram::write( VOLUME		    [ iterator ] );
		Sram::write( COMMAND   [ 0 ][ iterator ] );
		Sram::write( COMMAND   [ 1 ][ iterator ] );
		Sram::write( VALUE     [ 0 ][ iterator ] );
		Sram::write( VALUE     [ 1 ][ iterator ] );
	}
}

void TableCell::read(){
	// Part of instrument, do not need seek call here
	for( iterator = 0; iterator < PATTERN_LENGTH; iterator++ ){
		TRANSPOSE	[ iterator ] 	  = Sram::read();
		VOLUME		[ iterator ] 	  = Sram::read();
		COMMAND		[ 0 ][ iterator ] = Sram::read();
		COMMAND		[ 1 ][ iterator ] = Sram::read();
		VALUE		[ 0 ][ iterator ] = Sram::read();
		VALUE		[ 1 ][ iterator ] = Sram::read();
	}
}
