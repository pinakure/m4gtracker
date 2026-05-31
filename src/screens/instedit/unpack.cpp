void * InstEdit::_unpackPwm( Instrument *i ){
	static SETTINGS_PWM pwm = unpackPwm( i );
	return (void*)&pwm;
}
SETTINGS_PWM InstEdit::unpackPwm( Instrument *i ){
	SETTINGS_PWM ret;
	ret.SWEEPDIR 		= EXTRACT(i->SETTINGS[0], 7, 0x1);
	ret.ENVELOPEDIR		= EXTRACT(i->SETTINGS[0], 6, 0x1);
	ret.TSP_ENABLE		= EXTRACT(i->SETTINGS[0], 5, 0x1);
	ret.VOL_ENABLE		= EXTRACT(i->SETTINGS[0], 4, 0x1);
	ret.DUTYCYCLE 		= EXTRACT(i->SETTINGS[0], 2, 0x3);
	ret.TSP_LOOP		= i->SETTINGS[0] & 0x3;
	ret.VOL_LOOP		= i->SETTINGS[1] & 0x3;
	ret.LENGTH 			= EXTRACT(i->SETTINGS[2], 4, 0xF);
	ret.LEVEL			= i->SETTINGS[2] & 0xF;
	ret.SWEEPSPEED 		= i->SETTINGS[3] & 0xF;
	ret.SWEEPSTEPS 		= EXTRACT(i->SETTINGS[4], 4, 0xF);
	ret.VOLUMEFADE		= i->SETTINGS[4] & 0xF;
	ret.VOL_ENVELOPE	= i->SETTINGS[7];
	ret.TSP_ENVELOPE	= i->SETTINGS[8];
	ret.TSP_POSITION	= 0;// TRANSIENT - remove from struct at SRAM
	ret.VOL_POSITION	= 0;// TRANSIENT - remove from struct at SRAM
	ret.TSP_SPEED		= ( ret.TSP_ENVELOPE & 0xF0) >> 4;
	ret.VOL_SPEED		= ( ret.VOL_ENVELOPE & 0xF0) >> 4;
	ret.TSP_LENGTH		= ( ret.TSP_ENVELOPE & 0x0F);
	ret.VOL_LENGTH		= ( ret.VOL_ENVELOPE & 0x0F);
	for(int c=0; c<0x10; c++){
		ret.TSP[c]			= EXTRACT(i->SETTINGS[9+c], 4, 0xF);
		ret.VOL[c]			= i->SETTINGS[9+c] & 0xF;
	}
	return ret;
}	

void * InstEdit::_unpackWav( Instrument *i ){
	static SETTINGS_WAV wav = unpackWav( i );
	return (void*)&wav;
}
SETTINGS_WAV InstEdit::unpackWav( Instrument *i ){
	SETTINGS_WAV ret;
	int c, di;
	ret.PHASE			= EXTRACT(i->SETTINGS[0], 6, 0x1);	// 1
	ret.AM				= EXTRACT(i->SETTINGS[0], 5, 0x1);	// 1
	ret.DISTORTION		= i->SETTINGS[0] & 0xF;				// 4
	ret.OP1_TYPE		= EXTRACT(i->SETTINGS[1], 3, 0x7);	// 3
	ret.OP2_TYPE		= i->SETTINGS[1] & 0x7;				// 3
	ret.OP3_TYPE		= EXTRACT(i->SETTINGS[2], 3, 0x7);	// 3
	ret.OP4_TYPE		= i->SETTINGS[2] & 0x7;				// 3			
	for(c=0; c<4; c++){
		di = c<<1;
		ret.OP1_ADSR[c] 	= EXTRACT(i->SETTINGS[3+di], 4, 0xF);	// 4
		ret.OP2_ADSR[c] 	= i->SETTINGS[3+di] & 0xF;				// 4
		ret.OP3_ADSR[c] 	= EXTRACT(i->SETTINGS[4+di], 4, 0xF);	// 4
		ret.OP4_ADSR[c] 	= i->SETTINGS[4+di] & 0xF;				// 4
	}			
	for(c=0; c<8; c++){
		di = c<<1;
		ret.WAVEDATA[ di ] 	= EXTRACT(i->SETTINGS[11 + c], 4, 0xF);	// 4x8
		ret.WAVEDATA[di+1] 	= i->SETTINGS[11 + c] & 0xF;				// 4x8
	}
	ret.OP1_GATE 		= EXTRACT(i->SETTINGS[SETTING_WAV_OP1_GATE], 4, 0xF);	// 4
	ret.OP2_GATE 		= 		   i->SETTINGS[SETTING_WAV_OP2_GATE] & 0xF;				// 4
	ret.OP3_GATE 		= EXTRACT(i->SETTINGS[SETTING_WAV_OP3_GATE], 4, 0xF);	// 4
	ret.OP4_GATE 		= 		   i->SETTINGS[SETTING_WAV_OP4_GATE] & 0xF;				// 4
	return ret;
}

void * InstEdit::_unpackFmw( Instrument *i ){
	static SETTINGS_FMW fmw = unpackFmw( i );
	return (void*)&fmw;
}
SETTINGS_FMW InstEdit::unpackFmw( Instrument *i ){
	SETTINGS_FMW ret;
	int c, di;
	ret.DISTORTION		= EXTRACT(i->SETTINGS[0], 4, 0xF);	// 4
	if((i->SETTINGS[0] &0x7) > 0x5) i->SETTINGS[0] = (i->SETTINGS[0] & 0xF0) | 5;				// 3
	ret.ALGORITHM 		= i->SETTINGS[0] &= 0x7;				// 3
	ret.OP1_TYPE 		= EXTRACT(i->SETTINGS[1], 3, 0x7);	// 3
	ret.OP2_TYPE 		= i->SETTINGS[1] & 0x7;				// 3
	ret.OP3_TYPE 		= EXTRACT(i->SETTINGS[2], 3, 0x7);	// 3
	ret.OP4_TYPE 		= i->SETTINGS[2] & 0x7;				// 3
	for(c=0; c<4; c++){
		di = c<<1;
		ret.OP1_ADSR[c] 	= EXTRACT(i->SETTINGS[3+di], 4, 0xF);	// 4
		ret.OP2_ADSR[c] 	= i->SETTINGS[3+di] & 0xF;				// 4
		ret.OP3_ADSR[c] 	= EXTRACT(i->SETTINGS[4+di], 4, 0xF);	// 4
		ret.OP4_ADSR[c] 	= i->SETTINGS[4+di] & 0xF;				// 4
	}
	for(c=0; c<8; c++){
		di = c<<1;
		ret.WAVEDATA[ di ]	= EXTRACT(i->SETTINGS[11 + c], 4, 0xF);	// 4x8
		ret.WAVEDATA[di+1]	= i->SETTINGS[11 + c] & 0xF;				// 4x8
	}
	ret.OP1_GATE 		= EXTRACT(i->SETTINGS[SETTING_FMW_OP1_GATE], 4, 0xF);	// 4
	ret.OP2_GATE 		= 		   i->SETTINGS[SETTING_FMW_OP2_GATE] & 0xF;				// 4
	ret.OP3_GATE 		= EXTRACT(i->SETTINGS[SETTING_FMW_OP3_GATE], 4, 0xF);	// 4
	ret.OP4_GATE 		= 		   i->SETTINGS[SETTING_FMW_OP4_GATE] & 0xF;				// 4
	return ret;
}

void * InstEdit::_unpackSmp( Instrument *i ){
	static SETTINGS_SMP smp = unpackSmp( i );
	return (void*)&smp;
}
SETTINGS_SMP InstEdit::unpackSmp( Instrument *i ){
	SETTINGS_SMP ret;
	ret.START 	  		= EXTRACT(i->SETTINGS[0], 4, 0xF);	// 8
	ret.END 	  		= i->SETTINGS[0] & 0xF;				// 8
	ret.KIT 	  		= EXTRACT(i->SETTINGS[1], 3, 0x1F);	// 5
	ret.LOOP 	  		= EXTRACT(i->SETTINGS[1], 1, 0x3);	// 2
	ret.SYNTHMODE 		= i->SETTINGS[1] & 0x1;				// 1
	ret.FREQUENCY 		= i->SETTINGS[2];						// 8
	ret.ADSR[0]   		= EXTRACT(i->SETTINGS[3], 4, 0xF);	// 4
	ret.ADSR[1]   		= i->SETTINGS[3] & 0xF;				// 4
	ret.ADSR[2]   		= EXTRACT(i->SETTINGS[4], 4, 0xF);	// 4
	ret.ADSR[3]   		= i->SETTINGS[4] & 0xF;				// 4
	ret.KITINDEX  		= EXTRACT(i->SETTINGS[5], 4, 0xF);	// 4
	ret.B 		  		= i->SETTINGS[5];						// 4
	ret.S 		  		= EXTRACT(i->SETTINGS[6], 4, 0xF);	// 4
	ret.R 		  		= i->SETTINGS[6];						// 4
	// TODO: Reload SMPKIT pointers (12)
	return ret;
}
