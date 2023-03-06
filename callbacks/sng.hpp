#ifndef SNG_HPP
#define SNG_HPP

#include "../agb.h"

class SongEdit {
	public:
		static void update( RegionHandler* rh );
		static void load	( Control *c=NULL, bool bigstep=false, bool add=false, u32 *pointer=NULL );
		static void save 	( Control *c=NULL, bool bigstep=false, bool add=false, u32 *pointer=NULL );
		static void purge	( Control *c=NULL, bool bigstep=false, bool add=false, u32 *pointer=NULL );
		static void copy	( Control *c=NULL, bool bigstep=false, bool add=false, u32 *pointer=NULL );
		static void erase	( Control *c=NULL, bool bigstep=false, bool add=false, u32 *pointer=NULL );
		static void tapTempo( Control *c=NULL, bool bigstep=false, bool add=false, u32 *pointer=NULL );
		static void setTempo( Control *c	 , bool bigstep		 , bool add		 , u32 *pointer		 );
		static void select	( Control *c	 , bool bigstep		 , bool add		 , u32 *pointer		 );

};

#endif
