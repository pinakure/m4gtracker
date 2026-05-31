#ifndef LIVE_HPP
#define LIVE_HPP

#include "../agb.h"

typedef struct sLiveTable {
	u8				KEY[ 8] /*7*/;
	u8				INS[ 8] /*6*/;
	u8				CHAN[8] /*3*/;
	u8				VOL[ 8] /*4*/;
	u8				CMD[ 8] /*5*/;
	u8				VAL[ 8];
} LiveTable;

typedef struct sPerformance {
	LiveTable		LEFT;
	LiveTable		RIGHT;
	u8 				RETRIG;
	u8 				SPEED /*4*/;
	u8	 			QUANTIZE /*3*/;
	u8				LOCK;
} Performance;

typedef struct sPianoRoll {
	u8				MODE /*2*/;
	u8				OCTAVE /*4*/;
	u8				QUANTIZE /*3*/;
	u8				TRANSPOSE;
	u8				CHANNEL[2];
	u8				MIDICHAN[2];
} PianoRoll;

typedef struct sLive {
	Performance		PERFORM;
	PianoRoll		PIANO;
	u8	 			KEYPRESS[10];
} Live;

extern Live VAR_LIVE;

#endif
