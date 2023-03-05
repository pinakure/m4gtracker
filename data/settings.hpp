#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "../agb.h"

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

extern SETTINGS_PWM VAR_PWM;
extern SETTINGS_FMW VAR_FMW;
extern SETTINGS_SMP VAR_SMP;
extern SETTINGS_WAV VAR_WAV;

#endif
