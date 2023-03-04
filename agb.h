/** AGBLIB - M4GEEK 2011**/
/** Based on Emanuel Schleussinger's HAMLIB's mygba.h file which is also based (and uncredited >:< )
	on NO$GBA's low level documentation, Nintendo AGB SDK, and devkitGBA. I'm abstracting some defines, just those 
	needed to do all stuff, trying to not rely on HAM at all, so these common defines aren't but property of Nintendo,
	so feel free to use this libary without giving credit to anybody but you ;) **/
#ifndef AGBLIB_H
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
	
	
#endif

