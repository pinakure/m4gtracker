#ifndef TIM_HPP
#define TIM_HPP
	
	#include "../../agb.h" 

	#define TIM_HPP
	#define TIM_ENABLE			0x80
	#define TIM_CASCADE			0x04
	#define TIM_1				0x00
	#define TIM_64				0x01
	#define TIM_256				0x02
	#define TIM_1024			0x03
	#define TIM_DST				(*(u16*)(address_dst))
	#define TIM_CNT				(*(u16*)(address_cnt))
	
	class cTIM
	{
		private:
			u32		address_dst;
			u32		address_cnt;	
			u8		index;
		public:
			cTIM(u8 _index)
			{
				address_cnt = NULL;
				address_dst = NULL;
				Init(_index);
			}
			~cTIM(){}
			void Init(u8 _index);
			void Reset(void);
			void Run(void);
			void Pause(void);
			void Enable(void);
			void Disable(void);
			void Setup(u16 flags, u8 freq);
	};

	extern cTIM TIM0;
	extern cTIM TIM1;
	extern cTIM TIM2;
	extern cTIM TIM3;
#endif
