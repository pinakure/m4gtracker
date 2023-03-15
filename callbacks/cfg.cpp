#include "cfg.hpp"

#include "../macros.h"
#include "../timestamp.h"
#include "../modules/sys/sys.hpp"
#include "../modules/net/net.hpp"
#include "../modules/sram/sram.hpp"
#include "../modules/spu/mixer.hpp"
#include "../modules/spu/synth.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"
#include "../callbacks/debug.hpp"
#include "../data/data.hpp"

/*---------------------------------------------------------------------------------------------------------------------*/
const Callback cb_cfg_menuindex		= { modify5VAL					, EVENT_MODIFY_B	, &VAR_CFG.MENUSLOT				, NULL };
// LookNFeel Submenu		
const Callback cb_cfg_interface		= { modify1BIT					, EVENT_MODIFY_B	, &VAR_CFG.LOOKNFEEL.INTERFACE	, NULL };
const Callback cb_cfg_font			= { modify2BIT					, EVENT_MODIFY_B	, &VAR_CFG.LOOKNFEEL.FONT		, NULL };
const Callback cb_cfg_border		= { modify2BIT					, EVENT_MODIFY_B	, &VAR_CFG.LOOKNFEEL.BORDER		, NULL };
const Callback cb_cfg_showlogo		= { modify1BIT					, EVENT_KEYDOWN_B	, &VAR_CFG.LOOKNFEEL.SHOWLOGO	, NULL };
const Callback cb_cfg_startupsfx	= { modify1BIT					, EVENT_KEYDOWN_B	, &VAR_CFG.LOOKNFEEL.STARTUPSFX	, NULL };
const Callback cb_cfg_coloreditor	= { ColorEditor::show 			, EVENT_KEYUP_B		, NULL							, NULL };
// LinkMode Submenu
const Callback cb_cfg_linkmode		= { Net::alterMode				, EVENT_MODIFY_B	, &VAR_CFG.LINKMODE.LINKMODE	, NULL };
const Callback cb_cfg_masterclock	= { LinkMode::toggleMaster		, EVENT_KEYDOWN_B	, &VAR_CFG.LINKMODE.MASTERCLOCK	, NULL };
const Callback cb_cfg_syncrate		= { modify4BIT					, EVENT_MODIFY_B	, &VAR_CFG.LINKMODE.SYNCRATE	, NULL };
const Callback cb_cfg_syncticks  	= { modify8BIT					, EVENT_MODIFY_B	, &VAR_CFG.LINKMODE.SYNCTICKS	, NULL };
const Callback cb_cfg_receivesong	= { LinkMode::songRecv			, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_sendsong		= { LinkMode::songSend			, EVENT_KEYUP_B		, NULL 							, NULL };
// Behavior SubMenu
const Callback cb_cfg_autoload		= { modify1BIT					, EVENT_KEYDOWN_B	, &VAR_CFG.BEHAVIOR.AUTOLOAD	, NULL };
const Callback cb_cfg_autosave		= { modify1BIT					, EVENT_KEYDOWN_B	, &VAR_CFG.BEHAVIOR.AUTOSAVE	, NULL };
const Callback cb_cfg_keyrate		= { modify4BIT					, EVENT_MODIFY_B	, &VAR_CFG.BEHAVIOR.KEYRATE		, NULL };
const Callback cb_cfg_saveconfig	= { Config::save				, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_loadconfig	= { Config::load				, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_initconfig	= { Config::defaults			, EVENT_KEYUP_B		, NULL 							, NULL };
// Tracker SubMenu
const Callback cb_cfg_finetune		= { modify4BIT					, EVENT_MODIFY_B	, &VAR_CFG.TRACKER.FINETUNE		, NULL };
const Callback cb_cfg_prelisten		= { modify1BIT					, EVENT_KEYDOWN_B	, &VAR_CFG.TRACKER.PRELISTEN	, NULL };
const Callback cb_cfg_transpose		= { modify8BIT					, EVENT_MODIFY_B	, &VAR_CFG.TRACKER.TRANSPOSE	, NULL };
const Callback cb_cfg_inputmode		= { modify1BIT					, EVENT_MODIFY_B	, &VAR_CFG.TRACKER.INPUTMODE	, NULL };
const Callback cb_cfg_soundbias		= { modify8BIT					, EVENT_MODIFY_B	, &VAR_CFG.TRACKER.SOUNDBIAS	, NULL };
const Callback cb_cfg_mixer			= { Mixer::show					, EVENT_KEYUP_B		, NULL 							, NULL };
//	Memory Submenu		
const Callback cb_cfg_prefetch		= { modify1BIT					, EVENT_KEYDOWN_B	, &VAR_CFG.MEMORY.PREF	 		, NULL };
const Callback cb_cfg_backup		= { Config::backup				, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_revert		= { Config::revert				, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_memorytest	= { Debug::memoryTest			, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_format		= { Config::format 				, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_reset			= { Config::reset				, EVENT_KEYUP_B		, NULL 							, NULL };
/*---------------------------------------------------------------------------------------------------------------------*/

void Config::load(Control *c, bool bigstep, bool add, u32 *pointer){
	int i, di;
	u16 w;
	u8 h;

	static const BitField tracker_settings[8] = {
		{ &VAR_LIVE.PERFORM.RETRIG			, 6 , 	1 },
		{ &VAR_CFG.MEMORY.PREF				, 5 , 	1 },
		{ &VAR_CFG.TRACKER.HEADERTYPE		, 4 , 	1 },
		{ &VAR_CFG.TRACKER.INPUTMODE		, 3 , 	1 },
		{ &VAR_CFG.TRACKER.PRELISTEN		, 2 , 	1 },
		{ &VAR_CFG.BEHAVIOR.AUTOSAVE		, 0 , 	1 },
		{ NULL								, 0 ,   0 }, 
		{ NULL								, 0 ,   0 }, 
	};
		
	static const BitField look_n_feel[8] = {
		{ &VAR_CFG.BEHAVIOR.AUTOLOAD		, 7 , 	1 },
		{ &VAR_CFG.LOOKNFEEL.STARTUPSFX	 	, 6 , 	1 },
		{ &VAR_CFG.LOOKNFEEL.SHOWLOGO   	, 5 , 	1 },
		{ &VAR_CFG.LOOKNFEEL.INTERFACE  	, 4 , 	1 },
		{ &VAR_CFG.LOOKNFEEL.FONT			, 2 , 	2 },
		{ &VAR_CFG.LOOKNFEEL.BORDER 		, 0 , 	2 },
		{ NULL								, 0 ,  0 }, 
		{ NULL								, 0 ,  0 }, 
	};

	EXPECT(10, SAVING, SETTINGS);
	
	// Check Signature and version
	SRAM.seek(0);
	if(SRAM.read32	() != M4GEEK_SIGNATURE	) return Config::defaults(c, 0, 0, pointer);
	if(SRAM.read	() != M4G_VERSION		) return Config::defaults(c, 0, 0, pointer);
	
	// Palette colors
	for(i=0;i<8; i++){
		di = i<<1;
		readNibbles( VAR_CFG.PAL[ di ].R, VAR_CFG.PAL[ di ].G);
		readNibbles( VAR_CFG.PAL[ di ].B, VAR_CFG.PAL[di+1].R);
		readNibbles( VAR_CFG.PAL[di+1].G, VAR_CFG.PAL[di+1].B);
	}	
		
	/*! Look And Feel																*/
	readFields	( look_n_feel 														);
	/*! Link mode																	*/
	readByte	( VAR_CFG.LINKMODE.LINKMODE 										);
	readByte	( VAR_CFG.LINKMODE.MASTERCLOCK										);
	readByte	( VAR_CFG.LINKMODE.SYNCRATE 										);
	readByte	( VAR_CFG.LINKMODE.SYNCTICKS										);	
	/*! Behavior, live and tracker													*/
	readByte	( VAR_CFG.BEHAVIOR.KEYRATE 											);
	/*! Tracker settings 															*/
	readFields	( tracker_settings 													);	
	readByte	( VAR_CFG.TRACKER.TRANSPOSE 										);
	readByte	( VAR_CFG.TRACKER.SOUNDBIAS 										);
	readNibbles	( VAR_LIVE.PERFORM.SPEED		, VAR_CFG.TRACKER.FINETUNE	, 0xF	);
	readNibbles	( VAR_LIVE.PERFORM.QUANTIZE		, VAR_LIVE.PIANO.QUANTIZE	, 0x3	);
	readNibbles	( VAR_LIVE.PIANO.OCTAVE			, VAR_LIVE.PIANO.MODE 		, 0xF	);
	readByte	( VAR_LIVE.PIANO.TRANSPOSE);
	
	for(i=0; i<2; i++){
		readByte( VAR_LIVE.PIANO.CHANNEL	[i] );
		readByte( VAR_LIVE.PIANO.MIDICHAN	[i] );
	}
	
	//46dec
	
	// Live Settings and tables
	SRAM.seek(47);
	for( i=0 ; i < 8 ; i++ ){
		w = SRAM.read16();
		VAR_LIVE.PERFORM.LEFT.KEY	[i] = EXTRACT( w, 9, 0x7F);
		VAR_LIVE.PERFORM.LEFT.INS	[i] = EXTRACT( w, 5, 0x3F);
		VAR_LIVE.PERFORM.LEFT.CMD	[i] = w & 0x1F;

		w = SRAM.read16();
		VAR_LIVE.PERFORM.RIGHT.KEY	[i] = EXTRACT( w, 9, 0x7F);
		VAR_LIVE.PERFORM.RIGHT.INS	[i] = EXTRACT( w, 5, 0x3F);
		VAR_LIVE.PERFORM.RIGHT.CMD	[i] = w & 0x1F;
		
		h = SRAM.read();
		VAR_LIVE.PERFORM.LEFT.CHAN	[i] = EXTRACT( h, 4, 0xF);
		VAR_LIVE.PERFORM.LEFT.VOL	[i] = h & 0xF;
		
		h = SRAM.read();
		VAR_LIVE.PERFORM.RIGHT.CHAN	[i] = EXTRACT( h, 4, 0xF);
		VAR_LIVE.PERFORM.RIGHT.VOL	[i] = h & 0xF;
		
		VAR_LIVE.PERFORM.LEFT.VAL	[i] = SRAM.read();
		VAR_LIVE.PERFORM.RIGHT.VAL	[i] = SRAM.read();
	}
	
	//0x6F
	//SRAM.drawPosition(27, 1, 7);
	
	// Load default song values (overriden if autoload was true on load)
	SRAM.songDefaults( false );
	
	// Force skins and fonts to be reloaded
	VAR_CFG.RELOADSKIN = true;
	
	LookNFeel::update( &regHnd );
		
	Progress::disable();
	//regHnd.progress.enabled = false;
	//regHnd.redraw = true;
}

void Config::backup	(Control *c, bool bigstep, bool add, u32 *pointer){
	// DEPRECATED
	Debug::panic("This option made no sense, so its now deprecated");
}

void Config::revert (Control *c, bool bigstep, bool add, u32 *pointer){
	Debug::panic("This option made no sense, so its now deprecated");
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
	SRAM.write(VAR_CFG.LINKMODE.SYNCRATE);
	SRAM.write(VAR_CFG.LINKMODE.SYNCTICKS);
	
	// Behavior
	SRAM.write(VAR_CFG.BEHAVIOR.KEYRATE);
	SRAM.write(  (VAR_LIVE.PERFORM.RETRIG<<6) | (VAR_CFG.MEMORY.PREF<<5)  | (VAR_CFG.TRACKER.HEADERTYPE<<4) | (VAR_CFG.TRACKER.INPUTMODE<<3) | (VAR_CFG.TRACKER.PRELISTEN<<2) | VAR_CFG.BEHAVIOR.AUTOSAVE );/* 1 EXTRA BIT LEFT */
	
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

	Progress::disable();
	//regHnd.progress.enabled = false;
	//regHnd.redraw = true;
}

void Config::format (Control *c, bool bigstep, bool add, u32 *pointer){
	if(c){
		ReallyDialog::enable();
		if( !ReallyDialog::result ) return;
	}
	SRAM.erase();
	Config::defaults(c, 0,0,pointer);
	Config::save(c, 0,0,pointer);	
}

void Config::defaults (Control *c, bool bigstep, bool add, u32 *pointer){
	int i;
		
	#define SETTING(a, v)		{VAR_CFG.a = v; VAR_CFG.loadCount++;regHnd.update(1);}
	VAR_CFG.loadCount = 0;

	// regHnd.progress.set(0, 233, STATUS_DEFAULTS, STATUS_SETTINGS, &VAR_CFG.loadCount);
	Progress::enable(0, 233, STATUS_DEFAULTS, STATUS_SETTINGS, &VAR_CFG.loadCount);

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
	SETTING(LINKMODE.LINKMODE	, 0x00);
	SETTING(LINKMODE.MASTERCLOCK, 0x01);
	SETTING(LINKMODE.SYNCRATE	, 0x00); //0-16
	SETTING(LINKMODE.SYNCTICKS	, 0x00); //0-255
	//176
	SETTING(BEHAVIOR.AUTOLOAD, 0x01);
	SETTING(BEHAVIOR.KEYRATE, 0x08);
	SETTING(BEHAVIOR.AUTOSAVE, 0x00);
	SETTING(BEHAVIOR.VISTYPE, 0); //0 vis - 1 table
	//180
	SETTING(TRACKER.FINETUNE, 0x0);
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

	Progress::disable();
	regHnd.redraw = true;
}

void Config::reset (Control *c, bool bigstep, bool add, u32 *pointer){
	ReallyDialog::enable();
	if( ReallyDialog::result ) 
		Sys::reset();
	regHnd.redraw = true;
	regHnd.update(0);
}

void Config::reinit (Control *c, bool bigstep, bool add, u32 *pointer){
	Config::format(c, 0,0,pointer);
	SRAM.sharedDataSave( true );
}

void Config::memMap( RegionHandler* rh ){

}

void Config::update( RegionHandler* rh ){
	if( regHnd.redraw ){
		Gpu::ascii( 34, 1, "Build date", COLOR_DARK_BLUE);
		Gpu::ascii( 34, 2, TIMESTAMP, COLOR_DARK_CYAN 	);
		regHnd.redraw = false;
	}
}
