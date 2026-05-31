/** AGBLIB - M4GEEK 2011**/
/** Based on Emanuel Schleussinger's HAMLIB's mygba.h file which is also based (and uncredited >:< )
	on NO$GBA's low level documentation, Nintendo AGB SDK, and devkitGBA. I'm abstracting some defines, just those 
	needed to do all stuff, trying to not rely on HAM at all, so these common defines aren't but property of Nintendo,
	so feel free to use this libary without giving credit to anybody but you ;) **/
#ifndef AGBLIB_H

	#define TRANSIENT	
	#define BACKUP
	#define BITFIELD(x)		u8

	// If this is defined, each pattern will not have individual transpose setting
	#include <stdarg.h>
	#include <stdlib.h>
	#include "string.h"
	
	/* basic data types */	
	#ifndef __cplusplus
		typedef     unsigned char           bool;
	#endif	
	#ifndef TRUE
		#define TRUE 1
	#endif
	#ifndef FALSE
		#define FALSE 0
	#endif
	#ifndef NULL
		#define NULL ((void *)0)
	#endif

	typedef	const unsigned short int		cu16;
	typedef	const unsigned char				cu8;
	typedef	const unsigned int				cu32;
	typedef	const unsigned long long int	cu64;	
	typedef	unsigned char					u8;
	typedef	unsigned short int				u16;
	typedef	unsigned int					u32;
	typedef	unsigned long long int			u64;	
	typedef	signed char						s8;
	typedef	signed short int				s16;
	typedef	signed int						s32;
	typedef	signed long long int			s64;
	typedef	volatile unsigned char			vu8;
	typedef	volatile unsigned short int		vu16;
	typedef	volatile unsigned int			vu32;
	typedef volatile unsigned long long int	vu64;
	typedef	volatile signed char			vs8;
	typedef	volatile signed short int		vs16;
	typedef	volatile signed int				vs32;
	typedef	volatile signed long long int	vs64;	
	typedef void							(*voidptr)(void);
	typedef	s16	sfp16;						//1:7:8 fixed point
	typedef	s32	sfp32;						//1:19:8 fixed point
	typedef	u16	ufp16;						//8:8 fixed point
	typedef	u32	ufp32;						//24:8 fixed point

	#define FIXED s32

	/* BOOT Tags */	
	//#define MULTIBOOT	volatile const int __gba_multiboot = 1;  
	#define PREFETCH	R_WCNT = 0x0004 | 0x0010 | 0x4000;
	
	//MULTIBOOT

	/* Instruction loading selectors */
	#define MEM_IN_EWRAM __attribute__ ((section (".ewram"))) 
	#define MEM_IN_IWRAM __attribute__ ((section (".iwram"))) 
	#define MEM_FUNC_IN_IWRAM __attribute__ ((section (".iwram"), long_call))	
	/* system call macro */
	#ifdef  __GNUC__
		#ifndef CODE32
		#define SystemCall(No)   asm ("SWI    "#No)
		#else
		#define SystemCall(No)   asm ("SWI    "#No" << 16")
		#endif
	#else
		#ifndef CODE32
		#define SystemCall(No) __asm { SWI    No}
		#else
		#define SystemCall(No) __asm { SWI    No << 16}
		#endif
	#endif
		
	/* MEMORY MAP */
	#define REG_BASE		0x04000000
	#define BROM			0x00000000
	#define BROM_END		0x00004000
	#define IWRAM			0x03000000
	#define IWRAM_END		0x03008000
	#define WRAM			0x02000000
	#define WRAM_END		0x02040000
	#define VRAM			0x06000000  // VRAM
	#define VRAM_END		0x06018000
	#define PALETTE			0x05000000	// Palette RAM
	#define PALETTE_END		0x05000400
	#define BG_PALETTE		0x05000000	// BG Palette RAM
	#define BG_PALETTE_END	0x05000200
	#define OBJ_PALETTE		0x05000200	// OBJ Palette RAM
	#define OBJ_PALETTE_END	0x05000400
	#define BG_VRAM			0x06000000	// BG Character/Screen RAM
	#define BG_BMP0_VRAM	0x06000000	// BG Bitmap 0 RAM
	#define BG_BMP1_VRAM	0x0600a000	// BG Bitmap 1 RAM
	#define OBJ_MODE0_VRAM	0x06010000	// OBJ Character RAM
	#define OBJ_MODE1_VRAM	0x06010000
	#define OBJ_MODE2_VRAM	0x06010000
	#define OBJ_MODE3_VRAM	0x06014000
	#define OBJ_MODE4_VRAM	0x06014000
	#define OBJ_MODE5_VRAM	0x06014000
	#define OBJ_VRAM_END	0x06018000
	#define OAM				0x07000000	
	#define OAM_END			0x07000400
	/* MEMORY SIZES */
	#define XWRAM_SIZE		0x40000             
	#define IWRAM_SIZE		0x8000
	#define WRAM_SIZE		0x48000
	#define PALETTE_SIZE	(2*256*2)
	#define VRAM_SIZE		0x18000	
	#define OAM_SIZE		(8*128)             


	#define INTR_VECTOR_BUF	(IWRAM_END - 0x4)				// Interrupt Branch Address
	/** REGISTERS **/	
	#define R_DISPCNT		*(volatile u32*)0x4000000	
	#define R_STAT			*(volatile u16*)0x4000004		// DISPLAY Status
	#define R_VCOUNT		*(volatile u16*)0x4000006		// V Counter
	
	#define R_IE			*(volatile u16*)0x4000200
	#define R_IF			*(volatile u16*)0x4000202
	#define R_WCNT			*(volatile u16*)0x4000204
	#define R_IME			*(volatile u16*)0x4000208	
	#define R_PAUSE			*(volatile u16*)0x4000300	
	
#define AGBLIB_H

	#define PROGRAM_NAME			"m4geek01"
	#define _VER_M					0x0
	#define _VER_m					0x1
	#define _VER_s					0x3
	#define _VER_R					0xA

	/* IRQ CODES */
	#define IRQ_VBLANK		0
	#define IRQ_HBLANK		1
	#define IRQ_VCOUNT		2
	#define IRQ_TIMER0		3
	#define IRQ_TIMER1		4
	#define IRQ_TIMER2		5
	#define IRQ_TIMER3		6
	#define IRQ_SERIAL		7
	#define IRQ_DMA0		8
	#define IRQ_DMA1		9
	#define IRQ_DMA2		10
	#define IRQ_DMA3		11
	#define IRQ_KEYPAD		12
	#define IRQ_AGBPAK		13

	/*-------------------------------------------------------------------------
								 INPUT DEFINES
	-------------------------------------------------------------------------*/
	/* KEYPAD SCANCODES */
	typedef enum eAGBKeys {
		KEY_A		= 0x0001,
		KEY_B		= 0x0002,
		KEY_SELECT	= 0x0004,
		KEY_START	= 0x0008,
		KEY_RIGHT	= 0x0010,
		KEY_LEFT	= 0x0020,
		KEY_UP		= 0x0040,
		KEY_DOWN	= 0x0080,
		KEY_R		= 0x0100,
		KEY_L		= 0x0200,
	} AGBKeys;
	#define KEY_A					0x0001
	#define KEY_B					0x0002
	#define KEY_SELECT				0x0004
	#define KEY_START				0x0008
	#define KEY_RIGHT				0x0010
	#define KEY_LEFT				0x0020
	#define KEY_UP					0x0040
	#define KEY_DOWN				0x0080
	#define KEY_R					0x0100
	#define KEY_L					0x0200
	
	/* KEYPAD POSITION INDEXES */
	#define KEY_INDEX_A				0x0
	#define KEY_INDEX_B				0x1
	#define KEY_INDEX_SELECT		0x2
	#define KEY_INDEX_START			0x3
	#define KEY_INDEX_RIGHT			0x4
	#define KEY_INDEX_LEFT			0x5
	#define KEY_INDEX_UP			0x6
	#define KEY_INDEX_DOWN			0x7
	#define KEY_INDEX_R				0x8
	#define KEY_INDEX_L				0x9



#define SOUND3BANK32		0x0000	// Use two banks of 32 steps each
#define SOUND3BANK64		0x0020	// Use one bank of 64 steps
#define SOUND3SETBANK0		0x0000	// Bank to play 0 or 1 (non set bank is written to)
#define SOUND3SETBANK1		0x0040
#define SOUND3PLAY			0x0080	// Output sound

#define SOUND3OUTPUT0		0x0000	// Mute output
#define SOUND3OUTPUT1		0x2000	// Output unmodified
#define SOUND3OUTPUT12		0x4000	// Output 1/2 
#define SOUND3OUTPUT14		0x6000	// Output 1/4 
#define SOUND3OUTPUT34		0x8000  // Output 3/4

#define SOUND3PLAYONCE		0x4000	// Play sound once
#define SOUND3PLAYLOOP		0x0000	// Play sound looped
#define SOUND3INIT			0x8000	// Makes the sound restart

#define REG_SG30       *(vu32*)0x4000070		//???
#define REG_SOUND3CNT  *(vu32*)0x4000070		//???
#define REG_SG30_L     *(vu16*)0x4000070		//???
#define REG_SOUND3CNT_L *(vu16*)0x4000070	//???
#define REG_SG30_H     *(vu16*)0x4000072		//???
#define REG_SOUND3CNT_H *(vu16*)0x4000072	//???
#define REG_SG31       *(vu16*)0x4000074		//???
#define REG_SOUND3CNT_X *(vu16*)0x4000074	//???

#define REG_WAVE_RAM0  *(vu32*)0x4000090		//???
#define REG_SGWR0_L    *(vu16*)0x4000090		//???
#define REG_SGWR0_H    *(vu16*)0x4000092		//???
#define REG_WAVE_RAM1  *(vu32*)0x4000094		//???
#define REG_SGWR1_L    *(vu16*)0x4000094		//???
#define REG_SGWR1_H    *(vu16*)0x4000096		//???
#define REG_WAVE_RAM2  *(vu32*)0x4000098		//???
#define REG_SGWR2_L    *(vu16*)0x4000098		//???
#define REG_SGWR2_H    *(vu16*)0x400009A		//???
#define REG_WAVE_RAM3  *(vu32*)0x400009C		//???
#define REG_SGWR3_L    *(vu16*)0x400009C		//???
#define REG_SGWR3_H    *(vu16*)0x400009E		//???


#define REG_SGCNT0     *(vu32*)0x4000080		
#define REG_SGCNT0_L   *(vu16*)0x4000080		
#define REG_SOUNDCNT   *(vu32*)0x4000080
#define REG_SOUNDCNT_L *(vu16*)0x4000080		//DMG sound control

#define REG_SGCNT0_H   *(vu16*)0x4000082		
#define REG_SOUNDCNT_H *(vu16*)0x4000082		//Direct sound control

#define REG_SGCNT1     *(vu16*)0x4000084		
#define REG_SOUNDCNT_X *(vu16*)0x4000084	    //Extended sound control

#define REG_SGBIAS     *(vu16*)0x4000088		
#define REG_SOUNDBIAS  *(vu16*)0x4000088		//bit rate+sound bias
	
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

	
#endif

