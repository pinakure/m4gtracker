#ifndef _MODULES_KEY
#define _MODULES_KEY

#include "../../agb.h"

#define REG_KEYINPUT    (REG_BASE + 0x130)  // Key Input
#define REG_KEYCNT      (REG_KEYINPUT + 02) // Key Interrupt Control

class KEY{
	public:
		static bool		retrig;
		static vu16		keytrig;
		static u16 		keyinput;
		static u16		keyrate,
						keytimer[10];
		static 	u16		keydown,
						keyup,				
						keyrepeat,
						keypress;
		static void init(void);
		static void update(void);
		static bool down(u16 keyflag);
		static bool up(u16 keyflag);
		static bool press(u16 keyflag);
		static bool repeat(u8 index);
};

#endif
