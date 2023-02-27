#ifndef _DATATYPE_CALLBACK
#define _DATATYPE_CALLBACK

typedef struct _Control Control;
typedef struct _Callback Callback;

typedef struct _Callback {
	void(*callback)	(Control*, bool, bool, u32*); // Function Pointer to be called on trigger
	const u16 msg;								// Message causing this callback to trigger
	void  *var;									// Variable bound
	const Callback *next;						// If not null, another callback will be checked, only if message didnt match
} Callback;


#endif
