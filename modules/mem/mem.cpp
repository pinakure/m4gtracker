/* ----------------------------------------------------------------------------
AUTHOR		 	Al P.Area ( Smiker )
PURPOSE			Unused. Abandoned attempt to manage memory from software. 
ORIGINAL DATE 	2016, October
REVISION DATE 	2023-02-28
EXAMPLES		TIM0.Setup(0x0004,1);  //Configure overflow reg on 0xFFFF-0x0004, freq 1
				TIM0.Enable();         //Activate timer (start counting)
 --------------------------------------------------------------------------- */

#include "mem.hpp"

typedef struct{

}t_chunk;


void MEM::init(){
	
}

void MEM::test(){
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

	for(x=0;x<256;x++)
	{
		//GPU.PrintF(0,0, " ADDR8=%p  ADDR16=%p      ADDR32=%p  ",test8, test16, test32);
		//GPU.SetTile(BG_TEXT, TILESET_HEXADECIMAL + test8[x], x%30, 2+(x/30), 0x6); 
	}
	free(test8);
	free(test16);
	free(test32);
}

