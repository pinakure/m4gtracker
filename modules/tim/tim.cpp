#include "../int/int.hpp" 
#include "../tim/tim.hpp" 

/*------------------------------------------------------------------------------
                             Timer Control Class                                
--------------------------------------------------------------------------------
 This module is the responsible of handling TIMER registers, setup the counters
 and take general control on the system timers. It abstracts the whole timer 
 registers in two sentences to enable a timer:
 i.e:
		TIM0.Setup(0x0004,1);  //Configure overflow reg on 0xFFFF-0x0004, freq 1
		TIM0.Enable();         //Activate timer (start counting)
Pretty much convenient huh? ;)
------------------------------------------------------------------------------*/
TIM TIM0(0);
TIM TIM1(1);
TIM TIM2(2);
TIM TIM3(3);

#define GUARDC(a)
#define UNGUARD()
#define ASSERT(a,b)

TIM::TIM( u8 _index ){
	init(_index);
}		

void TIM::init( u8 _index ){
	GUARDC( TIM::init );
	
	address_cnt = NULL;
	address_dst = NULL;
			
	ASSERT( index < 3, 03 );
	
	index = _index;	
	address_dst = 0x4000100 + ( index << 2 );
	address_cnt = 0x4000102 + ( index << 2 );
	reset();	
	UNGUARD();
}

void TIM::reset(){
	GUARDC( TIM::reset );
	ASSERT( address_dst != NULL, 01 );
	ASSERT( address_cnt != NULL, 02 );
	
	TIM_DST = 0;
    TIM_CNT = 0;
	
	UNGUARD();
}

void TIM::enable(){
	GUARDC( TIM::enable );
	ASSERT( address_dst != NULL, 01 );
	ASSERT( address_cnt != NULL, 02 );
	 
	TIM_CNT |= TIM_ENABLE;

	UNGUARD();
}

void TIM::disable(){
	GUARDC( TIM::disable );
	ASSERT( address_dst != NULL, 01 );
	ASSERT( address_cnt != NULL, 02 );
	
	TIM_CNT &= 0x7f;
	
	UNGUARD();
}

void TIM::setup( u16 flags, u8 freq ){
	GUARDC( TIM::setup );
	ASSERT( address_dst != NULL, 01 );
	ASSERT( address_cnt != NULL, 02 );
	
	TIM_DST = ( 0xFFFF - flags );
    TIM_CNT = freq;	
	INT::enable( 3 + index );
	UNGUARD();
}
