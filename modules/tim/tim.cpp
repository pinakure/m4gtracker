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
cTIM TIM0(0);
cTIM TIM1(1);
cTIM TIM2(2);
cTIM TIM3(3);




#define GUARDC(a)
#define UNGUARD()
#define ASSERT(a,b)

void cTIM::Init(u8 _index)
{
	GUARDC(cTIM::Init);
	ASSERT(index<3, 03);
	
	index = _index;	
	address_dst = 0x4000100+(index<<2);
	address_cnt = 0x4000102+(index<<2);
	Reset();	
	UNGUARD();
}

void cTIM::Reset(void)
{
	GUARDC(cTIM::Reset);
	ASSERT(address_dst!=NULL, 01);ASSERT(address_cnt!=NULL, 02);
	
	TIM_DST=0;
    TIM_CNT=0;
	
	UNGUARD();
}

void cTIM::Enable(void)
{
	GUARDC(cTIM::Enable);
	ASSERT(address_dst!=NULL, 01);ASSERT(address_cnt!=NULL, 02);
	 
	TIM_CNT |=TIM_ENABLE;

	UNGUARD();
}

void cTIM::Disable(void)
{
	GUARDC(cTIM::Disable);
	ASSERT(address_dst!=NULL, 01);ASSERT(address_cnt!=NULL, 02);
	
	TIM_CNT &=0x7f;
	
	UNGUARD();
}

void cTIM::Setup(u16 flags, u8 freq)
{
	GUARDC(cTIM::Setup);
	ASSERT(address_dst!=NULL, 01);ASSERT(address_cnt!=NULL, 02);
	
	TIM_DST = (0xFFFF - flags);
    TIM_CNT = freq;	
	INT.Enable(3+index);
	UNGUARD();
}
