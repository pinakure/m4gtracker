
class Spu{
	private:
	
	public:
		void mute(int channel);// move to mixer
		void solo(int channel);// move to mixer
	
		void Init(int bpm);
		
		static bool initialized;
		
		void update();		// Called each program cycle		
		
	};

extern Spu SPU;

