#ifndef SYNTH_HPP
#define SYNTH_HPP

#include "../../agb.h"
#include "../../data/settings.hpp"
#include "../../data/channel.hpp"

#include "adsr.hpp"

class Synth {
	private:
		static void triggerPwm1( Channel *channel );
		static void triggerPwm2( Channel *channel );
		static void triggerNze ( Channel *channel );
		static void triggerWav ( Channel *channel );
		static void triggerFmw ( Channel *channel );
		static void triggerSmp ( Channel *channel );
	
		static void noteOnPwm1( Channel* channel );
		static void noteOnPwm2( Channel* channel );
		static void noteOnNze ( Channel* channel );
		static void noteOnWav ( Channel* channel );
		static void noteOnFmw ( Channel* channel );
		static void noteOnSmp ( Channel* channel );
	
		static void noteOffPwm1( Channel* channel );
		static void noteOffPwm2( Channel* channel );
		static void noteOffNze ( Channel* channel );
		static void noteOffWav ( Channel* channel );
		static void noteOffFmw ( Channel* channel );
		static void noteOffSmp ( Channel* channel );
	
	public:
		friend class Channel;
		static u16 	lfo;
		
		static void init();
		
		
		static void loadWav( u8 data[ 16 ] );
		static void loadFmw( u8 data[ 16 ] , u8 mult );
		static void loadSmp( u8 data[ 16 ] );
		
		static void renderWav( SETTINGS_WAV *settings, u8 vol );
		static void renderFmw( SETTINGS_FMW *settings, u8 vol );
		static void renderSmp( SETTINGS_SMP *settings, u8 vol );

		static void polysynth( u16 note );
		
};

#endif
