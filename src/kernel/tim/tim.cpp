#include "tim.hpp"
#include "../int/int.hpp"

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
Timer timer0(0);
Timer timer1(1);
Timer timer2(2);
Timer timer3(3);

Timer::Timer( u8 timer_index ){
	address_cnt = NULL;
	address_dst = NULL;
	init( timer_index );
}

void Timer::init( u8 timer_index ){
	index = timer_index;	
	address_dst = 0x4000100 + ( index << 2 );
	address_cnt = 0x4000102 + ( index << 2 );
	reset();
}

void Timer::reset( ){
	TIM_DST = 0;
    TIM_CNT = 0;
}

void Timer::enable( ){
	TIM_CNT |= TIM_ENABLE;
}

void Timer::disable( ){
	TIM_CNT &= 0x7f;
}

void Timer::setup( u16 flags, u8 freq ){
	TIM_DST = ( 0xFFFF - flags );
    TIM_CNT = freq;	
	Interrupt::enable( 3 + index );
}
