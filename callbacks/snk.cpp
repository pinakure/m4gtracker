/* ----------------------------------------------------------------------------
AUTHOR		 	Al P.Area ( Smiker )
PURPOSE			Snake minigame class and update routine
ORIGINAL DATE 	2016, October
REVISION DATE 	2023-02-28
 --------------------------------------------------------------------------- */
#include "callbacks.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"
#include "../modules/sram/sram.hpp"
#include "../modules/sys/sys.hpp"

enum E_SnakeDirs {
	SNAKE_UP,
	SNAKE_RIGHT,
	SNAKE_DOWN,
	SNAKE_LEFT
};

typedef struct _SnakeCell{
	u8 x;
	u8 y;
}SnakeCell;

class SnakeGame{
	private:

	public:
		bool turbo;
		bool redraw;
		u8 tickLevel;
		u8 tick;
		u8 speed;
		E_SnakeDirs direction;
		E_SnakeDirs nextdirection;
		
		u8 data[32*32];
		SnakeCell cells[32];
		u8 length;
		
		SnakeGame(void){
			redraw = true;
		}
		
		void resetPosition(void){
			cells[0].x = 15;
			cells[0].y = 15;
			cells[1].x = 16;
			cells[1].y = 15;
			cells[2].x = 17;
			cells[2].y = 15;
		}
		
		void start(void){
			direction = SNAKE_RIGHT;
			nextdirection = SNAKE_RIGHT;
			speed = 1;
			tickLevel = 255;
			length = 3;	
			tick = tickLevel;
			redraw = true;			
			resetPosition();
		}
		
		void draw(){
			GPU::vs->clear();
			
			int i;
			for(i=0;i<length; i++){
				GPU::vs->set(cells[i].x, cells[i].y);
			}
			
			GPU::vs->draw(11,6);
			redraw = false;			
		}
		
		void move(void){
			for(int i=1;i<length; i++){
				cells[i].x = cells[i-1].x;
				cells[i].y = cells[i-1].y;
			}
			
			if(direction==SNAKE_UP)cells[0].y--;
			else if(direction==SNAKE_DOWN)cells[0].y++;
			else if(direction==SNAKE_LEFT)cells[0].x--;
			else if(direction==SNAKE_RIGHT)cells[0].x++;
			
			cells[0].x &= 31;
			cells[0].y &= 31;
		}
		
		void update(){
			if(tick>0){
				tick--;
				
			} else {
				move();				
				redraw = true;
				direction = nextdirection;
				tick = tickLevel>>turbo;
			}
			
			if(redraw)draw();
		}
};

void updateSnk(){
	static SnakeGame snake;
	 
	if(KEYPRESS_LEFT  && (snake.direction !=SNAKE_RIGHT) ) snake.nextdirection = SNAKE_LEFT	; else
	if(KEYPRESS_UP    && (snake.direction !=SNAKE_DOWN ) ) snake.nextdirection = SNAKE_UP	; else
	if(KEYPRESS_RIGHT && (snake.direction !=SNAKE_LEFT ) ) snake.nextdirection = SNAKE_RIGHT; else
	if(KEYPRESS_DOWN  && (snake.direction !=SNAKE_UP   ) ) snake.nextdirection = SNAKE_DOWN	;
	
	snake.turbo = KEYPRESS_B ? true : false;
	
	if(KEYUP_START){
		snake.start();
	}
	
	snake.update();	
}

