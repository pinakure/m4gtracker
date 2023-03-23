#ifndef INT_HPP
#define INT_HPP

#include "../../agb.h"

#define ADR_REG_IFBIOS			0x03007FF8
#define REG_IFBIOS				*(vu16*)ADR_REG_IFBIOS//Interrupt ACK

/* Interrupt Flags */
#define VBLANK_IF		0x0001		// V Blank Interrupt
#define HBLANK_IF		0x0002		// H Blank Interrupt
#define VCOUNT_IF		0x0004		// V Counter Corresponding Interrupt
#define TIMER0_IF		0x0008		// Timer 0 Interrupt
#define TIMER1_IF		0x0010		// Timer 1 Interrupt
#define TIMER2_IF		0x0020		// Timer 2 Interrupt
#define TIMER3_IF		0x0040		// Timer 3 Interrupt
#define SIO_IF			0x0080		// Serial Communication Interrupt
#define DMA0_IF			0x0100		// DMA0 Interrupt
#define DMA1_IF			0x0200		// DMA1 Interrupt
#define DMA2_IF			0x0400		// DMA2 Interrupt
#define DMA3_IF			0x0800		// DMA3 Interrupt
#define KEYPAD_IF		0x1000		// Key Interrupt
#define AGBPAK_IF		0x2000		// Game Pak Interrupt

/* DISPSTAT IF Values */
#define STAT_VBLANK     0x0001		// During V Blank
#define STAT_HBLANK     0x0002		// During H Blank
#define STAT_VCOUNT     0x0004		// Matching with V Counter
#define STAT_VCOUNT_CM  0xff00		// VCounter Compare Mask
#define STAT_VCOUNT_CS  8			// VCounter Compare Shift
#define STAT_VBLANK_IE  0x0008		// V Blank Interrupt Request, Enable
#define STAT_HBLANK_IE  0x0010		// H Blank Interrupt Request, Enable
#define STAT_VCOUNT_IE  0x0020		// V Counter Corresponding 
	
class Interrupt {	
	public:
		static void init		();
		static void disable	( u8 value );
		static void enable		( u8 value );
};

extern volatile bool 	SYS_QUERYKEY;		 //True when a key is triggered
extern u32				SYS_PROFILEDTIME;				 //Profiled time (Debug Only)
extern volatile bool 	SYS_SOUNDTIME;		 //Sound processing flag
extern vu32 				SYS_PROFILETIMER;			 //Profiling timer (Debug Only)
extern vu32 				SYS_FPS;						 //Frames(program cycles) / sec
extern vu32 				SYS_FRAMES;					 //Current framecount	
extern vu32 				SYS_TIMER;					 //Global program timer

#endif 
