/* ----------------------------------------------------------------------------
AUTHOR		 	Al P.Area ( Smiker )
PURPOSE			Singleton. 
				Provides easy interfacing with the interrupt registers 
ORIGINAL DATE 	2016, October
REVISION DATE 	2023-02-28
 --------------------------------------------------------------------------- */
#include "int.hpp"
#include "../dma/dma.hpp"
#include "../sys/sys.hpp"

#define DISABLE_INTERRUPTS()	R_IME = 0x0;
#define ENABLE_INTERRUPTS()		R_IME = 0x1;
	
//vu32 SYS_TIMER=0;
//u32 SYS_PROFILEDTIME=0;
//vu32 SYS_PROFILETIMER=0;
//vu32 SYS_FPS=0;
//vu32 SYS_FRAMES=0;
volatile bool SYS_SOUNDTIME=false;
volatile bool SYS_QUERYKEY=false;

/*------------------------------------------------------------------------------
                            Interrupt Class & Handler
--------------------------------------------------------------------------------
 This module contains the class aiming to manage interrupt registers, and the
 rudimentary C Interrupt Manager. An ideal approach would be an ARM Assembler 
 routine to do this task for obvious speed reasons, but let's try out with this 
 approach in C++, as it may work 'as good' as in assembler.					 
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                               Interrupt Handler
--------------------------------------------------------------------------------
 This manager doesn't support dynamic interrupts. So if you want to use the 
 interrupt system for a specific IRQ you'll have to modify the table below 
 manually. Sorry for the inconvenience, but this makes things A LOT faster.	 
 ------------------------------------------------------------------------------*/
#define STAT_VBLANK    		0x0001		// During V Blank
#define STAT_HBLANK     	0x0002		// During H Blank
#define STAT_VCOUNT     	0x0004		// Matching with V Counter
#define STAT_VCOUNT_CM 		0xff00		// VCounter Compare Mask
#define STAT_VCOUNT_CS  	8			// VCounter Compare Shift
#define STAT_VBLANK_IE  	0x0008		// V Blank Interrupt Request, Enable
#define STAT_HBLANK_IE  	0x0010		// H Blank Interrupt Request, Enable
#define STAT_VCOUNT_IE  	0x0020		// V Counter Corresponding 
#define TIM_IRQ				0x0040

#define SENDER_BASE			0x04000000

#define VIDEO_IE			0x0004
#define TIME0_IE			0x0102
#define TIME1_IE			0x0106
#define TIME2_IE			0x010A
#define TIME3_IE			0x010E
#define SIO_IE				0x0128
#define DMA0_IE				0x00BA
#define DMA1_IE				0x00C6
#define DMA2_IE				0x00D2
#define DMA3_IE				0x00DE
#define KEY_IE				0x0132

const u16 IntrSender[28] =  {	
	VIDEO_IE,	STAT_VBLANK_IE, 
	VIDEO_IE,	STAT_VBLANK_IE,
	VIDEO_IE,	STAT_VCOUNT_IE,
	TIME0_IE,	TIM_IRQ,
	TIME1_IE,	TIM_IRQ,
	TIME2_IE,	TIM_IRQ,
	TIME3_IE,	TIM_IRQ,
	SIO_IE,		0x4000,
	DMA0_IE,	0x4000,
	DMA1_IE,	0x4000,
	DMA2_IE,	0x4000,
	DMA3_IE,	0x4000,
	KEY_IE,		0x4000,
	0x0000,		0x0000
};


const u16 IntrFlags[14] = {	
	VBLANK_IF, // IF 0x0001 VBLANK
	HBLANK_IF, // IF 0x0002 HBLANK
	VCOUNT_IF, // IF 0x0004 VCOUNT
	TIMER0_IF, // IF 0x0008 TIMER0
	TIMER1_IF, // IF 0x0010 TIMER1
	TIMER2_IF, // IF 0x0020 TIMER2
	TIMER3_IF, // IF 0x0040 TIMER3
	SIO_IF,	   // IF 0x0080 SIO
	DMA0_IF,   // IF 0x0100 DMA0 
	DMA1_IF,   // IF 0x0200 DMA1 
	DMA2_IF,   // IF 0x0400 DMA2 
	DMA3_IF,   // IF 0x0800 DMA3 
	KEYPAD_IF, // IF 0x1000 KEY 
	AGBPAK_IF  // IF 0x2000 GAMEPA
};

const voidptr IntrTable[14] = {	
	INT::sigVBlank,	// VBLANK ISR
	INT::sigHBlank,	// HBLANK ISR 
	INT::sigVCount, // VCOUNT ISR
	INT::sigTimer0, // TIMER0 ISR
	INT::sigTimer1, // TIMER1 ISR
	INT::sigTimer2, // TIMER2 ISR
	INT::sigTimer3, // TIMER3 ISR
	INT::sigDummy,  // SIO ISR
	INT::sigDummy,  // DMA0 ISR
	INT::sigDummy,  // DMA1 ISR
	INT::sigDummy,  // DMA2 ISR
	INT::sigDummy,  // DMA3 ISR
	INT::sigDummy,  //INT::sigKeyPad, // KEY ISR
	INT::sigAgbPak  // GAMEPAK ISR
};

voidptr	IntrTableBuf[14];
u32     IntrMainBuf[0x1000]; 

extern "C" void int_main();

#define DEFINT(n, flag) \
	if (v & flag) \
	{ \
		R_IF = flag; \
		IntrTable[n](); \
		return; \
	}

extern "C" void IntHandler(){
	u16 v = R_IF;
	//* IMPORTANT NOTE! OTHERWHISE THE INTHANDLER MAY NOT WORK PROPERLY!!!   *//
	//* Remember to always serve first interrupts called less often, if not  *//
	//* The Int handler will only notice the faster irq's, and that's bad.   *//
	DEFINT(0, VBLANK_IF);
	DEFINT(1, HBLANK_IF);
	//DEFINT(2, VCOUNT_IF);
	DEFINT(3, TIMER0_IF);
	//DEFINT(4, TIMER1_IF);
	//DEFINT(5, TIMER2_IF);
	//DEFINT(6, TIMER3_IF);
	//DEFINT(12,KEYPAD_IF);
	DEFINT(13,AGBPAK_IF);
	//STOP(0xA55E47FF);
}
#undef DEFINT

/*------------------------------------------------------------------------------
                          Interrupt Serve Routines 
------------------------------------------------------------------------------*/
void INT::sigDummy(){		
	DISABLE_INTERRUPTS();
	ENABLE_INTERRUPTS();
}

void INT::sigVBlank(){	
/*
We know AGB can do 60 VBlanks per second, so if we count 60 Interrupts,we
also know a second has elapsed, so it's time to calculate the CPS rate
(cycles of program per second), but call it SYS_FPS to be more convenient
*/
	DISABLE_INTERRUPTS();
    SYS::timer++;
	if( SYS::timer == 60 ){
		SYS::fps 	= SYS::frames;
		SYS::frames = 0;
		SYS::timer  = 0;		
	}	
	SYS_SOUNDTIME 	= true;
	REG_IFBIOS 		= VBLANK_IF; //Interrupt ACK!!
	/***************************************************************************/
	SYS_QUERYKEY = true;	
	/***************************************************************************/
	ENABLE_INTERRUPTS();
}

void INT::sigHBlank(){	
	DISABLE_INTERRUPTS();
	REG_IFBIOS = HBLANK_IF;	//Interrupt ACK!!	
	ENABLE_INTERRUPTS();
}

void INT::sigVCount(){	
	DISABLE_INTERRUPTS();
	REG_IFBIOS = VCOUNT_IF;	//Interrupt ACK!!
	ENABLE_INTERRUPTS();
}

void INT::sigTimer0(){
	DISABLE_INTERRUPTS();
	SYS::profile_timer += 1;
	if( SYS::profile_timer < 10 ) SYS::profiled_time = 0;
	REG_IFBIOS = TIMER0_IF;	//Interrupt ACK!!
	ENABLE_INTERRUPTS();
}

void INT::sigTimer1(){
	DISABLE_INTERRUPTS();
	REG_IFBIOS = TIMER1_IF; //Interrupt ACK!!
	ENABLE_INTERRUPTS();
}

void INT::sigTimer2(){
	DISABLE_INTERRUPTS();
	REG_IFBIOS = TIMER2_IF; //Interrupt ACK!!
	ENABLE_INTERRUPTS();
}
	
void INT::sigTimer3(){
	DISABLE_INTERRUPTS();
	REG_IFBIOS = TIMER3_IF; //Interrupt ACK!!
	ENABLE_INTERRUPTS();
}
	
void INT::sigKeyPad(){
	DISABLE_INTERRUPTS();
	//SYS_QUERYKEY = true;
	
	REG_IFBIOS = KEYPAD_IF;	//Interrupt ACK!!
	ENABLE_INTERRUPTS();
}

void INT::sigAgbPak(){
	DISABLE_INTERRUPTS();
	/*should branch into AGBLoop, saving ret address for latter returning*/
	REG_IFBIOS = AGBPAK_IF;	//Interrupt ACK!!
	ENABLE_INTERRUPTS();
}

/*------------------------------------------------------------------------------
                            Interrupt Class Methods
 ------------------------------------------------------------------------------*/
void INT::init(){	
  //DmaClear(3,	0			, XWRAM			, XWRAM_SIZE			, 32); //Clear XWRAM
  //DmaClear(3,	0			, IWRAM			, IWRAM_SIZE - 0x200	, 32); //Clear IWRAM
  //DmaClear(3,	0			, VRAM			, VRAM_SIZE				, 32); //Clear VRAM
  //DmaClear(3, 160			, OAM			, OAM_SIZE				, 32); //Clear OAM
  //DmaClear(3,	0			, PALETTE		, PALETTE_SIZE			, 32); //Clear PALETTE	
	DmaCopy( 3, IntrTable 	, IntrTableBuf	, sizeof(IntrTableBuf)	, 32); //Set IntrTable
	DmaCopy( 3, int_main  	, IntrMainBuf	, sizeof(IntrMainBuf)	, 32); //Set Intr_Main
	*(vu32 *)INTR_VECTOR_BUF = (vu32 )IntrMainBuf;
}

void INT::disable(u8 value){
// Disable specific interrupts	
	R_IME=0;
	R_IE	&=	~IntrFlags[value];	

	u16 flag = IntrSender[(value<<1)+1];
	u32 addr = SENDER_BASE + IntrSender[(value<<1)];
	
	*(vu16*)addr &= ~flag;
	R_IME=1;
}

void INT::enable(u8 value){	
// Enable Specific interrupts
	R_IME=0;
	R_IE	|=	IntrFlags[value];
	
	u16 flag = IntrSender[(value<<1)+1];
	u32 addr = SENDER_BASE + IntrSender[(value<<1)];
	
	*(vu16*)addr |= flag;
	R_IME=1;
}
