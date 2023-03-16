#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "../agb.h"

#include "../modules/regionhandler/regionhandler.hpp"

typedef enum eSnakeDirs {
	SNAKE_UP,
	SNAKE_RIGHT,
	SNAKE_DOWN,
	SNAKE_LEFT
}SnakeDirs;

typedef struct sSnakeCell{
	u8 x;
	u8 y;
} SnakeCell;

class SnakeGame {
	private:
	public:
		static bool 		turbo;
		static bool 		redraw_game;
		static bool 		redraw_hud;
		static u8 			tickLevel;
		static u8 			tick;
		static u8 			speed;
		static SnakeDirs 	direction;
		static SnakeDirs 	nextdirection;
		static u16			sound_timer;
		
		static u8 			lives;
		static u8 			level;
		static u32 			score;
		static u32 			hiscore;
		static u8 			length;
		static u8 			last_lives;
		static u8 			last_level;
		static u32 			last_score;
		static u32 			last_hiscore;
		static u8 			last_length;

		static u32 			counter_a;
		static u32 			counter_b;
		
		static bool 		grow;
			
		static u8 			data[32*32];
		static SnakeCell 	cells[32];
		static SnakeCell 	fruit;

		static void init();
		static void resetPosition();
		static void start();
		static void drawGame();
		static void drawHud();
		static void move();
		static void eat();
		static void sound();
		static void hit();
		static void newFruit();
		static void update( RegionHandler* rh );
		
		
};

#endif
