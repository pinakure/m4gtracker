#ifndef COLOREDITOR_HPP
#define COLOREDITOR_HPP

#include "../../agb.h"

#include "../../data/control.hpp"
#include "../../modules/regionhandler/regionhandler.hpp"

class ColorEditor {
	public:
		static void show	( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void update	( RegionHandler* rh );
		
};

#endif
