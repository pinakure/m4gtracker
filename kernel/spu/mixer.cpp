#include "mixer.hpp"
#include "synth.hpp"
#include "../../data/channel.hpp"
#include "../../debug.hpp"
#include "../../data/settings.hpp"
#include "../../gbfs.h"

#include "../gpu/gpu.hpp"
#include "../gpu/virtualscreen.hpp"
#include "sequencer.hpp"
#include "../key/key.hpp"


const u32 DSOUND_FREQ_TABLE[] = {
		// Finetune 0
	523		, 554		, 587		, 621		, 659		, 697		, 740		, 785		, 832		, 881		, 932		, 988 		, // C0-B0
	1045	, 1107		, 1174		, 1242		, 1318		, 1394		, 1481		, 1569		, 1663		, 1761		, 1864		, 1975		, // C1-B1
	2090	, 2215		, 2348		, 2485		, 2639		, 2796		, 2963		, 3139		, 3326		, 3523		, 3728		, 3950		, // C2-B2
	4181	, 4430		, 4697		, 4971		, 5279		, 5593		, 5926		, 6279		, 6653		, 7046		, 7457		, 7901		, // C3-B3
	8363	, 8860		, 9395		, 9943		, 10559		, 11186		, 11852		, 12559		, 13306		, 14092		, 14914		, 15838		, // C4-B4
	16726	, 17720		, 18839		, 19886		, 21180		, 22372		, 23705		, 25208		, 26713		, 28185		, 29829		, 31677		, // C5-B5
	33453	, 35441		, 37679		, 39772		, 42613		, 44744		, 47727		, 50416		, 53426		, 56818		, 59659		, 63920		, // C6-B6
	66906	, 70882		, 75358		, 79544		, 85226		, 89488		, 95454		, 100832	, 106852	, 113636	, 119318	, 127840	, // C7-B7
	133812	, 141764	, 150716	, 159088	, 170452	, 178976	, 190908	, 201664	, 213704	, 227272	, 238636	, 255680	, // C8-B8
	267624	, 283528	, 301432	, 318176	, 340904	, 357952	, 381816	, 403328	, 427408	, 454544	, 477272	, 511360	, // C9-B9
	535248	, 567056	, 602864	, 636352	, 681808	, 715904	, 763632	, 806656	, 854816	, 909088	, 954544	, 1022720	, // 120 - 131 CA-BA
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 132 - 143 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 144 - 155 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 156 - 167 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 168 - 179 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 180 - 191 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 192 - 203 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 204 - 215 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 216 - 227 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 228 - 239 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 240 - 251 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 252 - 263 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 132 - 143 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 144 - 155 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 156 - 167 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 168 - 179 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 180 - 191 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 192 - 203 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 204 - 215 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 216 - 227 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 228 - 239 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 240 - 251 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 252 - 263 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 132 - 143 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 144 - 155 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 156 - 167 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 168 - 179 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 180 - 191 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 192 - 203 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 204 - 215 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 216 - 227 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 228 - 239 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 240 - 251 CB-BB //WTF man!
	1070496	, 1134112	, 1205728	, 1272704	, 1363616	, 1431808	, 1527264	, 1613312	, 1709632	, 1818176	, 1909088	, 2045440	, // 252 - 263 CB-BB //WTF man!
};

MEM_IN_EWRAM s8 sound_mix_buffer[ SMP_BUFFER_SIZE*4 ];
SoundBuffer		sound_buffer;
SoundChannel	Mixer::dsound[ VIRTUAL_CHANNEL_COUNT ];
bool			Mixer::dsound_mute[ VIRTUAL_CHANNEL_COUNT ];
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
	
	dsound[0].pos 		= 0;
	dsound[0].inc 		= 0;
	dsound[0].data 	= (s8*) FMDATA;
	dsound[0].length 	= FM_BUFFER_SIZE << 12;
	
	dsound[1].pos 		= 0;
	dsound[1].length 	= 0;
	dsound[1].inc 		= 0;
	dsound[1].data 	= NULL;
	
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
   
   SoundChannel *fm 	= &dsound[0];
   SoundChannel *smp 	= &dsound[1];
   
   Channel *smp_channel = VAR_CHANNEL + CHANNEL_SMP;
   
   sample_a = 0x00;
   sample_b = 0x00;
   
   u16 smp_loop_inc = Synth::loop_smp ? smp->inc : 0;
   
   for(i = 0; i < sound_buffer.mixBufferSize; i++){

		// copy a sample
		//sample_b = 0x00;
		
		// Pick a sample from FMW buffer
		if( fm->data && !*dsound_mute){
			sample_a = ( fm->data[ fm->pos>>12 ] );
			//sample_a = fm->data[ fm->pos & 0xF ];
			fm->pos += fm->inc;
			if(fm->pos >= fm->length ) fm->pos -= fm->length;// This sample loops when finished
		} else sample_a = 0;
		
		// WORKAROUND
		//sound_buffer.curMixBuffer[i] = sample_a;
		if( smp->data && !*(dsound_mute+1)){
			sample_b = ( smp->data[ smp->pos>>12 ] * smp_channel->volume )>>4 ;//*0xF;//* vol_b;
			smp->pos += smp->inc;
			/* !! */
			// sample_b >>= 2;
			/* !! */
			if( smp->pos >= smp->length) { 
				smp->pos -= smp->length;
				smp->inc = smp_loop_inc;
				if(! Synth::loop_smp ) {
					sample_b = 0;
					smp->inc = 0;
					smp->pos = 0;
				}
			}
		} else sample_b = 0;
		// Sum both input sample bytes into a single output byte 
		sound_buffer.curMixBuffer[i] = ( sample_a + sample_b ) >> 1;
	}

}

void Mixer::noteOn1(u32 freq){
	// dsound[0].inc = (freq << 12) / 16000;	
	dsound[0].pos = 0;
	dsound[0].inc = (((u64)(freq)) << 5) / 125;//5734;	
	// HALT
}

void Mixer::noteOn2(u32 freq){
	dsound[1].inc = (freq<<5)/125;//5734;
	//dsound[1].inc = (freq<<12)/5734;
	Instrument *i = &VAR_INSTRUMENTS[ VAR_CHANNEL[ CHANNEL_SMP ].inst];
	SETTINGS_SMP smp = InstEdit::unpackSmp(i);
	Synth::loop_smp = smp.LOOP;
	Mixer::load( &smp , 1);
	//Gpu::number(14,1,VAR_CHANNEL[ CHANNEL_SMP ].inst,0xF);
	//Gpu::number(20,1,smp.KITINDEX,0xF);
	//if(new_note[5]){
		// dsound[1].pos = 0;
		//dsound[1].inc = (DSOUND_FREQ_TABLE[key_note[5]]<<12) / 16000;//5734;	
	//}
}


#define SAMPLE_COUNT 125
void Mixer::load( SETTINGS_SMP *smp, int chan=1){
	//loadSample
	size_t index = smp->KITINDEX;
	index <<=1;
	const GBFS_FILE *dat = find_first_gbfs_file((const void*)find_first_gbfs_file);
	smp_data 	= (u32*)gbfs_get_nth_obj(dat, index  , NULL, &smp_data_size);
	smp_name 	= (char *)gbfs_get_nth_obj(dat, index+1, NULL, NULL);
	
	dsound[1].pos 		= 0;
	dsound[1].data = (s8*)smp_data;
	//dsound[1].length 	= smp_data_size<<12;
	smp_data_size <<= 12;
	dsound[1].length = smp_data_size - (smp_data_size &0XF); //remove last bytes from sample to ensure it is multiple of 16
	Synth::smp_start = (dsound[1].length >>8 ) * smp->START;
	Synth::smp_end 	 = (smp->END > smp->START) ? (dsound[1].length >>8 ) * smp->END : dsound[1].length;
	Gpu::number(10, 0, dsound[1].length	, 0x2);
	Gpu::number(10, 1, smp->START, 0x4);
	Gpu::number(20, 1, smp->END, 0x4);
	Gpu::number(10, 2, Synth::smp_start , 0xf);
	Gpu::number(20, 2, Synth::smp_end, 0x3);
	dsound[1].length = (smp->END > smp->START) ? Synth::smp_end - Synth::smp_start : dsound[1].length - Synth::smp_start; //remove last bytes from sample to ensure it is multiple of 16
	dsound[1].pos 	 = Synth::smp_start;
	
	// if(KEYPRESS_SELECT){
		// VirtualScreen::clear();
		// Gpu::drawDialog(13,0,17,20,"SMP Info");
		// /**/
		// Gpu::ascii (42,1, "smp_data");
		// Gpu::ascii (42,2, "0x", COLOR_CYAN);
		// Gpu::hexnum(22,2,(unsigned int)&smp_data, 0x2);
		// /**/
		// Gpu::ascii (42,3, "smp_name");
		// Gpu::ascii (42,4, smp_name, 0x2);
		// /**/
		// Gpu::ascii (42,5, "index");
		// Gpu::number(21,6, index, 0x2);
		// /**/
		// Gpu::ascii (42,7, "smp_data_size");
		// Gpu::ascii (42,8, "0x", COLOR_CYAN);
		// Gpu::hexnum(22,8,smp_data_size, 0x2);
		// /**/
		// Gpu::ascii (42,9, "dat");
		// Gpu::ascii (42,10, "0x", COLOR_CYAN);
		// Gpu::hexnum(22,10,(unsigned int)&dat, 0x2);
		// /**/
		// Gpu::ascii (42,11, "chan.length");
		// Gpu::ascii (42,12, "0x", COLOR_CYAN);
		// Gpu::number(22,12, dsound[1].length, 0x2);
		// /**/
		// Gpu::ascii (42,13, "chan.position");
		// Gpu::ascii (42,14, "0x", COLOR_CYAN);
		// Gpu::number(22,14, dsound[1].pos, 0x2);
		// // while( KEYPRESS_SELECT ){ Key::update(); }
	// }
}

void Mixer::enablePwm1(){ (*(vu16*)0x4000080)|= 0x1177; }
void Mixer::enablePwm2(){ (*(vu16*)0x4000080)|= 0x2277; }
void Mixer::enableWav (){ (*(vu16*)0x4000080)|= 0x4477; }
void Mixer::enableNze (){ (*(vu16*)0x4000080)|= 0x8877; }
void Mixer::enableFmw (){ 
	dsound_mute[0] = false;
	(*(vu16*)0x4000082)|= 0xBB0E; 
} // 0x0800:FIFOA RESET | 0x0000:DSOUND0TIMER0 | 0x0200:DSOUND0LEFT | 0x0100:DSOUND0RIGHT | 0x0008:DSOUND1VOL100 | 0x0004:DSOUND0VOL100 | 0x0002:DMGSOUNDVOL100
void Mixer::enableSmp (){ 
	dsound_mute[1] = false;
	(*(vu16*)0x4000082)|= 0xBB0E; 
} // 0x8000:FIFOB RESET | 0x4000:DSOUND1TIMER1 | 0x2000:DSOUND1LEFT | 0x1000:DSOUND1RIGHT | 0x0008:DSOUND1VOL100 | 0x0004:DSOUND0VOL100 | 0x0002:DMGSOUNDVOL100


void Mixer::disablePwm1(){ (*(vu16*)0x4000080) &= ~0x1100; }
void Mixer::disablePwm2(){ (*(vu16*)0x4000080) &= ~0x2200; }
void Mixer::disableWav (){ (*(vu16*)0x4000080) &= ~0x4400; }
void Mixer::disableNze (){ (*(vu16*)0x4000080) &= ~0x8800; }
void Mixer::disableFmw (){ 
	dsound_mute[0] = true;
	memset( (void*)FMDATA, 0, sizeof(u8)*FM_BUFFER_SIZE);
	// (*(vu16*)0x4000082) &= ~0x0B00;
}
void Mixer::disableSmp (){ 
	dsound_mute[1] = true;
	// (*(vu16*)0x4000082) &= ~0xB000; 
}

void Mixer::start(){
	for(Channel *channel = VAR_CHANNEL, *end= channel + CHANNEL_COUNT; channel<end; channel++){
		if( !channel->mute ) channel->enable();
	}
	enabled = true;
}

void Mixer::stop(){
	for(Channel *channel=VAR_CHANNEL, *end=channel+CHANNEL_COUNT; channel<end; channel++){
		channel->disable();
	}
	enabled = false;
}

void Mixer::updateMetronome( u8 time, u8 beats_per_bar ) {
	if( time + 1 == 0 ){						// beep	
		*((volatile u16*)0x04000068) = 0x8181;
		*((volatile u16*)0x0400006C) = 0xC7b7;		
	} else if(!(( time + 1 ) % beats_per_bar)){	// boop, boop, boop						
		*((volatile u16*)0x04000068) = 0x8181; 
		*((volatile u16*)0x0400006C) = 0xC770;	
	}
}	

void Mixer::mute( u8 channel_index ){//TODO: replace by channel pointer 
	Channel *channel = VAR_CHANNEL + channel_index;
	channel->mute ^= 1;
	// Since a channel was unmuted, disable solo on every channel 
	// Sync with audio registers
	if( channel->mute ) channel->disable();
	else channel->enable( );
	
	if(channel->mute) return;
	// If a channel was unmuted, disable solo on every channel 
	for(int i=0; i<CHANNEL_COUNT;i++){
		VAR_CHANNEL[i].solo = false;
	}
}

void Mixer::solo(u8 channel_index){
	Channel *channel = VAR_CHANNEL + channel_index;
	// If channel has solo enabled unmute channels and disable solo 
	if(channel->solo){
		Channel *p = VAR_CHANNEL;
		for(Channel *end = VAR_CHANNEL + CHANNEL_COUNT; p<end; p++){
			p->solo = false;
			p->mute = false;
			if( p == channel) continue;
			channel->enable();
		}
		return;
	}
		
	if( channel->mute ) channel->enable() ;
	channel->solo = true;
	channel->mute = false;
	
	// Mute all channels 
	for(Channel *p = VAR_CHANNEL, *end = p+CHANNEL_COUNT; p<end; p++){
		if( channel == p ) continue;
		p->mute = true;
		p->disable();
	}
	
	// Unmute and enable solo on selected channel 
}

void Mixer::show( Control *c, bool bigstep, bool add, u32 *pointer ){
	
}

void Mixer::update(  ){
}
