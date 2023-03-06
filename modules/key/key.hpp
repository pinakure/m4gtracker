#define REG_KEYINPUT    (REG_BASE + 0x130)  // Key Input
#define REG_KEYCNT      (REG_KEYINPUT + 02) // Key Interrupt Control



class cKEY
{
	private:
				
	public:
		vu16	keytrig;
		u16 keyinput;
		bool	retrig;
		u16		keyrate,
				keytimer[10];
		u16		keydown,
				keyup,				
				keyrepeat,
				keypress;
		cKEY(){}
		~cKEY(){}
		void init(void);
		void update(void);
		bool down(u16 keyflag);
		bool up(u16 keyflag);
		bool press(u16 keyflag);
		bool repeat(u8 index);

		bool activity();
};

extern cKEY KEY;
