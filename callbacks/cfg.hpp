#ifndef CFG_HPP
#define CFG_HPP

#include "../agb.h"

class Config {
	public:
		static void load	( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void backup	( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void revert 	( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void save	( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void defaults( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void reinit	( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void format 	( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void reset 	( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );

};

#include "linkmode.hpp"

class LookNFeel {
	public:
		static void init	( );
		static void logoWait( );
		static void logoFade( );
		static void update	( RegionHandler* rh );
		
};

class ColorEditor {
	public:
		static void run 	( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void update	( RegionHandler* rh );
		
};

#endif
