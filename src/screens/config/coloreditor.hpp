#ifndef COLOREDITOR_HPP
#define COLOREDITOR_HPP

#include "../../agb.h"

#include "../../data/control.hpp"
#include "../../data/color.hpp"
#include "../../kernel/regionhandler/regionhandler.hpp"

class ColorEditor {
	public:
		static bool enabled; 
		static void show			( Control *c = NULL, bool bigstep=0, bool add=0, u32 *pointer=NULL );
		static void update			(  );
		static void init			( );
		static void setColorChannel	( u8 color, u8 index, s8 q );
		static void setColor		( u8 color, u8 r, u8 g, u8 b );
		static void getColor		( u8 color, Color *entry );
		
		// SRAM Methods
		static void read			( );
		static void write			( );
};

#endif
