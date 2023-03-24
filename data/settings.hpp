#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "../agb.h"

typedef struct _SETTINGS_PWM {	
	u8				SWEEPDIR;		// 	    1 : 1
	u8				ENVELOPEDIR;	//      1 : 2
	u8				TSP_ENABLE;		//      1 : 3
	u8				VOL_ENABLE;		//      1 : 4 
	u8				DUTYCYCLE;		//      2 : 5
	u8				TSP_LOOP;		//      2 : 7
	u8				VOL_LOOP;		//      2 : 9 	
	u8				LENGTH; 		//      4 : 11
	u8				LEVEL;			//      4 : 15
	u8				TSP_LENGTH;		//      4 : 19
	u8				SWEEPSPEED; 	//      4 : 23
	u8				SWEEPSTEPS; 	//      4 : 27
	u8				VOLUMEFADE;		//      4 : 31
	u8				TSP_POSITION;	//      4 : 35
	u8				VOL_POSITION;	//      4 : 39
	u8				VOL_LENGTH;		//      4 : 43
	u8				VOL_ENVELOPE;	//      8 : 47
	u8				TSP_ENVELOPE;	//      8 : 53
	u8				TSP[16];		// 4 x 16 : 61
	u8				VOL[16];		// 4 X 16 : 125
	u8				TSP_SPEED;		//      4 : 19
	u8				VOL_SPEED;		//      4 : 43
									//          189 BITS
}SETTINGS_PWM;

typedef struct _SETTINGS_WAV {
	u8	 			PHASE			;//   	1 : 1
	u8	 			AM 				;//   	1 : 2
	u8	 			DISTORTION		;//   	4 : 3 
	u8 				OP1_TYPE 		;//   	3 : 7
	u8 				OP1_ADSR[4] 	;//   	4 : 20
	u8 				OP2_TYPE 		;//   	3 : 24
	u8 				OP2_ADSR[4] 	;//   	4 : 27
	u8 				OP3_TYPE 		;//   	3 : 31
	u8 				OP3_ADSR[4] 	;// 	4 : 34
	u8 				OP4_TYPE 		;//   	3 : 38
	u8 				OP4_ADSR[4] 	;//     4 : 41
	u8 				WAVEDATA[16] 	;// 4 x 16 : TRANSIENT ;	
	u8 				OP1_GATE 		;//   	4 : 45
	u8 				OP2_GATE 		;//   	4 : 49
	u8 				OP3_GATE 		;//   	4 : 53
	u8 				OP4_GATE 		;//   	4 : 57	
	
}SETTINGS_WAV;

typedef struct _SETTINGS_FMW {
	u8 				DISTORTION		;//      4 : 1
	u8 				ALGORITHM 		;//      3 : 5
	u8 				OP1_TYPE 		;//      3 : 8
	u8 				OP1_ADSR[4] 	;//      4 : 11
	u8 				OP2_TYPE 		;//      3 : 15
	u8 				OP2_ADSR[4] 	;//      4 : 18
	u8 				OP3_TYPE 		;//      3 : 22
	u8 				OP3_ADSR[4] 	;//      4 : 25
	u8 				OP4_TYPE 		;//      3 : 29
	u8 				OP4_ADSR[4] 	;//      4 : 32
	u8 				WAVEDATA[16] 	;// 4 x 16 : 36
	u8 				OP1_GATE 		;//   	4 : 45
	u8 				OP2_GATE 		;//   	4 : 49
	u8 				OP3_GATE 		;//   	4 : 53
	u8 				OP4_GATE 		;//   	4 : 57	
									// 			100
}SETTINGS_FMW;

#define FM_BUFFER_SIZE		32
extern u8 FMDATA[FM_BUFFER_SIZE];

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

extern SETTINGS_PWM VAR_PWM;
extern SETTINGS_FMW VAR_FMW;
extern SETTINGS_SMP VAR_SMP;
extern SETTINGS_WAV VAR_WAV;

#endif
