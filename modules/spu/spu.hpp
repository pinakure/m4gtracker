#ifndef _MODULES_SPU
#define _MODULES_SPU

#include "../../agb.h"
#include "../../data/variables.hpp"

#define SOUND_PWM_MASTER_RIGHT(vol)		(vol&0x7)
#define SOUND_PWM_MASTER_LEFT(vol)		((vol&0x7)<<4)
#define SOUND_PWM_ENABLE_RIGHT(index)	((1<<8)<<index)
#define SOUND_PWM_ENABLE_LEFT(index)	((1<<12)<<index)
#define SOUND_PWM_ENABLE_PSG_FIFO		0x0080

#define BPM_MAGIC 6271			//100BPM = 6271Hz per beat	

class SPU{
	public:
		
		static int	timer_target;
		static int 	current_pattern; //NOT USED! 
		static int 	current_ticks;
		static int 	target_tick[6];
		static int 	current_beats;
		static int 	beats_per_bar;
		static int 	sec_per_beat;
		static bool retrig_note[6];
		static bool enable_metronome;	
		
		static bool playing;
		
		static void mute( int channel );
		static void solo( int channel );
	
		static void init( int bpm );
		static void setTempo( int bpm );
		static void play( bool fromStart );	
		static void stop();	
		
		static void noteOnPWM1();
		static void noteOnPWM2();
		static void noteOnWAV();
		static void noteOnNZE();
		
		
		static bool setKey(Channel*, u8 key , u8 vol );
		static void setCmd(Channel*, u8 cmd , u8 val );
		static void setIns(Channel*, u8 ins , u8 vol , bool retrig_note );
		//static void setVal(Channel*, u8 val , u8 vol );
		//static void setVol(Channel*, u8 vol );
		static void triggerChannel(Channel *chan, int channel_index );

		static void update();				// Called each program cycle		
		static void enable(int channel);	//Enable given sound channel
		static void disable();				//Disable general sound system
		
		static void jumpToPatternAsync(int p);	//jumps asyncronously to pattern p
};

#endif
