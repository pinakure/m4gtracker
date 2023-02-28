#ifndef TIM_HPP
#define TIM_HPP

#define TIM_ENABLE			0x80
#define TIM_CASCADE			0x04
#define TIM_1				0x00
#define TIM_64				0x01
#define TIM_256				0x02
#define TIM_1024			0x03
#define TIM_DST				(*(u16*)(address_dst))
#define TIM_CNT				(*(u16*)(address_cnt))

class TIM {
	private:
		u32		address_dst;
		u32		address_cnt;	
		u8		index;
	public:
		TIM(u8 _index);
		void init(u8 _index);
		void reset();
		void run();
		void pause();
		void enable();
		void disable();
		void setup(u16 flags, u8 freq);
};

extern TIM TIM0;
extern TIM TIM1;
extern TIM TIM2;
extern TIM TIM3;

#endif
