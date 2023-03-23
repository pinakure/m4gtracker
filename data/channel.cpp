#include "channel.hpp"
#include "../debug.hpp"
#include "../kernel/spu/synth.hpp"
#include "../kernel/spu/mixer.hpp"
#include "../kernel/sram/sram.hpp"
#include "../screens/tracker.hpp"
#include "../screens/songedit.hpp"
#include "../screens/live/performance.hpp"
#include "../data/song.hpp"

void Channel::init( u8 channel_index ){
	
	pattern			= &VAR_PATTERN[ channel_index ];
	song_patterns   = &VAR_SONG.PATTERNS[ channel_index ];
	cells 			= &VAR_CELLS[ channel_index ];
	
	type 			= ( ChannelType ) channel_index;
	index			= channel_index;

	mute			= false;
	solo			= false;

	// Current value
	key				= 0;
	cmd				= 0;
	inst			= 0;
	value			= 0;
	volume			= 0;
	
	CELL			= NULL;
	NEXTCELL		= NULL;
	VOLUME			= 0;
	lastpeak		= 0;
	peak 			= 0;
	PLAYING			= 0;
	
	LENGTH			= 0;
	LASTSTEP		= 0;
	STEP 			= 0;
	JUMPSTEP		= 0;	
	POSITION		= 0;
	NEXTPOSITION	= 0;
	LASTPOSITION	= 0; 
	
	
	reset			= false;
	retrig			= false;
	transpose		= 0;
	fine_tune		= 0;
	target_tick		= 0;
	tsp_position	= 0;
	tsp_loop 		= LOOP_NONE;
	vol_position	= 0;
	vol_loop 		= LOOP_NONE;

	// Bind specific functions ( Channel differs vastly from channel to channel but Im lazy enough to avoid doing a child class )
	switch( type ){
		case CHANNEL_PWM1	: trigger = &Synth::triggerPwm1	; noteOn = &Synth::noteOnPwm1	; noteOff = &Synth::noteOffPwm1	; disable = &Mixer::disablePwm1	; enable = &Mixer::enablePwm1	; break;
		case CHANNEL_PWM2	: trigger = &Synth::triggerPwm2	; noteOn = &Synth::noteOnPwm2	; noteOff = &Synth::noteOffPwm2	; disable = &Mixer::disablePwm2	; enable = &Mixer::enablePwm2	; break;
		case CHANNEL_NZE	: trigger = &Synth::triggerNze	; noteOn = &Synth::noteOnNze	; noteOff = &Synth::noteOffNze	; disable = &Mixer::disableNze	; enable = &Mixer::enableNze	; break;
		case CHANNEL_WAV	: trigger = &Synth::triggerWav	; noteOn = &Synth::noteOnWav	; noteOff = &Synth::noteOffWav	; disable = &Mixer::disableWav	; enable = &Mixer::enableWav	; break;
		case CHANNEL_FMW	: trigger = &Synth::triggerFmw	; noteOn = &Synth::noteOnFmw	; noteOff = &Synth::noteOffFmw	; disable = &Mixer::disableFmw	; enable = &Mixer::enableFmw	; break;
		case CHANNEL_SMP	: trigger = &Synth::triggerSmp	; noteOn = &Synth::noteOnSmp	; noteOff = &Synth::noteOffSmp	; disable = &Mixer::disableSmp	; enable = &Mixer::enableSmp	; break;
	}
	
	// Bind Tracker controls to each channel accordingly
	switch( type ){
		case CHANNEL_PWM1:	
			tracker_controls[ 0 ] = CHAN0_CONTROLS ;
			tracker_controls[ 1 ] = CHAN0_CONTROLS + (0x10*5);
			tracker_controls[ 2 ] = CHAN0_CONTROLS + (0x10*6);
			tracker_controls[ 3 ] = CHAN0_CONTROLS + (0x10*7);
			tracker_controls[ 4 ] = CHAN0_CONTROLS + (0x10*8);
			tracker_controls[ 5 ] = CHAN0_CONTROLS + (0x10*9);
			break;
		case CHANNEL_PWM2:	
			tracker_controls[ 0 ] = CHAN1_CONTROLS + (0x10*5);
			tracker_controls[ 1 ] = CHAN1_CONTROLS ;
			tracker_controls[ 2 ] = CHAN1_CONTROLS + (0x10*6);
			tracker_controls[ 3 ] = CHAN1_CONTROLS + (0x10*7);
			tracker_controls[ 4 ] = CHAN1_CONTROLS + (0x10*8);
			tracker_controls[ 5 ] = CHAN1_CONTROLS + (0x10*9);
			break;
		case CHANNEL_NZE:	
			tracker_controls[ 0 ] = CHAN2_CONTROLS + (0x10*5);
			tracker_controls[ 1 ] = CHAN2_CONTROLS + (0x10*6);
			tracker_controls[ 2 ] = CHAN2_CONTROLS ;
			tracker_controls[ 3 ] = CHAN2_CONTROLS + (0x10*7);
			tracker_controls[ 4 ] = CHAN2_CONTROLS + (0x10*8);
			tracker_controls[ 5 ] = CHAN2_CONTROLS + (0x10*9);
			break;
		case CHANNEL_WAV:	
			tracker_controls[ 0 ] = CHAN3_CONTROLS + (0x10*5);
			tracker_controls[ 1 ] = CHAN3_CONTROLS + (0x10*6);
			tracker_controls[ 2 ] = CHAN3_CONTROLS + (0x10*7);
			tracker_controls[ 3 ] = CHAN3_CONTROLS ;
			tracker_controls[ 4 ] = CHAN3_CONTROLS + (0x10*8);
			tracker_controls[ 5 ] = CHAN3_CONTROLS + (0x10*9);
			break;
		case CHANNEL_FMW:	
			tracker_controls[ 0 ] = CHAN4_CONTROLS + (0x10*5);
			tracker_controls[ 1 ] = CHAN4_CONTROLS + (0x10*6);
			tracker_controls[ 2 ] = CHAN4_CONTROLS + (0x10*7);
			tracker_controls[ 3 ] = CHAN4_CONTROLS + (0x10*8);
			tracker_controls[ 4 ] = CHAN4_CONTROLS ;
			tracker_controls[ 5 ] = CHAN4_CONTROLS + (0x10*9);
			break;
		case CHANNEL_SMP:
			tracker_controls[ 0 ] = CHAN5_CONTROLS + (0x10*5);
			tracker_controls[ 1 ] = CHAN5_CONTROLS + (0x10*6);
			tracker_controls[ 2 ] = CHAN5_CONTROLS + (0x10*7);
			tracker_controls[ 3 ] = CHAN5_CONTROLS + (0x10*8);
			tracker_controls[ 4 ] = CHAN5_CONTROLS + (0x10*9);
			tracker_controls[ 5 ] = CHAN5_CONTROLS ;
			break;
	}
}

void Channel::seek(){
	Sram::seek		( DATA_BASE_ADDRESS 					);// Address =  0x080
	Sram::forward 	( SONG_DETAILS_SIZE * SONG_SLOT_COUNT 	);// Address += 0x020 * 6 = 0x0C0 + 0x080 = 0x140 ( 320 )
	Sram::forward 	( GROOVE_TABLE_SIZE * SONG_SLOT_COUNT	);// Address += 0x010 * 6 = 0x060 + 0x140 = 0x1A0 ( 416 )
	Sram::forward 	( LIVE_TABLE_SIZE	* SONG_SLOT_COUNT	);// Adresss += 0x060 * 6 = 0x240 + 0x1A0 = 0x3E0 ( 992 )
	Sram::forward 	( ORDER_DATA_SIZE 	* Song::slot 		);// Adresss += 0xC00 * slot : 3072 bytes for pattern data
	Sram::next();	
}

void Channel::clearAll(){
	// Empty Order chains
	Channel *target = VAR_CHANNEL+CHANNEL_COUNT;
	for(Channel* channel = VAR_CHANNEL; channel<target; channel++){
		channel->clear();
	}
}

void Channel::readAll(){
	Channel::seek();
	
	Channel *target = VAR_CHANNEL+CHANNEL_COUNT;
	for(Channel* channel = VAR_CHANNEL; channel<target; channel++){
		channel->read();
	}
}

void Channel::writeAll(){
	Channel::seek();
	
	Channel *target = VAR_CHANNEL+CHANNEL_COUNT;
	for(Channel* channel = VAR_CHANNEL; channel<target; channel++){
		channel->write();
	}
}
	
static u8 *ord;	
static u8 *tsp;
static u8 *tgt;	
	
void Channel::read(){
	tsp = song_patterns->TRANSPOSE;
	ord = song_patterns->ORDER;
	tgt = ord+ORDER_COUNT; 
		
	while( ord < tgt ){ /* 0 - 255 */
		*ord = Sram::read();
		*tsp = Sram::read();
		ord++; tsp++;
	}
}

void Channel::write(){
	tsp = song_patterns->TRANSPOSE;
	ord = song_patterns->ORDER;
	tgt = ord+ORDER_COUNT; 
	
	while( ord < tgt ){ /* 0 - 255 */
		Sram::write( *ord );
		Sram::write( *tsp );
		ord++; tsp++;
	}
}

void Channel::clear(){
	tsp = song_patterns->TRANSPOSE;
	ord = song_patterns->ORDER;
	tgt = ord+ORDER_COUNT; 
	
	while( ord < tgt ){ /* 0 - 255 */
		*ord = 0x00;
		*tsp = 0x00;
		ord++; tsp++;
	}
	song_patterns->POSITION = 0;
}
