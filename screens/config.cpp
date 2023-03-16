#include "config.hpp"

#include "../macros.h"
#include "../timestamp.h"
#include "../modules/sys/sys.hpp"
#include "../modules/net/net.hpp"
#include "../modules/sram/sram.hpp"
#include "../modules/spu/mixer.hpp"
#include "../modules/spu/synth.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"
#include "../debug.hpp"
#include "../data/data.hpp"
#include "../screens/live.hpp"

// Callback used @ Slot change
const Callback 	Config::menuindex = { modify5VAL , EVENT_MODIFY_B , &VAR_CFG.MENUSLOT , NULL };


void Config::load(Control *c, bool bigstep, bool add, u32 *pointer){
	int i, di;
	u16 w;
	u8 h;

	static const BitField tracker_settings[8] = {
		{ &Live::PERFORM.RETRIG				, 6 , 	1 },
		{ &VAR_CFG.MEMORY.PREF				, 5 , 	1 },
		{ &VAR_CFG.TRACKER.HEADERTYPE		, 4 , 	1 },
		{ &VAR_CFG.TRACKER.INPUTMODE		, 3 , 	1 },
		{ &VAR_CFG.TRACKER.PRELISTEN		, 2 , 	1 },
		{ &VAR_CFG.BEHAVIOR.POWERSAVING		, 1 , 	1 },
		{ &VAR_CFG.BEHAVIOR.AUTOSAVE		, 0 , 	1 },
		{ NULL								, 0 ,   0 }, 
	};
		
	static const BitField look_n_feel[8] = {
		{ &VAR_CFG.BEHAVIOR.AUTOLOAD		, 7 , 	1 },
		{ &VAR_CFG.LOOKNFEEL.STARTUPSFX	 	, 6 , 	1 },
		{ &VAR_CFG.LOOKNFEEL.SHOWLOGO   	, 5 , 	1 },
		{ &VAR_CFG.LOOKNFEEL.INTERFACE  	, 4 , 	1 },
		{ &VAR_CFG.LOOKNFEEL.FONT			, 2 , 	2 },
		{ &VAR_CFG.LOOKNFEEL.BORDER 		, 0 , 	2 },
		{ NULL								, 0 ,   0 }, 
		{ NULL								, 0 ,   0 }, 
	};

	
	EXPECT(10, SAVING, SETTINGS);
	
	
	// Check Signature and version
	Sram::seek(0);
	if(! Sram::checkSignature() ) return Config::format( c, 0, 0, pointer );
	Sram::seek(5);
	
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
	readNibbles	( Live::PERFORM.SPEED		, VAR_CFG.TRACKER.FINETUNE	, 0xF	);
	readNibbles	( Live::PERFORM.QUANTIZE		, Live::PIANO.QUANTIZE	, 0x3	);
	readNibbles	( Live::PIANO.OCTAVE			, Live::PIANO.MODE 		, 0xF	);
	readByte	( Live::PIANO.TRANSPOSE);
	
	for(i=0; i<2; i++){
		readByte( Live::PIANO.CHANNEL	[i] );
		readByte( Live::PIANO.MIDICHAN	[i] );
	}
	
	//46dec
	
	// Live Settings and tables
	Sram::seek(47);
	for( i=0 ; i < 8 ; i++ ){
		w = Sram::read16();
		Live::PERFORM.LEFT.KEY	[i] = EXTRACT( w, 9, 0x7F);
		Live::PERFORM.LEFT.INS	[i] = EXTRACT( w, 5, 0x3F);
		Live::PERFORM.LEFT.CMD	[i] = w & 0x1F;

		w = Sram::read16();
		Live::PERFORM.RIGHT.KEY	[i] = EXTRACT( w, 9, 0x7F);
		Live::PERFORM.RIGHT.INS	[i] = EXTRACT( w, 5, 0x3F);
		Live::PERFORM.RIGHT.CMD	[i] = w & 0x1F;
		
		h = Sram::read();
		Live::PERFORM.LEFT.CHAN	[i] = EXTRACT( h, 4, 0xF);
		Live::PERFORM.LEFT.VOL	[i] = h & 0xF;
		
		h = Sram::read();
		Live::PERFORM.RIGHT.CHAN	[i] = EXTRACT( h, 4, 0xF);
		Live::PERFORM.RIGHT.VOL	[i] = h & 0xF;
		
		Live::PERFORM.LEFT.VAL	[i] = Sram::read();
		Live::PERFORM.RIGHT.VAL	[i] = Sram::read();
	}
	
	//0x6F
	//Sram::drawPosition(27, 1, 7);
	
	// Load default song values (overriden if autoload was true on load)
	Sram::songDefaults( false );
	
	// Force skins and fonts to be reloaded
	VAR_CFG.RELOADSKIN = true;
	
	LookNFeel::update( );
		
	Progress::disable();
	//RegionHandler::progress.enabled = false;
	//RegionHandler::redraw = true;
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
	Sram::seek(0);
	EXPECT(10, SAVING, SETTINGS);
	
	// Signature and version
	Sram::write32(M4GEEK_SIGNATURE); //signature (cafe aces)
	Sram::write(M4G_VERSION);		//version
	
	// Palette colors
	for(i=0;i<8; i++){
		di = i<<1;
		Sram::write(VAR_CFG.PAL[ di ].R << 4 | VAR_CFG.PAL[ di ].G);  
		Sram::write(VAR_CFG.PAL[ di ].B << 4 | VAR_CFG.PAL[di+1].R);			
		Sram::write(VAR_CFG.PAL[di+1].G << 4 | VAR_CFG.PAL[di+1].B);
	}
		
		
	// Look And Feel
	Sram::write( (VAR_CFG.BEHAVIOR.AUTOLOAD<<7) | (VAR_CFG.LOOKNFEEL.STARTUPSFX<<6) | (VAR_CFG.LOOKNFEEL.SHOWLOGO << 5) | (VAR_CFG.LOOKNFEEL.INTERFACE<<0x4) | (VAR_CFG.LOOKNFEEL.FONT << 2) | VAR_CFG.LOOKNFEEL.BORDER);
	
	// Link mode
	Sram::write(VAR_CFG.LINKMODE.LINKMODE);
	Sram::write(VAR_CFG.LINKMODE.MASTERCLOCK);
	Sram::write(VAR_CFG.LINKMODE.SYNCRATE);
	Sram::write(VAR_CFG.LINKMODE.SYNCTICKS);
	
	// Behavior
	Sram::write(VAR_CFG.BEHAVIOR.KEYRATE);
	Sram::write(  (Live::PERFORM.RETRIG<<6) | (VAR_CFG.MEMORY.PREF<<5)  | (VAR_CFG.TRACKER.HEADERTYPE<<4) | (VAR_CFG.TRACKER.INPUTMODE<<3) | (VAR_CFG.TRACKER.PRELISTEN<<2) | (VAR_CFG.BEHAVIOR.POWERSAVING<<1) |VAR_CFG.BEHAVIOR.AUTOSAVE );/* 1 EXTRA BIT LEFT */
	
	// Tracker
	Sram::write(VAR_CFG.TRACKER.TRANSPOSE);
	Sram::write(VAR_CFG.TRACKER.SOUNDBIAS);
	Sram::write( (Live::PERFORM.SPEED<<4) |(VAR_CFG.TRACKER.FINETUNE & 0x0F));
	Sram::write((Live::PERFORM.QUANTIZE<<4) | Live::PIANO.QUANTIZE);
	//40 bytes
	
	Sram::write((Live::PIANO.OCTAVE<<4) | Live::PIANO.MODE);
	Sram::write(Live::PIANO.TRANSPOSE);
	for(i=0; i<2; i++){
		Sram::write(Live::PIANO.CHANNEL[i]);
		Sram::write(Live::PIANO.MIDICHAN[i]);
	}

	//46dec
	
	// Dump Live Settings and tables
	Sram::seek(47);
	for(i=0;i<8;i++){		
		Sram::write16(((Live::PERFORM.LEFT.KEY[i]&0x7f)<<9) | ((Live::PERFORM.LEFT.INS[i]&0x3f)<<5) | (Live::PERFORM.LEFT.CMD[i]&0x1f));
		Sram::write16(((Live::PERFORM.RIGHT.KEY[i]&0x7f)<<9) | ((Live::PERFORM.RIGHT.INS[i]&0x3f)<<5) | (Live::PERFORM.RIGHT.CMD[i]&0x1f));
		Sram::write(((Live::PERFORM.LEFT.CHAN[i]&0x0f)<<4) | (Live::PERFORM.LEFT.VOL[i]&0x0f));
		Sram::write(((Live::PERFORM.RIGHT.CHAN[i]&0x0f)<<4) | (Live::PERFORM.RIGHT.VOL[i]&0x0f));
		Sram::write(Live::PERFORM.LEFT.VAL[i]);
		Sram::write(Live::PERFORM.RIGHT.VAL[i]);
	}
	
	//Sram::drawPosition(27, 1, 7);
	//0x6F

	Progress::disable();
	//RegionHandler::progress.enabled = false;
	//RegionHandler::redraw = true;
}

void Config::format (Control *c, bool bigstep, bool add, u32 *pointer){
	if(c){
		ReallyDialog::enable();
		if( !ReallyDialog::result ) return;
	}
	Sram::erase();
	Config::defaults(c, 0,0,pointer);
	Config::save(c, 0,0,pointer);	
}

void Config::defaults (Control *c, bool bigstep, bool add, u32 *pointer){
	int i;
		
	#define SETTING(a, v)		{VAR_CFG.a = v; VAR_CFG.loadCount++;/*RegionHandler::update(1);*/}
	VAR_CFG.loadCount = 0;

	// RegionHandler::progress.set(0, 233, STATUS_DEFAULTS, STATUS_SETTINGS, &VAR_CFG.loadCount);
	Progress::enable(0, 233, STATUS_DEFAULTS, STATUS_SETTINGS, &VAR_CFG.loadCount);
	
	for(i=0; i<16; i++) {
		SETTING(PAL[i].R, 0x00);
		SETTING(PAL[i].G, 0x00);
		SETTING(PAL[i].B, 0x00);
	}
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
	SETTING(BEHAVIOR.AUTOLOAD	, 0x01);
	SETTING(BEHAVIOR.KEYRATE	, 0x08);
	SETTING(BEHAVIOR.POWERSAVING, 0x00);
	SETTING(BEHAVIOR.AUTOSAVE	, 0x00);
	SETTING(BEHAVIOR.VISTYPE	, 0); //0 vis - 1 table
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
	
	Sram::songDefaults( true );

	Progress::disable();
	RegionHandler::redraw = true;
}

void Config::reset (Control *c, bool bigstep, bool add, u32 *pointer){
	ReallyDialog::enable();
	if( ReallyDialog::result ) 
		Sys::reset();
	RegionHandler::redraw = true;
	RegionHandler::update(0);
}

void Config::reinit (Control *c, bool bigstep, bool add, u32 *pointer){
	Config::format(c, 0,0,pointer);
	Sram::sharedDataSave( true );
}

void Config::memMap(  ){

}

void Config::update(  ){
	if( RegionHandler::redraw ){
		Gpu::ascii( 34, 1, "Build date", COLOR_DARK_BLUE);
		Gpu::ascii( 34, 2, TIMESTAMP, COLOR_DARK_CYAN 	);
		RegionHandler::redraw = false;
	}
}
