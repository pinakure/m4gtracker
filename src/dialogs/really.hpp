#ifndef REALLYDIALOG_HPP
#define REALLYDIALOG_HPP

#include "../agb.h"

class ReallyDialog {
	private:
		static bool 	enabled;
		static bool 	highlight;		
		static u16 	background;
		static void 	draw		();
		static void 	confirm	();
		static void 	cancel	();
	public:
		static bool 	result;
		static void 	enable	();
};

#endif
