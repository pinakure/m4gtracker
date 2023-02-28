#ifndef _DATA_VARIABLES
#define _DATA_VARIABLES

#include "../agb.h"

/***********************************************************************************************
************************************************************************************************

INPUT VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/
typedef struct _INPUT {
	u8			A;	
	u8			B;	
	u8			L;	
	u8			R;	
	u8			SELECT;	
	u8			START;	
	u8			UP;	
	u8			DOWN;	
	u8			LEFT;	
	u8			RIGHT;	
}INPUT;

extern INPUT VAR_INPUT; // Input mirror

extern u8 VAR_KEY[4];

/***********************************************************************************************
************************************************************************************************

PATTERN VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/
typedef struct sPatternCell {
	u16				key[16] /*7*/;
	u8				ins[16] /*6*/;
	u8				vol[16] /*4*/;
	u8				cmd[16] /*5*/;
	u8				val[16];
}PatternCell;

typedef struct sPattern {	//Pattern data for a single channel!!!
	u8				order[256];
		//set pointer to first visible position (in pat screen), and mannipulate order directly by POINTER[x]
	u8 				*pointer;
	u8				position;	//Internal position
}Pattern;


/***********************************************************************************************
************************************************************************************************

SONG VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/
typedef struct sGrooveTable {
	u8				step[16] /*8*/;
	u8				enable;
	u8				length /*4*/;
}GrooveTable;


class Song {
public:
	GrooveTable		groove;
	PatternCell		data[128];		// This should BE ON PATTERN STRUCTURE!!!*/
	Pattern			patterns[6];	// Orders and pattern for each
	u8				transpose;
	u8				bpm;
	u8				pattern_length;
	u8				title[14];
	char			artist[14];
	u8 				tap_ticks;
	bool			not_empty;		// If true, song has data	
};

extern Song songs[6]; //Song data for 6 song slots
extern Song song; 	  //Current loaded song (copied from active one)

// This may be moved to song!
extern PatternCell 	VAR_DATA[128];
extern Pattern 		VAR_PATTERN[6]; 
extern PatternCell 	VAR_CELLS[6]; 	// Current pattern cells, 6 pointers dereferenced

/***********************************************************************************************
************************************************************************************************

INSTRUMENT VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/

typedef struct sTableCell {
	u8				TRANSPOSE[16] /*8*/;
	u8				VOLUME[16] /*4*/;
	u8				COMMAND[2][16]/*5*/;
	u8				VALUE[2][16]/*8*/;
	u8				JUMP[2]/*4*/;
	u8				POSITION[2]/*4*/;
	bool			PLAYING;
	
}TableCell;

typedef struct sSettingsPwm {	
	u8				SWEEPDIR;		/*1*/	/*1*/
	u8				ENVELOPEDIR;	/*1*/	/*2*/
	u8				TSP_ENABLE;		/*1*/	/*3*/
	u8				VOL_ENABLE;		/*1*/	/*4*/
	u8				DUTYCYCLE;		/*2*/	/*6*/
	u8				TSP_LOOP;		/*2*/	/*8*/
	u8				VOL_LOOP;		/*2*/	/*10*/	
	u8				LENGTH; 		/*4*/	/*14*/
	u8				LEVEL;			/*4*/	/*18*/
	u8				TSP_LENGTH;		/*4*/	/*22*/
	u8				SWEEPSPEED; 	/*4*/	/*26*/
	u8				SWEEPSTEPS; 	/*4*/	/*30*/
	u8				VOLUMEFADE;		/*4*/	/*34*/
	u8				TSP_POSITION;	/*4*/	/*38*/
	u8				VOL_POSITION;	/*4*/	/*42*/
	u8				VOL_LENGTH;		/*4*/	/*46*/
	u8				VOL_ENVELOPE;	/*8*/	/*54*/
	u8				TSP_ENVELOPE;	/*8*/	/*62*/
	u8				TSP[16];		/*16x4*//*126*/
	u8				VOL[16];		/*16x4*//*190*/
} SettingsPwm;

typedef struct sSettingsWav {
	u8	 			PHASE;
	u8	 			AM;
	u8	 			MIXPERCENT /*4*/;
	u8 				OP1_TYPE /*3*/;
	u8 				OP1_ADSR[4] /*4*/;
	u8 				OP2_TYPE /*3*/;
	u8 				OP2_ADSR[4] /*4*/;
	u8 				OP3_TYPE /*3*/;
	u8 				OP3_ADSR[4] /*4*/;
	u8 				OP4_TYPE /*3*/;
	u8 				OP4_ADSR[4] /*4*/;
	u8 				WAVEDATA[16] /*4x16*/;	
	void 			*WAVPRESET[5] /*4x16*/;	
} SettingsWav;

typedef struct sSettingsFmw {
	u8 				MULT /*4*/;
	u8 				ALGORITHM /*3*/;
	u8 				OP1_TYPE /*3*/;
	u8 				OP1_ADSR[4] /*4*/;
	u8 				OP2_TYPE /*3*/;
	u8 				OP2_ADSR[4] /*4*/;
	u8 				OP3_TYPE /*3*/;
	u8 				OP3_ADSR[4] /*4*/;
	u8 				OP4_TYPE /*3*/;
	u8 				OP4_ADSR[4] /*4*/;
	u8 				WAVEDATA[16] /*4x16*/;
} SettingsFmw;

typedef struct sSettingsSmp {
	u8 				START /*8*/;
	u8 				END /*8*/;
	u8 				LOOP /*2*/;
	u8 				SYNTHMODE;
	u8 				FREQUENCY /*8*/;
	u8 				ADSR[4] /*4*/;
	u8 				B /*?*/;
	u8 				S /*?*/;
	u8 				R /*?*/;
	u8 				KIT /*5*/;
	u8 				KITINDEX /*4*/;
	void 			*SMPKIT[12] /*4x16*/;	
} SettingsSmp;

typedef struct sInstrument {
	TableCell		TABLE;
	u8				VISPOSITION[2];
	char 			NAME[6];
	u8				TYPE /*2*/;
	u8				SETTINGS[32];
}Instrument;

extern SettingsPwm VAR_PWM;
extern SettingsFmw VAR_FMW;
extern SettingsSmp VAR_SMP;
extern SettingsWav VAR_WAV;

extern Instrument VAR_INSTRUMENTS[64];
extern Instrument VAR_INSTRUMENT; // Current instrument MUST be operated from here
/***********************************************************************************************
************************************************************************************************

CFG VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/
typedef struct sCfgPal {
	u8 				r /*4*/;
	u8 				g /*4*/;
	u8 				b /*4*/;
} CfgPal;

typedef struct sCfgLookNFeel {
	u8	 			interface;
	u8				font /*3*/;
	u8				border/*3*/;
	u8	 			show_logo;
	u8	 			startup_sfx;
} CfgLookNFeel;

typedef struct sCfgLinkMode {
	u8 				link_mode/*2*/;
	u8				master_clock;
	u8 				midi_chan/*4*/;
	u8 				clock_tempo;
} CfgLinkMode;

typedef struct sCfgBehavior {
	u8				auto_load;
	u8				key_rate /*4*/;
	u8				button_set /*2*/;
	u8				vis_type;
} CfgBehavior;

typedef struct sCfgTracker {
	u8 				fine_tune;
	u8				prelisten;
	u8				transpose;
	u8				input_mode;
	u8				sound_bias;
	// NOT USED
	u8				header_type;
} CfgTracker;

typedef struct sCfgMemory {
	u8				pref;
	int nothing;
} CfgMemory;

class CFG {
	public:
		static u32				load_count;
		static CfgPal			pal[16];
		static CfgLookNFeel		look_n_feel;
		static CfgLinkMode		link_mode;
		static CfgBehavior		behavior;
		static CfgTracker		tracker;
		static CfgMemory		memory;
		static u8 				slot 				/*3*/;//Currently selected song
		static u8				menu_slot			/*3*/;	
		static u8	 			really_return;
		static char				alpha_return[14];
		static u8	 			clip_return			/*2*/;
		static u8	 			progress 			/*6*/;
		static u8	 			instrument_vis_type;
		static GrooveTable		groove; 			// Pointer to the table being edited/displayed on INS screeen
		static TableCell		instrument_table; 	// Pointer to the table being edited/displayed on INS screeen
		static u8 				current_channel		/*3*/;
		static u8 				current_instrument	/*6*/;
		static u8 				order_position;		// Index where the PAT screen begins to display pattern orders
		static u8				mute[2][3]; 		// Two mute arrays, PPN - WFS
		static u8				solo[2][3]; 		// Two solo arrays, PPN - WFS
		static bool				reload_skin;
};

/***********************************************************************************************
************************************************************************************************

LIVE VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/
typedef struct sLiveTable {
	u8				key[ 8] /*7*/;
	u8				ins[ 8] /*6*/;
	u8				chan[8] /*3*/;
	u8				vol[ 8] /*4*/;
	u8				cmd[ 8] /*5*/;
	u8				val[ 8];
} LiveTable;

typedef struct sLivePerform {
	LiveTable		left;
	LiveTable		right;
	u8 				retrig;
	u8 				speed /*4*/;
	u8	 			quantize /*3*/;
	u8				lock;
} LivePerform;

typedef struct sLivePiano {
	u8				mode /*2*/;
	u8				octave /*4*/;
	u8				quantize /*3*/;
	u8				transpose;
	u8				channel[2];
	u8				midi_chan[2];
} LivePiano;

typedef struct sLive {
	LivePerform		perform;
	LivePiano		piano;
	u8	 			key_press[10];
} Live;

extern Live live;

/***********************************************************************************************
************************************************************************************************

CHANNEL VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/
typedef struct sChannel {
	bool 			mute;
	bool			solo;

	u8 				key;
	u8 				cmd;
	u8 				inst;
	u8 				value;
	u8 				volume;

	PatternCell*	cell;
	PatternCell*	next_cell;
	// u8				VOLUME /*5*/;
	u8				last_peak /*4*/;
	u8				peak /*4*/;
	bool			playing;

	u8				length;
	u8				last_step /*4*/;
	u8				step /*4*/; 		// Current pattern step
	u8				jump_step /*4*/; 	// Next pattern step

	u8				position /*8*/;  	// Index on chanel Pattern Order array
	u8				next_position /*8*/; // Next index on channel pattern order array
	u8				last_position /*8*/; // Next index on channel pattern order array
} Channel;

extern Channel channel[6];

/***********************************************************************************************
************************************************************************************************

 GLOBALS HERE

************************************************************************************************
************************************************************************************************/

#endif
