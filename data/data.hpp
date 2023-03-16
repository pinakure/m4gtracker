#ifndef DATA_HPP
#define DATA_HPP

#include "song.hpp" 
#include "input.hpp" 
#include "settings.hpp" 
#include "instrument.hpp" 
#include "config.hpp"
#include "channel.hpp"
#include "regions.hpp"
#include "caches.hpp"
#include "layers.hpp"
#include "control.hpp"

extern MEM_IN_EWRAM Song			VAR_SONG;
extern MEM_IN_EWRAM Input 			VAR_INPUT; 
extern MEM_IN_EWRAM SETTINGS_PWM 	VAR_PWM;
extern MEM_IN_EWRAM SETTINGS_FMW 	VAR_FMW;
extern MEM_IN_EWRAM SETTINGS_SMP 	VAR_SMP;
extern MEM_IN_EWRAM SETTINGS_WAV 	VAR_WAV;
extern MEM_IN_EWRAM Instrument 		VAR_INSTRUMENT;
extern MEM_IN_EWRAM CFG 			VAR_CFG;
extern MEM_IN_EWRAM u8 				VAR_KEY			[ 4 ];
extern MEM_IN_EWRAM Pattern 		VAR_PATTERN		[ CHANNEL_COUNT ];
extern MEM_IN_EWRAM Instrument 		VAR_INSTRUMENTS	[ INSTRUMENT_COUNT ];
extern MEM_IN_EWRAM Channel 		VAR_CHANNEL		[ CHANNEL_COUNT ];
extern MEM_IN_EWRAM PatternCell 	VAR_CELLS		[ CHANNEL_COUNT ]; 
extern MEM_IN_EWRAM PatternCell 	VAR_DATA		[ PATTERN_COUNT ]; 

#endif
