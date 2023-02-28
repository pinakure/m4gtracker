#ifndef _MODULES_SNAKE
#define _MODULES_SNAKE

#include "../../agb.h"

#include "../gpu/gpu.hpp"

enum SnakeDir {
	SNAKE_UP,
	SNAKE_RIGHT,
	SNAKE_DOWN,
	SNAKE_LEFT
};

typedef struct sSnakeCell{
	u8 x;
	u8 y;
}SnakeCell;

class SNK{
	public:
		static bool 			turbo;
		static bool 			game_start;
		static u8 				tick_level;
		static u8 				tick;
		static u8 				speed;
		static SnakeDir 		direction;
		static SnakeDir 		next_direction;
		
		static u8 				data[32*32];
		static SnakeCell 		cells[32];
		static u8 				length;
		static VirtualScreen 	*vs;
			
		
		static void init();
		static void resetPosition();		
		static void start();
		static void draw();
		static void move();
		static void update();
};

#endif 
