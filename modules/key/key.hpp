#ifndef KEY_HPP
#define KEY_HPP

#include "../../agb.h"

// #define KEYDOWN_A			((Sys::keyboard & 0x0003) == 0x0001)
// #define KEYDOWN_B			((Sys::keyboard & 0x000C) == 0x0004)
// #define KEYDOWN_SELECT		((Sys::keyboard & 0x0030) == 0x0010)
// #define KEYDOWN_START		((Sys::keyboard & 0x00C0) == 0x0040)
// #define KEYDOWN_R			((Sys::keyboard & 0x0300) == 0x0100)
// #define KEYDOWN_L			((Sys::keyboard & 0x0C00) == 0x0400)
// #define KEYDOWN_RIGHT		((Sys::cursor & 0x03) == 0x01)
// #define KEYDOWN_LEFT			((Sys::cursor & 0x0C) == 0x04)
// #define KEYDOWN_UP			((Sys::cursor & 0x30) == 0x10)
// #define KEYDOWN_DOWN			((Sys::cursor & 0xC0) == 0x40)

// #define KEYPRESS_A			((Sys::keyboard & 0x0003) == 0x0003)
// #define KEYPRESS_B			((Sys::keyboard & 0x000C) == 0x000C)
// #define KEYPRESS_SELECT		((Sys::keyboard & 0x0030) == 0x0030)
// #define KEYPRESS_START		((Sys::keyboard & 0x00C0) == 0x00C0)
// #define KEYPRESS_R			((Sys::keyboard & 0x0300) == 0x0300)
// #define KEYPRESS_L			((Sys::keyboard & 0x0C00) == 0x0C00)
// #define KEYPRESS_RIGHT		((Sys::cursor & 0x03) == 0x03)
// #define KEYPRESS_LEFT		((Sys::cursor & 0x0C) == 0x0C)
// #define KEYPRESS_UP			((Sys::cursor & 0x30) == 0x30)
// #define KEYPRESS_DOWN		((Sys::cursor & 0xC0) == 0xC0)

// #define KEYUP_A				((Sys::keyboard & 0x0003) == 0x0002)
// #define KEYUP_B				((Sys::keyboard & 0x000C) == 0x0008)
// #define KEYUP_SELECT			((Sys::keyboard & 0x0030) == 0x0020)
// #define KEYUP_START			((Sys::keyboard & 0x00C0) == 0x0080)
// #define KEYUP_R				((Sys::keyboard & 0x0300) == 0x0200)
// #define KEYUP_L				((Sys::keyboard & 0x0C00) == 0x0800)
// #define KEYUP_RIGHT			((Sys::cursor & 0x03) == 0x02)
// #define KEYUP_LEFT			((Sys::cursor & 0x0C) == 0x08)
// #define KEYUP_UP				((Sys::cursor & 0x30) == 0x20)
// #define KEYUP_DOWN			((Sys::cursor & 0xC0) == 0x80)

#define KEYDOWN_A				Key::down(KEY_A)
#define KEYDOWN_B				Key::down(KEY_B)
#define KEYDOWN_SELECT			Key::down(KEY_SELECT)
#define KEYDOWN_START			Key::down(KEY_START)
#define KEYDOWN_R				Key::down(KEY_R)
#define KEYDOWN_L				Key::down(KEY_L)
#define KEYDOWN_RIGHT			Key::down(KEY_RIGHT)
#define KEYDOWN_LEFT			Key::down(KEY_LEFT)
#define KEYDOWN_UP				Key::down(KEY_UP)
#define KEYDOWN_DOWN			Key::down(KEY_DOWN)
#define KEYPRESS_A				Key::press(KEY_A)
#define KEYPRESS_B				Key::press(KEY_B)
#define KEYPRESS_SELECT			Key::press(KEY_SELECT)
#define KEYPRESS_START			Key::press(KEY_START)
#define KEYPRESS_R				Key::press(KEY_R)
#define KEYPRESS_L				Key::press(KEY_L)
#define KEYPRESS_RIGHT			Key::press(KEY_RIGHT)
#define KEYPRESS_LEFT			Key::press(KEY_LEFT)
#define KEYPRESS_UP				Key::press(KEY_UP)
#define KEYPRESS_DOWN			Key::press(KEY_DOWN)
#define KEYUP_A					Key::up(KEY_A)
#define KEYUP_B					Key::up(KEY_B)
#define KEYUP_SELECT			Key::up(KEY_SELECT)
#define KEYUP_START				Key::up(KEY_START)
#define KEYUP_R					Key::up(KEY_R)
#define KEYUP_L					Key::up(KEY_L)
#define KEYUP_RIGHT				Key::up(KEY_RIGHT)
#define KEYUP_LEFT				Key::up(KEY_LEFT)
#define KEYUP_UP				Key::up(KEY_UP)
#define KEYUP_DOWN				Key::up(KEY_DOWN)
#define KEYUPDATE				Key::update
#define KEYFORCENOINPUT			Key::forceNoInput
#define KEYACTIVITY				Key::activity
#define KEYINIT					Key::init

#define REG_KEYINPUT    		(REG_BASE + 0x130)  // Key Input
#define REG_KEYCNT      		(REG_KEYINPUT + 02) // Key Interrupt Control

class Key {
	private:
				
	public:
		static vu16	keytrig;
		static u16 	keyinput;
		static bool	retrig;
		static u8*	keyrate;
		static u16	keytimer[10];
		static u16	keydown,
					keyup,				
					keyrepeat,
					keypress;
	
		static void init			( );
		static void update			( );
		static bool down			( u16 keyflag 	);
		static bool up				( u16 keyflag 	);
		static bool press			( u16 keyflag 	);
		static bool repeat			( u8 index		);
		static void forceNoInput	( );
		static bool activity		( );
};

extern volatile bool SYS_QUERYKEY;		 //True when a key is triggered

#endif
