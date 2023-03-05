#include "sequencer.hpp"
#include "mixer.hpp"
#include "../net/net.hpp"
#include "../../data/channel.hpp"
#include "../../data/song.hpp"
#include "../../data/helpers.hpp"

extern "C" { int CheckBPMClock(int timerTarget); }; /* Since these are ASM instructions, we have to ensure CPP does not mangle them! */

int		Sequencer::timerTarget		= 0;
int 	Sequencer::currentTicks		= 0;
int 	Sequencer::currentPattern	= 0;
int 	Sequencer::currentBeats		= 0;
int 	Sequencer::beatsPerBar		= 0;
int 	Sequencer::secPerBeat		= 0;
bool	Sequencer::playing 			= false;
	
extern Song VAR_SONG;	
extern void cellSyncChannel(u8 c);

void Sequencer::init( int bpm ){
	
	/* Setup tempo timer */
	/* Set timer 2 prescaler to F/256 and reset it */
	*((volatile u16*)0x0400010A) = 0x0;    //Disable (bit 7)
	*((volatile u16*)0x04000108) = 0x0000; //Set counter to 0
	*((volatile u16*)0x0400010A) = 0x82;   //Enable (bit 7)
	
	/* Set timer 3 prescaler to F/256 and reset it */
	*((volatile u16*)0x0400010E) = 0x0;    //Disable (bit 7)
	*((volatile u16*)0x0400010C) = 0x0000; //Set counter to 0
	*((volatile u16*)0x0400010E) = 0x82;   //Enable (bit 7)
	
	/* Setup time measurement vars */ 
	currentTicks 	= 0;
	currentBeats 	= 0;
	playing			= 0;
	beatsPerBar 	= 4;
	setTempo( bpm );
}

void Sequencer::setTempo(int bpm){
	timerTarget = (BPM_MAGIC / bpm)*10; 
	VAR_SONG.BPM = bpm;
	*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
	*((volatile u16*)0x04000108) = 0x0000; //'reset' timer counter
	*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)		
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
	//if( VAR_CFG.BEHAVIOR.AUTOSAVE) SRAM.songSave();
	return was_playing;
}

bool Sequencer::updateChannel( u8 chan ){
	// DONT UPDATE CHANNELS WHEN THEY ARE ON PATTERN 0 (this is empty)
	if(VAR_SONG.PATTERNS[chan].ORDER[VAR_CHANNEL[chan].POSITION] == 0x00){			
		// Rewind until a pattern break (0x00) is found, or the beginning of the chain is reach
		while(VAR_CHANNEL[chan].POSITION > 0){
		
			VAR_CHANNEL[chan].POSITION--;
			
			if(VAR_SONG.PATTERNS[chan].ORDER[VAR_CHANNEL[chan].POSITION] == 0x00) {
				VAR_CHANNEL[chan].POSITION++;
				return true;
			}
		}
		// If after rewind, first pattern is still 0, stop this channel
		if(VAR_SONG.PATTERNS[chan].ORDER[0] == 0x00) return VAR_CHANNEL[chan].PLAYING = false;
		return true;
	}
	VAR_CHANNEL[chan].PLAYING = true;
		
	VAR_CHANNEL[chan].LASTSTEP =  VAR_CHANNEL[chan].STEP;
	VAR_CHANNEL[chan].STEP++;
	
	if(VAR_CHANNEL[chan].STEP == 16 /*VAR_CHANNEL[chan].LENGTH*/){
		VAR_CHANNEL[chan].STEP = 0;
		VAR_CHANNEL[chan].LASTPOSITION = VAR_CHANNEL[chan].POSITION;
		VAR_CHANNEL[chan].POSITION++;
		Sequencer::currentBeats=-1;
		if(VAR_SONG.PATTERNS[chan].ORDER[VAR_CHANNEL[chan].POSITION] == 0x00) return updateChannel(chan);
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
		if( updateChannel( 0 ) ) cellSyncChannel( 0 ); 
		if( updateChannel( 1 ) ) cellSyncChannel( 1 );
		if( updateChannel( 2 ) ) cellSyncChannel( 2 );
		if( updateChannel( 3 ) ) cellSyncChannel( 3 );
		if( updateChannel( 4 ) ) cellSyncChannel( 4 );
		if( updateChannel( 5 ) ) cellSyncChannel( 5 );			
		
		currentTicks=0;
		
		// Do metronome noise if enabled
		if( Mixer::enable_metronome ) 
			Mixer::updateMetronome( currentBeats, beatsPerBar );	
		currentBeats++;
	} 	
	
	// Prepare analog sync to be written/read
	Net::clear();// WARNING: This wont allow slave mode to work...
	
	// If current tick%4 == 0, time to trigger a note on every channel
	if( !( currentTicks & 0x3 ) ){
		
		// Send Analog Master Sync signal or Receive Slave suync signal
		Net::update();
		
		for(int i=0, s = 0; i<6; i++){
			Channel *channel = &VAR_CHANNEL[ i ];
			
			channel->retrig = false;
			
			s = channel->STEP;
			if( VAR_CELLS[ i ].KEY[ s ] > 0x0) setKey( channel, VAR_CELLS[i].KEY[s] , VAR_CELLS[i].VOL[s] ); 
			if( VAR_CELLS[ i ].INS[ s ] > 0x0) setIns( channel, VAR_CELLS[i].INS[s] , VAR_CELLS[i].VOL[s] ); 
			if( VAR_CELLS[ i ].CMD[ s ] > 0x0) setCmd( channel, VAR_CELLS[i].CMD[s] , VAR_CELLS[i].VAL[s] ); 
			
			// Handle Commands
			// ...
			
			// Handle Groove Table
			channel->target_tick = 0;
			
			if( ( currentTicks & 0x3 ) == channel->target_tick ){
				// trigger sound settings
				channel->trigger( channel );
				channel->target_tick = 0;
			}
		}
	}
	
	currentTicks++;		

	/* Debug output
		gpu.set(1,0,0, ((Sequencer::currentTicks>>2)&1) == 0?0x6F : 0x60);
		gpu.set(1,0,1, 0x5);

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
