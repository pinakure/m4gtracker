#ifndef CONTROL_HPP
#define CONTROL_HPP

#include "../agb.h"

#include "cache.hpp"
#include "callback.hpp"

//Controls are processed even when they are not reachable in navigation order!
#define CONTROL_TERMINATOR			{ 0xFF, 0xFF, NULL, NULL, NULL, NULL, NULL, NULL, NULL }

typedef struct sTransient {
	static u8 bit1;
	static u8 bit2;
	static u8 bit3;
	static u8 bit4;
	static u8 bit5;
	static u8 bit6;
	static u8 bit7;
	static u8 bit8;

	static u8 note;
	static u8 value;
	static u8 volume;
	static u8 instrument;
	static u8 command;

	static u8 changed;
} Transient;

typedef struct sControl {
	const u8 		x;		// Horizontal origin in the map
	const u8 		y;		// Vertical origin in the map
	const Control*	up;		// Control reached when jumping up
	const Control*	right;	// Control reached when jumping right
	const Control*	down;	// Control reached when jumping down
	const Control*	left;	// Control reached when jumping left
	const Cache*	cache;	// Cache this control uses to represent var
	u8*				var;	// Variable binded (8 bit ponter)
	const Callback* callback;
} Control;

extern void modifyNote 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modifyCommand	( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modifyInst 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modifyVolume 	( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modifyValue 	( Control *c, bool bigstep, bool add, u32 *pointer );

extern void pasteCommand 	( Control *c, bool bigstep, bool add, u32 *pointer );
extern void pasteInst 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void pasteNote 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void pasteVolume		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void pasteValue		( Control *c, bool bigstep, bool add, u32 *pointer );

extern void paste2BIT		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void paste3BIT		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void paste4BIT		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void paste5BIT		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void paste6BIT		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void paste7BIT		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void paste8BIT		( Control *c, bool bigstep, bool add, u32 *pointer );

extern void modifyValue		( Control *c, bool bigstep, bool add, u32 *pointer , u8 quant);
extern void modify3VAL 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modify5VAL 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modify6VAL 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modify27VAL		( Control *c, bool bigstep, bool add, u32 *pointer );

extern void modify1BIT 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modify2BIT 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modify3BIT 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modify4BIT 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modify5BIT 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modify6BIT 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modify7BIT 		( Control *c, bool bigstep, bool add, u32 *pointer );
extern void modify8BIT 		( Control *c, bool bigstep, bool add, u32 *pointer );

extern void ALPHA14			( Control *c, bool bigstep, bool add, u32 *pointer );
extern void ALPHA6			( Control *c, bool bigstep, bool add, u32 *pointer );

#endif
