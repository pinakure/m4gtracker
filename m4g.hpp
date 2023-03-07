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

#include "agb.h"

#define CONTROL_TERMINATOR			{ 0xFF, 0xFF, NULL, NULL, NULL, NULL, NULL, NULL, NULL }

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

#include "data/enum.h"
#include "data/helpers.hpp"

#define BUTTON_ALTER		KEY_A	
#define BUTTON_NAVIGATE	KEY_SELECT

#include "data/datatypes.h"

/* Display update controller Callbacks (@ callbacks.c) */


void RECEIVESONG(Control *c, bool bigstep, bool add, u32 *pointer);
void SENDSONG(Control *c, bool bigstep, bool add, u32 *pointer);

void MIXER(Control *c, bool bigstep, bool add, u32 *pointer);

void MEMORYTEST(Control *c, bool bigstep, bool add, u32 *pointer);
void REINITIALIZE(Control *c, bool bigstep, bool add, u32 *pointer);
void FORMATMEMORY(Control *c, bool bigstep, bool add, u32 *pointer);
void RESET(Control *c, bool bigstep, bool add, u32 *pointer);

void LOADCONFIG(Control *c, bool bigstep, bool add, u32 *pointer);
void SAVECONFIG(Control *c, bool bigstep, bool add, u32 *pointer);
void DEFAULTCONFIG(Control *c, bool bigstep, bool add, u32 *pointer);

void ALPHA14(Control *c, bool bigstep, bool add, u32 *pointer);
void ALPHA6(Control *c, bool bigstep, bool add, u32 *pointer);

void BPMUPDATE(Control *c, bool bigstep, bool add, u32 *pointer);

/* Variable Operator callbacks */
void modify8BIT(Control*, bool bigstep, bool add, u32 *pointer);


#define EXTRACT(packed, position, mask)		((packed >> position) & mask)
#define PORT(a)		*((u8*)a)
#define M4G_VERSION			0x81
#define M4GEEK_SIGNATURE	0xE5ACFECA

#define EXPECT(a, up, down)			{VAR_CFG.loadCount = 0;regHnd.progress.set(0, a, STATUS_##up, STATUS_##down, &VAR_CFG.loadCount);}
#define OK()						{VAR_CFG.loadCount++;regHnd.update(1);	DECIMAL_DOUBLE(28,1,9, SRAM.position-1);}


#define PRINTPOINTER(x, y, c, a)	TEXT(x,y, 0x6, c-13); \
						HEXADECIMAL_DOUBLE(x+1,y, 0x6, (((unsigned)a)&0xFF000000) >> 24); \
						HEXADECIMAL_DOUBLE(x+2,y, 0x6, (((unsigned)a)&0x00FF0000) >> 16); \
						HEXADECIMAL_DOUBLE(x+3,y, 0x6, (((unsigned)a)&0x0000FF00) >>  8); \
						HEXADECIMAL_DOUBLE(x+4,y, 0x6, ((unsigned)a)&0x000000FF);

#define PRINTVAR(x, y, c, a)		TEXT(x,y, 0x6, c-13); \
						HEXADECIMAL_DOUBLE(x+1,y, 6, a);

#include "data/variables.h"
#include "callbacks/callbacks.h"



#include "data/tables.hpp"
#include "data/caches.hpp"
#include "data/controls.hpp"
#include "data/displays.hpp"
#include "data/regions.hpp"
#include "data/viewports.hpp"
#include "data/layers.hpp"
#include "data/tileset.hpp"
#include "data/palette.hpp"

// Global variables
#include "data/data.hpp"

#endif
