#define BPM_MAGIC 6271			//100BPM = 6271Hz per beat	
class Spu{
	private:
	
	public:
		
		int	timerTarget;
		int currentPattern; //NOT USED! 
		int currentTicks;
		int targetTick[6];
		int currentBeats;
		int beatsPerBar;
		int secPerBeat;
		bool retrig_note[6];
		bool enable_metronome;	
		
		bool playing;
		
		void mute(int channel);
		void solo(int channel);
	
		Spu(){};
		
		void Init(int bpm);
		void setTempo(int bpm);
		void play(bool fromStart);	
		void stop(void);	
		
		void noteOnPWM1(void);
		void noteOnPWM2(void);
		void noteOnWAV(void);
		void noteOnNZE(void);

		
		
		bool setKey(	u8 channel, u8 key, u8 vol );
		void setCmd(	u8 channel, u8 cmd, u8 value );
		void setInst(  u8 channel, u8 inst, u8 vol, bool retrig_note );
		void triggerChannel( int channelIndex );

		void update();		// Called each program cycle		
		void enable();		//Enable given sound channel
		void disable();		//Disable general sound system
		
		void jumpToPatternAsync(int p);	//jumps asyncronously to pattern p
};

extern Spu SPU;

