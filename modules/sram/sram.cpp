Sram SRAM;

extern "C" {
	u8 	 SRAM_ReadByte(u16 position);
	void SRAM_WriteByte(u16 position, u8 byte);
};

void Sram::Init(void){
	// Set WAITCNT (Waitstate controller) to 8 wait cycle mode	
	
	waitstateBackup = PORT(0x4000204);
	// TODO: Read first 8 bytes
	// If header does not match m4geek signature, erase SRAM, then save settings
	
	// Set SRAM Write cursor to 0.
	seek(0);
	
	// Restore old waitstate value
}

void Sram::seek(int p){ 
	position = p;	
}

void Sram::forward(int p){ 
	position += p;	
}

void Sram::erase(void){
	int i;
	seek(0);
	for(i=0; i<0x8000; i++){
		write(0x00);
	}	
}

void Sram::drawPosition(u8 x, u8 y, u8 color) { 
	HEXADECIMAL_DOUBLE(x+1,	y,	color, position&0xFF);
	HEXADECIMAL_DOUBLE(x,	y,	color, position>>8);
}

void Sram::write(u8 a){ 
	SRAM_WriteByte(position++, a);
}

void Sram::write16(u16 a) { 
	write(a&0x00FF); 
	write((a&0xFF00) >> 8);
}

void Sram::write32(u32 a) {  
	write( a & 0x000000FF); 
	write((a & 0x0000FF00) >> 8); 
	write((a & 0x00FF0000) >> 16); 
	write((a & 0xFF000000) >> 24); 
}

u8 Sram::read(){
	u8 r = SRAM_ReadByte(position++);
	return r;
}

u16 Sram::read16() { 
	u8 a, b;
	a = read();
	b = read();
	return (b<<8) | a;
}

u32 Sram::read32() { 
	u8 a, b, c, d;
	a = read();
	b = read();
	c = read();
	d = read();
	return (d<<24) | (c<<16) | (b<<8) | a;
}

void Sram::songSave(){
	int i;
	
	seek(0x80);
	forward(32 * VAR_CFG.SLOT);

	// Write Song details
	write( VAR_SONG.TRANSPOSE);
	write( VAR_SONG.BPM);
	write( (VAR_SONG.PATTERNLENGTH <<4) | (VAR_SONG.GROOVE.LENGTH));
	write( (VAR_SONG.GROOVE.ENABLE <<2) | VAR_SONG.NOTEMPTY);//6 bit left unused!!
		
	// Write Artist and title
	for(i=0; i<14;i++){
		write(VAR_SONG.TITLE[i]);
		write(VAR_SONG.ARTIST[i]);
	} 

	drawPosition(27, 2, 2);	
	//0x140
	
	// Write Groove Steps
	forward(16 * VAR_CFG.SLOT);
	for(i=0; i<16; i++){
		write(VAR_SONG.GROOVE.STEP[i]);
	}
	
	//0x1A0
	drawPosition(27, 3, 2);	
	seek(0x200);
		
	forward(1536 * VAR_CFG.SLOT);	
	for(i=0; i<256; i++){
		write(VAR_SONG.PATTERNS[0].ORDER[i]);
		write(VAR_SONG.PATTERNS[1].ORDER[i]);
		write(VAR_SONG.PATTERNS[2].ORDER[i]);
		write(VAR_SONG.PATTERNS[3].ORDER[i]);
		write(VAR_SONG.PATTERNS[4].ORDER[i]);
		write(VAR_SONG.PATTERNS[5].ORDER[i]);
	}
	
	drawPosition(27, 4, 2);	
	
	sharedDataSave();
}

void Sram::songLoad(){
	int i;
	u8 h;
	
	seek(0x80);
	forward(32 * VAR_CFG.SLOT);
	
	// Song details
	VAR_SONG.TRANSPOSE = read();
	VAR_SONG.BPM = read();
		
	h = read();
	VAR_SONG.PATTERNLENGTH = EXTRACT( h, 4, 0xF);
	VAR_SONG.GROOVE.LENGTH = h & 0xF;
		
	h = read();
	VAR_SONG.GROOVE.ENABLE = EXTRACT( h, 2, 0x1);
	VAR_SONG.NOTEMPTY = h & 0x1;
		
	// Artist and title
	for(i=0; i<14;i++){
		VAR_SONG.TITLE[i]  = read();
		VAR_SONG.ARTIST[i] = read();
	} 

	drawPosition(27, 2, 6);	

	forward(16 * VAR_CFG.SLOT);
	for(i=0; i<16; i++){
		VAR_SONG.GROOVE.STEP[i] = read();
	}
		
	//1A0
	drawPosition(27, 3, 6);	
	seek(0x200);
	
	
	forward(1536 * VAR_CFG.SLOT);
	for(i=0; i<256; i++){
		VAR_SONG.PATTERNS[0].ORDER[i] = read();
		VAR_SONG.PATTERNS[1].ORDER[i] = read();
		VAR_SONG.PATTERNS[2].ORDER[i] = read();
		VAR_SONG.PATTERNS[3].ORDER[i] = read();
		VAR_SONG.PATTERNS[4].ORDER[i] = read();
		VAR_SONG.PATTERNS[5].ORDER[i] = read();
	}
	
	patternSync(VAR_CFG.ORDERPOSITION);
	cellSync();
	
	drawPosition(27, 4, 6);	
	
	sharedDataLoad();
}

void Sram::songDefaults(void){
	int i,di;
	
	for(i=0; i<14; i++){
		VAR_SONG.TITLE[i]  = 0xFF;
		VAR_SONG.ARTIST[i] = 0xFF;
	}
	//Sequencer::setTempo(0x80);
	VAR_SONG.BPM = 0x80;
	VAR_SONG.PATTERNLENGTH = 0xF;
	
	for(i=0; i<6; i++){
		VAR_INSTRUMENT.NAME[i] = 0xFF;
	}
	
	// Pattern data clear
	for(i=0; i<128; i++){
		for(di=0; di<16; di++){
			VAR_DATA[i].KEY[di] = 0;
			VAR_DATA[i].INS[di] = 0;
			VAR_DATA[i].VOL[di] = 0;
			VAR_DATA[i].CMD[di] = 0;
			VAR_DATA[i].VAL[di] = 0;
		}
	}

	// Live Perform table clear
	for(i=0; i<8; i++){
		VAR_LIVE.PERFORM.LEFT.KEY[i] = 0;
		VAR_LIVE.PERFORM.LEFT.INS[i] = 0;
		VAR_LIVE.PERFORM.LEFT.CHAN[i] = 0;
		VAR_LIVE.PERFORM.LEFT.VOL[i] = 0;
		VAR_LIVE.PERFORM.LEFT.CMD[i] = 0;
		VAR_LIVE.PERFORM.LEFT.VAL[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.KEY[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.INS[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.CHAN[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.VOL[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.CMD[i] = 0;
		VAR_LIVE.PERFORM.RIGHT.VAL[i] = 0;
	}

	// Empty Order chains
	for(i=0; i<6; i++){
		for(di=0; di<256; di++){
			VAR_SONG.PATTERNS[i].ORDER[di] = 0;
		}
		VAR_SONG.PATTERNS[i].POSITION = 0;
	}	
}





void Sram::sharedDataSave(void){
	int i, di;
	// Dump pattern data
	seek(0x2600);
	for(i=0;i<128;i++){
		for(di=0;di<16;di++){
			write(VAR_DATA[i].KEY[di]);
			write(VAR_DATA[i].INS[di]);
			write(VAR_DATA[i].VOL[di]);
			write(VAR_DATA[i].CMD[di]);
			write(VAR_DATA[i].VAL[di]);
		}
	}	
	
	//0x4E00
	drawPosition(27, 5, 2);	
	seek(0x5000);
	
	//Dump instruments (these are shared along all songs)
	instcopy(&VAR_INSTRUMENT, &VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT]);

	for(i=0; i<64; i++){
		write(VAR_INSTRUMENTS[i].TYPE);
		for(di=0; di<6; di++){
			write(VAR_INSTRUMENTS[i].NAME[di]);
		}
		for(di=0; di<32; di++){
			write(VAR_INSTRUMENTS[i].SETTINGS[di]);
		}
		for(di=0; di<16; di++){
			write(VAR_INSTRUMENTS[i].TABLE.TRANSPOSE[di]);
			write(VAR_INSTRUMENTS[i].TABLE.VOLUME[di]);
			write(VAR_INSTRUMENTS[i].TABLE.COMMAND[0][di]);
			write(VAR_INSTRUMENTS[i].TABLE.COMMAND[1][di]);
			write(VAR_INSTRUMENTS[i].TABLE.VALUE[0][di]);
			write(VAR_INSTRUMENTS[i].TABLE.VALUE[1][di]);
		}
	}

	drawPosition(27, 5, 2);	
}

void Sram::sharedDataLoad(void){
	int i, di;

	seek(0x2600);

	// Pattern data
	for(i=0;i<128;i++){
		for(di=0;di<16;di++){
			VAR_DATA[i].KEY[di] = read();
			VAR_DATA[i].INS[di] = read();
			VAR_DATA[i].VOL[di] = read();
			VAR_DATA[i].CMD[di] = read();
			VAR_DATA[i].VAL[di] = read();
		}
	}	
	
	//0x4E00
	drawPosition(27, 5, 6);	
	seek(0x5000);
	
	//Load instruments (these are shared along all songs)
	for(i=0; i<64; i++){
		VAR_INSTRUMENTS[i].TYPE = read();
		for(di=0; di<6; di++){
			VAR_INSTRUMENTS[i].NAME[di] = read();
		}
		for(di=0; di<32; di++){
			VAR_INSTRUMENTS[i].SETTINGS[di] = read();
		}
		// Dump instrument table
		for(di=0; di<16; di++){
			VAR_INSTRUMENTS[i].TABLE.TRANSPOSE[di] 	= read();
			VAR_INSTRUMENTS[i].TABLE.VOLUME[di] 	= read();
			VAR_INSTRUMENTS[i].TABLE.COMMAND[0][di] = read();
			VAR_INSTRUMENTS[i].TABLE.COMMAND[1][di] = read();
			VAR_INSTRUMENTS[i].TABLE.VALUE[0][di] 	= read();
			VAR_INSTRUMENTS[i].TABLE.VALUE[1][di] 	= read();
		}
	}

	instcopy(&VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT], &VAR_INSTRUMENT);
	
	Sequencer::setTempo( VAR_SONG.BPM );

	drawPosition(27, 5, 6);
}

void Sram::dataBackup(void){
	int i, di;
	
	// Dump 6 Song Headers (32 bytes each one, 192 total)
	seek(0x80);
	for(i=0; i<6; i++){
		// Song details
		write( VAR_SONGS[i].TRANSPOSE);
		write( VAR_SONGS[i].BPM);
		write( (VAR_SONGS[i].PATTERNLENGTH <<4) | (VAR_SONGS[i].GROOVE.LENGTH));
		write( (VAR_SONGS[i].GROOVE.ENABLE <<2) | VAR_SONGS[i].NOTEMPTY);//6 bit left unused!!
		
		// Artist and title
		for(di=0; di<14;di++){
			write(VAR_SONGS[i].TITLE[di]);
			write(VAR_SONGS[i].ARTIST[di]);
		} 
	} 

	//0x140
	
	// Groove Steps
	for(i=0; i<6; i++){
		for(di=0; di<16; di++){
			write(VAR_SONGS[i].GROOVE.STEP[di]);
		}
	} 
	
	//0x1A0
	
	// Dump pattern orders (256 * 6)
	seek(0x200);
	for(i=0; i<6; i++){
		for(di=0; di<256; di++){
			write(VAR_SONGS[i].PATTERNS[0].ORDER[di]);
			write(VAR_SONGS[i].PATTERNS[1].ORDER[di]);
			write(VAR_SONGS[i].PATTERNS[2].ORDER[di]);
			write(VAR_SONGS[i].PATTERNS[3].ORDER[di]);
			write(VAR_SONGS[i].PATTERNS[4].ORDER[di]);
			write(VAR_SONGS[i].PATTERNS[5].ORDER[di]);
		}
	}
	
	sharedDataSave();
	
	drawPosition(27, 1, 5);
}

void Sram::dataRevert(void){
	int i, di;
	u8  h;
	
	// Load 6 Song Headers (32 bytes each one, 192 total)
	seek(0x80);
	for(i=0; i<6; i++){
		// Song details
		VAR_SONGS[i].TRANSPOSE = read();
		VAR_SONGS[i].BPM = read();
		
		h = read();
		VAR_SONGS[i].PATTERNLENGTH = EXTRACT( h, 4, 0xF);
		VAR_SONGS[i].GROOVE.LENGTH = h & 0xF;
		
		h = read();
		VAR_SONGS[i].GROOVE.ENABLE = EXTRACT( h, 2, 0x1);
		VAR_SONGS[i].NOTEMPTY = h & 0x1;
		
		// Artist and title
		for(di=0; di<14;di++){
			VAR_SONGS[i].TITLE[di] = read();
			VAR_SONGS[i].ARTIST[di] = read();
		} 
	} 

	// Groove Steps (96 bytes)
	for(i=0; i<6; i++){
		for(di=0; di<16; di++){
			VAR_SONGS[i].GROOVE.STEP[di] = read();
		}
	} 
	
	
	// Dump pattern orders (256 * 6)
	seek(0x200);
	for(i=0; i<6; i++){
		for(di=0; di<256; di++){
			VAR_SONGS[i].PATTERNS[0].ORDER[di] = read();
			VAR_SONGS[i].PATTERNS[1].ORDER[di] = read();
			VAR_SONGS[i].PATTERNS[2].ORDER[di] = read();
			VAR_SONGS[i].PATTERNS[3].ORDER[di] = read();
			VAR_SONGS[i].PATTERNS[4].ORDER[di] = read();
			VAR_SONGS[i].PATTERNS[5].ORDER[di] = read();
		}
	}
	
	sharedDataLoad();
	
	drawPosition(27, 1, 7);
}
