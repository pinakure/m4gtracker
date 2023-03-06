#include "cfg.hpp"

CALLBACK( cb_cfg_menuindex	, modify5VAL		, EVENT_MODIFY_B			, &VAR_CFG.MENUSLOT				, NULL);

CALLBACK( cb_cfg_interface	, modify1BIT		, EVENT_MODIFY_B			, &VAR_CFG.LOOKNFEEL.INTERFACE	, NULL);
CALLBACK( cb_cfg_font		, modify2BIT		, EVENT_MODIFY_B			, &VAR_CFG.LOOKNFEEL.FONT		, NULL);
CALLBACK( cb_cfg_border		, modify2BIT		, EVENT_MODIFY_B			, &VAR_CFG.LOOKNFEEL.BORDER		, NULL);
CALLBACK( cb_cfg_showlogo	, modify1BIT		, EVENT_KEYDOWN_B			, &VAR_CFG.LOOKNFEEL.SHOWLOGO	, NULL);
CALLBACK( cb_cfg_startupsfx	, modify1BIT		, EVENT_KEYDOWN_B			, &VAR_CFG.LOOKNFEEL.STARTUPSFX	, NULL);
CALLBACK( cb_cfg_coloreditor, ColorEditor::run 	, EVENT_KEYUP_B				, NULL							, NULL);

CALLBACK( cb_cfg_linkmode	, Net::alterMode	, EVENT_MODIFY_B			, &VAR_CFG.LINKMODE.LINKMODE	, NULL);
CALLBACK( cb_cfg_masterclock, modify1BIT		, EVENT_KEYDOWN_B			, &VAR_CFG.LINKMODE.MASTERCLOCK	, NULL);
CALLBACK( cb_cfg_midichan	, modify4BIT		, EVENT_MODIFY_B			, &VAR_CFG.LINKMODE.MIDICHAN	, NULL);
CALLBACK( cb_cfg_clocktempo , modify8BIT		, EVENT_MODIFY_B			, &VAR_CFG.LINKMODE.CLOCKTEMPO	, NULL);
CALLBACK( cb_cfg_receivesong, RECEIVESONG		, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_sendsong	, SENDSONG			, EVENT_KEYUP_B				, NULL 							, NULL);

CALLBACK( cb_cfg_autoload	, modify1BIT		, EVENT_KEYDOWN_B			, &VAR_CFG.BEHAVIOR.AUTOLOAD	, NULL);
CALLBACK( cb_cfg_keyrate	, modify4BIT		, EVENT_MODIFY_B			, &VAR_CFG.BEHAVIOR.KEYRATE		, NULL);
CALLBACK( cb_cfg_buttonset	, modify2BIT		, EVENT_MODIFY_B			, &VAR_CFG.BEHAVIOR.BUTTONSET	, NULL);
CALLBACK( cb_cfg_saveconfig	, Config::save		, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_loadconfig	, Config::load		, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_initconfig	, Config::defaults	, EVENT_KEYUP_B				, NULL 							, NULL);

CALLBACK( cb_cfg_finetune	, modify4BIT		, EVENT_MODIFY_B			, &VAR_CFG.TRACKER.FINETUNE		, NULL);
CALLBACK( cb_cfg_prelisten	, modify1BIT		, EVENT_KEYDOWN_B			, &VAR_CFG.TRACKER.PRELISTEN	, NULL);
CALLBACK( cb_cfg_transpose	, modify8BIT		, EVENT_MODIFY_B			, &VAR_CFG.TRACKER.TRANSPOSE	, NULL);
CALLBACK( cb_cfg_inputmode	, modify1BIT		, EVENT_MODIFY_B			, &VAR_CFG.TRACKER.INPUTMODE	, NULL);
CALLBACK( cb_cfg_soundbias	, modify8BIT		, EVENT_MODIFY_B			, &VAR_CFG.TRACKER.SOUNDBIAS	, NULL);
CALLBACK( cb_cfg_mixer		, MIXER				, EVENT_KEYUP_B				, NULL 							, NULL);

CALLBACK( cb_cfg_prefetch	, modify1BIT		, EVENT_KEYDOWN_B			, &VAR_CFG.MEMORY.PREF	 		, NULL);
CALLBACK( cb_cfg_backup		, Config::backup	, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_revert		, Config::revert	, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_memorytest	, MEMORYTEST		, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_format		, Config::format 	, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_reset		, Config::reset		, EVENT_KEYUP_B				, NULL 							, NULL);

/*---------------------------------------------------------------------------*/

void Config::load(Control *c, bool bigstep, bool add, u32 *pointer){
	int i, di;
	u16 w;
	u8 h;
	EXPECT(10, SAVING, SETTINGS);
	
	// Check Signature and version
	SRAM.seek(0);
	if(SRAM.read32() != M4GEEK_SIGNATURE) return Config::defaults(c, 0, 0, pointer);
	if(SRAM.read() != M4G_VERSION) return Config::defaults(c, 0, 0, pointer);
	
	// Palette colors
	for(i=0;i<8; i++){
		di = i<<1;
		
		h = SRAM.read();		
		VAR_CFG.PAL[ di ].R = h << 4;
		VAR_CFG.PAL[ di ].G = h & 0x0F;
		
		h = SRAM.read();
		VAR_CFG.PAL[ di ].B = h << 4;
		VAR_CFG.PAL[di+1].R = h & 0x0F;
		
		h = SRAM.read();
		VAR_CFG.PAL[di+1].G = h << 4;
		VAR_CFG.PAL[di+1].B = h & 0x0F;
	}	
		
	// Look And Feel
	h = SRAM.read();	
	VAR_CFG.BEHAVIOR.AUTOLOAD 		= EXTRACT(h, 7, 0x1);
	VAR_CFG.LOOKNFEEL.STARTUPSFX 	= EXTRACT(h, 6, 0x1);
	VAR_CFG.LOOKNFEEL.SHOWLOGO 		= EXTRACT(h, 5, 0x1);
	VAR_CFG.LOOKNFEEL.INTERFACE 	= EXTRACT(h, 4, 0x1);
	VAR_CFG.LOOKNFEEL.FONT 			= EXTRACT(h, 2, 0x3);
	VAR_CFG.LOOKNFEEL.BORDER		= h & 0x3;
	
	// Link mode
	VAR_CFG.LINKMODE.LINKMODE 		= SRAM.read();
	VAR_CFG.LINKMODE.MASTERCLOCK 	= SRAM.read();
	VAR_CFG.LINKMODE.MIDICHAN 		= SRAM.read();
	VAR_CFG.LINKMODE.CLOCKTEMPO 	= SRAM.read();
	
	// Behavior, live and tracker
	VAR_CFG.BEHAVIOR.KEYRATE 		= SRAM.read();

	h = SRAM.read();
	VAR_LIVE.PERFORM.RETRIG 		= EXTRACT(h, 6, 0x1);
	VAR_CFG.MEMORY.PREF				= EXTRACT(h, 5, 0x1);
	VAR_CFG.TRACKER.HEADERTYPE		= EXTRACT(h, 4, 0x1);
	VAR_CFG.TRACKER.INPUTMODE		= EXTRACT(h, 3, 0x1);
	VAR_CFG.TRACKER.PRELISTEN		= EXTRACT(h, 2, 0x1);
	VAR_CFG.BEHAVIOR.BUTTONSET		= h & 0x3;
	
	VAR_CFG.TRACKER.TRANSPOSE = SRAM.read();
	VAR_CFG.TRACKER.SOUNDBIAS = SRAM.read();
	
	h = SRAM.read();
	VAR_LIVE.PERFORM.SPEED 			= EXTRACT(h, 4, 0xf);
	VAR_CFG.TRACKER.FINETUNE 		= h & 0x0F;
	
	h = SRAM.read();
	VAR_LIVE.PERFORM.QUANTIZE		= EXTRACT(h, 4, 0x3);
	VAR_LIVE.PIANO.QUANTIZE			= h & 0x3;
	
	h = SRAM.read();
	VAR_LIVE.PIANO.OCTAVE			= EXTRACT(h, 4, 0xf);
	VAR_LIVE.PIANO.MODE				= h & 0xf;
	
	VAR_LIVE.PIANO.TRANSPOSE 		= SRAM.read();
	
	for(i=0; i<2; i++){
		VAR_LIVE.PIANO.CHANNEL[i] = SRAM.read();
		VAR_LIVE.PIANO.MIDICHAN[i]= SRAM.read();
	}
	
	//46dec
	
	// Live Settings and tables
	SRAM.seek(47);
	for(i=0;i<8;i++){		
		w = SRAM.read16();
		VAR_LIVE.PERFORM.LEFT.KEY[i] = EXTRACT( w, 9, 0x7F);
		VAR_LIVE.PERFORM.LEFT.INS[i] = EXTRACT( w, 5, 0x3F);
		VAR_LIVE.PERFORM.LEFT.CMD[i] = w & 0x1F;

		w = SRAM.read16();
		VAR_LIVE.PERFORM.RIGHT.KEY[i] = EXTRACT( w, 9, 0x7F);
		VAR_LIVE.PERFORM.RIGHT.INS[i] = EXTRACT( w, 5, 0x3F);
		VAR_LIVE.PERFORM.RIGHT.CMD[i] = w & 0x1F;
		
		h = SRAM.read();
		VAR_LIVE.PERFORM.LEFT.CHAN[i] = EXTRACT( h, 4, 0xF);
		VAR_LIVE.PERFORM.LEFT.VOL[i] = h & 0xF;
		
		h = SRAM.read();
		VAR_LIVE.PERFORM.RIGHT.CHAN[i] = EXTRACT( h, 4, 0xF);
		VAR_LIVE.PERFORM.RIGHT.VOL[i] = h & 0xF;
		
		VAR_LIVE.PERFORM.LEFT.VAL[i] = SRAM.read();
		VAR_LIVE.PERFORM.RIGHT.VAL[i] = SRAM.read();
	}
	
	//0x6F
	//SRAM.drawPosition(27, 1, 7);
	
	// Load default song values (overriden if autoload was true on load)
	SRAM.songDefaults( false );
	
	// Force skins and fonts to be reloaded
	VAR_CFG.RELOADSKIN = true;
	
	LookNFeel::update( &regHnd );
		
	regHnd.progress.enabled = false;
	//regHnd.redraw = true;
}

void Config::backup	(Control *c, bool bigstep, bool add, u32 *pointer){
	SRAM.dataBackup( true );	
}

void Config::revert (Control *c, bool bigstep, bool add, u32 *pointer){
	SRAM.dataRevert( true );
}

void Config::save (Control *c, bool bigstep, bool add, u32 *pointer){
	int i, di;
	SRAM.seek(0);
	EXPECT(10, SAVING, SETTINGS);
	
	// Signature and version
	SRAM.write32(M4GEEK_SIGNATURE); //signature (cafe aces)
	SRAM.write(M4G_VERSION);		//version
	
	// Palette colors
	for(i=0;i<8; i++){
		di = i<<1;
		SRAM.write(VAR_CFG.PAL[ di ].R << 4 | VAR_CFG.PAL[ di ].G);  
		SRAM.write(VAR_CFG.PAL[ di ].B << 4 | VAR_CFG.PAL[di+1].R);			
		SRAM.write(VAR_CFG.PAL[di+1].G << 4 | VAR_CFG.PAL[di+1].B);
	}
		
		
	// Look And Feel
	SRAM.write( (VAR_CFG.BEHAVIOR.AUTOLOAD<<7) | (VAR_CFG.LOOKNFEEL.STARTUPSFX<<6) | (VAR_CFG.LOOKNFEEL.SHOWLOGO << 5) | (VAR_CFG.LOOKNFEEL.INTERFACE<<0x4) | (VAR_CFG.LOOKNFEEL.FONT << 2) | VAR_CFG.LOOKNFEEL.BORDER);
	
	// Link mode
	SRAM.write(VAR_CFG.LINKMODE.LINKMODE);
	SRAM.write(VAR_CFG.LINKMODE.MASTERCLOCK);
	SRAM.write(VAR_CFG.LINKMODE.MIDICHAN);
	SRAM.write(VAR_CFG.LINKMODE.CLOCKTEMPO);
	
	// Behavior
	SRAM.write(VAR_CFG.BEHAVIOR.KEYRATE);
	SRAM.write(  (VAR_LIVE.PERFORM.RETRIG<<6) | (VAR_CFG.MEMORY.PREF<<5)  | (VAR_CFG.TRACKER.HEADERTYPE<<4) | (VAR_CFG.TRACKER.INPUTMODE<<3) | (VAR_CFG.TRACKER.PRELISTEN<<2) | VAR_CFG.BEHAVIOR.BUTTONSET);
	
	// Tracker
	SRAM.write(VAR_CFG.TRACKER.TRANSPOSE);
	SRAM.write(VAR_CFG.TRACKER.SOUNDBIAS);
	SRAM.write( (VAR_LIVE.PERFORM.SPEED<<4) |(VAR_CFG.TRACKER.FINETUNE & 0x0F));
	SRAM.write((VAR_LIVE.PERFORM.QUANTIZE<<4) | VAR_LIVE.PIANO.QUANTIZE);
	//40 bytes
	
	SRAM.write((VAR_LIVE.PIANO.OCTAVE<<4) | VAR_LIVE.PIANO.MODE);
	SRAM.write(VAR_LIVE.PIANO.TRANSPOSE);
	for(i=0; i<2; i++){
		SRAM.write(VAR_LIVE.PIANO.CHANNEL[i]);
		SRAM.write(VAR_LIVE.PIANO.MIDICHAN[i]);
	}

	//46dec
	
	// Dump Live Settings and tables
	SRAM.seek(47);
	for(i=0;i<8;i++){		
		SRAM.write16(((VAR_LIVE.PERFORM.LEFT.KEY[i]&0x7f)<<9) | ((VAR_LIVE.PERFORM.LEFT.INS[i]&0x3f)<<5) | (VAR_LIVE.PERFORM.LEFT.CMD[i]&0x1f));
		SRAM.write16(((VAR_LIVE.PERFORM.RIGHT.KEY[i]&0x7f)<<9) | ((VAR_LIVE.PERFORM.RIGHT.INS[i]&0x3f)<<5) | (VAR_LIVE.PERFORM.RIGHT.CMD[i]&0x1f));
		SRAM.write(((VAR_LIVE.PERFORM.LEFT.CHAN[i]&0x0f)<<4) | (VAR_LIVE.PERFORM.LEFT.VOL[i]&0x0f));
		SRAM.write(((VAR_LIVE.PERFORM.RIGHT.CHAN[i]&0x0f)<<4) | (VAR_LIVE.PERFORM.RIGHT.VOL[i]&0x0f));
		SRAM.write(VAR_LIVE.PERFORM.LEFT.VAL[i]);
		SRAM.write(VAR_LIVE.PERFORM.RIGHT.VAL[i]);
	}
	
	//SRAM.drawPosition(27, 1, 7);
	//0x6F

	regHnd.progress.enabled = false;
	//regHnd.redraw = true;
}

void Config::format (Control *c, bool bigstep, bool add, u32 *pointer){
	if(c){
		ReallyDialog r;	
		r.enable();
		if(!r.result)return;
	}
	SRAM.erase();
	Config::defaults(c, 0,0,pointer);
	Config::save(c, 0,0,pointer);	
}

void Config::defaults (Control *c, bool bigstep, bool add, u32 *pointer){
	int i;
		
	#define SETTING(a, v)		{VAR_CFG.a = v; VAR_CFG.loadCount++;regHnd.update(1);}
	VAR_CFG.loadCount = 0;

	regHnd.progress.set(0, 233, STATUS_DEFAULTS, STATUS_SETTINGS, &VAR_CFG.loadCount);

	for(i=0; i<16; i++) {
		SETTING(PAL[i].R, 0x00);
		SETTING(PAL[i].G, 0x00);
		SETTING(PAL[i].B, 0x00);
		
		SETTING(GROOVE.STEP[i], 0x00);
		SETTING(INSTRUMENTTABLE.TRANSPOSE[i], 0x80);
		SETTING(INSTRUMENTTABLE.VOLUME[i], 0xF);
		SETTING(INSTRUMENTTABLE.COMMAND[0][i], 0);
		SETTING(INSTRUMENTTABLE.COMMAND[1][i], 0);
		SETTING(INSTRUMENTTABLE.VALUE[0][i], 0x00);
		SETTING(INSTRUMENTTABLE.VALUE[1][i], 0x00);
		
	}
	//160
	SETTING(INSTRUMENTTABLE.JUMP[0], 0x0F);
	SETTING(INSTRUMENTTABLE.JUMP[1], 0x0F);
	SETTING(INSTRUMENTTABLE.POSITION[0], 0x00);
	SETTING(INSTRUMENTTABLE.POSITION[1], 0x00);
	SETTING(INSTRUMENTTABLE.PLAYING, 0x00);
	//165
	SETTING(GROOVE.ENABLE, 0x0);
	SETTING(GROOVE.LENGTH, 0x0);
	//167
	SETTING(LOOKNFEEL.INTERFACE, 0x00);
	SETTING(LOOKNFEEL.FONT, 0x00);
	SETTING(LOOKNFEEL.BORDER, 0x00);
	SETTING(LOOKNFEEL.SHOWLOGO, 0x01);
	SETTING(LOOKNFEEL.STARTUPSFX, 0x01);
	//172
	SETTING(LINKMODE.LINKMODE, 0x00);
	SETTING(LINKMODE.MASTERCLOCK, 0x01);
	SETTING(LINKMODE.MIDICHAN, 0x00);
	SETTING(LINKMODE.CLOCKTEMPO, 0x8C); //140 BPM
	//176
	SETTING(BEHAVIOR.AUTOLOAD, 0x01);
	SETTING(BEHAVIOR.KEYRATE, 0x08);
	SETTING(BEHAVIOR.BUTTONSET, 0x00);
	SETTING(BEHAVIOR.VISTYPE, 0); //0 vis - 1 table
	//180
	SETTING(TRACKER.FINETUNE, 0x80);
	SETTING(TRACKER.PRELISTEN, 0x1);
	SETTING(TRACKER.TRANSPOSE, 0x80);
	SETTING(TRACKER.INPUTMODE, 0x0);
	SETTING(TRACKER.SOUNDBIAS, 0x00);
	SETTING(TRACKER.HEADERTYPE, 0x0); //N-U
	//186
	SETTING(MEMORY.PREF, 0x1); //READONLY
	SETTING(SLOT, 0);
	SETTING(MENUSLOT, 0);//45items.-
	SETTING(INSTRUMENTVISTYPE, 0);//45items.-
	//190
	
	// really of these?
	SETTING(CURRENTCHANNEL, 0);
	SETTING(CURRENTINSTRUMENT, 1);
	SETTING(ORDERPOSITION, 0);
	//193
	
	for(i=0; i<3; i++){
		SETTING(MUTE[0][i], 0);
		SETTING(MUTE[1][i], 0);
		SETTING(SOLO[0][i], 0);
		SETTING(SOLO[1][i], 0);
	}//205
	#undef SETTING
	
	SRAM.songDefaults( true );

	regHnd.progress.enabled = false;
	regHnd.redraw = true;
}

void Config::reset (Control *c, bool bigstep, bool add, u32 *pointer){
	sys.reset();
}

void Config::reinit (Control *c, bool bigstep, bool add, u32 *pointer){
	Config::format(c, 0,0,pointer);
	SRAM.sharedDataSave( true );
}

/*---------------------------------------------------------------------------*/

void ColorEditor::run(Control *c, bool bigstep, bool add, u32 *pointer){
	
}

void ColorEditor::update(RegionHandler* rh){

}

/*---------------------------------------------------------------------------*/

void MIXER(Control *c, bool bigstep, bool add, u32 *pointer){
	
}

void MEMORYTEST(Control *c, bool bigstep, bool add, u32 *pointer){
	u8 a1,a2,a3;
	int i;
	
	for(i=0;i<0x8000; i++){
		SRAM.seek(i); a1 = SRAM.read();
		SRAM.seek(i); a2 = SRAM.read();
		SRAM.seek(i); a3 = SRAM.read();
		
		if(((a1!=a2) || (a2!=a3)) || a1!=a3){
			SRAM.drawPosition(27,1,7);
			return;
		}
	}
	SRAM.drawPosition(27,1,0xF);
}

//----------------------------------------------------------------------------------------
void RECEIVESONG(Control *c, bool bigstep, bool add, u32 *pointer){

}
void SENDSONG(Control *c, bool bigstep, bool add, u32 *pointer){

}

#define MOSAIC_CNT *(u16*)0x400004C
#define REG_BG0 *(u16*)0x4000008
#define REG_BG1 *(u16*)0x400000A
#define REG_BG2 *(u16*)0x400000C
#define REG_BG3 *(u16*)0x400000E

void LookNFeel::logoFade(){
	int freeze=0;
	REG_BG0 |= 0x0040;
	REG_BG1 |= 0x0040;
	REG_BG2 |= 0x0040;
	REG_BG3 |= 0x0040;
	if( VAR_CFG.LOOKNFEEL.STARTUPSFX ) Mixer::start();
			
	while( freeze < 0xfff>>3 ){
		regHnd.drawVerticalCache(8,7,&CACHE_LOGOTYPE,0,0);
		sys.update();
		MOSAIC_CNT = 0xFF - ((freeze >> 1)&0xFF);
		if( VAR_CFG.LOOKNFEEL.STARTUPSFX ) {
			Synth::polysynth( freeze >> 1 );
		}
		if(!gpu.isVblank()) {
			freeze++;
			for(int i=0;i<16;i++) {
				int c1 = Palette[4+((freeze)&0x7)];
				int r, g, b;
				if(!(freeze&7)||!(freeze&3)){
					r = (u8)( ( (c1 & 0x001f)	   )  * ((freeze*3) / (0xf * 3))); 
					g = (u8)( ( (c1 & 0x02e0) >> 5  ) * ((freeze*3) / (0xf * 3)));
					b = (u8)( ( (c1 & 0x3d00) >> 10 ) * ((freeze*3) / (0xf * 3)));
				} else {
					r = 0;
					g = 0;
					b = 0;
				}
				c1 = (b << 10) | (g << 5) | r;
				*(u16*)(BG_PALETTE+(i*0x20)) 	  =	c1;
				*(u16*)(BG_PALETTE+(i*0x20)+0x10) =	c1;
			}		
		}
	}
	if( VAR_CFG.LOOKNFEEL.STARTUPSFX ) Mixer::stop();
	
	REG_BG0 	^= 0x0040;
	REG_BG1 	^= 0x0040;
	REG_BG2 	^= 0x0040;
	REG_BG3 	^= 0x0040;
	MOSAIC_CNT 	 = 0x0000;
}

void LookNFeel::logoWait(){
	int freeze=0;
	
	gpu.loadPalette();
	while( freeze < 2048 ){
		regHnd.drawVerticalCache(8,7,&CACHE_LOGOTYPE,0,0);
		sys.update();
		if( KEY.activity() ) {
			break;
		}
		/*
		for(int i=0;i<16;i++) {
			*(u16*)(BG_PALETTE+(i*0x20))		 =	Palette[(i+freeze)&0xFF];
			*(u16*)(BG_PALETTE+(i*0x20)+0x10)	 =	Palette[((i+freeze)+i*0x2)&0xFF];
		}*/
		 
		freeze++;
	}
	gpu.loadPalette();
}

void LookNFeel::init(){
	if( VAR_CFG.LOOKNFEEL.SHOWLOGO ){
		logoFade();
		logoWait();
	}
}

// Called when a look and feel value is changed
void LookNFeel::update( RegionHandler* rh ){
	static u8 lastBorder = VAR_CFG.LOOKNFEEL.BORDER;
	static u8 lastFont = VAR_CFG.LOOKNFEEL.FONT;
	static u32 bigIndex = 0;
	static u8 index = 3;
	
	#define ROWSIZE 		(16*32)
	#define FONTSIZE 		(ROWSIZE * 3)
	
	//#define CYCLEPOS 		CHAR_BASE0_ADDR+(ROWSIZE*4)
	#define CYCLEPOS 		CHAR_BASE0_ADDR+(ROWSIZE*4)
	#define CYCLEPOS1 	CYCLEPOS+(index*32)
	#define CYCLEPOS2 	CYCLEPOS+(32*6)+(index*32)
	#define CYCLEBAK		CHAR_BASE0_ADDR+(ROWSIZE*10)
	if(bigIndex < 100) { bigIndex++; } else {
		// Draw M4G logo
		DmaCopy(3, CYCLEPOS1, CYCLEBAK, 32, 16);
		DmaCopy(3, CYCLEPOS2, CYCLEPOS1, 32, 16);
		DmaCopy(3, CYCLEBAK, CYCLEPOS2, 32, 16);
	
		index++;
		if(index == 6)index=0;
		
		
		
		bigIndex = 0x0;
	}
	
	// Reload HUD style
	if((VAR_CFG.LOOKNFEEL.BORDER != lastBorder)||(VAR_CFG.RELOADSKIN)){
		lastBorder = VAR_CFG.LOOKNFEEL.BORDER;
		DmaCopy(3, CHAR_BASE1_ADDR+ROWSIZE+(ROWSIZE*lastBorder),CHAR_BASE1_ADDR, ROWSIZE, 16);
	}

	// Reload font		
	if((VAR_CFG.LOOKNFEEL.FONT != lastFont)||(VAR_CFG.RELOADSKIN)){
		lastFont = VAR_CFG.LOOKNFEEL.FONT;
		DmaCopy(3, CHAR_BASE2_ADDR+(ROWSIZE*36)+(FONTSIZE*lastFont),CHAR_BASE2_ADDR, FONTSIZE, 16);
		VAR_CFG.RELOADSKIN = false;
	}
	#undef FONTSIZE
	#undef ROWSIZE	
}


static void otherBlit(const u16 *map_address, int startx, int starty, int x, int y, int width, int height){
	int offsetSrc;
	int offsetDst;
	
	width = width << 1; //double word per item!
	
	x<<=1;
	
	//R_DISPCNT = DISP_FORCE_HBLANK | (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;			
	for(int sy=starty; sy<height+starty; sy++){
		offsetSrc = (sy<<6)+startx;
		offsetDst = (y<<6)+x;
		
		DmaCopy(3, map_address + offsetSrc, (SCREEN_BASE0_ADDR)+offsetDst, width, 16);
		DmaCopy(3, map_address + 4096 + offsetSrc, (SCREEN_BASE1_ADDR)+offsetDst, width, 16);
		DmaCopy(3, map_address + 8192 + offsetSrc, (SCREEN_BASE2_ADDR)+offsetDst, width, 16);
		y++;
	}
	//R_DISPCNT = (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;
}

void updateLINKMODE(RegionHandler* rh){
	const Region *c = &REGION_MAP_4_LINKSTATUS;	
	otherBlit(MAPDATA + ((MAP_CFG * 3) << 12), c->x, c->y, 0xb, 0xf, c->width, c->height);
}

void updateBEHAVIOR(RegionHandler* rh){
}

void updateTRACKER(RegionHandler* rh){
	const Region *c = &REGION_MAP_4_CHANNELMIXER;	
	otherBlit(MAPDATA + ((MAP_CFG * 3) << 12), c->x, c->y, 0xb, 0xf, c->width, c->height);
}

void updateMEMORY(RegionHandler* rh){
}
void updateCHANNELMIXER(RegionHandler* rh){
}
void updateMEMORYSONGMAP(RegionHandler* rh){
}
void updateMEMORYTEST(RegionHandler* rh){
}
void updateLINKSTATUS(RegionHandler* rh){
}
void updateCREDITS(RegionHandler* rh){
}
