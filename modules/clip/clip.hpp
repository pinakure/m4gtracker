#ifndef CLIP_HPP
#define CLIP_HPP

#include "../../agb.h"

typedef enum eClipboardAction {
	CLIP_NONE,
	CLIP_CUT,
	CLIP_PASTE,
	CLIP_COPY,
	CLIP_CLONE
}ClipboardAction;

class Clip {
	public:
		static bool 			redraw;
		static bool 			visible;
		static bool 			blink_monitor;
		static ClipboardAction 	action;
		
		static void init();
		static void draw( RegionHandler *rh );
		static void update( RegionHandler *rh );
		static void show();
		static void hide();
		static void activate();
		static void processInput();

		static void copy();
		static void paste();
		static void clone();
		static void cut();
};

#endif
