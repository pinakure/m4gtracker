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
typedef struct _PATTERN_CELL {
	u16				KEY[16] /*7*/;
	u8				INS[16] /*6*/;
	u8				VOL[16] /*4*/;
	u8				CMD[16] /*5*/;
	u8				VAL[16];
}PATTERN_CELL;

extern PATTERN_CELL VAR_CELLS[6]; // Current pattern cells, 6 pointers dereferenced

typedef struct _PATTERN {	//Pattern data for a single channel!!!
	u8				ORDER[256];
		//set pointer to first visible position (in pat screen), and mannipulate order directly by POINTER[x]
	u8 				*POINTER;
	u8				POSITION;	//Internal position
}PATTERN;


extern PATTERN_CELL	VAR_DATA[128];
extern PATTERN VAR_PATTERN[6]; 

/***********************************************************************************************
************************************************************************************************

SONG VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/
typedef struct _GROOVE_TABLE {
	u8				STEP[16] /*8*/;
	u8				ENABLE;
	u8				LENGTH /*4*/;
}GROOVE_TABLE;

typedef struct _SONG {
	GROOVE_TABLE	GROOVE;
	PATTERN_CELL	DATA[128];//This should BE ON PATTERN STRUCTURE!!!*/
	PATTERN			PATTERNS[6];//orders and pattern for each
	u8				TRANSPOSE;
	u8				BPM;
	u8				PATTERNLENGTH /*4*/;
	u8				TITLE[14];
	char			ARTIST[14];
	u8 				TAPTICKS;
	bool			NOTEMPTY;		// If true, song has data
}SONG;

extern SONG VAR_SONGS[6]; //Song data for 6 song slots
extern SONG VAR_SONG; //Current loaded song (copied from active one)

/***********************************************************************************************
************************************************************************************************

INSTRUMENT VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/

typedef struct _TABLE_CELL {
	u8				TRANSPOSE[16] /*8*/;
	u8				VOLUME[16] /*4*/;
	u8				COMMAND[2][16]/*5*/;
	u8				VALUE[2][16]/*8*/;
	u8				JUMP[2]/*4*/;
	u8				POSITION[2]/*4*/;
	bool			PLAYING;
	
}TABLE_CELL;

typedef struct _SETTINGS_PWM {	
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
}SETTINGS_PWM;

typedef struct _SETTINGS_WAV {
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
	
}SETTINGS_WAV;

typedef struct _SETTINGS_FMW {
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
}SETTINGS_FMW;

typedef struct _SETTINGS_SMP {
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
}SETTINGS_SMP;

typedef struct _INSTRUMENT {
	TABLE_CELL		TABLE;
	u8				VISPOSITION[2];
	char 			NAME[6];
	u8				TYPE /*2*/;
	u8				SETTINGS[32];
}INSTRUMENT;

extern SETTINGS_PWM VAR_PWM;
extern SETTINGS_FMW VAR_FMW;
extern SETTINGS_SMP VAR_SMP;
extern SETTINGS_WAV VAR_WAV;

extern INSTRUMENT VAR_INSTRUMENTS[64];
extern INSTRUMENT VAR_INSTRUMENT; // Current instrument MUST be operated from here
/***********************************************************************************************
************************************************************************************************

CFG VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/
typedef struct _CFG_PAL {
	u8 				R /*4*/;
	u8 				G /*4*/;
	u8 				B /*4*/;
}CFG_PAL;

typedef struct _CFG_LOOKNFEEL {
	u8	 			INTERFACE;
	u8				FONT /*3*/;
	u8				BORDER /*3*/;
	u8	 			SHOWLOGO;
	u8	 			STARTUPSFX;
}CFG_LOOKNFEEL;

typedef struct _CFG_LINKMODE {
	u8 				LINKMODE /*2*/;
	u8				MASTERCLOCK;
	u8 				MIDICHAN /*4*/;
	u8 				CLOCKTEMPO;
}CFG_LINKMODE;

typedef struct _CFG_BEHAVIOR {
	u8				AUTOLOAD;
	u8				KEYRATE /*4*/;
	u8				BUTTONSET /*2*/;
	u8				VISTYPE;
}CFG_BEHAVIOR;

typedef struct _CFG_TRACKER {
	u8 				FINETUNE;
	u8				PRELISTEN;
	u8				TRANSPOSE;
	u8				INPUTMODE;
	u8				SOUNDBIAS;
	// NOT USED
	u8				HEADERTYPE;
}CFG_TRACKER;

typedef struct _CFG_MEMORY {
	u8				PREF;
	int nothing;
}CFG_MEMORY;

typedef struct _CFG {
	u32				loadCount;
	CFG_PAL 		PAL[16];
	CFG_LOOKNFEEL	LOOKNFEEL;
	CFG_LINKMODE	LINKMODE;
	CFG_BEHAVIOR 	BEHAVIOR;
	CFG_TRACKER		TRACKER;
	CFG_MEMORY		MEMORY;
	u8 				SLOT /*3*/;//Currently selected song
	u8				MENUSLOT /*3*/;	
	u8	 			REALLYRETURN;
	char			ALPHARETURN[14];
	u8	 			CLIPRETURN /*2*/;
	u8	 			PROGRESS /*6*/;
	u8	 			INSTRUMENTVISTYPE;
	GROOVE_TABLE	GROOVE; 			// Pointer to the table being edited/displayed on INS screeen
	TABLE_CELL 		INSTRUMENTTABLE; 	// Pointer to the table being edited/displayed on INS screeen
	u8 				CURRENTCHANNEL /*3*/;
	u8 				CURRENTINSTRUMENT 	/*6*/;
	u8 				ORDERPOSITION; 		// Index where the PAT screen begins to display pattern orders
	u8				MUTE[2][3]; // Two mute arrays, PPN - WFS
	u8				SOLO[2][3]; // Two solo arrays, PPN - WFS
	bool			RELOADSKIN;
}CFG;

extern CFG VAR_CFG;

/***********************************************************************************************
************************************************************************************************

LIVE VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/
typedef struct _LIVE_TABLE {
	u8				KEY[ 8] /*7*/;
	u8				INS[ 8] /*6*/;
	u8				CHAN[8] /*3*/;
	u8				VOL[ 8] /*4*/;
	u8				CMD[ 8] /*5*/;
	u8				VAL[ 8];
}LIVE_TABLE;

typedef struct _LIVE_PERFORM {
	LIVE_TABLE		LEFT;
	LIVE_TABLE		RIGHT;
	u8 				RETRIG;
	u8 				SPEED /*4*/;
	u8	 			QUANTIZE /*3*/;
	u8				LOCK;
}LIVE_PERFORM;

typedef struct _LIVE_PIANO {
	u8				MODE /*2*/;
	u8				OCTAVE /*4*/;
	u8				QUANTIZE /*3*/;
	u8				TRANSPOSE;
	u8				CHANNEL[2];
	u8				MIDICHAN[2];
}LIVE_PIANO;

typedef struct _LIVE {
	LIVE_PERFORM	PERFORM;
	LIVE_PIANO		PIANO;
	u8	 			KEYPRESS[10];
}LIVE;

extern LIVE VAR_LIVE;

/***********************************************************************************************
************************************************************************************************

CHANNEL VARIABLE STRUCTURE

************************************************************************************************
************************************************************************************************/
typedef struct _CHANNEL {
	bool 			mute;
	bool			solo;

	u8 				key;
	u8 				cmd;
	u8 				inst;
	u8 				value;
	u8 				volume;

	PATTERN_CELL	*CELL;
	PATTERN_CELL	*NEXTCELL;
	u8				VOLUME /*5*/;
	u8				lastpeak /*4*/;
	u8				peak /*4*/;
	bool			PLAYING;

	u8				LENGTH;
	u8				LASTSTEP /*4*/;
	u8				STEP /*4*/; 		// Current pattern step
	u8				JUMPSTEP /*4*/; 	// Next pattern step

	u8				POSITION /*8*/;  	// Index on chanel Pattern Order array
	u8				NEXTPOSITION /*8*/; // Next index on channel pattern order array
	u8				LASTPOSITION /*8*/; // Next index on channel pattern order array
}CHANNEL;

extern CHANNEL VAR_CHANNEL[6];

/***********************************************************************************************
************************************************************************************************

 GLOBALS HERE

************************************************************************************************
************************************************************************************************/
