#ifndef MIXER_HPP
#define MIXER_HPP

#include "../../agb.h"

#include "../../data/control.hpp"
#include "../../data/channel.hpp"
#include "../regionhandler/regionhandler.hpp"
#include "../../data/settings.hpp"


#define TIMER0			(*(u32*)0x4000100)
#define TIMER0_LO		(*(u16*)0x4000100)
#define TIMER0_HI		(*(u16*)0x4000102)
#define TIMER1			(*(u32*)0x4000104)
#define TIMER1_LO		(*(u16*)0x4000104)
#define TIMER1_HI		(*(u16*)0x4000106)
#define SOUND_EX 		(*(vu16*)0x4000084)	//SOUNDCNT_X
#define SOUND_HI		(*(vu16*)0x4000082) //SOUNDCNT_H
#define DMA1_SRC		(*(u32*)0x40000BC) //SOUNDCNT_H
#define DMA1_DST		(*(u32*)0x40000C0) //SOUNDCNT_H
#define DMA1_CHI		(*(vu16*)0x40000C6) //SOUNDCNT_H
#define DMA1_CLO		(*(vu16*)0x4000082) //SOUNDCNT_H


#define VIRTUAL_CHANNEL_COUNT 	2

//#define SMP_BUFFER_SIZE			304
#define SMP_BUFFER_SIZE			96
typedef struct soundBuffer {
   s8 *mixBufferBase;
   s8 *curMixBuffer;
   size_t mixBufferSize;
   u8 activeBuffer;

} SoundBuffer ;
extern SoundBuffer sound_buffer;

typedef struct _soundChannel {
   s8*	data;   // pointer to the raw sound data in ROM
   u32 	pos;    // current position in the data
   u32 	length; // length of the whole sound
   u32 	inc;
   u32 	vol;
} SoundChannel;
// extern SoundChannel channel[ VIRTUAL_CHANNEL_COUNT ];

class Mixer {
	private:
		static void enablePwm1();
		static void enablePwm2();
		static void enableWav();
		static void enableNze();
		static void enableFmw();
		static void enableSmp();
		
		static void disablePwm1();
		static void disablePwm2();
		static void disableWav();
		static void disableNze();
		static void disableFmw();
		static void disableSmp();

		static SoundChannel dsound		[ VIRTUAL_CHANNEL_COUNT ];
		static bool 		dsound_mute	[ VIRTUAL_CHANNEL_COUNT];
	public:
		friend class Channel;
		friend class Adsr;
		
		static bool enable_metronome;
		static bool enabled;
		
		static int last_level[ CHANNEL_COUNT ];
		static int level;
		static int key_note[ CHANNEL_COUNT ];
		static int new_note[ CHANNEL_COUNT ];

		static void init();
		static void mix();
		static void load( SETTINGS_SMP *smp, int chan);
		
		static void start();
		
		static void updateMetronome( u8 time, u8 beats_per_bar );
		
		static void stop();
		
		static void noteOn1	(u32 freq);// move to seq or synth
		static void noteOn2	(u32 freq);// move to seq or synth

		static void mute	(u8 channel_index);// move to mixer
		static void solo	(u8 channel_index);// move to mixer
		
		// These are interacted through Cfg Screen
		static void show 	( Control* c, bool bigstep, bool add, u32* pointer );
		static void update  (  );
};

extern const u32 DSOUND_FREQ_TABLE[];

#endif
