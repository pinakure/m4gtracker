#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include "../agb.h"

typedef struct sControl 	Control;
typedef struct sCallback 	Callback;

typedef struct sCallback {
	void (*callback)	(Control*, bool, bool, u32*); // Function Pointer to be called on trigger
	const u16 msg;								// Message causing this callback to trigger
	void  *var;									// Variable binded
	const Callback *next;						// If not null, another callback will be checked, only if message didnt match
} Callback;

#endif
