#ifndef _MODULES_SYS
#define _MODULES_SYS

#include "../../agb.h"

// #define KEYDOWN_A		((SYS::keyboard & 0x0003) == 0x0001)
// #define KEYDOWN_B		((SYS::keyboard & 0x000C) == 0x0004)
// #define KEYDOWN_SELECT	((SYS::keyboard & 0x0030) == 0x0010)
// #define KEYDOWN_START	((SYS::keyboard & 0x00C0) == 0x0040)
// #define KEYDOWN_R		((SYS::keyboard & 0x0300) == 0x0100)
// #define KEYDOWN_L		((SYS::keyboard & 0x0C00) == 0x0400)
// #define KEYDOWN_RIGHT	((SYS::cursor & 0x03) == 0x01)
// #define KEYDOWN_LEFT		((SYS::cursor & 0x0C) == 0x04)
// #define KEYDOWN_UP		((SYS::cursor & 0x30) == 0x10)
// #define KEYDOWN_DOWN		((SYS::cursor & 0xC0) == 0x40)

// #define KEYPRESS_A		((SYS::keyboard & 0x0003) == 0x0003)
// #define KEYPRESS_B		((SYS::keyboard & 0x000C) == 0x000C)
// #define KEYPRESS_SELECT	((SYS::keyboard & 0x0030) == 0x0030)
// #define KEYPRESS_START	((SYS::keyboard & 0x00C0) == 0x00C0)
// #define KEYPRESS_R		((SYS::keyboard & 0x0300) == 0x0300)
// #define KEYPRESS_L		((SYS::keyboard & 0x0C00) == 0x0C00)
// #define KEYPRESS_RIGHT	((SYS::cursor & 0x03) == 0x03)
// #define KEYPRESS_LEFT	((SYS::cursor & 0x0C) == 0x0C)
// #define KEYPRESS_UP		((SYS::cursor & 0x30) == 0x30)
// #define KEYPRESS_DOWN	((SYS::cursor & 0xC0) == 0xC0)

// #define KEYUP_A			((SYS::keyboard & 0x0003) == 0x0002)
// #define KEYUP_B			((SYS::keyboard & 0x000C) == 0x0008)
// #define KEYUP_SELECT		((SYS;:keyboard & 0x0030) == 0x0020)
// #define KEYUP_START		((SYS::keyboard & 0x00C0) == 0x0080)
// #define KEYUP_R			((SYS::keyboard & 0x0300) == 0x0200)
// #define KEYUP_L			((SYS::keyboard & 0x0C00) == 0x0800)
// #define KEYUP_RIGHT		((SYS::cursor & 0x03) == 0x02)
// #define KEYUP_LEFT		((SYS::cursor & 0x0C) == 0x08)
// #define KEYUP_UP			((SYS::cursor & 0x30) == 0x20)
// #define KEYUP_DOWN		((SYS::cursor & 0xC0) == 0x80)
#define KEYDOWN_A			KEY::down(KEY_A)
#define KEYDOWN_B			KEY::down(KEY_B)
#define KEYDOWN_SELECT		KEY::down(KEY_SELECT)
#define KEYDOWN_START		KEY::down(KEY_START)
#define KEYDOWN_R			KEY::down(KEY_R)
#define KEYDOWN_L			KEY::down(KEY_L)
#define KEYDOWN_RIGHT		KEY::down(KEY_RIGHT)
#define KEYDOWN_LEFT		KEY::down(KEY_LEFT)
#define KEYDOWN_UP			KEY::down(KEY_UP)
#define KEYDOWN_DOWN		KEY::down(KEY_DOWN)

#define KEYPRESS_A			KEY::press(KEY_A)
#define KEYPRESS_B			KEY::press(KEY_B)
#define KEYPRESS_SELECT		KEY::press(KEY_SELECT)
#define KEYPRESS_START		KEY::press(KEY_START)
#define KEYPRESS_R			KEY::press(KEY_R)
#define KEYPRESS_L			KEY::press(KEY_L)
#define KEYPRESS_RIGHT		KEY::press(KEY_RIGHT)
#define KEYPRESS_LEFT		KEY::press(KEY_LEFT)
#define KEYPRESS_UP			KEY::press(KEY_UP)
#define KEYPRESS_DOWN		KEY::press(KEY_DOWN)

#define KEYUP_A				KEY::up(KEY_A)
#define KEYUP_B				KEY::up(KEY_B)
#define KEYUP_SELECT		KEY::up(KEY_SELECT)
#define KEYUP_START			KEY::up(KEY_START)
#define KEYUP_R				KEY::up(KEY_R)
#define KEYUP_L				KEY::up(KEY_L)
#define KEYUP_RIGHT			KEY::up(KEY_RIGHT)
#define KEYUP_LEFT			KEY::up(KEY_LEFT)
#define KEYUP_UP			KEY::up(KEY_UP)
#define KEYUP_DOWN			KEY::up(KEY_DOWN)

#define REG_KEYINPUT    	(REG_BASE + 0x130)  // Key Input
#define REG_KEYCNT      	(REG_KEYINPUT + 02) // Key Interrupt Control

class SYS {
	public:
		static bool var_reset;
		
		static u16 keyboard;
		static u8 cursor;
		
		static void init();
		static void reset();
		static void update();
		static void updateInput();
};

#endif
