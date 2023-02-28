#ifndef _M4G_MACROS
#define _M4G_MACROS

#define BUTTON_ALTER		KEY_A	
#define BUTTON_NAVIGATE	KEY_SELECT

#define EXTRACT(packed, position, mask)		((packed >> position) & mask)
#define PORT(a)		*((u8*)a)

#define M4G_VERSION			0x81
#define M4GEEK_SIGNATURE	0xE5ACFECA

#define EXPECT(a, up, down)			{CFG::loadCount = 0;REGHND::progress.set(0, a, STATUS_##up, STATUS_##down, &CFG::loadCount);}
#define OK()						{CFG::loadCount++;REGHND::update(1);	DECIMAL_DOUBLE(28,1,9, SRAM.position-1);}

#define PRINTPOINTER(x, y, c, a)	TEXT(x,y, 0x6, c-13); \
						HEXADECIMAL_DOUBLE(x+1,y, 0x6, (((unsigned)a)&0xFF000000) >> 24); \
						HEXADECIMAL_DOUBLE(x+2,y, 0x6, (((unsigned)a)&0x00FF0000) >> 16); \
						HEXADECIMAL_DOUBLE(x+3,y, 0x6, (((unsigned)a)&0x0000FF00) >>  8); \
						HEXADECIMAL_DOUBLE(x+4,y, 0x6, ((unsigned)a)&0x000000FF);

#define PRINTVAR(x, y, c, a)		TEXT(x,y, 0x6, c-13); \
						HEXADECIMAL_DOUBLE(x+1,y, 6, a);



#endif 
