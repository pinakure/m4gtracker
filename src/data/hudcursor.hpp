#ifndef HUDCURSOR_HPP
#define HUDCURSOR_HPP

#include "../agb.h"
#include "sprite.hpp"

class HudCursor {
	public:
		static Point 	size;
		static Point 	target;
		static Sprite 	playback[24];
		static Sprite 	sprite[8];
		static Sprite   clip_icons[4];
		static Sprite   clip_selection;
		static Sprite   tsp_table_position;
		static Sprite   vol_table_position;
		
		static void init();
		static void render();
		static void update();
};

#endif
