#ifndef _MODULES_SPU
#define _MODULES_SPU

#include "../../agb.h"

#define BPM_MAGIC 6271			//100BPM = 6271Hz per beat	
class SPU{
	public:
		
		static int	timerTarget;
		static int currentPattern; //NOT USED! 
		static int currentTicks;
		static int targetTick[6];
		static int currentBeats;
		static int beatsPerBar;
		static int secPerBeat;
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
		
		
		static void setKey(u8 channel, u8 key		);
		static void setCmd(u8 channel, u8 cmd		);
		static void setInst(u8 channel, u8 inst		);
		static void setValue(u8 channel, u8 value	);
		static void setVolume(u8 channel, u8 volume );
		static void triggerChannel(int channelIndex );

		static void update();				// Called each program cycle		
		static void enable(int channel);	//Enable given sound channel
		static void disable();				//Disable general sound system
		
		static void jumpToPatternAsync(int p);	//jumps asyncronously to pattern p
};

#endif
