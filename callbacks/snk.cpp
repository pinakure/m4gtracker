#include "snk.hpp" 
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"
#include "../data/helpers.hpp"
#include "../modules/spu/sequencer.hpp"
#include "../modules/spu/mixer.hpp"
#include "../modules/spu/synth.hpp"

bool 		SnakeGame::turbo;
bool 		SnakeGame::redraw_game 	= true;
bool 		SnakeGame::redraw_hud 	= true;
bool 		SnakeGame::grow			= false;
u8 			SnakeGame::tickLevel;
u8 			SnakeGame::tick;
u8 			SnakeGame::speed;
SnakeDirs 	SnakeGame::direction;
SnakeDirs 	SnakeGame::nextdirection;
u8 			SnakeGame::data[32*32];
SnakeCell 	SnakeGame::cells[32];
SnakeCell 	SnakeGame::fruit;
u8 			SnakeGame::lives 		= 3;
u8 			SnakeGame::level		= 0;
u32			SnakeGame::score		= 0;	
u32 		SnakeGame::hiscore		= 0;
u8			SnakeGame::length		= 3;
u8 			SnakeGame::last_lives 	= 0xFF;
u8 			SnakeGame::last_level 	= 0x20;
u32			SnakeGame::last_score 	= 0xFFFFFFFF;
u32			SnakeGame::last_hiscore = 0xFFFFFFFF;
u8			SnakeGame::last_length	= 0xFFFF;
u32			SnakeGame::counter_a	= 0xFFFFFFFF;
u32			SnakeGame::counter_b	= 0xFFFFFFFF;
u16			SnakeGame::sound_timer  = 0x0;
	
#define PHI 0x9e3779b9
static u32 q[32], rc=362436;

static void randomSeed(u32 seed){
	int i;
	q[0] = seed;
	q[1] = seed + PHI;
	q[2] = seed + PHI + PHI;
	
	for( i = 3; i<32; i++){
		q[i] = q[i-3] ^ q[i-2] ^ q[i-1];
	}
	
}

static u32 random(){
	u64 t, a= 18782L;
	static u32 i=4095;
	u32 x,r=0xfffffffe;
	i = (i+1) & 31;
	t = a * q[i]+rc;
	rc = (t>>32);
	x = t + rc;
	if( x < rc ) {
		x++;
		rc++;
	}
	return(q[i] = r - x);
}
	
void SnakeGame::init(){
	randomSeed(1024);
	lives 		= 0;
	sound_timer = 0;
}

void SnakeGame::newFruit(){
	fruit.x = random() % 32;
	fruit.y = random() % 32;
	redraw_game = true;
}
	
void SnakeGame::resetPosition(){
	length = 3;
	cells[0].x = 15;
	cells[0].y = 15;
	cells[1].x = 16;
	cells[1].y = 15;
	cells[2].x = 17;
	cells[2].y = 15;
	newFruit();
}

void SnakeGame::start(){
	if( lives > 0) return;
	direction 		= SNAKE_RIGHT;
	nextdirection 	= SNAKE_RIGHT;
	speed 			= 1;
	tickLevel 		= 255;
	length 			= 3;	
	grow 			= false;
	turbo  			= 0;
	tick 			= tickLevel;
	redraw_game 	= true;			
	redraw_hud		= true;			
	lives 			= 3;
	score			= 0;	
	level			= 0;
	hiscore			= 123456789;
	last_lives 		= 0xFF;
	last_level 		= 0xFF;
	last_length		= 0xFFFF;
	last_score 		= 0xFFFFFFFF;
	last_hiscore	= 0xFFFFFFFF;
	resetPosition();
}

void SnakeGame::drawHud(){
	redraw_hud 		= false;
		
	//if( last_length != SnakeGame::length){
		//DECIMAL_DOUBLE_TWOTILES(1, 2, 0x55, SnakeGame::length);
		//last_length = SnakeGame::length;
	//}
	
	if( last_lives != lives){
		BIGHEX(6,4,0xFF, lives);
		last_lives = lives;
	}
		
	if( last_level != level){
		BIGHEX(6,7,0xFF, level);
		last_level = level;
	}
	
	if( last_score != score){
		gpu.set(         2, 0, 12, 0x00);
		gpu.set(         2, 1, 12, 0x00);
		gpu.set(         2, 2, 12, 0x00);
		gpu.set(         2, 3, 12, 0x00);
		gpu.set(         2, 4, 12, 0x00);
		gpu.set(         2, 5, 12, 0x00);
		gpu.set(         2, 6, 12, 0x00);
		DECIMAL_DOUBLE( 	2, 12, 0xDD, (score/100000000	) % 100);
		DECIMAL_DOUBLE( 	3, 12, 0xDD, (score/1000000		) % 100);
		DECIMAL_DOUBLE( 	4, 12, 0xDD, (score/10000		) % 100);
		DECIMAL_DOUBLE( 	5, 12, 0xDD, (score/100			) % 100);
		DECIMAL_DOUBLE( 	6, 12, 0xDD, (score				) % 100);
		last_score = score;
	}

	if( last_hiscore != hiscore){
		gpu.set( 		 2, 0, 15, 0x00);
		gpu.set( 		 2, 1, 15, 0x00);
		gpu.set( 		 2, 2, 15, 0x00);
		gpu.set( 		 2, 3, 15, 0x00);
		gpu.set( 		 2, 4, 15, 0x00);
		gpu.set( 		 2, 5, 15, 0x00);
		gpu.set( 		 2, 6, 15, 0x00);
		DECIMAL_DOUBLE( 	2, 15, 0xDD, (hiscore/100000000		) % 100);
		DECIMAL_DOUBLE( 	3, 15, 0xDD, (hiscore/1000000		) % 100);
		DECIMAL_DOUBLE( 	4, 15, 0xDD, (hiscore/10000			) % 100);
		DECIMAL_DOUBLE( 	5, 15, 0xDD, (hiscore/100			) % 100);
		DECIMAL_DOUBLE( 	6, 15, 0xDD, (hiscore				) % 100);
		last_hiscore = hiscore;
	}
}

void SnakeGame::sound( ){
	if( Sequencer::playing ) return;
	Mixer::start();
	*((volatile u16*)0x04000068) = 0x8181;
	*((volatile u16*)0x0400006C) = 0xC000 | (((8 * length)*8)-8);
	*((volatile u16*)0x04000062) = 0x8381;
	*((volatile u16*)0x04000064) = 0xC000 | (((8 * length)*8)-10);

	sound_timer = 0xFF;
}

void SnakeGame::eat(){
	sound();
	score += 100;
	if(score > hiscore) hiscore = score;
	redraw_hud = true;
	newFruit();
	grow = true;
}

void SnakeGame::hit(){
	if( lives == 1){
		lives = 0;
		// Game over	
	} else {
		lives--;
		// Hit sound?
	}
	gpu.vs->clear();
	resetPosition();
	redraw_game = true;
}

void SnakeGame::drawGame(){
	gpu.vs->clear();
	
	gpu.vs->set( fruit.x, fruit.y );

	int i,o;
	for(i=0;i < length-1; i++){
		gpu.vs->set(cells[i].x, cells[i].y);
		for( o=0; o < length-1; o++){
			if(i == o)continue;
			if( (cells[i].x == cells[o].x) && (cells[i].y == cells[o].y) ) hit();
		}
	}
	if( (cells[0].x == fruit.x) && (cells[0].y == fruit.y) ) eat();
	
	gpu.vs->draw(11,6);
	redraw_game = false;			
}

void SnakeGame::move(){
	
	if(score>1) {
		score-= turbo ? 1 : 2;
		redraw_hud = true;
	}	
	
	// Save last cell in case we need to grow later 
	int grow_x = cells[ 0 ].x;
	int grow_y = cells[ 0 ].y;
	
	// Make trail follow head
	for(int i=length-1;i>0; i--){
		cells[i].x = cells[i-1].x;
		cells[i].y = cells[i-1].y;
	}
	
	// Move head
	if( direction == SNAKE_UP	 ) cells[0].y--; else 
	if( direction == SNAKE_DOWN	 ) cells[0].y++; else 
	if( direction == SNAKE_LEFT	 ) cells[0].x--; else 
	if( direction == SNAKE_RIGHT ) cells[0].x++;
	
	cells[0].x &= 31;
	cells[0].y &= 31;
	
	// Grow if needed
	if(grow && (SnakeGame::length < 32)){
		cells[ SnakeGame::length ].x = grow_x;
		cells[ SnakeGame::length ].y = grow_y;
		redraw_hud = true;
		SnakeGame::length++;
	}
	grow = false;
}

void SnakeGame::update( RegionHandler* rh){
	
	
	// Process input 
	if(KEYPRESS_LEFT  && ( direction !=SNAKE_RIGHT) ) nextdirection = SNAKE_LEFT  ; else
	if(KEYPRESS_UP    && ( direction !=SNAKE_DOWN ) ) nextdirection = SNAKE_UP	  ; else
	if(KEYPRESS_RIGHT && ( direction !=SNAKE_LEFT ) ) nextdirection = SNAKE_RIGHT ; else
	if(KEYPRESS_DOWN  && ( direction !=SNAKE_UP   ) ) nextdirection = SNAKE_DOWN  ;
	
	if(KEY.press( KEY_A )) eat();
	//if(KEY.press( KEY_A )) newFruit();
	
	turbo = KEYPRESS_B ? true : false;
	
	if(KEYUP_START){
		start();
	}
	
	if(lives == 0) return;
	// Update logic	
	
	if(tick>0){
		tick--;
	} else {
		move();	
		redraw_game = true;
		direction = nextdirection;
		tick = tickLevel>>turbo;
	}
	
	// Render, if needed	
	
	// Render, if needed	
	
	OnNewRegion( 
		redraw_game = true; 
		last_lives 		= 0xFF;
		last_level 		= 0xFF;
		last_length		= 0x20;
		last_score 		= 0xFFFFFFFF;
		last_hiscore	= 0xFFFFFFFF;
		redraw_hud  = true; 
	);
	
	if(redraw_game) drawGame();
	if(redraw_hud ) drawHud();
}
