#ifndef CFG_HPP
#define CFG_HPP

#include "../agb.h"

#include "../data/control.hpp"
#include "../modules/regionhandler/regionhandler.hpp"

class Config {
	public:
		static void load	 ( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void backup	 ( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void revert 	 ( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void save	 ( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void defaults ( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void reinit	 ( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void format 	 ( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void reset 	 ( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void memMap	 ( RegionHandler* rh );
};

#include "linkmode.hpp"
#include "looknfeel.hpp"
#include "coloreditor.hpp"
#include "behavior.hpp"

#endif
