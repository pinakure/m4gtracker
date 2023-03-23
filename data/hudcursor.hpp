#ifndef HUDCURSOR_HPP
#define HUDCURSOR_HPP

#include "../agb.h"

enum SpriteSize{
	SPRITESIZE_8X8__16x8__8X16,
	SPRITESIZE_16X16__32x8__8X32,
	SPRITESIZE_32X32__32x16__16X32,
	SPRITESIZE_64X64__64x32__32X64,
};

enum SpriteMode{
	SPRITEMODE_NORMAL,
	SPRITEMODE_SEMITRANSPARENT,
	SPRITEMODE_OBJWINDOW,
	SPRITEMODE_FORBIDDEN,
};

enum SpriteShape{
	SPRITESHAPE_SQUARE,
	SPRITESHAPE_HORIZONTAL,
	SPRITESHAPE_VERTICAL,
	SPRITESHAPE_FORBIDDEN,
};

#define ALIGN4  __attribute__((aligned(4)))

typedef struct tagOBJ_ATTR {
    u16 attr0;
    u16 attr1;
    u16 attr2;
    s16 fill;
}ALIGN4 OBJ_ATTR;

typedef struct OBJ_AFFINE{
    u16 fill0[3]; s16 pa;
    u16 fill1[3]; s16 pb;
    u16 fill2[3]; s16 pc;
    u16 fill3[3]; s16 pd;
} ALIGN4 OBJ_AFFINE;

typedef struct sPoint {
	s16 x;
	s16 y;
}Point;

class Sprite {
	public:
		
		Point  		delta;
		Point  		position;
		u8 			index;
		bool 		flip_h;
		bool 		flip_v;
		SpriteSize 	size;
		SpriteMode 	mode;
		u8 			trans_index;
		u16 		tile_number; 	// 0-1023
		u8 			palette; 		// 0-15
		u8 			priority; 		// 0-3
		bool 		double_size;
		bool 		disable;
		bool 		rotoscale;
		u8 			colors;// 0=16/16    1=256/1
		bool 		mosaic;
		u8 			shape;// 0 square // 1 horz // 2 vert // 3 forbidden
	
		u8 			channel_index;
	
		Sprite		( u8 index=0, u8 coord_x=0, u8 coord_y=0 );
		void init	( u8 index );
		void render	( );
	
};

class HudCursor {
	public:
		static Point 	size;
		static Point 	target;
		static Sprite 	playback[24];
		static Sprite 	sprite[8];
		
		static void init();
		static void render();
		static void update();
};

#endif
