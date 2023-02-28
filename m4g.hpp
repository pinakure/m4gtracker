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

#include "macros.hpp"

#include "data/enum.h"
#include "data/datatypes.h"

/* Display update controller Callbacks (@ callbacks.c) */
void updateLIVE2();
void updateLIVE1();
void updateHLP();
void updateSNG();
void updateINS_WAV();
void updateINS_FMW();
void updateINS_SMP();
void updateINS_PWM();
void updatePAT();
void updateTABLE();
void updateVIS();
void updateHEADER1();
void updateHEADER0();
void updateCHANNEL0();
void updateCHANNEL1();
void updateCHANNEL2();
void updateCHANNEL3();
void updateCHANNEL4();
void updateCHANNEL5();
void updateSNK();
void updateLOOKNFEEL();
void updateLINKMODE();
void updateBEHAVIOR();
void updateTRACKER();
void updateMEMORY();
void updateCOLOREDITOR();
void updateCHANNELMIXER();
void updateMEMORYSONGMAP();
void updateMEMORYTEST();
void updateLINKSTATUS();
void updateCREDITS();
void updatePROGRESS();

#include "data/variables.hpp"
#include "callbacks/callbacks.hpp"

#include "data/tables.hpp"

#include "data/layers.hpp"
#include "data/tileset.hpp"
#include "data/palette.hpp"

#include "data/regions.hpp"

// Global variables

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
