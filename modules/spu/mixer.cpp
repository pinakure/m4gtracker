#include "mixer.hpp"
#include "../../data/channel.hpp"
#include "../../debug.hpp"
#include "../../data/settings.hpp"

const u16 DSOUND_FREQ_TABLE[] = {
		// Finetune 0
	 2090,  2215,  2348,  2485,  2639,  2796,  2963,  3139,  3326,  3523,  3728,  3950, 	// C0-B0
	 4181,  4430,  4697,  4971,  5279,  5593,  5926,  6279,  6653,  7046,  7457,  7901, 	// C1-B1
	 8363,  8860,  9395,  9943, 10559, 11186, 11852, 12559, 13306, 14092, 14914, 15838, 	// C2-B2
	16726, 17720, 18839, 19886, 21180, 22372, 23705, 25208, 26713, 28185, 29829, 31677, 	// C3-B3
	33453, 35441, 37679, 39772, 42613, 44744, 47727, 50416, 53426, 56818, 59659, 63920, 	// C4-B4
		// Finetune 1
	 2105,  2231,  2364,  2503,  2655,  2809,  2977,  3156,  3345,  3544,  3752,  3977, 	// C0-B0
	 4211,  4463,  4728,  5006,  5310,  5619,  5955,  6313,  6690,  7088,  7504,  7954, 	// C1-B1
	 8422,  8926,  9469, 10026, 10621, 11256, 11931, 12648, 13406, 14204, 15040, 15909, 	// C2-B2
	16884, 17897, 18939, 20109, 21306, 22512, 23863, 25386, 26913, 28409, 30080, 31960, 	// C3-B3
	33769, 35795, 38080, 40219, 42613, 45310, 47727, 51136, 54235, 56818, 60670, 63920, 	// C4-B4
		// Finetune 2
	 2120,  2248,  2380,  2524,  2671,  2831,  2997,  3178,  3364,  3565,  3775,  4003, 	// C0-B0
	 4241,  4496,  4760,  5048,  5342,  5663,  5995,  6357,  6728,  7130,  7551,  8007, 	// C1-B1
	 8482,  8993,  9520, 10111, 10685, 11327, 12011, 12738, 13456, 14261, 15103, 16051, 	// C2-B2
	16964, 17987, 19040, 20223, 21434, 22655, 24023, 25568, 26913, 28636, 30335, 32248, 	// C3-B3
	34090, 36157, 38080, 40676, 43127, 45310, 48372, 51136, 54235, 57734, 60670, 65082, 	// C4-B4
		// Finetune 3
	 2135,  2262,  2399,  2542,  2691,  2849,  3023,  3201,  3389,  3593,  3808,  4031, 	// C0-B0
	 4271,  4525,  4798,  5084,  5382,  5699,  6046,  6403,  6779,  7187,  7616,  8062, 	// C1-B1
	 8543,  9062,  9596, 10169, 10781, 11399, 12093, 12829, 13558, 14375, 15232, 16124, 	// C2-B2
	17127, 18170, 19244, 20338, 21563, 22799, 24186, 25752, 27117, 28867, 30594, 32248, 	// C3-B3
	34418, 36525, 38489, 40676, 43127, 45891, 48372, 51877, 54235, 57734, 61716, 65082, 	// C4-B4
		// Finetune 4
	 2151,  2279,  2415,  2560,  2711,  2872,  3043,  3224,  3415,  3615,  3832,  4058, 	// C0-B0
	 4302,  4559,  4830,  5120,  5423,  5745,  6087,  6449,  6831,  7231,  7664,  8116, 	// C1-B1
	 8604,  9131,  9674, 10256, 10847, 11509, 12175, 12922, 13662, 14492, 15362, 16270, 	// C2-B2
	17209, 18262, 19348, 20572, 21694, 23093, 24350, 25938, 27324, 29101, 30858, 32541, 	// C3-B3
	34418, 36525, 38908, 41144, 43652, 46487, 49034, 51877, 55069, 58681, 61716, 65082, 	// C4-B4
		// Finetune 5
	 2166,  2297,  2431,  2578,  2732,  2891,  3064,  3248,  3441,  3645,  3865,  4095, 	// C0-B0
	 4333,  4595,  4863,  5157,  5464,  5782,  6129,  6496,  6883,  7290,  7731,  8191, 	// C1-B1
	 8667,  9201,  9727, 10315, 10946, 11584, 12258, 13016, 13767, 14610, 15495, 16419, 	// C2-B2
	17376, 18451, 19454, 20691, 21960, 23243, 24517, 26128, 27534, 29340, 31126, 32839, 	// C3-B3
	34752, 36902, 38908, 41622, 44191, 46487, 49034, 52640, 55069, 58681, 62799, 65535, 	// C4-B4
		// Finetune 6
	 2182,  2312,  2451,  2597,  2749,  2914,  3085,  3271,  3468,  3675,  3890,  4123, 	// C0-B0
	 4365,  4624,  4903,  5195,  5498,  5829,  6171,  6543,  6937,  7350,  7781,  8247, 	// C1-B1
	 8730,  9249,  9806, 10405, 11013, 11659, 12343, 13111, 13874, 14730, 15563, 16495, 	// C2-B2
	17461, 18546, 19667, 20811, 22095, 23395, 24686, 26320, 27748, 29583, 31126, 33143, 	// C3-B3
	35093, 37286, 39335, 41622, 44191, 47099, 49715, 52640, 55930, 59659, 62799, 65535, 	// C4-B4
		// Finetune 7
	 2198,  2330,  2468,  2616,  2770,  2934,  3112,  3296,  3488,  3697,  3916,  4152, 	// C0-B0
	 4397,  4660,  4937,  5233,  5541,  5868,  6225,  6592,  6977,  7395,  7832,  8305, 	// C1-B1
	 8794,  9321,  9888, 10466, 11082, 11736, 12472, 13208, 13982, 14791, 15699, 16649, 	// C2-B2
	17633, 18643, 19776, 20933, 22233, 23549, 25031, 26515, 27965, 29583, 31399, 33453, 	// C3-B3
	35441, 37286, 39772, 42112, 44744, 47099, 50416, 53426, 55930, 59659, 62799, 65535, 	// C4-B4
		// Finetune 8
	 1973,  2090,  2215,  2348,  2485,  2639,  2796,  2963,  3139,  3326,  3523,  3728, 	// C0-B0
	 3946,  4181,  4430,  4697,  4971,  5279,  5593,  5926,  6279,  6653,  7046,  7457, 	// C1-B1
	 7901,  8363,  8860,  9395,  9943, 10559, 11186, 11852, 12559, 13306, 14092, 14914, 	// C2-B2
	15838, 16726, 17720, 18839, 19886, 21180, 22372, 23705, 25208, 26713, 28185, 29829, 	// C3-B3
	31677, 33453, 35441, 37679, 39772, 42613, 44744, 47727, 50416, 53426, 56818, 59659, 	// C4-B4
		// Finetune 9
	 1988,  2105,  2231,  2364,  2503,  2651,  2814,  2977,  3156,  3345,  3544,  3752, 	// C0-B0
	 3977,  4211,  4463,  4728,  5006,  5303,  5628,  5955,  6313,  6690,  7088,  7504, 	// C1-B1
	 7954,  8422,  8926,  9469, 10026, 10621, 11256, 11931, 12648, 13406, 14204, 15040, 	// C2-B2
	15909, 16884, 17897, 18939, 20109, 21306, 22512, 23863, 25386, 26913, 28409, 30080, 	// C3-B3
	31960, 33769, 35795, 38080, 40219, 42613, 45310, 47727, 51136, 54235, 56818, 60670, 	// C4-B4
		// Finetune 10
	 2001,  2120,  2248,  2380,  2524,  2671,  2831,  2997,  3178,  3364,  3565,  3775, 	// C0-B0
	 4003,  4241,  4496,  4760,  5048,  5342,  5663,  5995,  6357,  6728,  7130,  7551, 	// C1-B1
	 8007,  8482,  8993,  9520, 10111, 10685, 11327, 12011, 12738, 13456, 14261, 15103, 	// C2-B2
	16051, 16964, 17987, 19040, 20223, 21434, 22655, 24023, 25568, 26913, 28636, 30335, 	// C3-B3
	32248, 34090, 36157, 38080, 40676, 43127, 45310, 48372, 51136, 54235, 57734, 60670, 	// C4-B4
		// Finetune 11
	 2017,  2135,  2262,  2399,  2542,  2691,  2849,  3023,  3201,  3389,  3593,  3808, 	// C0-B0
	 4035,  4271,  4525,  4798,  5084,  5382,  5699,  6046,  6403,  6779,  7187,  7616, 	// C1-B1
	 8080,  8543,  9062,  9596, 10169, 10781, 11399, 12093, 12829, 13558, 14375, 15232, 	// C2-B2
	16197, 17127, 18170, 19244, 20338, 21563, 22799, 24186, 25752, 27117, 28867, 30594, 	// C3-B3
	32541, 34418, 36525, 38489, 40676, 43127, 45891, 48372, 51877, 54235, 57734, 61716, 	// C4-B4
		// Finetune 12
	 2031,  2151,  2279,  2415,  2560,  2711,  2872,  3043,  3224,  3415,  3623,  3832, 	// C0-B0
	 4063,  4302,  4559,  4830,  5120,  5423,  5745,  6087,  6449,  6831,  7246,  7664, 	// C1-B1
	 8135,  8604,  9131,  9674, 10256, 10847, 11509, 12175, 12922, 13662, 14492, 15362, 	// C2-B2
	16270, 17209, 18262, 19348, 20572, 21694, 23093, 24350, 25938, 27324, 29101, 30858, 	// C3-B3
	32541, 34418, 36525, 38908, 41144, 43652, 46487, 49034, 51877, 55069, 58681, 61716, 	// C4-B4
		// Finetune 13
	 2045,  2166,  2297,  2431,  2578,  2732,  2891,  3064,  3248,  3441,  3645,  3865, 	// C0-B0
	 4090,  4333,  4595,  4863,  5157,  5464,  5782,  6129,  6496,  6883,  7290,  7731, 	// C1-B1
	 8191,  8667,  9201,  9727, 10315, 10946, 11584, 12258, 13016, 13767, 14610, 15495, 	// C2-B2
	16419, 17376, 18451, 19454, 20691, 21960, 23243, 24517, 26128, 27534, 29340, 31126, 	// C3-B3
	32839, 34752, 36902, 38908, 41622, 44191, 46487, 49034, 52640, 55069, 58681, 62799, 	// C4-B4
		// Finetune 14
	 2061,  2182,  2312,  2451,  2597,  2749,  2914,  3085,  3271,  3468,  3675,  3890, 	// C0-B0
	 4123,  4365,  4624,  4903,  5195,  5498,  5829,  6171,  6543,  6937,  7350,  7781, 	// C1-B1
	 8247,  8730,  9249,  9806, 10405, 11013, 11659, 12343, 13111, 13874, 14730, 15563, 	// C2-B2
	16495, 17461, 18546, 19667, 20811, 22095, 23395, 24686, 26320, 27748, 29583, 31126, 	// C3-B3
	33143, 35093, 37286, 39335, 41622, 44191, 47099, 49715, 52640, 55930, 59659, 62799, 	// C4-B4
		// Finetune 15
	 2076,  2198,  2330,  2468,  2616,  2770,  2934,  3112,  3296,  3488,  3697,  3916, 	// C0-B0
	 4152,  4397,  4660,  4937,  5233,  5541,  5868,  6225,  6592,  6977,  7395,  7832, 	// C1-B1
	 8305,  8794,  9321,  9888, 10466, 11082, 11736, 12472, 13208, 13982, 14791, 15699, 	// C2-B2
	16649, 17633, 18643, 19776, 20933, 22233, 23549, 25031, 26515, 27965, 29583, 31399, 	// C3-B3
	33453, 35441, 37286, 39772, 42112, 44744, 47099, 50416, 53426, 55930, 59659, 62799, 	// C4-B4

};

MEM_IN_EWRAM s8 sound_mix_buffer[ SMP_BUFFER_SIZE*4 ];
SoundBuffer		sound_buffer;
// SoundChannel	channel[ VIRTUAL_CHANNEL_COUNT ];
SoundChannel	Mixer::channel[ VIRTUAL_CHANNEL_COUNT ];
u32*			smp_data;
u32 			smp_data_size;
char*			smp_name;

int 			Mixer::last_level[ CHANNEL_COUNT ];
int 			Mixer::level;
int 			Mixer::key_note[ CHANNEL_COUNT ];
int 			Mixer::new_note[ CHANNEL_COUNT ];
bool 			Mixer::enable_metronome = false;
bool 			Mixer::enabled = false;


void Mixer::init(){
	
	enabled = false;
	
	channel[0].pos 		= 0;
	channel[0].inc 		= 0;
	channel[0].data 	= (s8*) FMDATA;
	channel[0].length 	= FM_BUFFER_SIZE << 12;
	
	channel[1].pos 		= 0;
	channel[1].length 	= 0;
	channel[1].inc 		= 0;
	channel[1].data 	= NULL;
	
	sound_buffer.mixBufferBase = sound_mix_buffer;
	
	sound_buffer.mixBufferSize = SMP_BUFFER_SIZE;
	sound_buffer.curMixBuffer  = sound_mix_buffer;
	sound_buffer.activeBuffer  = 1;
	
	//SOUND_HI  = 0xBBEE; 	//SOUNDA_VOLUME_100 | SOUNDA_LOUT | SOUNDA_ROUT | SOUNDA_FIFORESET
	//SOUND_EX  = 0x80; 		//SOUND_ENABLE;  

	TIMER0_LO = 62610;		//for SMP_BUFFER_SIZE = 96
	// TIMER0_LO = 64612; 	//for SMP_BUFFER_SIZE = 304

	TIMER0_HI = 0x80; 		//TIMER_ENABLE
	
	DMA1_CHI = 0; 			//Disable DMA1
	DMA1_DST = (u32)&(*(volatile u32*)0x40000A0);

}

#include "../../data/instrument.hpp"
#include "../../data/settings.hpp"
#include "../../screens/instedit.hpp"

void Mixer::mix(){
   size_t i;
   s16 sample_a;
   s16 sample_b;
//  s16 vol_a = 0xF & 0xF;
//   s16 vol_b = 0xF & 0xF;
   
   SoundChannel *fm = &channel[0];
   
   sample_a = 0x00;
   sample_b = 0x00;
   for(i = 0; i < sound_buffer.mixBufferSize; i++){

		// copy a sample
		//sample_b = 0x00;
		
		// Pick a sample from FMW buffer
		if( fm->data ){
			sample_a = ( fm->data[ fm->pos>>12 ] );
			//sample_a = fm->data[ fm->pos & 0xF ];
			fm->pos += fm->inc;
			if(fm->pos >= fm->length ) fm->pos -= fm->length;// This sample loops when finished
		}
		// WORKAROUND
		sound_buffer.curMixBuffer[i] = sample_a;
		/*
			// Pick a sample from SMP sample buffer
			if(channel[1].pos < channel[1].length){
				sample_b = channel[1].data[ channel[1].pos>>12 ] * vol_b;
				channel[1].pos += channel[1].inc;
				sample_b >>= 2;
			} else channel[1].pos = channel[1].length;
			// Sum both input sample bytes into a single output byte 
			// sound_buffer.curMixBuffer[i] = ( sample_a + sample_b ) >> 1;
		*/
	}

}

void Mixer::noteOn1(u16 freq){
	channel[0].inc = (freq << 12) / 16000;	
}

void Mixer::noteOn2(u16 freq){
	//if(new_note[5]){
		channel[1].pos = 0;
		//channel[1].inc = (DSOUND_FREQ_TABLE[key_note[5]]<<12) / 16000;//5734;	
		channel[1].inc = freq;
	//}
}


#define SAMPLE_COUNT 125
void Mixer::load(size_t index, int chan){
	/*//loadSample
	index <<=1;
	const GBFS_FILE *dat= find_first_gbfs_file((const void*)find_first_gbfs_file);
	smp_data = (u32*)gbfs_get_nth_obj(dat, index  , NULL, &smp_data_size);
	smp_name = (char *)gbfs_get_nth_obj(dat, index+1, NULL, NULL);
	*/
	// Update channel settings
	channel[chan].data  	= 0;
	channel[chan].pos 		= 0;
	channel[chan].length 	= smp_data_size<<12;
	channel[chan].inc 		= (4000<<12)/5734;
	/*
	//channel[chan].length = smp_data_size - (smp_data_size &0XF); //remove last bytes from sample to ensure it is multiple of 16
	
	channel[chan].data = (s8*)smp_data;
	
	if(chan)noteOnSmp1();
	else noteOnSmp2(); 
	drawSampleData();
	*/
}

void Mixer::enablePwm1(){ (*(vu16*)0x4000080)|= 0x1177; }
void Mixer::enablePwm2(){ (*(vu16*)0x4000080)|= 0x2277; }
void Mixer::enableWav (){ (*(vu16*)0x4000080)|= 0x4477; }
void Mixer::enableNze (){ (*(vu16*)0x4000080)|= 0x8877; }
void Mixer::enableFmw (){ (*(vu16*)0x4000082)|= 0x0B0E; } // 0x0800:FIFOA RESET | 0x0000:DSOUND0TIMER0 | 0x0200:DSOUND0LEFT | 0x0100:DSOUND0RIGHT | 0x0008:DSOUND1VOL100 | 0x0004:DSOUND0VOL100 | 0x0002:DMGSOUNDVOL100
void Mixer::enableSmp (){ (*(vu16*)0x4000082)|= 0xB00E; } // 0x8000:FIFOB RESET | 0x4000:DSOUND1TIMER1 | 0x2000:DSOUND1LEFT | 0x1000:DSOUND1RIGHT | 0x0008:DSOUND1VOL100 | 0x0004:DSOUND0VOL100 | 0x0002:DMGSOUNDVOL100

void Mixer::start(){
	if( !VAR_CHANNEL[0].mute ) enablePwm1();
	if( !VAR_CHANNEL[1].mute ) enablePwm2();
	if( !VAR_CHANNEL[2].mute ) enableNze();
	if( !VAR_CHANNEL[3].mute ) enableWav();
	if( !VAR_CHANNEL[4].mute ) enableFmw();
	if( !VAR_CHANNEL[5].mute ) enableSmp();
	enabled = true;
}

void Mixer::disablePwm1(){ (*(vu16*)0x4000080) &= ~0x1100; }
void Mixer::disablePwm2(){ (*(vu16*)0x4000080) &= ~0x2200; }
void Mixer::disableWav (){ (*(vu16*)0x4000080) &= ~0x4400; }
void Mixer::disableNze (){ (*(vu16*)0x4000080) &= ~0x8800; }
void Mixer::disableSmp (){ (*(vu16*)0x4000082) &= ~0xB000; }
void Mixer::disableFmw (){ 
	(*(vu16*)0x4000082) &= ~0x0B00; 
	memset( (void*)FMDATA, 0, sizeof(u8)*FM_BUFFER_SIZE);
}

void Mixer::stop(){
	disablePwm1();
	disablePwm2();
	disableNze();
	disableWav();
	disableFmw();
	disableSmp();
	enabled = false;
}

void Mixer::disable( u8 channel ){
	switch(channel){
		case 0: return disablePwm1();
		case 1: return disablePwm2();
		case 2: return disableNze();
		case 3: return disableWav();
		case 4: return disableFmw();
		case 5: return disableSmp();
	}
}

void Mixer::enable( u8 channel ){
	switch(channel){
		case 0: return enablePwm1();
		case 1: return enablePwm2();
		case 2: return enableNze();
		case 3: return enableWav();
		case 4: return enableFmw();
		case 5: return enableSmp();
	}
}

/*###########################################################################*/

void Mixer::updateMetronome( u8 time, u8 beats_per_bar ) {
	if( time + 1 == 0 ){
		*((volatile u16*)0x04000068) = 0x8181;
		*((volatile u16*)0x0400006C) = 0xC7b7;				
	} else 
	if(!(( time + 1 ) % beats_per_bar)){								
		*((volatile u16*)0x04000068) = 0x8181;
		*((volatile u16*)0x0400006C) = 0xC770;
	}
}	

/*###########################################################################*/
void Mixer::mute( int channel ){
	VAR_CHANNEL[channel].mute ^= 1;
	// Since a channel was unmuted, disable solo on every channel 
	// Sync with audio registers
	if( VAR_CHANNEL[channel].mute ) Mixer::disable( channel );
	else Mixer::enable( channel );
	
	if(VAR_CHANNEL[channel].mute) return;
	// If a channel was unmuted, disable solo on every channel 
	for(int i=0; i<6;i++){
		VAR_CHANNEL[i].solo = false;
	}
}

void Mixer::solo(int channel){
	// If channel has solo enabled unmute channels and disable solo 
	if(VAR_CHANNEL[channel].solo){
		for(int i=0; i<6;i++){
			VAR_CHANNEL[channel].solo = false;
			VAR_CHANNEL[i].mute = false;
			if( channel == i ) continue;
			enable( i );
		}
		return;
	}
		
	if( VAR_CHANNEL[channel].mute ) enable( channel );
	VAR_CHANNEL[channel].solo = true;
	VAR_CHANNEL[channel].mute = false;
	
	// Mute all channels 
	for(int i=0; i<6;i++){
		if( channel == i ) continue;
		VAR_CHANNEL[i].mute = true;
		disable( i );
	}
	
	// Unmute and enable solo on selected channel 
}

void Mixer::show( Control *c, bool bigstep, bool add, u32 *pointer ){
	
}

void Mixer::update(  ){
}
