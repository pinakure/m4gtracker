#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include "../../agb.h"
#include "../../data/channel.hpp"

#define BPM_MAGIC 6271			//100BPM = 6271Hz per beat	

class Sequencer {
	private:
		static int	timerTarget;
		static int  currentPattern; //NOT USED! 
		static int  currentTicks;
		static int  beatsPerBar;
		static int  secPerBeat;
		
	public:
		static int  currentBeats;
		static bool playing;		
		static void init( int bpm );
		static void setTempo( int bpm );
		
 inline static void setCmd(	Channel* channel, u8 cmd, u8 val );
 inline static void setIns( Channel* channel, u8 ins, u8 vol );
 inline static void setKey(	Channel* channel, u8 key, u8 vol ); 

		static void update();
		static bool updateChannel( u8 index );

		static void play( bool from_start );
		static void jumpToPatternAsync( int pattern_index );
		static bool stop();
};

#endif
