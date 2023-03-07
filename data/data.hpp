#ifndef DATA_HPP
#define DATA_HPP

#include "song.hpp" 
#include "input.hpp" 
#include "settings.hpp" 
#include "instrument.hpp" 
#include "config.hpp"
#include "live.hpp"
#include "channel.hpp"
#include "regions.hpp"
#include "caches.hpp"
#include "displays.hpp"
#include "controls.hpp"
#include "../callbacks/callbacks.h"

extern MEM_IN_EWRAM Song			VAR_SONG;
extern MEM_IN_EWRAM Song			VAR_SONGS[6];
extern MEM_IN_EWRAM Input 			VAR_INPUT; 
extern MEM_IN_EWRAM u8 				VAR_KEY[4];
extern MEM_IN_EWRAM Pattern 		VAR_PATTERN[6];
extern MEM_IN_EWRAM SETTINGS_PWM 	VAR_PWM;
extern MEM_IN_EWRAM SETTINGS_FMW 	VAR_FMW;
extern MEM_IN_EWRAM SETTINGS_SMP 	VAR_SMP;
extern MEM_IN_EWRAM SETTINGS_WAV 	VAR_WAV;
extern MEM_IN_EWRAM Instrument 		VAR_INSTRUMENTS[64];
extern MEM_IN_EWRAM Instrument 		VAR_INSTRUMENT;
extern MEM_IN_EWRAM CFG 			VAR_CFG;
extern MEM_IN_EWRAM Live 			VAR_LIVE;
extern MEM_IN_EWRAM Channel 		VAR_CHANNEL[6];
extern MEM_IN_EWRAM PatternCell 	VAR_CELLS[6]; 
extern MEM_IN_EWRAM PatternCell 	VAR_DATA[128]; 

#endif
