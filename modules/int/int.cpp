/*------------------------------------------------------------------------------
                            Interrupt Class & Handler
--------------------------------------------------------------------------------
 This module contains the class whose duty is to manage the interrupt registers,
 and a rudimentary C Interrupt Manager.The ideal thing is supossed to be an ARM
 Assembler routine to do this task for obvious speed reasons, but let's try out
 this approach, maybe it can work as good as in assembler.					 
------------------------------------------------------------------------------*/
cINT INT;
/* ISR Predeclarations */
static void INT_Dummy();
static void INT_VBlank();
static void INT_HBlank();
static void INT_VCount();
static void INT_Timer0();
static void INT_Timer1();
static void INT_Timer2();
static void INT_Timer3();
static void INT_KeyPad();
static void INT_AGBPak();
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

const u16	IntrSender[28] =  {	VIDEO_IE,	STAT_VBLANK_IE, 
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
     

const u16	  IntrFlags[14] = {	VBLANK_IF, // IF 0x0001 VBLANK
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

const voidptr IntrTable[14] = {	INT_VBlank,	// VBLANK ISR
								INT_HBlank,	// HBLANK ISR 
								INT_VCount, // VCOUNT ISR
								INT_Timer0, // TIMER0 ISR
								INT_Timer1, // TIMER1 ISR
								INT_Timer2, // TIMER2 ISR
								INT_Timer3, // TIMER3 ISR
								INT_Dummy,  // SIO ISR
								INT_Dummy,  // DMA0 ISR
								INT_Dummy,  // DMA1 ISR
								INT_Dummy,  // DMA2 ISR
								INT_Dummy,  // DMA3 ISR
								INT_Dummy, //INT_KeyPad, // KEY ISR
								INT_AGBPak  // GAMEPAK ISR
							  };
int Toggle=0;

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
static void INT_Dummy(){		
}

#include "../spu/mixer.hpp"

static void INT_VBlank(){	
/*
We know AGB can do 60 VBlanks per second, so if we count 60 Interrupts,we
also know a second has elapsed, so it's time to calculate the CPS rate
(cycles of program per second), but call it SYS_FPS to be more convenient
*/
    R_IME=0x0;
	SYS_TIMER++;
	if(SYS_TIMER==60)
	{
		SYS_FPS=SYS_FRAMES;
		SYS_FRAMES = 0;
		SYS_TIMER=0;		
	}
	
	// From M0-K1
	if(sound_buffer.activeBuffer == 1)	// buffer 1 just got over
	{
	   // Start playing buffer 0
	  DMA1_CHI = 0;
	  DMA1_SRC = (u32)sound_buffer.mixBufferBase;
	  DMA1_CHI = 0xB640;

	   // Set the current buffer pointer to the start of buffer 1
	  sound_buffer.curMixBuffer = sound_buffer.mixBufferBase + sound_buffer.mixBufferSize;

	  sound_buffer.activeBuffer = 0;
	}
	else	// buffer 0 just got over
	{
	   // DMA points to buffer 1 already, so don't bother stopping and resetting it

	   // Set the current buffer pointer to the start of buffer 0
	  sound_buffer.curMixBuffer = sound_buffer.mixBufferBase;
	  sound_buffer.activeBuffer = 1;
	}
	
	SYS_SOUNDTIME=true;
	REG_IFBIOS = VBLANK_IF; //Interrupt ACK!!
	/***************************************************************************/
	SYS_QUERYKEY = true;	
	/***************************************************************************/
	R_IME=0x1;
	
	return Mixer::mix();
}

static void INT_HBlank(){	
	R_IME=0x0;	
	REG_IFBIOS = HBLANK_IF;	//Interrupt ACK!!	
	R_IME=0x1;
}

static void INT_VCount(){	
	R_IME=0x0;	
	REG_IFBIOS = VCOUNT_IF;	//Interrupt ACK!!
	R_IME=0x1;
}

extern void fmwRenderInterrupt();

static void INT_Timer0(){
	R_IME=0x0;
	SYS_PROFILETIMER+=1;
	if(SYS_PROFILETIMER<10)SYS_PROFILEDTIME=0;
	REG_IFBIOS = TIMER0_IF;	//Interrupt ACK!!
	R_IME=0x1;
}

static void INT_Timer1(){
	R_IME=0x0;
	//fmwRenderInterrupt();
	REG_IFBIOS = TIMER1_IF; //Interrupt ACK!!
	R_IME=0x1;
}

static void INT_Timer2(){
	R_IME=0x0;
	REG_IFBIOS = TIMER2_IF; //Interrupt ACK!!
	R_IME=0x1;
}
	
static void INT_Timer3(){
	R_IME=0x0;
	REG_IFBIOS = TIMER3_IF; //Interrupt ACK!!
	R_IME=0x1;
}
	
static void INT_KeyPad(){
	R_IME=0x0;
	//SYS_QUERYKEY = true;
	
	REG_IFBIOS = KEYPAD_IF;	//Interrupt ACK!!
	R_IME=0x1;
}

static void INT_AGBPak(){
	R_IME=0;
	/*should branch into AGBLoop, saving ret address for latter returning*/
	REG_IFBIOS = AGBPAK_IF;	//Interrupt ACK!!
	R_IME=1;
}


/*------------------------------------------------------------------------------
                            Interrupt Class Methods
 ------------------------------------------------------------------------------*/
void cINT::Init(){	
  //DmaClear(3,	0			, XWRAM			, XWRAM_SIZE			, 32); //Clear XWRAM
  //DmaClear(3,	0			, IWRAM			, IWRAM_SIZE - 0x200	, 32); //Clear IWRAM
	DmaClear(3,	0			, VRAM			, VRAM_SIZE				, 32); //Clear VRAM
	DmaClear(3, 160			, OAM			, OAM_SIZE				, 32); //Clear OAM
	DmaClear(3,	0			, PALETTE		, PALETTE_SIZE			, 32); //Clear PALETTE	
	DmaCopy( 3, IntrTable 	, IntrTableBuf	, sizeof(IntrTableBuf)	, 32); //Set IntrTable
	DmaCopy( 3, int_main  	, IntrMainBuf	, sizeof(IntrMainBuf)	, 32); //Set Intr_Main
	*(vu32 *)INTR_VECTOR_BUF = (vu32 )IntrMainBuf;
}

void cINT::Disable(u8 value){
// Disable specific interrupts	
	R_IME=0;
	R_IE	&=	~IntrFlags[value];	

	u16 flag = IntrSender[(value<<1)+1];
	u32 addr = SENDER_BASE + IntrSender[(value<<1)];
	
	*(vu16*)addr &= ~flag;
	R_IME=1;
}

void cINT::Enable(u8 value){	
// Enable Specific interrupts
	R_IME=0;
	R_IE	|=	IntrFlags[value];
	
	u16 flag = IntrSender[(value<<1)+1];
	u32 addr = SENDER_BASE + IntrSender[(value<<1)];
	
	*(vu16*)addr |= flag;
	R_IME=1;
}
