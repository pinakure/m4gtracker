#include "mixer.hpp"
#include "synth.hpp"
#include "sequencer.hpp"

Spu SPU;

bool 	Spu::initialized 		= false;


extern inline SETTINGS_PWM unpackPWM(Instrument *i);
extern inline SETTINGS_WAV unpackWAV(Instrument *i);
extern inline SETTINGS_SMP unpackSMP(Instrument *i);
extern inline SETTINGS_FMW unpackFMW(Instrument *i);

extern void cellSyncChannel(u8 c);

void Spu::Init(int bpm){
	
	
	Sequencer::init( bpm );
	Synth::init();
	
	/*-----------------------------------------------------------*/
	/* WORKAROUND TBF											 */
	/* ----------------------------------------------------------*/
	VAR_CFG.CURRENTINSTRUMENT = 0x1; // 0x0 is not accesible / usable
	/*----------------------------------------------------------*/
	
	/* ----------------------------------------------------------*/
	/* INITIALIZE DIRECTSOUND MIXER								 */
	/* ----------------------------------------------------------*/
	Mixer::init();
}
/*-------------------------------------------------------------------------*/
/* DEPRECATED: Remove these wrappers as soon as dependencies from old SPU methods are rebound to new classes */
void Spu::update(void){ Sequencer::update(); }
/*-------------------------------------------------------------------------*/
void Spu::mute(int channel){
	// ------------------------------------------------------------
	VAR_CHANNEL[channel].mute ^= 1;
	/* ------------------------------------------------------------
	Since a channel was unmuted, disable solo on every channel   */
	// Sync with audio registers
	/* ------------------------------------------------------------
	If a channel was unmuted, disable solo on every channel   	 */
	if(VAR_CHANNEL[channel].mute) return;
	for(int i=0; i<6;i++){
		VAR_CHANNEL[i].solo = false;
	}
}

void Spu::solo(int channel){
	/* ------------------------------------------------------------
	if channel has solo enabled unmute channels and disable solo */
	if(VAR_CHANNEL[channel].solo){
		for(int i=0; i<6;i++){
			VAR_CHANNEL[channel].solo = false;
			VAR_CHANNEL[i].mute = false;
		}
		return;
	}
	/* ------------------------------------------------------------
	Mute all channels 											 */
	for(int i=0; i<6;i++){
		VAR_CHANNEL[i].mute = true;
	}
	/* ------------------------------------------------------------
	Unmute and enable solo on selected channel 				     */
	VAR_CHANNEL[channel].solo = true;
	VAR_CHANNEL[channel].mute = false;
	
	// Sync with audio registers
}
/*-------------------------------------------------------------------------*/

