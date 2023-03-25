#include "sequencer.hpp"
#include "mixer.hpp"
#include "../net/net.hpp"
#include "../sram/sram.hpp"
#include "../../screens/songedit.hpp"
#include "../../screens/config/linkmode.hpp"
#include "../../data/channel.hpp"
#include "../../data/song.hpp"
#include "../../data/config.hpp"
#include "../../screens/tracker.hpp"

extern "C" { int CheckBPMClock(int timerTarget); }; /* Since these are ASM instructions, we have to ensure CPP does not mangle them! */

int		Sequencer::timerTarget		= 0;
int 	Sequencer::currentTicks		= 0;
int 	Sequencer::currentPattern	= 0;
int 	Sequencer::currentBeats		= 0;
int 	Sequencer::beatsPerBar		= 0;
int 	Sequencer::secPerBeat		= 0;
bool	Sequencer::playing 			= false;
	
extern Song VAR_SONG;	

void Sequencer::init( u8 bpm ){
	
	/* Setup tempo timer */
	/* Set timer 2 prescaler to F/256 and reset it */
	*((volatile u16*)0x0400010A) = 0x0;    //Disable (bit 7)
	*((volatile u16*)0x04000108) = 0x0000; //Set counter to 0
	*((volatile u16*)0x0400010A) = 0x82;   //Enable (bit 7)
	
	/* Set timer 3 prescaler to F/256 and reset it */
	*((volatile u16*)0x0400010E) = 0x0000;    //Disable (bit 7)
	*((volatile u16*)0x0400010C) = 0x0000; //Set counter to 0
	*((volatile u16*)0x0400010E) = 0x0082;   //Enable (bit 7)
	
	/* Setup time measurement vars */ 
	currentTicks 	= 0;
	currentBeats 	= 0;
	playing			= 0;
	beatsPerBar 	= 4;
	setTempo( bpm );
}

void Sequencer::setTempo(u8 bpm){
	timerTarget = (BPM_MAGIC / bpm)*10; 
	VAR_SONG.BPM = bpm;
	*((volatile u16*)0x0400010A) = 0x0000; //Disable timer (bit 7)
	*((volatile u16*)0x04000108) = 0x0000; //'reset' timer counter
	*((volatile u16*)0x0400010A) = 0x0082; //Enable (bit 7)		
}

void Sequencer::jumpToPatternAsync( int pattern_index ){
	currentTicks 	= 0;
	currentBeats 	= 0;
	currentPattern 	= pattern_index;
}

void Sequencer::play( bool from_start ){
	if( playing ) return;
	Mixer::start();
	
	currentBeats 	= 0;
	currentTicks 	= -1;
	
	if(from_start){
		currentPattern 	= 0;
		for(int i=0; i<6;i++){
			VAR_CHANNEL[i].STEP = 0;
			VAR_CHANNEL[i].POSITION = 0;
		}		
		playing = true;

		// Reset timer
		*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
		*((volatile u16*)0x04000108) = 0x0;
		*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)
		return;
	}
	
	for(int i=0; i<6;i++){
		VAR_CHANNEL[i].STEP = 255;
	}
	
	playing = true;
	// Reset timer
	*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
	*((volatile u16*)0x04000108) = 0x0;
	*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)
}

bool Sequencer::stop(){
	bool was_playing = playing;
	playing = false;
	Mixer::stop();
	if( VAR_CFG.BEHAVIOR.AUTOSAVE ) Sram::songSave(false);
	VAR_SONG.GROOVE.position = 0;	
	return was_playing;
}

bool Sequencer::updateChannel( u8 chan ){
	Channel *ch = &VAR_CHANNEL[ chan ];
	
	// If groove is enabled, update position
	if( VAR_SONG.GROOVE.ENABLE )
	ch->target_tick = VAR_SONG.GROOVE.STEP[ VAR_SONG.GROOVE.position ];
	
	// DONT UPDATE CHANNELS WHEN THEY ARE ON PATTERN 0 (this is empty)
	if( VAR_SONG.PATTERNS[chan].ORDER[ch->POSITION] == 0x00){			
		// Rewind until a pattern break (0x00) is found, or the beginning of the chain is reach
		while( ch->POSITION > 0){
		
			ch->POSITION--;
			
			if(VAR_SONG.PATTERNS[chan].ORDER[ch->POSITION] == 0x00) {
				ch->POSITION++;
				return true;
			}
		}
		// If after rewind, first pattern is still 0, stop this channel
		if(VAR_SONG.PATTERNS[chan].ORDER[0] == 0x00) return ch->PLAYING = false;
		return true;
	}
	ch->PLAYING = true;
		
	ch->LASTSTEP =  ch->STEP;
	ch->STEP++;
	
	if(ch->STEP == 16 /*ch->LENGTH*/){
		ch->STEP = 0;
		ch->LASTPOSITION = ch->POSITION;
		ch->POSITION++;
		Sequencer::currentBeats=-1;
		if(VAR_SONG.PATTERNS[chan].ORDER[ch->POSITION] == 0x00) return updateChannel(chan);
	}
	return true;
}

void Sequencer::update(){
	if( !CheckBPMClock( timerTarget ) ) return;
	
	// Restore timer, set it to exceeded time
	*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
	*((volatile u16*)0x04000108) = *((volatile u16*)0x04000108) - timerTarget; //'reset' timer counter
	*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)
	
	if( !playing ) return;
	
	//Jump to next pattern, or find previous loop entry			     
	if( !( currentTicks % ( beatsPerBar << 2 ) ) ){
		VAR_SONG.GROOVE.position = VAR_SONG.GROOVE.position < VAR_SONG.GROOVE.length ? VAR_SONG.GROOVE.position + 1:0X0;
		if( updateChannel( 0 ) ) Tracker::syncChannel( 0 ); 
		if( updateChannel( 1 ) ) Tracker::syncChannel( 1 );
		if( updateChannel( 2 ) ) Tracker::syncChannel( 2 );
		if( updateChannel( 3 ) ) Tracker::syncChannel( 3 );
		if( updateChannel( 4 ) ) Tracker::syncChannel( 4 );
		if( updateChannel( 5 ) ) Tracker::syncChannel( 5 );			
		
		currentTicks=0;
		
		// Do metronome noise if enabled
		if( Mixer::enable_metronome ) 
			Mixer::updateMetronome( currentBeats, beatsPerBar );	
		currentBeats++;
	} 	
	
	// Prepare analog sync to be written/read
	Net::clear();// WARNING: This wont allow slave mode to work...
	
	// if( !( (currentTicks*( LinkMode::sync_rate + 1)) & ((2 << LinkMode::sync_ticks )-1) ) ){
	
	if( !(currentTicks & (( ( LinkMode::sync_rate + 1) * (1 << LinkMode::sync_ticks ) )-1 ) )){
		Net::update();
	}	
	
	if( !( currentTicks	& 0x3 ) ){
	// If current tick%4 == 0, time to trigger a note on every channel
		
		// Send Analog Master Sync signal or Receive Slave sync signal
		//Net::update();
		
		Channel *channel = VAR_CHANNEL;
		for(Channel *end = channel+CHANNEL_COUNT; channel<end; channel++ ){
			
			channel->retrig = false;
			
			u8 key = *(channel->cells->KEY + channel->STEP);
			u8 ins = *(channel->cells->INS + channel->STEP);
			u8 cmd = *(channel->cells->CMD + channel->STEP);
			u8 vol = *(channel->cells->VOL + channel->STEP);
			u8 val = *(channel->cells->VAL + channel->STEP);
			
			if( key ) setKey( channel, key , vol ); 
			if( ins ) setIns( channel, ins , vol ); 
			if( cmd ) setCmd( channel, cmd , val ); 
			
			// Handle Commands
			// ...
			
			// Handle Groove Table
			//channel->target_tick = 0;
			// this causes unwanted retriggers
			if( ( currentTicks) == channel->target_tick ){
				
			//if( ( currentTicks & 0x3 ) == channel->target_tick ){
				
				// trigger sound settings
				channel->trigger( channel );
				channel->target_tick = 0;
			}
		}
	}
	
	currentTicks++;	

	/* Debug output
		Gpu::set(1,0,0, ((Sequencer::currentTicks>>2)&1) == 0?0x6F : 0x60);
		Gpu::set(1,0,1, 0x5);

		DECIMAL_DOUBLE(0,0,9, Sequencer::currentTicks>>2);
		DECIMAL_DOUBLE(2,2,1, Sequencer::currentBeats);
		DECIMAL_DOUBLE(3,2,2, Sequencer::currentPattern);
		VUMETER_V1(0,0,4, (Sequencer::currentBeats % 4) < 1);				
		WAVE_SINGLE(0,1,4, (SequencercurrentBeats % 4));				
	*/
}

void Sequencer::setKey(	Channel* channel, u8 key, u8 vol ){ 
	channel->reset  	= key > 0;
	channel->retrig 	= true;
	channel->key    	= key; 	
	channel->volume 	= vol; 
	channel->lastpeak 	= channel->peak; 
	channel->peak 		= vol;
}
void Sequencer::setIns( Channel* channel, u8 ins, u8 vol ){ 
	channel->inst   	= ins;
	if( channel->retrig ) return; 	
	channel->volume 	= vol; 
	channel->lastpeak 	= channel->peak; 
	channel->peak 		= vol;	
}	
void Sequencer::setCmd(	Channel* channel, u8 cmd, u8 val ){ 
	channel->cmd    	= cmd;	
	channel->value  	= val; 
}
