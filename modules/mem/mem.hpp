class cMEM 
{
	private:
		
	public:
		cMEM(){}
		~cMEM(){}
		void Init(void);
		void Test(void);
		void Get(u8 blocks);
		void Free(u16 addr);
};

extern cMEM MEM;

