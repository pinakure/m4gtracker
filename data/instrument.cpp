#include "instrument.hpp"
#include "../debug.hpp"
#include "../modules/sram/sram.hpp"
#include "../screens/songedit.hpp"
#include "../screens/live/performance.hpp"

static u8 iterator;

void Instrument::init(){
	Instrument *target = VAR_INSTRUMENTS + INSTRUMENT_COUNT;
	iterator = 0;
	for( Instrument *instrument = VAR_INSTRUMENTS; instrument < target; instrument++){
		instrument->index = iterator;
		iterator++;
	}
}

void Instrument::seek(){
	// seek(0x5100);// SHOULD BE 0x7080!!!!
	Sram::seek		( DATA_BASE_ADDRESS 					); // Address =  0x080
	Sram::forward 	( SONG_DETAILS_SIZE * SONG_SLOT_COUNT 	); // Address += 0x020 * 6 = 0x00C0 + 0x080 = 0x0140 ( 320 )
	Sram::forward 	( GROOVE_TABLE_SIZE * SONG_SLOT_COUNT	); // Address += 0x010 * 6 = 0x0060 + 0x140 = 0x01A0 ( 416 )
	Sram::forward 	( LIVE_TABLE_SIZE	* SONG_SLOT_COUNT	); // Adresss += 0x060 * 6 = 0x0240 + 0x1A0 = 0x03E0 ( 992 )
	Sram::forward 	( ORDER_DATA_SIZE   * SONG_SLOT_COUNT 	); // Adresss += 0xC00 * 6 = 0x4800 + 0x080 : 0x4880 = end of songs data
	Sram::forward 	( PATTERN_DATA_SIZE 					); // Adresss += 0x5 * 0x10 * 0x80 (0x2800) + 0x4880 : 0x7080 = end of shared pattern data
	Sram::next();	
}

void Instrument::readAll(){
	seek();
	for( Instrument *instrument = VAR_INSTRUMENTS, *end = instrument + INSTRUMENT_COUNT; instrument < end; instrument++ ){
		instrument->read();
	}	
	// Load first instrumnt
	InstEdit::copy(&VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT], &VAR_INSTRUMENT);
}

void Instrument::writeAll(){
	// Write unsaved settings to memory
	InstEdit::copy(&VAR_INSTRUMENT, &VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT]);
	
	seek();
	for( Instrument *instrument = VAR_INSTRUMENTS, *end = instrument + INSTRUMENT_COUNT; instrument < end; instrument++ ){
		instrument->write();
	}
}

void Instrument::clearAll(){
	for( Instrument *instrument = VAR_INSTRUMENTS, *end = instrument + INSTRUMENT_COUNT; instrument < end; instrument++ ){
		instrument->clear();
	}
}

void readPackedString( const char *output, u8 string_length, u8 char_bit_size ){
	
}
void writePackedString( const char *output, u8 string_length, u8 char_bit_size ){
	int total_bitsize = (string_length * char_bit_size) / 8;
}

void Instrument::read(){
	TYPE = Sram::read();
	for( iterator=0; iterator < INSTRUMENT_NAME_SIZE; iterator++ ){
		NAME[ iterator ] = Sram::read();		
	}
	for( iterator=0; iterator < SETTINGS_SIZE; iterator++ ){
		SETTINGS[ iterator ] = Sram::read();
	}
	TABLE.read();
}

void Instrument::write(){
	Sram::write( TYPE );
	for( iterator=0; iterator < INSTRUMENT_NAME_SIZE; iterator++ ){
		Sram::write( NAME[ iterator ] );
	}
	for( iterator=0; iterator < SETTINGS_SIZE; iterator++ ){
		Sram::write( SETTINGS[ iterator ] );
	}
	TABLE.write();
}

void Instrument::clear(){
	for( iterator=0; iterator < INSTRUMENT_NAME_SIZE ; iterator++ ){
		NAME[ iterator ] = 0xFF;
	}
}
