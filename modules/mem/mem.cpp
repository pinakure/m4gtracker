#include "mem.hpp"
#include "../../callbacks/debug.hpp"

typedef struct
{

}t_chunk;


void Mem::init(){
	
}

void Mem::test(){
	u8* test8=(u8*)malloc(sizeof(u8)*256);
	u16* test16=(u16*)malloc(sizeof(u16)*256);
	u32* test32=(u32*)malloc(sizeof(u32)*256);
	int x=0;
	
	for(x=0;x<256;x++)
	{
		test8[x]  = 0xCC;
		test16[x] = 0xCAFE;
		test32[x] = 0xDEADBABE;
	}

	for(x=0;x<256;x++){
		//Gpu::PrintF(0,0, " ADDR8=%p  ADDR16=%p      ADDR32=%p  ",test8, test16, test32);
		//Gpu::set(BG_TEXT, TILESET_HEXADECIMAL + test8[x], x%30, 2+(x/30), 0x6); 
	}
	/*
	Mem::free(test8);
	Mem::free(test16);
	Mem::free(test32);
	*/
}

void Mem::get(u8 blocks){
}

void Mem::free(u16 addr){
}

void Mem::update( RegionHandler* rh ){
	// REGION Callback for Memory Submenu in CFG Screen
	
}
