#ifndef GROOVETABLE_HPP
#define GROOVETABLE_HPP

#include "../agb.h"

typedef struct sGrooveTable {
	u8				STEP[16] /*8*/;
	u8				ENABLE;
	u8				LENGTH /*4*/;
	u8				position; // TRANSIENT
}GrooveTable;

#endif 
