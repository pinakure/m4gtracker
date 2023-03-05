
#define SOUND_PWM_MASTER_RIGHT(vol)		(vol&0x7)
#define SOUND_PWM_MASTER_LEFT(vol)		((vol&0x7)<<4)
#define SOUND_PWM_ENABLE_RIGHT(channel)	((1<<8)<<channel)
#define SOUND_PWM_ENABLE_LEFT(channel)	((1<<12)<<channel)
#define SOUND_PWM_ENABLE_PSG_FIFO		0x0080

#define SOUND4CNT_L		*(u16*)0x4000078
/*-----------------------------------------------------------------------------
	4000078h - SOUND4CNT_L (NR41, NR42) - Channel 4 Length/Envelope (R/W)
	  Bit        Expl.
	  0-5   W    Sound length; units of (64-n)/256s  (0-63)
	  6-7   -    Not used
	  8-10  R/W  Envelope Step-Time; units of n/64s  (1-7, 0=No Envelope)
	  11    R/W  Envelope Direction                  (0=Decrease, 1=Increase)
	  12-15 R/W  Initial Volume of envelope          (1-15, 0=No Sound)
	  16-31 -    Not used
	The Length value is used only if Bit 6 in NR44 is set.
-----------------------------------------------------------------------------*/
#define SOUND4CNT_H		*(u16*)0x400007C
/*-----------------------------------------------------------------------------
	400007Ch - SOUND4CNT_H (NR43, NR44) - Channel 4 Frequency/Control (R/W)
	The amplitude is randomly switched between high and low at the given frequency.
	A higher frequency will make the noise to appear 'softer'.
	When Bit 3 is set, the output will become more regular, and some frequencies
	will sound more like Tone than Noise.
	  Bit        Expl.
	  0-2   R/W  Dividing Ratio of Frequencies (r)
	  3     R/W  Counter Step/Width (0=15 bits, 1=7 bits)
	  4-7   R/W  Shift Clock Frequency (s)
	  8-13  -    Not used
	  14    R/W  Length Flag  (1=Stop output when length in NR41 expires)
	  15    W    Initial      (1=Restart Sound)
	  16-31 -    Not used
	Frequency = 524288 Hz / r / 2^(s+1)     ;For r=0 assume r=0.5 instead
-----------------------------------------------------------------------------*/

#define SOUND1CNT_H	*(u16*)(0x04000064)
#define SOUND2CNT_H	*(u16*)(0x0400006C)
#define SOUND1CNT_L *(u16*)(0x04000062)
#define SOUND1CNT_X *(u16*)(0x04000060)
#define SOUND2CNT_L *(u16*)(0x04000068)

/*
	WAVE CHANNEL:
	This channel can be used to output digital sound, the length of the sample
	buffer (Wave RAM) can be either 32 or 64 digits (4bit samples). This sound
	channel can be also used to output normal tones when initializing the Wave RAM
	by a square wave. This channel doesn't have a volume envelope register.
*/


#define SOUND3CNT_L *(u16*)(0x04000070)
/*
	4000070h - SOUND3CNT_L (NR30) - Channel 3 Stop/Wave RAM select (R/W)
	  Bit        Expl.
	  0-4   -    Not used
	  5     R/W  Wave RAM Dimension   (0=One bank/32 digits, 1=Two banks/64 digits)
	  6     R/W  Wave RAM Bank Number (0-1, see below)
	  7     R/W  Sound Channel 3 Off  (0=Stop, 1=Playback)
	  8-15  -    Not used
	The currently selected Bank Number (Bit 6) will be played back, while
	reading/writing to/from wave RAM will address the other (not selected) bank.
	When dimension is set to two banks, output will start by replaying the
	currently selected bank.
*/
#define SOUND3_BANK_0 	0x0000
#define SOUND3_BANK_1 	0x0020
#define SOUND3_PLAY		0x0040
#define WAVE_BANK(a)	SOUND3_BANK_##a

#define SOUND3CNT_H *(u16*)(0x04000072)
/*
	4000072h - SOUND3CNT_H (NR31, NR32) - Channel 3 Length/Volume (R/W)
	  Bit        Expl.
	  0-7   W    Sound length; units of (256-n)/256s  (0-255)
	  8-12  -    Not used.
	  13-14 R/W  Sound Volume  (0=Mute/Zero, 1=100%, 2=50%, 3=25%)
	  15    R/W  Force Volume  (0=Use above, 1=Force 75% regardless of above)
	The Length value is used only if Bit 6 in NR34 is set.
*/
#define SOUND3_LEN_MAX 	0xFF
#define SOUND3_LEN_MIN  0x00
#define SOUND3_VOL_00	0x0000
#define SOUND3_VOL_25 	0x0600
#define SOUND3_VOL_50 	0x0400
#define SOUND3_VOL_75   0x8000
#define SOUND3_VOL_100 	0x0200
#define WAVE_VOLUME(a)	SOUND3_VOL_##a

#define SOUND3CNT_X *(u16*)(0x04000074)
/*
	4000074h - SOUND3CNT_X (NR33, NR34) - Channel 3 Frequency/Control (R/W)
	  Bit        Expl.
	  0-10  W    Sample Rate; 2097152/(2048-n) Hz   (0-2047)
	  11-13 -    Not used
	  14    R/W  Length Flag  (1=Stop output when length in NR31 expires)
	  15    W    Initial      (1=Restart Sound)
	  16-31 -    Not used
	The above sample rate specifies the number of wave RAM digits per second, the
	actual tone frequency depends on the wave RAM content, for example:
	  Wave RAM, single bank 32 digits   Tone Frequency
	  FFFFFFFFFFFFFFFF0000000000000000  65536/(2048-n) Hz
	  FFFFFFFF00000000FFFFFFFF00000000  131072/(2048-n) Hz
	  FFFF0000FFFF0000FFFF0000FFFF0000  262144/(2048-n) Hz
	  FF00FF00FF00FF00FF00FF00FF00FF00  524288/(2048-n) Hz
	  F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0  1048576/(2048-n) Hz
*/
#define SOUND3_FREQ_MAX 0x07FF
#define SOUND3_FREQ_MIN 0x0000
#define SOUND3_TRIGGER	0xC000
#define SOUND3_RETRIG	0x4000

#define WAVE_RAM0_L *(u16*)(0x04000090)
#define WAVE_RAM0_H *(u16*)(0x04000092)
#define WAVE_RAM1_L *(u16*)(0x04000094)
#define WAVE_RAM1_H *(u16*)(0x04000096)
#define WAVE_RAM2_L *(u16*)(0x04000098)
#define WAVE_RAM2_H *(u16*)(0x0400009A)
#define WAVE_RAM3_L *(u16*)(0x0400009C)
#define WAVE_RAM3_H *(u16*)(0x0400009E)
/*
	4000090h - WAVE_RAM0_L - Channel 3 Wave Pattern RAM (W/R)
	4000092h - WAVE_RAM0_H - Channel 3 Wave Pattern RAM (W/R)
	4000094h - WAVE_RAM1_L - Channel 3 Wave Pattern RAM (W/R)
	4000096h - WAVE_RAM1_H - Channel 3 Wave Pattern RAM (W/R)
	4000098h - WAVE_RAM2_L - Channel 3 Wave Pattern RAM (W/R)
	400009Ah - WAVE_RAM2_H - Channel 3 Wave Pattern RAM (W/R)
	400009Ch - WAVE_RAM3_L - Channel 3 Wave Pattern RAM (W/R)
	400009Eh - WAVE_RAM3_H - Channel 3 Wave Pattern RAM (W/R)
	This area contains 16 bytes (32 x 4bits) Wave Pattern data which is output by
	channel 3. Data is played back ordered as follows: MSBs of 1st byte, followed
	by LSBs of 1st byte, followed by MSBs of 2nd byte, and so on - this results in
	a confusing ordering when filling Wave RAM in units of 16bit data - ie. samples
	would be then located in Bits 4-7, 0-3, 12-15, 8-11.

	In the GBA, two Wave Patterns exists (each 32 x 4bits), either one may be
	played (as selected in NR30 register), the other bank may be accessed by the
	users. After all 32 samples have been played, output of the same bank (or other
	bank, as specified in NR30) will be automatically restarted.

	Internally, Wave RAM is a giant shift-register, there is no pointer which is
	addressing the currently played digit. Instead, the entire 128 bits are
	shifted, and the 4 least significant bits are output.
	Thus, when reading from Wave RAM, data might have changed its position. And,
	when writing to Wave RAM all data should be updated (it'd be no good idea to
	assume that old data is still located at the same position where it has been
	written to previously).
*/

#define BPM_MAGIC 6271			//100BPM = 6271Hz per beat	
class Spu{
	private:
	
	public:
		
		static bool retrig_note[6];
		static int 	targetTick[6];
		static u8   transpose[6];
		static bool	reset_channel[6];
		static u8	tsp_position[6];
		static u8	vol_position[6];
		static u8 	wav_adsr_position;
		static u8 	smp_adsr_position;
		static u8 	fmw_adsr_position;
		
		static u8 	wav_adsr_table[4][0x40];
		static u8 	fmw_adsr_table[4][0x40];
		static u8 	smp_adsr_table[0x40];
		

		int	 timerTarget;
		int  currentPattern; //NOT USED! 
		int  currentTicks;
		int  currentBeats;
		int  beatsPerBar;
		int  secPerBeat;
		bool enable_metronome;	
		
		bool playing;
		
		void mute(int channel);
		void solo(int channel);
	
		void Init(int bpm);
		void setTempo(int bpm);
		void play(bool fromStart);	
		void stop(void);	
		
		void noteOnPWM1();
		void noteOnPWM2();
		void noteOnWAV();
		void noteOnNZE();
		void noteOnFMW();
		void noteOnSMP();

		void enablePWM1();
		void enablePWM2();
		void enableWAV();
		void enableNZE();
		void enableFMW();
		void enableSMP();
		
		void disablePWM1();
		void disablePWM2();
		void disableWAV();
		void disableNZE();
		void disableFMW();
		void disableSMP();
		
		static bool initialized;
		
		bool setKey(	u8 channel, u8 key, u8 vol );
		void setCmd(	u8 channel, u8 cmd, u8 value );
		void setInst(  u8 channel, u8 inst, u8 vol, bool retrig_note );
		void triggerChannel( int channelIndex );

		
		void update();		// Called each program cycle		
		void enable();		//Enable given sound channel
		void disable();		//Disable general sound system
		
		void jumpToPatternAsync(int p);	//jumps asyncronously to pattern p

		// AM / FM / SMP
		void renderADSR( u8 adsr[4], u8 adsr_table[0x40] ); // ADSR Table Regeneration
		
		void updateWavADSR( SETTINGS_WAV *wav ); // Callback to be used @ ADSR modification / instrument change (WAVE CHANNEL)
		void updateFmwADSR( SETTINGS_FMW *fmw ); // Callback to be used @ ADSR modification / instrument change (WAVE CHANNEL)
		void updateSmpADSR( SETTINGS_SMP *smp ); // Callback to be used @ ADSR modification / instrument change (WAVE CHANNEL)
		
		void loadWavWAVEData( u8 data[ 16 ] );
		void loadFmwWAVEData( u8 data[ 16 ] );
		void loadSmpWAVEData( u8 data[ 16 ] );
		
		void renderWavWAVE( SETTINGS_WAV *settings, u8 vol );
		void renderFmwWAVE( SETTINGS_FMW *settings, u8 vol );
		void renderSmpWAVE( SETTINGS_SMP *settings, u8 vol );
		
	};

extern Spu SPU;

