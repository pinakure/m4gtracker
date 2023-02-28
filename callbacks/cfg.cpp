/* ----------------------------------------------------------------------------
AUTHOR		 	Al P.Area ( Smiker )
PURPOSE			Callback routines for the controls at CONFIG screen and 
				related update routines and helpers.
ORIGINAL DATE 	2016, October
REVISION DATE 	2023-02-28
 --------------------------------------------------------------------------- */
#include "callbacks.hpp"
#include "../data/enum.h"
#include "../data/regions.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/regionhandler/regionhandler.hpp"

#include "../macros.hpp"
#include "../data/layers.hpp"
#include "../modules/sys/sys.hpp"
#include "../modules/sram/sram.hpp"

#define CALLBACK(n, c, t, v, nx)			const Callback n = { c , t , v, nx}
CALLBACK( cb_cfg_menuindex	, modify5Val		, EVENT_MODIFY_B			, &CFG::menu_slot				, NULL);
//-----------------------------------------------------------------------------
CALLBACK( cb_cfg_interface	, modify1Bit		, EVENT_MODIFY_B			, &CFG::look_n_feel.interface	, NULL);
CALLBACK( cb_cfg_font		, modify2Bit		, EVENT_MODIFY_B			, &CFG::look_n_feel.font		, NULL);
CALLBACK( cb_cfg_border		, modify2Bit		, EVENT_MODIFY_B			, &CFG::look_n_feel.border		, NULL);
CALLBACK( cb_cfg_showlogo	, modify1Bit		, EVENT_KEYDOWN_B			, &CFG::look_n_feel.show_logo	, NULL);
CALLBACK( cb_cfg_startupsfx	, modify1Bit		, EVENT_KEYDOWN_B			, &CFG::look_n_feel.startup_sfx , NULL);
CALLBACK( cb_cfg_coloreditor, colorEditor		, EVENT_KEYUP_B				, NULL							, NULL);
//-----------------------------------------------------------------------------
CALLBACK( cb_cfg_linkmode	, modify2Bit		, EVENT_MODIFY_B			, &CFG::link_mode.link_mode		, NULL);
CALLBACK( cb_cfg_masterclock, modify1Bit		, EVENT_KEYDOWN_B			, &CFG::link_mode.master_clock	, NULL);
CALLBACK( cb_cfg_midichan	, modify4Bit		, EVENT_MODIFY_B			, &CFG::link_mode.midi_chan		, NULL);
CALLBACK( cb_cfg_clocktempo , modify8Bit		, EVENT_MODIFY_B			, &CFG::link_mode.clock_tempo	, NULL);
CALLBACK( cb_cfg_receivesong, receiveSong		, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_sendsong	, sendSong			, EVENT_KEYUP_B				, NULL 							, NULL);
//-----------------------------------------------------------------------------
CALLBACK( cb_cfg_autoload	, modify1Bit		, EVENT_KEYDOWN_B			, &CFG::behavior.auto_load		, NULL);
CALLBACK( cb_cfg_keyrate	, modify4Bit		, EVENT_MODIFY_B			, &CFG::behavior.key_rate		, NULL);
CALLBACK( cb_cfg_buttonset	, modify2Bit		, EVENT_MODIFY_B			, &CFG::behavior.button_set		, NULL);
CALLBACK( cb_cfg_saveconfig	, saveConfig		, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_loadconfig	, loadConfig		, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_initconfig	, defaultConfig		, EVENT_KEYUP_B				, NULL 							, NULL);
//-----------------------------------------------------------------------------
CALLBACK( cb_cfg_finetune	, modify4Bit		, EVENT_MODIFY_B			, &CFG::tracker.fine_tune		, NULL);
CALLBACK( cb_cfg_prelisten	, modify1Bit		, EVENT_KEYDOWN_B			, &CFG::tracker.prelisten		, NULL);
CALLBACK( cb_cfg_transpose	, modify8Bit		, EVENT_MODIFY_B			, &CFG::tracker.transpose		, NULL);
CALLBACK( cb_cfg_inputmode	, modify1Bit		, EVENT_MODIFY_B			, &CFG::tracker.input_mode		, NULL);
CALLBACK( cb_cfg_soundbias	, modify8Bit		, EVENT_MODIFY_B			, &CFG::tracker.sound_bias		, NULL);
CALLBACK( cb_cfg_mixer		, mixer				, EVENT_KEYUP_B				, NULL 							, NULL);
//-----------------------------------------------------------------------------
CALLBACK( cb_cfg_prefetch	, modify1Bit		, EVENT_KEYDOWN_B			, &CFG::memory.pref				, NULL);
CALLBACK( cb_cfg_backup		, slotUsage			, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_revert		, purgeSongs		, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_memorytest	, memoryTest		, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_format		, formatMemory		, EVENT_KEYUP_B				, NULL 							, NULL);
CALLBACK( cb_cfg_reset		, reset				, EVENT_KEYUP_B				, NULL 							, NULL);
#undef CALLBACK

void saveConfig(Control *c, bool bigstep, bool add, u32 *pointer){
	int i, di;
	SRAM::seek(0);
	EXPECT(10, SAVING, SETTINGS);
	
	// Signature and version
	SRAM::write32(M4GEEK_SIGNATURE); //signature (cafe aces)
	SRAM::write(M4G_VERSION);		//version
	
	// Palette colors
	for(i=0;i<8; i++){
		di = i<<1;
		SRAM::write(CFG::pal[ di ].r << 4 | CFG::pal[ di ].g);  
		SRAM::write(CFG::pal[ di ].b << 4 | CFG::pal[di+1].r);			
		SRAM::write(CFG::pal[di+1].g << 4 | CFG::pal[di+1].b);
	}
		
		
	// Look And Feel
	SRAM::write( (CFG::behavior.auto_load << 7) | (CFG::look_n_feel.startup_sfx << 6) | (CFG::look_n_feel.show_logo << 5) | (CFG::look_n_feel.interface << 0x4) | (CFG::look_n_feel.font << 2) | CFG::look_n_feel.border );
	
	// Link mode
	SRAM::write( CFG::link_mode.link_mode		);
	SRAM::write( CFG::link_mode.master_clock	);
	SRAM::write( CFG::link_mode.midi_chan		);
	SRAM::write( CFG::link_mode.clock_tempo		);
	
	// Behavior
	SRAM::write( CFG::behavior.key_rate			);
	SRAM::write(  (live.perform.retrig << 6) | (CFG::memory.pref << 5)  | (CFG::tracker.header_type << 4) | (CFG::tracker.input_mode << 3) | (CFG::tracker.prelisten << 2) | CFG::behavior.button_set);
	
	// Tracker
	SRAM::write( CFG::tracker.transpose			);
	SRAM::write( CFG::tracker.sound_bias		);
	SRAM::write( ( live.perform.speed 	 << 4 ) | ( CFG::tracker.fine_tune & 0x0F ) );
	SRAM::write( ( live.perform.quantize << 4 ) | live.piano.quantize );
	//40 bytes
	
	SRAM::write( ( live.piano.octave << 4) | live.piano.mode );
	SRAM::write( live.piano.transpose );
	for( i=0; i < 2; i++ ){
		SRAM::write(live.piano.channel[ i ] );
		SRAM::write(live.piano.midi_chan[ i ] );
	}

	//46dec
	
	// Dump Live Settings and tables
	SRAM::seek( 47 );
	for( i=0; i < 8; i++ ){		
		SRAM::write16(((live.perform.left.key[i]&0x7f)<<9) | ((live.perform.left.ins[i]&0x3f)<<5) | (live.perform.left.cmd[i]&0x1f));
		SRAM::write16(((live.perform.right.key[i]&0x7f)<<9) | ((live.perform.right.ins[i]&0x3f)<<5) | (live.perform.right.cmd[i]&0x1f));
		SRAM::write(((live.perform.left.chan[i]&0x0f)<<4) | (live.perform.left.vol[i]&0x0f));
		SRAM::write(((live.perform.right.chan[i]&0x0f)<<4) | (live.perform.right.vol[i]&0x0f));
		SRAM::write(live.perform.left.val[i]);
		SRAM::write(live.perform.right.val[i]);
	}
	
	SRAM::drawPosition(27, 1, 7);
	//0x6F

	REGHND::progress.enabled = false;
	//REGHND::redraw = true;
}

void loadConfig(Control *c, bool bigstep, bool add, u32 *pointer){
	int i, di;
	u16 w;
	u8 h;
	
	CFG::load_count = 0;
	REGHND::progress.set(0, 10, STATUS_SAVING, STATUS_SETTINGS, &(CFG::load_count));
	EXPECT(10, SAVING, SETTINGS);
	
	// Check Signature and version
	SRAM::seek(0);
	if(SRAM::read32() != M4GEEK_SIGNATURE) return defaultConfig(c, 0, 0, pointer);
	if(SRAM::read() != M4G_VERSION) return defaultConfig(c, 0, 0, pointer);
	// Palette colors
	for(i=0;i<8; i++){
		di = i<<1;
		
		h = SRAM::read();		
		CFG::pal[ di ].r = h << 4;
		CFG::pal[ di ].g = h & 0x0F;
		
		h = SRAM::read();
		CFG::pal[ di ].b = h << 4;
		CFG::pal[di+1].r = h & 0x0F;
		
		h = SRAM::read();
		CFG::pal[di+1].g = h << 4;
		CFG::pal[di+1].b = h & 0x0F;
	}	
		
	// Look And Feel
	h = SRAM::read();	
	CFG::behavior.auto_load			= EXTRACT(h, 7, 0x1);
	CFG::look_n_feel.startup_sfx 	= EXTRACT(h, 6, 0x1);
	CFG::look_n_feel.show_logo		= EXTRACT(h, 5, 0x1);
	CFG::look_n_feel.interface		= EXTRACT(h, 4, 0x1);
	CFG::look_n_feel.font			= EXTRACT(h, 2, 0x3);
	CFG::look_n_feel.border			= h & 0x3;
	
	// Link mode
	CFG::link_mode.link_mode		= SRAM::read();
	CFG::link_mode.master_clock		= SRAM::read();
	CFG::link_mode.midi_chan		= SRAM::read();
	CFG::link_mode.clock_tempo		= SRAM::read();
	
	// Behavior, live and tracker
	CFG::behavior.key_rate			= SRAM::read();

	h = SRAM::read();
	live.perform.retrig				= EXTRACT(h, 6, 0x1);
	CFG::memory.pref				= EXTRACT(h, 5, 0x1);
	CFG::tracker.header_type		= EXTRACT(h, 4, 0x1);
	CFG::tracker.input_mode			= EXTRACT(h, 3, 0x1);
	CFG::tracker.prelisten			= EXTRACT(h, 2, 0x1);
	CFG::behavior.button_set		= h & 0x3;
	
	CFG::tracker.transpose			= SRAM::read();
	CFG::tracker.sound_bias			= SRAM::read();
	
	h = SRAM::read();
	live.perform.speed				= EXTRACT(h, 4, 0xf);
	CFG::tracker.fine_tune			= h & 0x0F;
	
	h = SRAM::read();
	live.perform.quantize			= EXTRACT(h, 4, 0x3);
	live.piano.quantize				= h & 0x3;
	
	h = SRAM::read();
	live.piano.octave				= EXTRACT(h, 4, 0xf);
	live.piano.mode					= h & 0xf;
	
	live.piano.transpose			= SRAM::read();
	
	for( i=0; i < 2; i++ ){
		live.piano.channel[ i ] 	= SRAM::read();
		live.piano.midi_chan[ i ]	= SRAM::read();
	}
	
	//46dec
	
	// Live Settings and tables
	SRAM::seek(47);
	for(i=0;i<8;i++){		
		w = SRAM::read16();
		live.perform.left.key[i] = EXTRACT( w, 9, 0x7F);
		live.perform.left.ins[i] = EXTRACT( w, 5, 0x3F);
		live.perform.left.cmd[i] = w & 0x1F;

		w = SRAM::read16();
		live.perform.right.key[i] = EXTRACT( w, 9, 0x7F);
		live.perform.right.ins[i] = EXTRACT( w, 5, 0x3F);
		live.perform.right.cmd[i] = w & 0x1F;
		
		h = SRAM::read();
		live.perform.left.chan[i] = EXTRACT( h, 4, 0xF);
		live.perform.left.vol[i]  = h & 0xF;
		
		h = SRAM::read();
		live.perform.right.chan[i]= EXTRACT( h, 4, 0xF);
		live.perform.right.vol[i] = h & 0xF;
		
		live.perform.left.val[i]  = SRAM::read();
		live.perform.right.val[i] = SRAM::read();
	}
	
	//0x6F
	SRAM::drawPosition(27, 1, 7);
	
	// Load default song values (overriden if autoload was true on load)
	SRAM::songDefaults();
	
	// Force skins and fonts to be reloaded
	CFG::reload_skin = true;
	updateLookNFeel();
	
	
	REGHND::progress.enabled = false;
	//REGHND::redraw = true;
}

void defaultConfig(Control *c, bool bigstep, bool add, u32 *pointer){
	int i;
		
	#define SETTING(a, v)		{CFG::a = v; CFG::load_count++;REGHND::update(1);}
	CFG::load_count = 0;

	REGHND::progress.set(0, 233, STATUS_DEFAULTS, STATUS_SETTINGS, &CFG::load_count);

	for(i=0; i<16; i++) {
		SETTING( pal[i].r, 0x00);
		SETTING( pal[i].g, 0x00);
		SETTING( pal[i].b, 0x00);
		
		SETTING(groove.step[ i ]				, 0x00);
		SETTING(instrument_table.TRANSPOSE[i]	, 0x80);
		SETTING(instrument_table.VOLUME[i]		, 0xF);
		SETTING(instrument_table.COMMAND[0][i]	, 0);
		SETTING(instrument_table.COMMAND[1][i]	, 0);
		SETTING(instrument_table.VALUE[0][i]		, 0x00);
		SETTING(instrument_table.VALUE[1][i]		, 0x00);
		
	}
	//160
	SETTING(instrument_table.JUMP[0]		, 0x0F);
	SETTING(instrument_table.JUMP[1]		, 0x0F);
	SETTING(instrument_table.POSITION[0]	, 0x00);
	SETTING(instrument_table.POSITION[1]	, 0x00);
	SETTING(instrument_table.PLAYING		, 0x00);
	//165
	SETTING(groove.enable					, 0x0);
	SETTING(groove.length					, 0x0);
	//167
	SETTING(look_n_feel.interface			, 0x00);
	SETTING(look_n_feel.font				, 0x00);
	SETTING(look_n_feel.border				, 0x00);
	SETTING(look_n_feel.show_logo			, 0x01);
	SETTING(look_n_feel.startup_sfx			, 0x01);
	//172
	SETTING(link_mode.link_mode		, 0x00);
	SETTING(link_mode.master_clock	, 0x01);
	SETTING(link_mode.midi_chan		, 0x00);
	SETTING(link_mode.clock_tempo	, 0x8C); //140 BPM
	//176
	SETTING(behavior.auto_load		, 0x01);
	SETTING(behavior.key_rate		, 0x08);
	SETTING(behavior.button_set		, 0x00);
	SETTING(behavior.vis_type		, 0); //0 vis - 1 table
	//180
	SETTING(tracker.fine_tune		, 0x80);
	SETTING(tracker.prelisten		, 0x1);
	SETTING(tracker.transpose		, 0x80);
	SETTING(tracker.input_mode		, 0x0);
	SETTING(tracker.sound_bias		, 0x00);
	SETTING(tracker.header_type		, 0x0); //N-U
	//186
	SETTING(memory.pref				, 0x1); //READONLY
	SETTING(slot					, 0);
	SETTING(menu_slot				, 0);//45items.-
	SETTING(instrument_vis_type		, 0);//45items.-
	//190
	
	// really of these?
	SETTING(current_channel			, 0);
	SETTING(current_instrument		, 0);
	SETTING(order_position			, 0);
	//193
	
	for(i=0; i<3; i++){
		SETTING(mute[0][i], 0);
		SETTING(mute[1][i], 0);
		SETTING(solo[0][i], 0);
		SETTING(solo[1][i], 0);
	}//205
	#undef SETTING
	
	SRAM::songDefaults();

	REGHND::progress.enabled = false;
	REGHND::redraw = true;
}

void colorEditor(Control *c, bool bigstep, bool add, u32 *pointer){
	
}
void mixer(Control *c, bool bigstep, bool add, u32 *pointer){
	
}

//@refactor BACKUP
void slotUsage(Control *c, bool bigstep, bool add, u32 *pointer){
	//useless now.
	SRAM::dataBackup();
}

//@refactor REVERT
void purgeSongs(Control *c, bool bigstep, bool add, u32 *pointer){
	//useless now.
	SRAM::dataRevert();
}

void memoryTest(Control *c, bool bigstep, bool add, u32 *pointer){
	u8 a1,a2,a3;
	int i;
	
	for(i=0;i<0x8000; i++){
		SRAM::seek(i); a1 = SRAM::read();
		SRAM::seek(i); a2 = SRAM::read();
		SRAM::seek(i); a3 = SRAM::read();
		
		if(((a1!=a2) || (a2!=a3)) || a1!=a3){
			SRAM::drawPosition(27,1,7);
			return;
		}
	}
	SRAM::drawPosition(27,1,0xF);
}

void reset(Control *c, bool bigstep, bool add, u32 *pointer){
	SYS::reset();
}

void reinitialize(Control *c, bool bigstep, bool add, u32 *pointer){
	formatMemory(c, 0,0,pointer);
	SRAM::sharedDataSave();
}

void formatMemory(Control *c, bool bigstep, bool add, u32 *pointer){
	if(c){
		ReallyDialog r;	
		r.enable();
		if(!r.result)return;
	}
	SRAM::erase();
	defaultConfig(c, 0,0,pointer);
	saveConfig(c, 0,0,pointer);	
}
//----------------------------------------------------------------------------------------
void receiveSong(Control *c, bool bigstep, bool add, u32 *pointer){

}
void sendSong(Control *c, bool bigstep, bool add, u32 *pointer){

}

// Called when a look and feel value is changed
void updateLookNFeel(){
	static u8 lastBorder = CFG::look_n_feel.border;
	static u8 lastFont = CFG::look_n_feel.font;
	static u32 bigIndex = 0;
	static u8 index = 3;
	
	#define ROWSIZE 		(16*32)
	#define FONTSIZE 		(ROWSIZE * 3)
	
	//#define CYCLEPOS 		CHAR_BASE0_ADDR+(ROWSIZE*4)
	#define CYCLEPOS 		CHAR_BASE0_ADDR+(ROWSIZE*4)
	#define CYCLEPOS1 		CYCLEPOS+(index*32)
	#define CYCLEPOS2 		CYCLEPOS+(32*6)+(index*32)
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
	if( ( CFG::look_n_feel.border != lastBorder ) || ( CFG::reload_skin ) ){
		lastBorder = CFG::look_n_feel.border;
		DmaCopy(3, CHAR_BASE1_ADDR+ROWSIZE+(ROWSIZE*lastBorder),CHAR_BASE1_ADDR, ROWSIZE, 16);
	}

	// Reload font		
	if( ( CFG::look_n_feel.font != lastFont ) || ( CFG::reload_skin ) ){
		lastFont = CFG::look_n_feel.font;
		DmaCopy(3, CHAR_BASE2_ADDR+(ROWSIZE*36)+(FONTSIZE*lastFont),CHAR_BASE2_ADDR, FONTSIZE, 16);
		CFG::reload_skin = false;
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

void updateLinkMode(){
	const Region *c = &REGION_MAP_4_LINKSTATUS;	
	otherBlit(Layers::DATA + (( GPU::MAP_CFG * 3) << 12), c->x, c->y, 0xb, 0xf, c->width, c->height);
}

void updateBehavior(){
}

void updateTracker(){
	const Region *c = &REGION_MAP_4_CHANNELMIXER;	
	otherBlit(Layers::DATA + (( GPU::MAP_CFG * 3) << 12), c->x, c->y, 0xb, 0xf, c->width, c->height);
}

void updateMemory(){
}
void updateColorEditor(){
}
void updateChannelMixer(){
}
void updateMemorySongMap(){
}
void updateMemoryTest(){
}
void updateLinkStatus(){
}
void updateCredits(){
}
