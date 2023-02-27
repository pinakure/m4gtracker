#ifndef _M4G_HPP
#define _M4G_HPP
/*-----------------------------------------------------------------------------\
|\                   \\_           /\_     \''\''''l                          /|
|\                   \ \_        / \ \_    \  \   l                           /|
|\                   \  \_     /   \  \    \  \  l    _____                   /|
|\                   \   \_   \    \  \    \  \ l   /     \                   /|
|\                   \    \_  \    \  \    \  \ \  \_     \_                  /|
|\                   \     \__\    \  \___/   \ \   \____  /                  /|
|\                   \             \_         \ \       \ \                   /|
|\                   \    _     _  \\_        \ \_______/ \                   /|
|\                   \  / \_   \\_ \ \______  \         ,/                    /|
|\                   \ \   \__/  \ \       \  \       ,/                      /|
|\--------------------\/----------\/--------\_/\______/-----------------------/|
|\================== =   =     =  T R A C K E R  =     =   =  = = ============/|
|\  M4Geek, 2015-2015														  /|
|\                     Project Codename:    M4GEEK-01                         /|
|\                     Project Startday: 21 / 08 / 2015                       /|
|\  CODER:                                                               /|
|\    Smiker (Albert P.)                                                      /|
|\  C ASSESSOR:                                                               /|
|\    Mitikoro (Mike L.)                                                      /|
|\  GFX ARTIST:                                                               /|
|\    Smiker (Albert P.)                                                      /|
|\    ilKkE  (Ilijah M.)                                                      /|
|\  BETA TESTERS:                                                             /|
|\                                                                            /|
\-----------------------------------------------------------------------------*/

#include "../agb.h"


//MEM_FUNC_IN_IWRAM

// Global callbacks and vars

extern vu32 SYS_TIMER;						 //Global program timer
extern volatile bool SYS_QUERYKEY;			 //True when a key is triggered
extern int main(void);						 //Needed by INTERRUPTS!!!
extern u32	SYS_PROFILEDTIME;				 //Profiled time (Debug Only)
extern volatile bool SYS_SOUNDTIME;			 //Sound processing flag
extern vu32 SYS_PROFILETIMER;				 //Profiling timer (Debug Only)
extern vu32 SYS_FPS;						 //Frames(program cycles) / sec
extern vu32 SYS_FRAMES;						 //Current framecount	

enum E_StatusStrings {
	STATUS_SAVING,
	STATUS_LOADING,
	STATUS_RECEIVING,
	STATUS_SENDING,
	STATUS_SYNCWITH,
	STATUS_RESTORING,
	STATUS_PURGING,
	STATUS_TESTING,
	STATUS_REINIT,
	STATUS_CELLS,
	STATUS_DEFAULTS,
	STATUS_INSTDATA,
	STATUS_MASTER,
	STATUS_MEMORY,
	STATUS_MIDIIN,
	STATUS_MIDIOUT,
	STATUS_PATTERNS,
	STATUS_SETTINGS,
	STATUS_SLAVE,
	STATUS_SONG,
	STATUS_CORRUPTED
};

enum E_InstrumentTypes {
	INSTRUMENT_TYPE_PWM,
	INSTRUMENT_TYPE_WAV,
	INSTRUMENT_TYPE_FMW,
	INSTRUMENT_TYPE_SMP,
	INSTRUMENT_TYPE_COUNT
};

void PIXEL(u8 x, u8 y, u16 color, u16 value);
void VISPOS2(u8 x, u8 y, u16 color, u16 value);
void VISPOS1(u8 x, u8 y, u16 color, u16 value);
void NOTE(u8 x, u8 y, u16 color, u16 value);
void TABLEPOS(u8 x, u8 y, u16 color, u16 value);
void COMMAND(u8 x, u8 y, u16 color, u16 value);	
void VUMETER_V1(u8 x, u8 y, u16 color, u16 value);	
void VUMETER_H1(u8 x, u8 y, u16 color, u16 value);	
void VUMETER_H3(u8 x, u8 y, u16 color, u16 value);
void VUMETER_V3(u8 x, u8 y, u16 color, u16 value);
void TEXT(u8 x, u8 y, u16 color, u16 value);
void BIGTEXT(u8 x, u8 y, u16 color, u16 value);
void BIGDEC(u8 x, u8 y, u16 color, u16 value);
void BIGHEX(u8 x, u8 y, u16 color, u16 value);
void NIBBLE(u8 x, u8 y, u16 color, u16 value);
void WAVE_SINGLE(u8 x, u8 y, u16 color, u16 value);
void WAVE_DOUBLE(u8 x, u8 y, u16 color, u16 value);
void PROGRESS(u8 x, u8 y, u16 color, u16 value);
void STATUS(u8 x, u8 y, u16 color, u16 value);
void HEXADECIMAL(u8 x, u8 y, u16 color, u16 value);
void HEXADECIMAL_DOUBLE(u8 x, u8 y, u16 color, u16 value);
void HEXADECIMAL_TWOTILES(u8 x, u8 y, u16 color, u16 value);
void HEXADECIMAL_INSTRUMENT(u8 x, u8 y, u16 color, u16 value);
void DECIMAL(u8 x, u8 y, u16 color, u16 value);
void DECIMAL_DOUBLE(u8 x, u8 y, u16 color, u16 value);
void DECIMAL_TWOTILES(u8 x, u8 y, u16 color, u16 value);
void DECIMAL_DOUBLE_TWOTILES(u8 x, u8 y, u16 color, u16 value);

#define BUTTON_ALTER		KEY_A	
#define BUTTON_NAVIGATE	KEY_SELECT

#include "../data/enum.h"
#include "../data/datatypes.h"

/* Display update controller Callbacks (@ callbacks.c) */
void updateLIVE2(REGHND* rh);
void updateLIVE1(REGHND* rh);
void updateHLP(REGHND* rh);
void updateSNG(REGHND* rh);
void updateINS_WAV(REGHND* rh);
void updateINS_FMW(REGHND* rh);
void updateINS_SMP(REGHND* rh);
void updateINS_PWM(REGHND* rh);
void updatePAT(REGHND* rh);
void updateTABLE(REGHND* rh);
void updateVIS(REGHND* rh);
void updateHEADER1(REGHND* rh);
void updateHEADER0(REGHND* rh);
void updateCHANNEL0(REGHND* rh);
void updateCHANNEL1(REGHND* rh);
void updateCHANNEL2(REGHND* rh);
void updateCHANNEL3(REGHND* rh);
void updateCHANNEL4(REGHND* rh);
void updateCHANNEL5(REGHND* rh);
void updateSNK(REGHND* rh);
void updateLOOKNFEEL(REGHND* rh);
void updateLINKMODE(REGHND* rh);
void updateBEHAVIOR(REGHND* rh);
void updateTRACKER(REGHND* rh);
void updateMEMORY(REGHND* rh);
void updateCOLOREDITOR(REGHND* rh);
void updateCHANNELMIXER(REGHND* rh);
void updateMEMORYSONGMAP(REGHND* rh);
void updateMEMORYTEST(REGHND* rh);
void updateLINKSTATUS(REGHND* rh);
void updateCREDITS(REGHND* rh);
void updatePROGRESS(REGHND* rh);

/* Button Callbacks (callbacks.c) */
void SOLO0(Control *c, bool bigstep, bool add, u32 *pointer);
void SOLO1(Control *c, bool bigstep, bool add, u32 *pointer);
void SOLO2(Control *c, bool bigstep, bool add, u32 *pointer);
void SOLO3(Control *c, bool bigstep, bool add, u32 *pointer);
void SOLO4(Control *c, bool bigstep, bool add, u32 *pointer);
void SOLO5(Control *c, bool bigstep, bool add, u32 *pointer);
void MUTE0(Control *c, bool bigstep, bool add, u32 *pointer);
void MUTE1(Control *c, bool bigstep, bool add, u32 *pointer);
void MUTE2(Control *c, bool bigstep, bool add, u32 *pointer);
void MUTE3(Control *c, bool bigstep, bool add, u32 *pointer);
void MUTE4(Control *c, bool bigstep, bool add, u32 *pointer);
void MUTE5(Control *c, bool bigstep, bool add, u32 *pointer);

void LOADWAVPRESET0(Control *c, bool bigstep, bool add, u32 *pointer);
void LOADWAVPRESET1(Control *c, bool bigstep, bool add, u32 *pointer);
void LOADWAVPRESET2(Control *c, bool bigstep, bool add, u32 *pointer);
void LOADWAVPRESET3(Control *c, bool bigstep, bool add, u32 *pointer);
void LOADWAVPRESET4(Control *c, bool bigstep, bool add, u32 *pointer);
void LOADWAVPRESET5(Control *c, bool bigstep, bool add, u32 *pointer);

void COLOREDITOR(Control *c, bool bigstep, bool add, u32 *pointer);

void RECEIVESONG(Control *c, bool bigstep, bool add, u32 *pointer);
void SENDSONG(Control *c, bool bigstep, bool add, u32 *pointer);

void MIXER(Control *c, bool bigstep, bool add, u32 *pointer);

void SLOTUSAGE(Control *c, bool bigstep, bool add, u32 *pointer);
void PURGESONGS(Control *c, bool bigstep, bool add, u32 *pointer);
void MEMORYTEST(Control *c, bool bigstep, bool add, u32 *pointer);
void REINITIALIZE(Control *c, bool bigstep, bool add, u32 *pointer);
void FORMATMEMORY(Control *c, bool bigstep, bool add, u32 *pointer);
void RESET(Control *c, bool bigstep, bool add, u32 *pointer);

void LOADCONFIG(Control *c, bool bigstep, bool add, u32 *pointer);
void SAVECONFIG(Control *c, bool bigstep, bool add, u32 *pointer);
void DEFAULTCONFIG(Control *c, bool bigstep, bool add, u32 *pointer);

void LOADSLOT(Control *c, bool bigstep, bool add, u32 *pointer);
void SAVESLOT(Control *c, bool bigstep, bool add, u32 *pointer);
void PURGESLOT(Control *c, bool bigstep, bool add, u32 *pointer);
void COPYSLOT(Control *c, bool bigstep, bool add, u32 *pointer);
void ERASESLOT(Control *c, bool bigstep, bool add, u32 *pointer);
void TEMPOTAP(Control *c, bool bigstep, bool add, u32 *pointer);
void ALPHA14(Control *c, bool bigstep, bool add, u32 *pointer);
void ALPHA6(Control *c, bool bigstep, bool add, u32 *pointer);

void BPMUPDATE(Control *c, bool bigstep, bool add, u32 *pointer);

/* Variable Operator callbacks */
void modify1BIT(Control*, bool bigstep, bool add, u32 *pointer);
void modify2BIT(Control*, bool bigstep, bool add, u32 *pointer);
void modify3BIT(Control*, bool bigstep, bool add, u32 *pointer);
void modify4BIT(Control*, bool bigstep, bool add, u32 *pointer);
void modify5BIT(Control*, bool bigstep, bool add, u32 *pointer);
void modify6BIT(Control*, bool bigstep, bool add, u32 *pointer);
void modify8BIT(Control*, bool bigstep, bool add, u32 *pointer);
void modifyTEMPO(Control*, bool bigstep, bool add, u32 *pointer);
void modifyCHAR(Control*, bool bigstep, bool add, u32 *pointer); // Append char received to the string, if room left
void modifyNOTE(Control*, bool bigstep, bool add, u32 *pointer);
void modify3VAL(Control*, bool bigstep, bool add, u32 *pointer); // For variables which can take up do 3 different values
void modify5VAL(Control*, bool bigstep, bool add, u32 *pointer); // For variables which can take up do 5 different values
void modify6VAL(Control*, bool bigstep, bool add, u32 *pointer); // For variables which can take up to 6 different values
void modify27VAL(Control*, bool bigstep, bool add, u32 *pointer); 


void instLoad(void);
void patternSync(u8 position);
void cellSync(void);


void instrument1BIT(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument2BIT(Control *c, bool bigstep, bool add, u32 *pointer);
void instrumentType(Control *c, bool bigstep, bool add, u32 *pointer);
void instrumentIndex(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument3BIT(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument4BIT(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument5BIT(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument6BIT(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument7BIT(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument8BIT(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument3VAL(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument6VAL(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument5VAL(Control *c, bool bigstep, bool add, u32 *pointer);
void instrument27VAL(Control *c,bool bigstep, bool add, u32 *pointer);

#define EXTRACT(packed, position, mask)		((packed >> position) & mask)
#define PORT(a)		*((u8*)a)
#define M4G_VERSION			0x81
#define M4GEEK_SIGNATURE	0xE5ACFECA

#define EXPECT(a, up, down)			{CFG::loadCount = 0;REGHND::progress.set(0, a, STATUS_##up, STATUS_##down, &CFG::loadCount);}
#define OK()						{CFG::loadCount++;REGHND::update(1);	DECIMAL_DOUBLE(28,1,9, SRAM.position-1);}


#define PRINTPOINTER(x, y, c, a)	TEXT(x,y, 0x6, c-13); \
						HEXADECIMAL_DOUBLE(x+1,y, 0x6, (((unsigned)a)&0xFF000000) >> 24); \
						HEXADECIMAL_DOUBLE(x+2,y, 0x6, (((unsigned)a)&0x00FF0000) >> 16); \
						HEXADECIMAL_DOUBLE(x+3,y, 0x6, (((unsigned)a)&0x0000FF00) >>  8); \
						HEXADECIMAL_DOUBLE(x+4,y, 0x6, ((unsigned)a)&0x000000FF);

#define PRINTVAR(x, y, c, a)		TEXT(x,y, 0x6, c-13); \
						HEXADECIMAL_DOUBLE(x+1,y, 6, a);

#include "../data/variables.h"
#include "../callbacks/callbacks.h"



#include "../data/tables.c"
#include "../data/caches.c"
#include "../data/controls.c"
#include "../data/displays.c"
#include "../data/regions.c"
#include "../data/viewports.c"
#include "../data/layers.c"
#include "../data/tileset.c"
#include "../data/palette.c"
#include "../data/help.c"

// Global variables

vu32 SYS_TIMER=0;
u32 SYS_PROFILEDTIME=0;
vu32 SYS_PROFILETIMER=0;
vu32 SYS_FPS=0;
vu32 SYS_FRAMES=0;
volatile bool SYS_SOUNDTIME=false;
u8 SYS_ACTIVESCREEN=0;
u8 SYS_PARAM=0;
bool SYS_SYNCTRACKER = false;
volatile bool SYS_QUERYKEY=false;
MEM_IN_EWRAM SONG			VAR_SONG;
MEM_IN_EWRAM SONG			VAR_SONGS[6];
MEM_IN_EWRAM INPUT 			VAR_INPUT; 
MEM_IN_EWRAM u8 			VAR_KEY[4];
MEM_IN_EWRAM PATTERN 		VAR_PATTERN[6];
MEM_IN_EWRAM SETTINGS_PWM 	VAR_PWM;
MEM_IN_EWRAM SETTINGS_FMW 	VAR_FMW;
MEM_IN_EWRAM SETTINGS_SMP 	VAR_SMP;
MEM_IN_EWRAM SETTINGS_WAV 	VAR_WAV;
MEM_IN_EWRAM INSTRUMENT 	VAR_INSTRUMENTS[64];
MEM_IN_EWRAM INSTRUMENT 	VAR_INSTRUMENT;
MEM_IN_EWRAM CFG 			VAR_CFG;
MEM_IN_EWRAM LIVE 			VAR_LIVE;
MEM_IN_EWRAM CHANNEL 		VAR_CHANNEL[6];
MEM_IN_EWRAM PATTERN_CELL 	VAR_CELLS[6]; 
MEM_IN_EWRAM PATTERN_CELL 	VAR_DATA[128]; 

#endif
