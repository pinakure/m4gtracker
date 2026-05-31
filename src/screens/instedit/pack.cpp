/*
void InstEdit::packPwm( void* psettings, Instrument *i ){
	SETTINGS_PWM* settings = ( SETTINGS_PWM* ) psettings;
	i->SETTINGS[0] = ( settings->SWEEPDIR		<<7	)| ( settings->ENVELOPEDIR << 6 )| ( settings->TSP_ENABLE<<5 ) | ( settings->VOL_ENABLE<<4 ) | ( settings->DUTYCYCLE<<2 ) | settings->TSP_LOOP;
	i->SETTINGS[1] = ( settings->VOL_LOOP			);			
	i->SETTINGS[2] = ( settings->LENGTH 		<<4 )| ( settings->LEVEL			);
	i->SETTINGS[3] = ( settings->TSP_LENGTH		<<4 )| ( settings->SWEEPSPEED		);
	i->SETTINGS[4] = ( settings->SWEEPSTEPS		<<4 )| ( settings->VOLUMEFADE		);
	i->SETTINGS[5] = ( settings->TSP_POSITION	<<4 )| ( settings->VOL_POSITION		);
	i->SETTINGS[6] = ( settings->VOL_LENGTH		<<4 )| ( 0 							);
	i->SETTINGS[7] = ( settings->VOL_ENVELOPE 		);
	i->SETTINGS[8] = ( settings->TSP_ENVELOPE 		);
	u8 *s = &i->SETTINGS[9];
	u8 *t = settings->TSP;
	u8 *v = settings->VOL;
	for(u8 *end = s+16; t<end; t++, v++, s++){
		*s = (*t<<4) | (*v);
	}
}

void InstEdit::packWav( void *psettings , Instrument *i ){
	SETTINGS_WAV* settings = ( SETTINGS_WAV* ) psettings;
	i->SETTINGS[ SETTING_WAV_PHASE 							 ] = ( settings->PHASE 	 	<< 6 )| (settings->AM		<< 5)| settings->DISTORTION; 
	i->SETTINGS[ SETTING_WAV_OP1_TYPE | SETTING_WAV_OP2_TYPE ] = ( settings->OP1_TYPE  	<< 3 )| (settings->OP2_TYPE		);
	i->SETTINGS[ SETTING_WAV_OP3_TYPE | SETTING_WAV_OP4_TYPE ] = ( settings->OP3_TYPE  	<< 3 )| (settings->OP4_TYPE		);
	for(int c=0,di=0; c<4; c++){
		di = c<<1;
		i->SETTINGS[ ( SETTING_WAV_OP1_ADSR_1 | SETTING_WAV_OP2_ADSR_1 ) + di ] = (settings->OP1_ADSR[ c ] << 4 ) | settings->OP2_ADSR[ c ]; 
		i->SETTINGS[ ( SETTING_WAV_OP3_ADSR_1 | SETTING_WAV_OP4_ADSR_1 ) + di ] = (settings->OP3_ADSR[ c ] << 4 ) | settings->OP4_ADSR[ c ];
	}
	for(int c=0, di=0; c<8; c++){
		di = c<<1;
		i->SETTINGS[ SETTING_WAV_WAVEDATA0 + c ] = (settings->WAVEDATA[ di ] << 4 ) | settings->WAVEDATA[ di + 1 ];
	}
	i->SETTINGS[ ( SETTING_WAV_OP1_GATE | SETTING_WAV_OP2_GATE ) ] = ( settings->OP1_GATE << 4 ) | settings->OP2_GATE; 
	i->SETTINGS[ ( SETTING_WAV_OP3_GATE | SETTING_WAV_OP4_GATE ) ] = ( settings->OP3_GATE << 4 ) | settings->OP4_GATE; 
}

void InstEdit::packFmw( void *psettings , Instrument *i ){
	SETTINGS_FMW* settings = ( SETTINGS_FMW* ) psettings;
	i->SETTINGS[0] = ( settings->DISTORTION	<< 3 ) | settings->ALGORITHM;
	i->SETTINGS[1] = ( settings->OP1_TYPE	<< 3 ) | settings->OP2_TYPE;
	i->SETTINGS[2] = ( settings->OP3_TYPE	<< 3 ) | settings->OP4_TYPE;
	for(int c=0, di=0; c<4; c++){
		di = c<<1;
		i->SETTINGS[ 3 + di ] = (settings->OP1_ADSR[ c ] << 4 ) | settings->OP2_ADSR[ c ];
		i->SETTINGS[ 4 + di ] = (settings->OP3_ADSR[ c ] << 4 ) | settings->OP4_ADSR[ c ];
	}
	for(int c=0, di=0; c<8; c++){
		di = c<<1;
		i->SETTINGS[ 11 + c ] = (settings->WAVEDATA[ di ] << 4 ) | settings->WAVEDATA[ di + 1 ];
	}
}

void InstEdit::packSmp( void *psettings , Instrument *i ){
	SETTINGS_SMP* settings = ( SETTINGS_SMP* ) psettings;
	i->SETTINGS[0] = ( settings->START		<< 4 ) | (settings->END			);
	i->SETTINGS[2] = ( settings->FREQUENCY 		 );      
	i->SETTINGS[1] = ( settings->KIT		<< 3 ) | (settings->LOOP << 1	)| settings->SYNTHMODE;
	i->SETTINGS[3] = ( settings->ADSR[ 0 ]	<< 4 ) | settings->ADSR[ 1 ];
	i->SETTINGS[4] = ( settings->ADSR[ 2 ]	<< 4 ) | settings->ADSR[ 3 ];
	i->SETTINGS[5] = ( settings->KITINDEX	<< 4 ) | settings->B;
	i->SETTINGS[6] = ( settings->S			<< 4 ) | settings->R;
	// TODO: Reload SMPKIT pointers (12)
}

*/