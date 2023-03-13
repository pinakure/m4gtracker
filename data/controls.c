#include "controls.hpp"
#include "data.hpp"
#include "../modules/regionhandler/regionhandler.hpp"
#include "../modules/spu/sequencer.hpp"
#include "../callbacks/trk.hpp"
#include "../modules/spu/mixer.hpp"
#include "../modules/key/key.hpp"
#include "../macros.h"

u8 sTransient::bit1;
u8 sTransient::bit2;
u8 sTransient::bit3;
u8 sTransient::bit4;
u8 sTransient::bit5;
u8 sTransient::bit6;
u8 sTransient::bit7;
u8 sTransient::bit8;
u8 sTransient::note;
u8 sTransient::value;
u8 sTransient::volume;
u8 sTransient::instrument;
u8 sTransient::command;
u8 sTransient::changed;

#define VARIABLE			(*(u8*) c->var)

void modify1BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE^=0x1; Transient::bit1 = VARIABLE; Transient::changed = true; }
void modify2BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = ( bigstep ? ( add	?0x03:0x0) : (VARIABLE + (add?1:-1)) ) & 0x03; Transient::bit2 = VARIABLE; Transient::changed = true; }
void modify3BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = ( bigstep ? ( add	?0x07:0x0) : (VARIABLE + (add?1:-1)) ) & 0x07; Transient::bit3 = VARIABLE; Transient::changed = true; }
void modify4BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = ( bigstep ? ( add	?0x0F:0x0) : (VARIABLE + (add?1:-1)) ) & 0x0F; Transient::bit4 = VARIABLE; Transient::changed = true; }	
void modify5BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x04:0x1) * ( add ? 1 : -1 ) )      ) & 0x1F; Transient::bit5 = VARIABLE; Transient::changed = true; }
void modify6BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x04:0x1) * ( add ? 1 : -1 ) )      ) & 0x3F; Transient::bit6 = VARIABLE; Transient::changed = true; }
void modify7BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x04:0x1) * ( add ? 1 : -1 ) )      ) & 0x7F; Transient::bit7 = VARIABLE; Transient::changed = true; }
void modify8BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x10:0x1) * ( add ? 1 : -1 ) )      ) & 0xFF; Transient::bit8 = VARIABLE; Transient::changed = true; }
void modifyCHAR	(Control *c, bool bigstep, bool add, u32 *pointer){ }
void modifyTempo(Control *c, bool bigstep, bool add, u32 *pointer){	modify8BIT(c, bigstep, add, pointer); Sequencer::setTempo( VAR_SONG.BPM ); }

void modifyValue(Control *c, bool bigstep, bool add, u32 *pointer, u8 quant){	
	VARIABLE = ( bigstep ? (add ? quant - 1 : 0) : (VARIABLE + (add ? 1 : -1 )) ) % quant; 
	if( VARIABLE > quant - 1 ) VARIABLE = quant - 1; 	
}	
void modify3VAL (Control *c, bool bigstep, bool add, u32 *pointer){ return modifyValue(c, bigstep, add, pointer,  3); }
void modify5VAL (Control *c, bool bigstep, bool add, u32 *pointer){ return modifyValue(c, bigstep, add, pointer,  5); }
void modify6VAL (Control *c, bool bigstep, bool add, u32 *pointer){ return modifyValue(c, bigstep, add, pointer,  6); }
void modify27VAL(Control *c, bool bigstep, bool add, u32 *pointer){ return modifyValue(c, bigstep, add, pointer, 27); }

void modifyCommand (Control *c, bool bigstep, bool add, u32 *pointer){
	if(CURRENT_PATTERN == 0x00)return;
	
	if( bigstep &! add) VARIABLE = 0; else {
		VARIABLE += (add?1:-1);
		if((VARIABLE > 26) && (VARIABLE < 0x80)) VARIABLE = 26;
		else if(VARIABLE > 0x26) VARIABLE = 0;
	}
	Transient::command = VARIABLE;
	Transient::changed = true;
	
	Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL );
}

void modifyInst ( Control *c, bool bigstep, bool add, u32 *pointer ){
	if( CURRENT_PATTERN == 0x00 ) return;
	VARIABLE 			= (VARIABLE + ((bigstep?0x4:0x1) * (add?1:-1)) ) & 0x3F;	
	Transient::instrument= VARIABLE; 
	Transient::changed	= true; 
	if( VARIABLE == 0 ) VARIABLE = 1;
	Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
}

void modifyVolume (Control *c, bool bigstep, bool add, u32 *pointer){
	if(CURRENT_PATTERN == 0x00) return;
	VARIABLE 			= ( bigstep ? (add?0xF:0) : (VARIABLE + (add?1:-1)) ) & 0xF; 	
	Transient::volume 	= VARIABLE; 
	Transient::changed 	= true; 
	Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
}	

void modifyValue (Control *c, bool bigstep, bool add, u32 *pointer){
	if(CURRENT_PATTERN == 0x00) return;
	VARIABLE 			= (VARIABLE + ((bigstep?0x10:0x1)* (add?1:-1)) ) & 0xFF;
	Transient::value		= VARIABLE; 
	Transient::changed 	= true; 
	Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
}

void paste2BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit2; return; } Transient::bit2 = VARIABLE; Transient::changed = true; }
void paste3BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit3; return; } Transient::bit3 = VARIABLE; Transient::changed = true; }
void paste4BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit4; return; } Transient::bit4 = VARIABLE; Transient::changed = true; }
void paste5BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit5; return; } Transient::bit5 = VARIABLE; Transient::changed = true; }
void paste6BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit6; return; } Transient::bit6 = VARIABLE; Transient::changed = true; }
void paste7BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit7; return; } Transient::bit7 = VARIABLE; Transient::changed = true; }
void paste8BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit8; return; } Transient::bit7 = VARIABLE; Transient::changed = true; }

void pasteCommand (Control *c, bool bigstep, bool add, u32 *pointer){ 
	if(VARIABLE == 0x00){
		if(CURRENT_PATTERN == 0x00) return; 
		VARIABLE = Transient::command; 		
		Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
		return; 
	} 
	Transient::command = VARIABLE; 
	Transient::changed = true; 
}

void pasteInst (Control *c, bool bigstep, bool add, u32 *pointer){ 
	if(VARIABLE == 0x00){
		if(CURRENT_PATTERN == 0x00)return; 
		VARIABLE = Transient::instrument; 	
		Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
		return; 
	} 
	Transient::instrument = VARIABLE; 
	Transient::changed 	 = true; 
}

void pasteNote (Control *c, bool bigstep, bool add, u32 *pointer){ 
	if(VARIABLE == 0x00){
		if(CURRENT_PATTERN == 0x00) return; 
		VARIABLE = Transient::note; 			
		Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
		return; 
	} 
	Transient::note 		= VARIABLE; 
	Transient::changed 	= true; 
}

void pasteVolume (Control *c, bool bigstep, bool add, u32 *pointer){ 
	if(VARIABLE == 0x00){
		if(CURRENT_PATTERN == 0x00) return; 
		VARIABLE = Transient::volume;
		Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL );
		return; 
	} 
	Transient::volume	= VARIABLE; 
	Transient::changed   = true;
}

void pasteValue	(Control *c, bool bigstep, bool add, u32 *pointer){ 
	if(VARIABLE == 0x00){
		if(CURRENT_PATTERN == 0x00) return; 
		VARIABLE = Transient::value;
		Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
		return; 
	} 
	Transient::value 	= VARIABLE; 
	Transient::changed 	= true; 
}

#undef CURRENT_PATTERN
#undef VARIABLE

/*

DATA IN FILE PAST THIS POINT DEPRECATED:
This controls must reside in each correspondent callback/xxx file
Use as example LinkMode class at Config screen 

*/


//********************************************//
// CONFIG SCREEN::Viewport Regions' Controls  //
//********************************************// 
#define THIS(a) &LOOKNFEEL_CONTROLS[CONTROL_LOOKNFEEL_##a]
#define THISVAR(a) ((u8*)&(VAR_CFG.LOOKNFEEL.a))
const Control LOOKNFEEL_CONTROLS[CONTROL_LOOKNFEEL_MAX] = { 
	//  x    y      up						right					down					left					cache						var						callback	
	{	0x04 , 0x04 , NULL					, THIS(INTERFACE)		, NULL					, NULL					, NULL						, (u8*)&(VAR_CFG.MENUSLOT)	, &cb_cfg_menuindex		},
	{	0x19 , 0x06 , THIS(EDITPAL)			, THIS(INTERFACE)		, THIS(FONT)			, THIS(MENU1)			, &CACHE_INTERFACE			, THISVAR(INTERFACE)		, &cb_cfg_interface		},
	{	0x17 , 0x07 , THIS(INTERFACE)		, THIS(FONT)			, THIS(BORDER)			, THIS(MENU1)			, &CACHE_FONTSTYLE			, THISVAR(FONT)				, &cb_cfg_font			},
	{	0x17 , 0x08 , THIS(FONT)			, THIS(BORDER)			, THIS(SHOWLOGO)		, THIS(MENU1)			, &CACHE_BORDERSTYLE		, THISVAR(BORDER)			, &cb_cfg_border			},
	{	0x1b , 0x09 , THIS(BORDER)			, THIS(SHOWLOGO)		, THIS(STARTSFX)		, THIS(MENU1)			, &CACHE_CHECK				, THISVAR(SHOWLOGO)			, &cb_cfg_showlogo		},
	{	0x1b , 0x0a , THIS(SHOWLOGO)		, THIS(STARTSFX)		, THIS(EDITPAL)			, THIS(MENU1)			, &CACHE_CHECK				, THISVAR(STARTUPSFX)		, &cb_cfg_startupsfx		},
	{	0x1b , 0x0b , THIS(STARTSFX)		, THIS(EDITPAL)			, THIS(INTERFACE)		, THIS(MENU1)			, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_coloreditor	},
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS

#define THIS(a) &BEHAVIOR_CONTROLS[CONTROL_BEHAVIOR_##a]
#define THISVAR(a) ((u8*)&(VAR_CFG.BEHAVIOR.a))
const Control BEHAVIOR_CONTROLS[CONTROL_BEHAVIOR_MAX] = { 
	//  x    y     	up						right					down					left					cache						var							callback
	{	0x04 , 0x04 , NULL					, THIS(AUTOLOAD)		, NULL					, NULL					, NULL						, (u8*)&(VAR_CFG.MENUSLOT)	, &cb_cfg_menuindex		}, /*MENU3*/ 
	{	0x1b , 0x06 , THIS(DEFAULTS)		, THIS(AUTOLOAD)		, THIS(KEYRATE)			, THIS(MENU3)			, &CACHE_CHECK				, THISVAR(AUTOLOAD)			, &cb_cfg_autoload	 	}, /*AUTOLOAD*/
	{	0x1b , 0x07 , THIS(AUTOLOAD)		, THIS(KEYRATE)			, THIS(BUTTONSET)		, THIS(MENU3)			, &CACHE_HEXADECIMAL		, THISVAR(KEYRATE)			, &cb_cfg_keyrate		}, /*keyrate*/
	{	0x19 , 0x08 , THIS(KEYRATE)			, THIS(BUTTONSET)		, THIS(SAVE)			, THIS(MENU3)			, &CACHE_BUTTON_SET			, THISVAR(BUTTONSET)		, &cb_cfg_buttonset		}, /*buttonset*/
	{	0x1b , 0x09 , THIS(BUTTONSET)		, THIS(SAVE)			, THIS(LOAD)			, THIS(MENU3)			, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_saveconfig		}, /*save*/
	{	0x1b , 0x0a , THIS(SAVE)			, THIS(LOAD)			, THIS(DEFAULTS)		, THIS(MENU3)			, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_loadconfig		}, /*load*/
	{	0x1b , 0x0b , THIS(LOAD)			, THIS(DEFAULTS)		, THIS(AUTOLOAD)		, THIS(MENU3)			, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_initconfig		}, /*defaults*/
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS
	
#define THIS(a) &TRACKER_CONTROLS[CONTROL_TRACKER_##a]
#define THISVAR(a) ((u8*)&(VAR_CFG.TRACKER.a))
const Control TRACKER_CONTROLS[CONTROL_TRACKER_MAX] = { 
	//  x	 y     	up						right					down					left					cache						var							callback
	{	0x04 , 0x04 , NULL					, THIS(FINETUNE)		, NULL					, NULL					, NULL						, (u8*)&(VAR_CFG.MENUSLOT)	, &cb_cfg_menuindex		}, /*MENU4*/ 
	{	0x1b , 0x06 , THIS(MIXER)			, THIS(FINETUNE)		, THIS(PRELISTEN)		, THIS(MENU4)			, &CACHE_HEXADECIMAL		, THISVAR(FINETUNE)			, &cb_cfg_finetune		}, /*finetune*/
	{	0x1b , 0x07 , THIS(FINETUNE)		, THIS(PRELISTEN)		, THIS(TRANSPOSE)		, THIS(MENU4)			, &CACHE_CHECK				, THISVAR(PRELISTEN)		, &cb_cfg_prelisten		}, /*prelisten*/
	{	0x1b , 0x08 , THIS(PRELISTEN)		, THIS(TRANSPOSE)		, THIS(INPUTMODE)		, THIS(MENU4)			, &CACHE_HEXADECIMAL_DOUBLE	, THISVAR(TRANSPOSE)		, &cb_cfg_transpose		}, /*transpose*/
	{	0x1a , 0x09 , THIS(TRANSPOSE)		, THIS(INPUTMODE)		, THIS(SOUNDBIAS)		, THIS(MENU4)			, &CACHE_INPUT_TYPE			, THISVAR(INPUTMODE)		, &cb_cfg_inputmode		}, /*inputmode*/
	{	0x1b , 0x0a , THIS(INPUTMODE)		, THIS(SOUNDBIAS)		, THIS(MIXER)			, THIS(MENU4)			, &CACHE_HEXADECIMAL_DOUBLE	, THISVAR(SOUNDBIAS)		, &cb_cfg_soundbias		}, /*soundbias*/
	{	0x1b , 0x0b , THIS(SOUNDBIAS)		, THIS(MIXER)			, THIS(FINETUNE)		, THIS(MENU4)			, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_mixer			}, /*defaults*/
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS

#define THIS(a) &MEMORY_CONTROLS[CONTROL_MEMORY_##a]
#define THISVAR(a) ((u8*)&(VAR_CFG.MEMORY.a))
const Control MEMORY_CONTROLS[CONTROL_MEMORY_MAX] = { 
	//  x    y     	up						right					down					left					cache						var							callback
	{	0x04 , 0x04 , NULL					, THIS(PREFETCH)		, NULL					, NULL					, NULL						, (u8*)&(VAR_CFG.MENUSLOT)	, &cb_cfg_menuindex		}, /*MENU5*/ 
	{	0x1b , 0x06 , THIS(RESET)			, THIS(PREFETCH)		, THIS(SLOTUSAGE)		, THIS(MENU5)			, &CACHE_CHECK				, THISVAR(PREF)				, &cb_cfg_prefetch		}, /*prefetch*/
	{	0x1b , 0x07 , THIS(PREFETCH)		, THIS(SLOTUSAGE)		, THIS(PURGESONGS)		, THIS(MENU5)			, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_backup			}, /*slotusage ->backup*/
	{	0x1b , 0x08 , THIS(SLOTUSAGE)		, THIS(PURGESONGS)		, THIS(MEMORYTEST)		, THIS(MENU5)			, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_revert			}, /*purgesongs -> revert*/
	{	0x1b , 0x09 , THIS(PURGESONGS)		, THIS(MEMORYTEST)		, THIS(REINIT)			, THIS(MENU5)			, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_memorytest		}, /*memorytest*/
	{	0x1b , 0x0a , THIS(MEMORYTEST)		, THIS(REINIT)			, THIS(RESET)			, THIS(MENU5)			, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_format			}, /*reinit*/
	{	0x1b , 0x0b , THIS(REINIT)			, THIS(RESET)			, THIS(PREFETCH)		, THIS(MENU5)			, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_reset			}, /*reset*/
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS

#define INSTRUM(a) ((u8*)&(VAR_INSTRUMENT.a))
#define THIS(a) &INS_PWM_CONTROLS[CONTROL_INS_PWM_##a]
#define THISVAR(a) ((u8*)&(VAR_PWM.a))
const Control INS_PWM_CONTROLS[CONTROL_INS_PWM_MAX] = { 
	//	x	 y		up						right					down					left					cache							var									callback
	{	0x05 , 0x04 , THIS(VOLUMEFADE)		, THIS(TSP_TABLE_04)	, THIS(NAME)			, THIS(TSP_TABLE_07)	, &CACHE_HEXADECIMAL_INSTRUMENT	, (u8*)&(VAR_CFG.CURRENTINSTRUMENT)	, &cb_ins_index			},
	{	0x09 , 0x03 , THIS(TSP_ENVELOPE)	, THIS(TSP_TABLE_01)	, THIS(TSP_TABLE_04)	, THIS(INDEX)			, &CACHE_HEXADECIMAL			, THISVAR(TSP[0])					, &cb_ins_tsp_00			},
	{	0x0a , 0x03 , THIS(TSP_ENVELOPE)	, THIS(TSP_TABLE_02)	, THIS(TSP_TABLE_05)	, THIS(TSP_TABLE_00)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[1])					, &cb_ins_tsp_01			},
	{	0x0b , 0x03 , THIS(TSP_ENVELOPE)	, THIS(TSP_TABLE_03)	, THIS(TSP_TABLE_06)	, THIS(TSP_TABLE_01)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[2])					, &cb_ins_tsp_02			},
	{	0x0c , 0x03 , THIS(TSP_ENVELOPE)	, THIS(INDEX)			, THIS(TSP_TABLE_07)	, THIS(TSP_TABLE_02)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[3])					, &cb_ins_tsp_03			},
	{	0x09 , 0x04 , THIS(TSP_TABLE_00)	, THIS(TSP_TABLE_05)	, THIS(TSP_TABLE_08)	, THIS(INDEX)			, &CACHE_HEXADECIMAL			, THISVAR(TSP[4])					, &cb_ins_tsp_04			},
	{	0x0a , 0x04 , THIS(TSP_TABLE_01)	, THIS(TSP_TABLE_06)	, THIS(TSP_TABLE_09)	, THIS(TSP_TABLE_04)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[5])					, &cb_ins_tsp_05			},
	{	0x0b , 0x04 , THIS(TSP_TABLE_02)	, THIS(TSP_TABLE_07)	, THIS(TSP_TABLE_0A)	, THIS(TSP_TABLE_05)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[6])					, &cb_ins_tsp_06			},
	{	0x0c , 0x04 , THIS(TSP_TABLE_03)	, THIS(INDEX)			, THIS(TSP_TABLE_0B)	, THIS(TSP_TABLE_06)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[7])					, &cb_ins_tsp_07			},
	{	0x09 , 0x05 , THIS(TSP_TABLE_04)	, THIS(TSP_TABLE_09)	, THIS(TSP_TABLE_0C)	, THIS(NAME)			, &CACHE_HEXADECIMAL			, THISVAR(TSP[8])					, &cb_ins_tsp_08			},
	{	0x0a , 0x05 , THIS(TSP_TABLE_05)	, THIS(TSP_TABLE_0A)	, THIS(TSP_TABLE_0D)	, THIS(TSP_TABLE_08)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[9])					, &cb_ins_tsp_09			},
	{	0x0b , 0x05 , THIS(TSP_TABLE_06)	, THIS(TSP_TABLE_0B)	, THIS(TSP_TABLE_0E)	, THIS(TSP_TABLE_09)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[10])					, &cb_ins_tsp_0A			},
	{	0x0c , 0x05 , THIS(TSP_TABLE_07)	, THIS(NAME)			, THIS(TSP_TABLE_0F)	, THIS(TSP_TABLE_0A)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[11])					, &cb_ins_tsp_0B			},
	{	0x09 , 0x06 , THIS(TSP_TABLE_08)	, THIS(TSP_TABLE_0D)	, THIS(VOL_ENABLE)		, THIS(TYPE)			, &CACHE_HEXADECIMAL			, THISVAR(TSP[12])					, &cb_ins_tsp_0C			},
	{	0x0a , 0x06 , THIS(TSP_TABLE_09)	, THIS(TSP_TABLE_0E)	, THIS(VOL_ENABLE)		, THIS(TSP_TABLE_0C)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[13])					, &cb_ins_tsp_0D			},
	{	0x0b , 0x06 , THIS(TSP_TABLE_0A)	, THIS(TSP_TABLE_0F)	, THIS(VOL_ENABLE)		, THIS(TSP_TABLE_0D)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[14])					, &cb_ins_tsp_0E			},
	{	0x0c , 0x06 , THIS(TSP_TABLE_0B)	, THIS(TYPE)			, THIS(VOL_ENABLE)		, THIS(TSP_TABLE_0E)	, &CACHE_HEXADECIMAL			, THISVAR(TSP[15])					, &cb_ins_tsp_0F			},
	{	0x0c , 0x12 , THIS(STARTLEVEL)		, NULL					, THIS(TSP_ENABLE)		, NULL					, &CACHE_HEXADECIMAL			, THISVAR(VOLUMEFADE)				, &cb_ins_volumefade		},
	{	0x0c , 0x02 , THIS(TSP_MODE)		, THIS(INDEX)			, THIS(TSP_TABLE_03)	, THIS(INDEX)			, &CACHE_HEXADECIMAL_DOUBLE		, THISVAR(TSP_ENVELOPE)				, &cb_ins_tspenvelope 	},
	{	0x0c , 0x01 , THIS(TSP_ENABLE)		, THIS(INDEX)			, THIS(TSP_ENVELOPE)	, THIS(INDEX)			, &CACHE_PLAYMODES				, THISVAR(TSP_LOOP)					, &cb_ins_tspmode		},
	{	0x0c , 0x00 , THIS(VOLUMEFADE)		, THIS(INDEX)			, THIS(TSP_MODE)		, THIS(INDEX)			, &CACHE_CHECK					, THISVAR(TSP_ENABLE)				, &cb_ins_tspenable		},
	{	0x0c , 0x08 , THIS(TSP_TABLE_0F)	, THIS(DUTYCYCLE)		, THIS(VOL_MODE)		, THIS(DUTYCYCLE)		, &CACHE_CHECK					, THISVAR(VOL_ENABLE)				, &cb_ins_volenable		},
	{	0x0c , 0x09 , THIS(VOL_ENABLE)		, THIS(DUTYLEN)			, THIS(VOL_ENVELOPE)	, THIS(DUTYLEN)			, &CACHE_PLAYMODES				, THISVAR(VOL_LOOP)					, &cb_ins_volmode		},
	{	0x0c , 0x0a , THIS(VOL_MODE)		, THIS(DUTYLEN)			, THIS(VOL_TABLE_03)	, THIS(DUTYLEN)			, &CACHE_HEXADECIMAL_DOUBLE		, THISVAR(VOL_ENVELOPE)				, &cb_ins_volenvelope	},
	{	0x01 , 0x05 , THIS(INDEX)			, THIS(TSP_TABLE_08)	, THIS(TYPE)			, THIS(TSP_TABLE_0B)	, &CACHE_TEXT					, INSTRUM(NAME)						, &cb_ins_name			},
	{	0x06 , 0x07 , THIS(NAME)			, THIS(TSP_TABLE_0C)	, THIS(DUTYCYCLE)		, THIS(TSP_TABLE_0F)	, &CACHE_INSTYPES				, INSTRUM(TYPE)						, &cb_ins_type			},
	{	0x06 , 0x08 , THIS(TYPE)			, THIS(VOL_ENABLE)		, THIS(DUTYLEN)			, THIS(VOL_ENABLE)		, &CACHE_DUTYCYCLES				, THISVAR(DUTYCYCLE)				, &cb_ins_dutycycle		},
	{	0x06 , 0x09 , THIS(DUTYCYCLE)		, THIS(VOL_MODE)		, THIS(SWEEPDIR)		, THIS(VOL_MODE)		, &CACHE_HEXADECIMAL			, THISVAR(LENGTH)					, &cb_ins_length			},
	{	0x06 , 0x0c , THIS(DUTYLEN)			, THIS(VOL_TABLE_04)	, THIS(SWEEPSPEED)		, THIS(VOL_TABLE_07)	, &CACHE_FADES					, THISVAR(SWEEPDIR)					, &cb_ins_sweepdir		},
	{	0x06 , 0x0d , THIS(SWEEPDIR)		, THIS(VOL_TABLE_08)	, THIS(SWEEPSTEPS)		, THIS(VOL_TABLE_0B)	, &CACHE_HEXADECIMAL			, THISVAR(SWEEPSPEED)				, &cb_ins_sweepspeed		},
	{	0x06 , 0x0e , THIS(SWEEPSPEED)		, THIS(VOL_TABLE_0C)	, THIS(ENVELOPEDIR)		, THIS(VOL_TABLE_0F)	, &CACHE_HEXADECIMAL			, THISVAR(SWEEPSTEPS)				, &cb_ins_sweepsteps		},
	{	0x0c , 0x11 , THIS(ENVELOPEDIR)		, NULL					, THIS(VOLUMEFADE)		, NULL					, &CACHE_HEXADECIMAL			, THISVAR(LEVEL)					, &cb_ins_pwm_level		},
	{	0x0c , 0x10 , THIS(VOL_TABLE_0F)	, NULL					, THIS(STARTLEVEL)		, NULL					, &CACHE_FADES					, THISVAR(ENVELOPEDIR)				, &cb_ins_envelopedir	},
	{	0x09 , 0x0b , THIS(VOL_ENVELOPE)	, THIS(VOL_TABLE_01)	, THIS(VOL_TABLE_04)	, THIS(SWEEPDIR)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[0])					, &cb_ins_vol_00			},
	{	0x0a , 0x0b , THIS(VOL_ENVELOPE)	, THIS(VOL_TABLE_02)	, THIS(VOL_TABLE_05)	, THIS(VOL_TABLE_00)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[1])					, &cb_ins_vol_01			},
	{	0x0b , 0x0b , THIS(VOL_ENVELOPE)	, THIS(VOL_TABLE_03)	, THIS(VOL_TABLE_06)	, THIS(VOL_TABLE_01)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[2])					, &cb_ins_vol_02			},
	{	0x0c , 0x0b , THIS(VOL_ENVELOPE)	, THIS(SWEEPDIR)		, THIS(VOL_TABLE_07)	, THIS(VOL_TABLE_02)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[3])					, &cb_ins_vol_03			},
	{	0x09 , 0x0c , THIS(VOL_TABLE_00)	, THIS(VOL_TABLE_05)	, THIS(VOL_TABLE_08)	, THIS(SWEEPDIR)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[4])					, &cb_ins_vol_04			},
	{	0x0a , 0x0c , THIS(VOL_TABLE_01)	, THIS(VOL_TABLE_06)	, THIS(VOL_TABLE_09)	, THIS(VOL_TABLE_04)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[5])					, &cb_ins_vol_05			},
	{	0x0b , 0x0c , THIS(VOL_TABLE_02)	, THIS(VOL_TABLE_07)	, THIS(VOL_TABLE_0A)	, THIS(VOL_TABLE_05)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[6])					, &cb_ins_vol_06			},
	{	0x0c , 0x0c , THIS(VOL_TABLE_03)	, THIS(SWEEPDIR)		, THIS(VOL_TABLE_0B)	, THIS(VOL_TABLE_06)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[7])					, &cb_ins_vol_07			},
	{	0x09 , 0x0d , THIS(VOL_TABLE_04)	, THIS(VOL_TABLE_09)	, THIS(VOL_TABLE_0C)	, THIS(SWEEPSPEED)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[8])					, &cb_ins_vol_08			},
	{	0x0a , 0x0d , THIS(VOL_TABLE_05)	, THIS(VOL_TABLE_0A)	, THIS(VOL_TABLE_0D)	, THIS(VOL_TABLE_08)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[9])					, &cb_ins_vol_09			},
	{	0x0b , 0x0d , THIS(VOL_TABLE_06)	, THIS(VOL_TABLE_0B)	, THIS(VOL_TABLE_0E)	, THIS(VOL_TABLE_09)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[10])					, &cb_ins_vol_0A			},
	{	0x0c , 0x0d , THIS(VOL_TABLE_07)	, THIS(SWEEPSPEED)		, THIS(VOL_TABLE_0F)	, THIS(VOL_TABLE_0A)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[11])					, &cb_ins_vol_0B			},
	{	0x09 , 0x0e , THIS(VOL_TABLE_08)	, THIS(VOL_TABLE_0D)	, THIS(ENVELOPEDIR)		, THIS(SWEEPSTEPS)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[12])					, &cb_ins_vol_0C			},
	{	0x0a , 0x0e , THIS(VOL_TABLE_09)	, THIS(VOL_TABLE_0E)	, THIS(ENVELOPEDIR)		, THIS(VOL_TABLE_0C)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[13])					, &cb_ins_vol_0D			},
	{	0x0b , 0x0e , THIS(VOL_TABLE_0A)	, THIS(VOL_TABLE_0F)	, THIS(ENVELOPEDIR)		, THIS(VOL_TABLE_0D)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[14])					, &cb_ins_vol_0E			},
	{	0x0c , 0x0e , THIS(VOL_TABLE_0B)	, THIS(SWEEPSTEPS)		, THIS(ENVELOPEDIR)		, THIS(VOL_TABLE_0E)	, &CACHE_HEXADECIMAL			, THISVAR(VOL[15])					, &cb_ins_vol_0F			},
	CONTROL_TERMINATOR
};
#undef THIS
#undef THISVAR

#define THIS(a) &INS_WAV_CONTROLS[CONTROL_INS_WAV_##a]
#define THISVAR(a) ((u8*)&(VAR_WAV.a))
const Control INS_WAV_CONTROLS[CONTROL_INS_WAV_MAX] = { 
	//  x	 y		up						right					down					left					cache							var									callback		
	{	0x05 , 0x04 , THIS(WAVPRESET_05)	, THIS(OP1_ADSR_02)		, THIS(NAME)			, THIS(OP1_TYPE)		, &CACHE_HEXADECIMAL_INSTRUMENT	, (u8*)&(VAR_CFG.CURRENTINSTRUMENT)	, &cb_ins_index			},
	{	0x01 , 0x05 , THIS(INDEX)			, THIS(OP2_ADSR_00)		, THIS(TYPE)			, THIS(OP2_TYPE)		, &CACHE_TEXT					, INSTRUM(NAME)						, &cb_ins_name			},
	{	0x06 , 0x07 , THIS(NAME)			, THIS(OP2_ADSR_00)		, THIS(PHASE)			, THIS(OP2_TYPE)		, &CACHE_INSTYPES				, INSTRUM(TYPE)						, &cb_ins_type			},
	{	0x06 , 0x08 , THIS(TYPE)			, THIS(OP2_ADSR_02)		, THIS(AM)				, THIS(OP2_TYPE)		, &CACHE_CHECK					, THISVAR(PHASE)					, &cb_ins_phase			},
	{	0x06 , 0x09 , THIS(PHASE)			, THIS(OP2_ADSR_02)		, THIS(MIXPERCENT)		, THIS(OP3_TYPE)		, &CACHE_CHECK					, THISVAR(AM)						, &cb_ins_am				},
	{	0x04 , 0x0a , THIS(AM)				, THIS(OP3_ADSR_00)		, THIS(WAVPRESET_00)	, THIS(OP3_TYPE)		, &CACHE_PERCENTS				, THISVAR(MIXPERCENT)				, &cb_ins_mixpercent		},
	{	0x0c , 0x01 , THIS(OP4_TYPE)		, THIS(INDEX)			, THIS(OP2_TYPE)		, THIS(OP1_ADSR_01)		, &CACHE_OPERATORS				, THISVAR(OP1_TYPE)					, &cb_ins_wav_op1type	},
	{	0x09 , 0x02 , THIS(OP4_ADSR_02)		, THIS(OP1_ADSR_01)		, THIS(OP1_ADSR_02)		, THIS(INDEX)			, &CACHE_HEXADECIMAL			, THISVAR(OP1_ADSR[0])				, &cb_ins_wav_op1adsr_0	},
	{	0x0b , 0x02 , THIS(OP4_ADSR_03)		, THIS(OP1_TYPE)		, THIS(OP1_ADSR_03)		, THIS(OP1_ADSR_00)		, &CACHE_HEXADECIMAL			, THISVAR(OP1_ADSR[1])				, &cb_ins_wav_op1adsr_1	},
	{	0x09 , 0x03 , THIS(OP1_ADSR_00)		, THIS(OP1_ADSR_03)		, THIS(OP2_ADSR_00)		, THIS(INDEX)			, &CACHE_HEXADECIMAL			, THISVAR(OP1_ADSR[2])				, &cb_ins_wav_op1adsr_2	},
	{	0x0b , 0x03 , THIS(OP1_ADSR_01)		, THIS(OP1_TYPE)		, THIS(OP2_ADSR_01)		, THIS(OP1_ADSR_02)		, &CACHE_HEXADECIMAL			, THISVAR(OP1_ADSR[3])				, &cb_ins_wav_op1adsr_3	},
	{	0x0c , 0x06 , THIS(OP1_TYPE)		, THIS(NAME)			, THIS(OP3_TYPE)		, THIS(OP2_ADSR_01)		, &CACHE_OPERATORS				, THISVAR(OP2_TYPE)					, &cb_ins_wav_op2type	},
	{	0x0c , 0x0b , THIS(OP2_TYPE)		, THIS(MIXPERCENT)		, THIS(OP4_TYPE)		, THIS(OP3_ADSR_01)		, &CACHE_OPERATORS				, THISVAR(OP3_TYPE)					, &cb_ins_wav_op3type	},
	{	0x0c , 0x10 , THIS(OP3_TYPE)		, THIS(WAVPRESET_03)	, THIS(OP1_TYPE)		, THIS(OP4_ADSR_01)		, &CACHE_OPERATORS				, THISVAR(OP4_TYPE)					, &cb_ins_wav_op4type	},
	{	0x02 , 0x0d , THIS(MIXPERCENT)		, THIS(OP3_ADSR_02)		, THIS(WAVPRESET_01)	, THIS(OP3_ADSR_03)		, &CACHE_TEXT					, THISVAR(WAVPRESET[0])				, &cb_ins_wav_preset0	},
	{	0x02 , 0x0e , THIS(WAVPRESET_00)	, THIS(OP3_ADSR_02)		, THIS(WAVPRESET_02)	, THIS(OP3_ADSR_03)		, &CACHE_TEXT					, THISVAR(WAVPRESET[1])				, &cb_ins_wav_preset1	},
	{	0x02 , 0x0f , THIS(WAVPRESET_01)	, THIS(OP4_TYPE)		, THIS(WAVPRESET_03)	, THIS(OP4_TYPE)		, &CACHE_TEXT					, THISVAR(WAVPRESET[2])				, &cb_ins_wav_preset2	},
	{	0x02 , 0x10 , THIS(WAVPRESET_02)	, THIS(OP4_TYPE)		, THIS(WAVPRESET_04)	, THIS(OP4_TYPE)		, &CACHE_TEXT					, THISVAR(WAVPRESET[3])				, &cb_ins_wav_preset3	},
	{	0x02 , 0x11 , THIS(WAVPRESET_03)	, THIS(OP4_ADSR_00)		, THIS(WAVPRESET_05)	, THIS(OP4_ADSR_01)		, &CACHE_TEXT					, THISVAR(WAVPRESET[4])				, &cb_ins_wav_preset4	},
	{	0x02 , 0x12 , THIS(WAVPRESET_04)	, THIS(OP4_ADSR_02)		, THIS(INDEX)			, THIS(OP4_ADSR_03)		, &CACHE_TEXT					, THISVAR(WAVPRESET[5])				, &cb_ins_wav_preset5	},
	{	0x09 , 0x07 , THIS(OP1_ADSR_02)		, THIS(OP2_ADSR_01)		, THIS(OP2_ADSR_02)		, THIS(TYPE)			, &CACHE_HEXADECIMAL			, THISVAR(OP2_ADSR[0])				, &cb_ins_wav_op2adsr_0	},
	{	0x0b , 0x07 , THIS(OP1_ADSR_03)		, THIS(OP2_TYPE)		, THIS(OP2_ADSR_03)		, THIS(OP2_ADSR_00)		, &CACHE_HEXADECIMAL			, THISVAR(OP2_ADSR[1])				, &cb_ins_wav_op2adsr_1	},
	{	0x09 , 0x08 , THIS(OP2_ADSR_00)		, THIS(OP2_ADSR_03)		, THIS(OP3_ADSR_00)		, THIS(PHASE)			, &CACHE_HEXADECIMAL			, THISVAR(OP2_ADSR[2])				, &cb_ins_wav_op2adsr_2	},
	{	0x0b , 0x08 , THIS(OP2_ADSR_01)		, THIS(OP2_TYPE)		, THIS(OP3_ADSR_01)		, THIS(OP2_ADSR_02)		, &CACHE_HEXADECIMAL			, THISVAR(OP2_ADSR[3])				, &cb_ins_wav_op2adsr_3	},
	{	0x09 , 0x0c , THIS(OP2_ADSR_02)		, THIS(OP3_ADSR_01)		, THIS(OP3_ADSR_02)		, THIS(MIXPERCENT)		, &CACHE_HEXADECIMAL			, THISVAR(OP3_ADSR[0])				, &cb_ins_wav_op3adsr_0	},
	{	0x0b , 0x0c , THIS(OP2_ADSR_03)		, THIS(OP3_TYPE)		, THIS(OP3_ADSR_03)		, THIS(OP3_ADSR_00)		, &CACHE_HEXADECIMAL			, THISVAR(OP3_ADSR[1])				, &cb_ins_wav_op3adsr_1	},
	{	0x09 , 0x0d , THIS(OP3_ADSR_00)		, THIS(OP3_ADSR_03)		, THIS(OP4_ADSR_00)		, THIS(WAVPRESET_00)	, &CACHE_HEXADECIMAL			, THISVAR(OP3_ADSR[2])				, &cb_ins_wav_op3adsr_2	},
	{	0x0b , 0x0d , THIS(OP3_ADSR_01)		, THIS(OP3_TYPE)		, THIS(OP4_ADSR_01)		, THIS(OP3_ADSR_02)		, &CACHE_HEXADECIMAL			, THISVAR(OP3_ADSR[3])				, &cb_ins_wav_op3adsr_3	},
	{	0x09 , 0x11 , THIS(OP3_ADSR_02)		, THIS(OP4_ADSR_01)		, THIS(OP4_ADSR_02)		, THIS(WAVPRESET_04)	, &CACHE_HEXADECIMAL			, THISVAR(OP4_ADSR[0])				, &cb_ins_wav_op4adsr_0	},
	{	0x0b , 0x11 , THIS(OP3_ADSR_03)		, THIS(OP4_TYPE)		, THIS(OP4_ADSR_03)		, THIS(OP4_ADSR_00)		, &CACHE_HEXADECIMAL			, THISVAR(OP4_ADSR[1])				, &cb_ins_wav_op4adsr_1	},
	{	0x09 , 0x12 , THIS(OP4_ADSR_00)		, THIS(OP4_ADSR_03)		, THIS(OP1_ADSR_00)		, THIS(WAVPRESET_05)	, &CACHE_HEXADECIMAL			, THISVAR(OP4_ADSR[2])				, &cb_ins_wav_op4adsr_2	},
	{	0x0b , 0x12 , THIS(OP4_ADSR_01)		, THIS(OP4_TYPE)		, THIS(OP1_ADSR_01)		, THIS(OP4_ADSR_02)		, &CACHE_HEXADECIMAL			, THISVAR(OP4_ADSR[3])				, &cb_ins_wav_op4adsr_3	},
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS
	
#define THIS(a) &INS_FMW_CONTROLS[CONTROL_INS_FMW_##a]
#define THISVAR(a) ((u8*)&(VAR_FMW.a))
const Control INS_FMW_CONTROLS[CONTROL_INS_FMW_MAX] = { 
	//  x	 y		up						right					down					left					cache							var									callback		
	{	0x05 , 0x04 , THIS(MODE)			, THIS(FM1_ADSR_02)		, THIS(NAME)			, THIS(FM1_ADSR_03)		, &CACHE_HEXADECIMAL_INSTRUMENT	, (u8*)&(VAR_CFG.CURRENTINSTRUMENT)	, &cb_ins_index			},
	{	0x01 , 0x05 , THIS(INDEX)			, THIS(FM2_ADSR_00)		, THIS(TYPE)			, THIS(FM2_ADSR_01)		, &CACHE_TEXT					, INSTRUM(NAME)						, &cb_ins_name			},
	{	0x06 , 0x07 , THIS(NAME)			, THIS(FM2_ADSR_02)		, THIS(FM1_TYPE)		, THIS(FM2_ADSR_03)		, &CACHE_INSTYPES				, INSTRUM(TYPE)						, &cb_ins_type			},
	{	0x06 , 0x0a , THIS(FM1_TYPE)		, THIS(FM3_ADSR_02)		, THIS(FM3_TYPE)		, THIS(FM3_ADSR_03)		, &CACHE_OPERATORS				, THISVAR(OP2_TYPE)					, &cb_ins_fm_op2type		},
	{	0x06 , 0x0c , THIS(FM3_TYPE)		, THIS(FM4_ADSR_00)		, THIS(MULT)			, THIS(FM4_ADSR_01)		, &CACHE_OPERATORS				, THISVAR(OP4_TYPE)					, &cb_ins_fm_op4type		},
	{	0x06 , 0x0b , THIS(FM2_TYPE)		, THIS(FM4_ADSR_01)		, THIS(FM4_TYPE)		, THIS(FM4_ADSR_01)		, &CACHE_OPERATORS				, THISVAR(OP3_TYPE)					, &cb_ins_fm_op3type		},
	{	0x06 , 0x09 , THIS(TYPE)			, THIS(FM3_ADSR_00)		, THIS(FM2_TYPE)		, THIS(FM3_ADSR_01)		, &CACHE_OPERATORS				, THISVAR(OP1_TYPE)					, &cb_ins_fm_op1type		},
	{	0x06 , 0x0d , THIS(FM4_TYPE)		, THIS(FM4_ADSR_00)		, THIS(MODE)			, THIS(FM4_ADSR_01)		, &CACHE_HEXADECIMAL			, THISVAR(MULT)						, &cb_ins_fm_mult		},
	{	0x02 , 0x10 , THIS(MULT)			, THIS(FM4_ADSR_02)		, THIS(INDEX)			, THIS(FM4_ADSR_03)		, &CACHE_ALGORITHMS				, THISVAR(ALGORITHM)				, &cb_ins_algorithm		},
	{	0x09 , 0x05 , THIS(FM1_ADSR_02)		, THIS(FM2_ADSR_01)		, THIS(FM2_ADSR_02)		, THIS(NAME)			, &CACHE_HEXADECIMAL			, THISVAR(OP2_ADSR[0])				, &cb_ins_fm_op2adsr_0	},
	{	0x0b , 0x05 , THIS(FM1_ADSR_03)		, THIS(NAME)			, THIS(FM2_ADSR_03)		, THIS(FM2_ADSR_00)		, &CACHE_HEXADECIMAL			, THISVAR(OP2_ADSR[1])				, &cb_ins_fm_op2adsr_1	},
	{	0x09 , 0x06 , THIS(FM2_ADSR_00)		, THIS(FM2_ADSR_03)		, THIS(FM3_ADSR_00)		, THIS(NAME)			, &CACHE_HEXADECIMAL			, THISVAR(OP2_ADSR[2])				, &cb_ins_fm_op2adsr_2	},
	{	0x0b , 0x06 , THIS(FM2_ADSR_01)		, THIS(NAME)			, THIS(FM3_ADSR_01)		, THIS(FM2_ADSR_02)		, &CACHE_HEXADECIMAL			, THISVAR(OP2_ADSR[3])				, &cb_ins_fm_op2adsr_3	},
	{	0x09 , 0x09 , THIS(FM2_ADSR_02)		, THIS(FM3_ADSR_01)		, THIS(FM3_ADSR_02)		, THIS(FM1_TYPE)		, &CACHE_HEXADECIMAL			, THISVAR(OP3_ADSR[0])				, &cb_ins_fm_op3adsr_0	},
	{	0x0b , 0x09 , THIS(FM2_ADSR_03)		, THIS(FM1_TYPE)		, THIS(FM3_ADSR_03)		, THIS(FM3_ADSR_00)		, &CACHE_HEXADECIMAL			, THISVAR(OP3_ADSR[1])				, &cb_ins_fm_op3adsr_1	},
	{	0x09 , 0x0a , THIS(FM3_ADSR_00)		, THIS(FM3_ADSR_03)		, THIS(FM4_ADSR_00)		, THIS(FM2_TYPE)		, &CACHE_HEXADECIMAL			, THISVAR(OP3_ADSR[2])				, &cb_ins_fm_op3adsr_2	},
	{	0x0b , 0x0a , THIS(FM3_ADSR_01)		, THIS(FM2_TYPE)		, THIS(FM4_ADSR_01)		, THIS(FM3_ADSR_02)		, &CACHE_HEXADECIMAL			, THISVAR(OP3_ADSR[3])				, &cb_ins_fm_op3adsr_3	},
	{	0x09 , 0x0d , THIS(FM3_ADSR_02)		, THIS(FM4_ADSR_01)		, THIS(FM4_ADSR_02)		, THIS(MULT)			, &CACHE_HEXADECIMAL			, THISVAR(OP4_ADSR[0])				, &cb_ins_fm_op4adsr_0	},
	{	0x0b , 0x0d , THIS(FM3_ADSR_03)		, THIS(MULT)			, THIS(FM4_ADSR_03)		, THIS(FM4_ADSR_00)		, &CACHE_HEXADECIMAL			, THISVAR(OP4_ADSR[1])				, &cb_ins_fm_op4adsr_1	},
	{	0x09 , 0x0e , THIS(FM4_ADSR_00)		, THIS(FM4_ADSR_03)		, THIS(MODE)			, THIS(MULT)			, &CACHE_HEXADECIMAL			, THISVAR(OP4_ADSR[2])				, &cb_ins_fm_op4adsr_2	},
	{	0x0b , 0x0e , THIS(FM4_ADSR_01)		, THIS(MULT)			, THIS(MODE)			, THIS(FM4_ADSR_02)		, &CACHE_HEXADECIMAL			, THISVAR(OP4_ADSR[3])				, &cb_ins_fm_op4adsr_3	},
	{	0x09 , 0x01 , THIS(MODE)			, THIS(FM1_ADSR_01)		, THIS(FM1_ADSR_02)		, THIS(INDEX)			, &CACHE_HEXADECIMAL			, THISVAR(OP1_ADSR[0])				, &cb_ins_fm_op1adsr_0	},
	{	0x0b , 0x01 , THIS(MODE)			, THIS(INDEX)			, THIS(FM1_ADSR_03)		, THIS(FM1_ADSR_00)		, &CACHE_HEXADECIMAL			, THISVAR(OP1_ADSR[1])				, &cb_ins_fm_op1adsr_1	},
	{	0x09 , 0x02 , THIS(FM1_ADSR_00)		, THIS(FM1_ADSR_03)		, THIS(FM2_ADSR_00)		, THIS(INDEX)			, &CACHE_HEXADECIMAL			, THISVAR(OP1_ADSR[2])				, &cb_ins_fm_op1adsr_2	},
	{	0x0b , 0x02 , THIS(FM1_ADSR_01)		, THIS(INDEX)			, THIS(FM2_ADSR_01)		, THIS(FM1_ADSR_02)		, &CACHE_HEXADECIMAL			, THISVAR(OP1_ADSR[3])				, &cb_ins_fm_op1adsr_3	},
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS

const Control INS_SMP_CONTROLS[CONTROL_INS_SMP_MAX] = { 
#define THIS(a) &INS_SMP_CONTROLS[CONTROL_INS_SMP_##a]
#define THISVAR(a) ((u8*)&(VAR_SMP.a))
	//  x	 y		up						right					down					left					cache							var									callback
	{	0x05 , 0x04 , THIS(SMPKIT_0A)		, THIS(B)				, THIS(NAME)			, THIS(B)				, &CACHE_HEXADECIMAL_INSTRUMENT	, (u8*)&(VAR_CFG.CURRENTINSTRUMENT)	, &cb_ins_index			},
	{	0x01 , 0x05 , THIS(INDEX)			, THIS(S)				, THIS(TYPE)			, THIS(S)				, &CACHE_TEXT					, INSTRUM(NAME)						, &cb_ins_name			},
	{	0x06 , 0x07 , THIS(NAME)			, THIS(R)				, THIS(START)			, THIS(R)				, &CACHE_INSTYPES				, INSTRUM(TYPE)						, &cb_ins_type			},
	{	0x06 , 0x08 , THIS(TYPE)			, THIS(PLAYMODE)		, THIS(END)				, THIS(PLAYMODE)		, &CACHE_HEXADECIMAL_DOUBLE		, THISVAR(START)					, &cb_ins_smp_start		},
	{	0x06 , 0x09 , THIS(START)			, THIS(LOOPMODE)		, THIS(KIT)				, THIS(LOOPMODE)		, &CACHE_HEXADECIMAL_DOUBLE		, THISVAR(END)						, &cb_ins_smp_end		},
	{	0x0c , 0x01 , THIS(SMPKIT_0B)		, THIS(INDEX)			, THIS(ADSR_01)			, THIS(ADSR_01)			, &CACHE_HEXADECIMAL_DOUBLE		, THISVAR(FREQUENCY)				, &cb_ins_smp_frequency	},
	{	0x0b , 0x04 , THIS(ADSR_03)			, THIS(INDEX)			, THIS(S)				, THIS(INDEX)			, &CACHE_HEXADECIMAL			, THISVAR(B)						, &cb_ins_smp_b			},
	{	0x0b , 0x05 , THIS(B)				, THIS(NAME)			, THIS(R)				, THIS(NAME)			, &CACHE_HEXADECIMAL			, THISVAR(S)						, &cb_ins_smp_s			},
	{	0x0b , 0x06 , THIS(S)				, THIS(TYPE)			, THIS(PLAYMODE)		, THIS(TYPE)			, &CACHE_HEXADECIMAL			, THISVAR(R)						, &cb_ins_smp_r			},
	{	0x0c , 0x08 , THIS(R)				, THIS(START)			, THIS(LOOPMODE)		, THIS(START)			, &CACHE_PLAYMODES				, THISVAR(SYNTHMODE)				, &cb_ins_smp_synthmode	},
	{	0x0c , 0x09 , THIS(PLAYMODE)		, THIS(END)				, THIS(KIT)				, THIS(END)				, &CACHE_LOOPMODES				, THISVAR(LOOP)						, &cb_ins_smp_loop		},
	{	0x04 , 0x0b , THIS(END)				, THIS(KIT)				, THIS(SMPKIT_00)		, THIS(KIT)				, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(KIT)						, &cb_ins_smp_kit		},
	{	0x01 , 0x0d , THIS(KIT)				, THIS(SMPKIT_01)		, THIS(SMPKIT_02)		, THIS(SMPKIT_00)		, &CACHE_TEXT					, THISVAR(SMPKIT[0])				, &cb_ins_smpkit_00		},
	{	0x07 , 0x0d , THIS(KIT)				, THIS(SMPKIT_0B)		, THIS(SMPKIT_03)		, THIS(SMPKIT_00)		, &CACHE_TEXT					, THISVAR(SMPKIT[1])				, &cb_ins_smpkit_01		},
	{	0x01 , 0x0e , THIS(SMPKIT_00)		, THIS(SMPKIT_03)		, THIS(SMPKIT_04)		, THIS(SMPKIT_00)		, &CACHE_TEXT					, THISVAR(SMPKIT[2])				, &cb_ins_smpkit_02		},
	{	0x07 , 0x0e , THIS(SMPKIT_01)		, THIS(SMPKIT_0B)		, THIS(SMPKIT_05)		, THIS(SMPKIT_02)		, &CACHE_TEXT					, THISVAR(SMPKIT[3])				, &cb_ins_smpkit_03		},
	{	0x01 , 0x0f , THIS(SMPKIT_02)		, THIS(SMPKIT_05)		, THIS(SMPKIT_06)		, THIS(SMPKIT_00)		, &CACHE_TEXT					, THISVAR(SMPKIT[4])				, &cb_ins_smpkit_04		},
	{	0x07 , 0x0f , THIS(SMPKIT_03)		, THIS(SMPKIT_0B)		, THIS(SMPKIT_07)		, THIS(SMPKIT_04)		, &CACHE_TEXT					, THISVAR(SMPKIT[5])				, &cb_ins_smpkit_05		},
	{	0x01 , 0x10 , THIS(SMPKIT_04)		, THIS(SMPKIT_07)		, THIS(SMPKIT_08)		, THIS(SMPKIT_00)		, &CACHE_TEXT					, THISVAR(SMPKIT[6])				, &cb_ins_smpkit_06		},
	{	0x07 , 0x10 , THIS(SMPKIT_05)		, THIS(SMPKIT_0B)		, THIS(SMPKIT_09)		, THIS(SMPKIT_06)		, &CACHE_TEXT					, THISVAR(SMPKIT[7])				, &cb_ins_smpkit_07		},
	{	0x01 , 0x11 , THIS(SMPKIT_06)		, THIS(SMPKIT_09)		, THIS(SMPKIT_0A)		, THIS(SMPKIT_00)		, &CACHE_TEXT					, THISVAR(SMPKIT[8])				, &cb_ins_smpkit_08		},
	{	0x07 , 0x11 , THIS(SMPKIT_07)		, THIS(SMPKIT_0B)		, THIS(SMPKIT_0B)		, THIS(SMPKIT_08)		, &CACHE_TEXT					, THISVAR(SMPKIT[9])				, &cb_ins_smpkit_09		},
	{	0x01 , 0x12 , THIS(SMPKIT_08)		, THIS(SMPKIT_0B)		, THIS(INDEX)			, THIS(SMPKIT_00)		, &CACHE_TEXT					, THISVAR(SMPKIT[10])				, &cb_ins_smpkit_0A		},
	{	0x07 , 0x12 , THIS(SMPKIT_09)		, THIS(SMPKIT_0B)		, THIS(INDEX)			, THIS(SMPKIT_0A)		, &CACHE_TEXT					, THISVAR(SMPKIT[11])				, &cb_ins_smpkit_0B		},
	{	0x09 , 0x02 , THIS(FREQUENCY)		, THIS(ADSR_01)			, THIS(ADSR_02)			, THIS(INDEX)			, &CACHE_HEXADECIMAL			, THISVAR(ADSR[0])   				, &cb_ins_smp_adsr_0		},
	{	0x0b , 0x02 , THIS(FREQUENCY)		, THIS(FREQUENCY)		, THIS(ADSR_03)			, THIS(ADSR_00)			, &CACHE_HEXADECIMAL			, THISVAR(ADSR[1])   				, &cb_ins_smp_adsr_1		},
	{	0x09 , 0x03 , THIS(ADSR_00)			, THIS(ADSR_03)			, THIS(B)				, THIS(INDEX)			, &CACHE_HEXADECIMAL			, THISVAR(ADSR[2])   				, &cb_ins_smp_adsr_2		},
	{	0x0b , 0x03 , THIS(ADSR_01)			, THIS(FREQUENCY)		, THIS(B)				, THIS(ADSR_02)			, &CACHE_HEXADECIMAL			, THISVAR(ADSR[3])   				, &cb_ins_smp_adsr_3		},
	CONTROL_TERMINATOR
};
#undef THIS
#undef THISVAR
#undef INSTRUM


//
// Instrument table hidden region controls
//

const Control TABLE_CONTROLS[CONTROL_TABLE_MAX] = { 
#define THIS(a) &TABLE_CONTROLS[CONTROL_TABLE_##a]
#define INSTRUM(a) ((u8*)&(VAR_INSTRUMENT.a))
	//  x	 y		up						right					down					left					cache							var									callback		
	{	0x0f , 0x01 , THIS(TRANSPOSE_0F)	, THIS(VOLUME_00)		, THIS(TRANSPOSE_01)	, THIS(COMMAND2_00)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[0]) 		, &cb_ins_table_transpose_00	}, 
	{	0x0f , 0x02 , THIS(TRANSPOSE_00)	, THIS(VOLUME_01)		, THIS(TRANSPOSE_02)	, THIS(COMMAND2_01)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[1]) 		, &cb_ins_table_transpose_01	}, 
	{	0x0f , 0x03 , THIS(TRANSPOSE_01)	, THIS(VOLUME_02)		, THIS(TRANSPOSE_03)	, THIS(COMMAND2_02)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[2]) 		, &cb_ins_table_transpose_02	}, 
	{	0x0f , 0x04 , THIS(TRANSPOSE_02)	, THIS(VOLUME_03)		, THIS(TRANSPOSE_04)	, THIS(COMMAND2_03)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[3]) 		, &cb_ins_table_transpose_03	}, 
	{	0x0f , 0x05 , THIS(TRANSPOSE_03)	, THIS(VOLUME_04)		, THIS(TRANSPOSE_05)	, THIS(COMMAND2_04)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[4]) 		, &cb_ins_table_transpose_04	}, 
	{	0x0f , 0x06 , THIS(TRANSPOSE_04)	, THIS(VOLUME_05)		, THIS(TRANSPOSE_06)	, THIS(COMMAND2_05)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[5]) 		, &cb_ins_table_transpose_05	}, 
	{	0x0f , 0x07 , THIS(TRANSPOSE_05)	, THIS(VOLUME_06)		, THIS(TRANSPOSE_07)	, THIS(COMMAND2_06)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[6]) 		, &cb_ins_table_transpose_06	}, 
	{	0x0f , 0x08 , THIS(TRANSPOSE_06)	, THIS(VOLUME_07)		, THIS(TRANSPOSE_08)	, THIS(COMMAND2_07)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[7]) 		, &cb_ins_table_transpose_07	}, 
	{	0x0f , 0x09 , THIS(TRANSPOSE_07)	, THIS(VOLUME_08)		, THIS(TRANSPOSE_09)	, THIS(COMMAND2_08)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[8]) 		, &cb_ins_table_transpose_08	}, 
	{	0x0f , 0x0a , THIS(TRANSPOSE_08)	, THIS(VOLUME_09)		, THIS(TRANSPOSE_0A)	, THIS(COMMAND2_09)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[9]) 		, &cb_ins_table_transpose_09	}, 
	{	0x0f , 0x0b , THIS(TRANSPOSE_09)	, THIS(VOLUME_0A)		, THIS(TRANSPOSE_0B)	, THIS(COMMAND2_0A)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[10])		, &cb_ins_table_transpose_0A	}, 
	{	0x0f , 0x0c , THIS(TRANSPOSE_0A)	, THIS(VOLUME_0B)		, THIS(TRANSPOSE_0C)	, THIS(COMMAND2_0B)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[11])		, &cb_ins_table_transpose_0B	}, 
	{	0x0f , 0x0d , THIS(TRANSPOSE_0B)	, THIS(VOLUME_0C)		, THIS(TRANSPOSE_0D)	, THIS(COMMAND2_0C)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[12])		, &cb_ins_table_transpose_0C	}, 
	{	0x0f , 0x0e , THIS(TRANSPOSE_0C)	, THIS(VOLUME_0D)		, THIS(TRANSPOSE_0E)	, THIS(COMMAND2_0D)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[13])		, &cb_ins_table_transpose_0D	}, 
	{	0x0f , 0x0f , THIS(TRANSPOSE_0D)	, THIS(VOLUME_0E)		, THIS(TRANSPOSE_0F)	, THIS(COMMAND2_0E)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[14])		, &cb_ins_table_transpose_0E	}, 
	{	0x0f , 0x10 , THIS(TRANSPOSE_0E)	, THIS(VOLUME_0F)		, THIS(TRANSPOSE_00)	, THIS(COMMAND2_0F)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.TRANSPOSE[15])		, &cb_ins_table_transpose_0F	}, 
	{	0x13 , 0x01 , THIS(VOLUME_0F)		, THIS(COMMAND1_00)		, THIS(VOLUME_01)		, THIS(TRANSPOSE_00)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[0]) 			, &cb_ins_table_volume_00	}, 
	{	0x13 , 0x02 , THIS(VOLUME_00)		, THIS(COMMAND1_01)		, THIS(VOLUME_02)		, THIS(TRANSPOSE_01)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[1]) 			, &cb_ins_table_volume_01	}, 
	{	0x13 , 0x03 , THIS(VOLUME_01)		, THIS(COMMAND1_02)		, THIS(VOLUME_03)		, THIS(TRANSPOSE_02)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[2]) 			, &cb_ins_table_volume_02	}, 
	{	0x13 , 0x04 , THIS(VOLUME_02)		, THIS(COMMAND1_03)		, THIS(VOLUME_04)		, THIS(TRANSPOSE_03)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[3]) 			, &cb_ins_table_volume_03	}, 
	{	0x13 , 0x05 , THIS(VOLUME_03)		, THIS(COMMAND1_04)		, THIS(VOLUME_05)		, THIS(TRANSPOSE_04)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[4]) 			, &cb_ins_table_volume_04	}, 
	{	0x13 , 0x06 , THIS(VOLUME_04)		, THIS(COMMAND1_05)		, THIS(VOLUME_06)		, THIS(TRANSPOSE_05)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[5]) 			, &cb_ins_table_volume_05	}, 
	{	0x13 , 0x07 , THIS(VOLUME_05)		, THIS(COMMAND1_06)		, THIS(VOLUME_07)		, THIS(TRANSPOSE_06)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[6]) 			, &cb_ins_table_volume_06	}, 
	{	0x13 , 0x08 , THIS(VOLUME_06)		, THIS(COMMAND1_07)		, THIS(VOLUME_08)		, THIS(TRANSPOSE_07)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[7]) 			, &cb_ins_table_volume_07	}, 
	{	0x13 , 0x09 , THIS(VOLUME_07)		, THIS(COMMAND1_08)		, THIS(VOLUME_09)		, THIS(TRANSPOSE_08)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[8]) 			, &cb_ins_table_volume_08	}, 
	{	0x13 , 0x0a , THIS(VOLUME_08)		, THIS(COMMAND1_09)		, THIS(VOLUME_0A)		, THIS(TRANSPOSE_09)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[9]) 			, &cb_ins_table_volume_09	}, 
	{	0x13 , 0x0b , THIS(VOLUME_09)		, THIS(COMMAND1_0A)		, THIS(VOLUME_0B)		, THIS(TRANSPOSE_0A)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[10])			, &cb_ins_table_volume_0A	}, 
	{	0x13 , 0x0c , THIS(VOLUME_0A)		, THIS(COMMAND1_0B)		, THIS(VOLUME_0C)		, THIS(TRANSPOSE_0B)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[11])			, &cb_ins_table_volume_0B	}, 
	{	0x13 , 0x0d , THIS(VOLUME_0B)		, THIS(COMMAND1_0C)		, THIS(VOLUME_0D)		, THIS(TRANSPOSE_0C)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[12])			, &cb_ins_table_volume_0C	}, 
	{	0x13 , 0x0e , THIS(VOLUME_0C)		, THIS(COMMAND1_0D)		, THIS(VOLUME_0E)		, THIS(TRANSPOSE_0D)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[13])			, &cb_ins_table_volume_0D	}, 
	{	0x13 , 0x0f , THIS(VOLUME_0D)		, THIS(COMMAND1_0E)		, THIS(VOLUME_0F)		, THIS(TRANSPOSE_0E)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[14])			, &cb_ins_table_volume_0E	}, 
	{	0x13 , 0x10 , THIS(VOLUME_0E)		, THIS(COMMAND1_0F)		, THIS(VOLUME_00)		, THIS(TRANSPOSE_0F)	, &CACHE_HEXADECIMAL			, INSTRUM(TABLE.VOLUME[15])			, &cb_ins_table_volume_0F	}, 
	{	0x17 , 0x01 , THIS(VALUE1_0F)		, THIS(COMMAND2_00)		, THIS(VALUE1_01)		, THIS(COMMAND1_00)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][0]) 		, &cb_ins_table_value1_00	}, 
	{	0x17 , 0x02 , THIS(VALUE1_00)		, THIS(COMMAND2_01)		, THIS(VALUE1_02)		, THIS(COMMAND1_01)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][1]) 		, &cb_ins_table_value1_01	}, 
	{	0x17 , 0x03 , THIS(VALUE1_01)		, THIS(COMMAND2_02)		, THIS(VALUE1_03)		, THIS(COMMAND1_02)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][2]) 		, &cb_ins_table_value1_02	}, 
	{	0x17 , 0x04 , THIS(VALUE1_02)		, THIS(COMMAND2_03)		, THIS(VALUE1_04)		, THIS(COMMAND1_03)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][3]) 		, &cb_ins_table_value1_03	}, 
	{	0x17 , 0x05 , THIS(VALUE1_03)		, THIS(COMMAND2_04)		, THIS(VALUE1_05)		, THIS(COMMAND1_04)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][4]) 		, &cb_ins_table_value1_04	}, 
	{	0x17 , 0x06 , THIS(VALUE1_04)		, THIS(COMMAND2_05)		, THIS(VALUE1_06)		, THIS(COMMAND1_05)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][5]) 		, &cb_ins_table_value1_05	}, 
	{	0x17 , 0x07 , THIS(VALUE1_05)		, THIS(COMMAND2_06)		, THIS(VALUE1_07)		, THIS(COMMAND1_06)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][6]) 		, &cb_ins_table_value1_06	}, 
	{	0x17 , 0x08 , THIS(VALUE1_06)		, THIS(COMMAND2_07)		, THIS(VALUE1_08)		, THIS(COMMAND1_07)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][7]) 		, &cb_ins_table_value1_07	}, 
	{	0x17 , 0x09 , THIS(VALUE1_07)		, THIS(COMMAND2_08)		, THIS(VALUE1_09)		, THIS(COMMAND1_08)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][8]) 		, &cb_ins_table_value1_08	}, 
	{	0x17 , 0x0a , THIS(VALUE1_08)		, THIS(COMMAND2_09)		, THIS(VALUE1_0A)		, THIS(COMMAND1_09)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][9]) 		, &cb_ins_table_value1_09	}, 
	{	0x17 , 0x0b , THIS(VALUE1_09)		, THIS(COMMAND2_0A)		, THIS(VALUE1_0B)		, THIS(COMMAND1_0A)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][10])		, &cb_ins_table_value1_0A	}, 
	{	0x17 , 0x0c , THIS(VALUE1_0A)		, THIS(COMMAND2_0B)		, THIS(VALUE1_0C)		, THIS(COMMAND1_0B)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][11])		, &cb_ins_table_value1_0B	}, 
	{	0x17 , 0x0d , THIS(VALUE1_0B)		, THIS(COMMAND2_0C)		, THIS(VALUE1_0D)		, THIS(COMMAND1_0C)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][12])		, &cb_ins_table_value1_0C	}, 
	{	0x17 , 0x0e , THIS(VALUE1_0C)		, THIS(COMMAND2_0D)		, THIS(VALUE1_0E)		, THIS(COMMAND1_0D)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][13])		, &cb_ins_table_value1_0D	}, 
	{	0x17 , 0x0f , THIS(VALUE1_0D)		, THIS(COMMAND2_0E)		, THIS(VALUE1_0F)		, THIS(COMMAND1_0E)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][14])		, &cb_ins_table_value1_0E	}, 
	{	0x17 , 0x10 , THIS(VALUE1_0E)		, THIS(COMMAND2_0F)		, THIS(VALUE1_00)		, THIS(COMMAND1_0F)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[0][15])		, &cb_ins_table_value1_0F	}, 
	{	0x16 , 0x01 , THIS(COMMAND1_0F)		, THIS(VALUE1_00)		, THIS(COMMAND1_01)		, THIS(VOLUME_00)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][0]) 		, &cb_ins_table_command1_00	},
	{	0x16 , 0x02 , THIS(COMMAND1_00)		, THIS(VALUE1_01)		, THIS(COMMAND1_02)		, THIS(VOLUME_01)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][1]) 		, &cb_ins_table_command1_01	},
	{	0x16 , 0x03 , THIS(COMMAND1_01)		, THIS(VALUE1_02)		, THIS(COMMAND1_03)		, THIS(VOLUME_02)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][2]) 		, &cb_ins_table_command1_02	},
	{	0x16 , 0x04 , THIS(COMMAND1_02)		, THIS(VALUE1_03)		, THIS(COMMAND1_04)		, THIS(VOLUME_03)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][3]) 		, &cb_ins_table_command1_03	},
	{	0x16 , 0x05 , THIS(COMMAND1_03)		, THIS(VALUE1_04)		, THIS(COMMAND1_05)		, THIS(VOLUME_04)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][4]) 		, &cb_ins_table_command1_04	},
	{	0x16 , 0x06 , THIS(COMMAND1_04)		, THIS(VALUE1_05)		, THIS(COMMAND1_06)		, THIS(VOLUME_05)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][5]) 		, &cb_ins_table_command1_05	},
	{	0x16 , 0x07 , THIS(COMMAND1_05)		, THIS(VALUE1_06)		, THIS(COMMAND1_07)		, THIS(VOLUME_06)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][6]) 		, &cb_ins_table_command1_06	},
	{	0x16 , 0x08 , THIS(COMMAND1_06)		, THIS(VALUE1_07)		, THIS(COMMAND1_08)		, THIS(VOLUME_07)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][7]) 		, &cb_ins_table_command1_07	},
	{	0x16 , 0x09 , THIS(COMMAND1_07)		, THIS(VALUE1_08)		, THIS(COMMAND1_09)		, THIS(VOLUME_08)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][8]) 		, &cb_ins_table_command1_08	},
	{	0x16 , 0x0a , THIS(COMMAND1_08)		, THIS(VALUE1_09)		, THIS(COMMAND1_0A)		, THIS(VOLUME_09)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][9]) 		, &cb_ins_table_command1_09	},
	{	0x16 , 0x0b , THIS(COMMAND1_09)		, THIS(VALUE1_0A)		, THIS(COMMAND1_0B)		, THIS(VOLUME_0A)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][10])		, &cb_ins_table_command1_0A	},
	{	0x16 , 0x0c , THIS(COMMAND1_0A)		, THIS(VALUE1_0B)		, THIS(COMMAND1_0C)		, THIS(VOLUME_0B)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][11])		, &cb_ins_table_command1_0B	},
	{	0x16 , 0x0d , THIS(COMMAND1_0B)		, THIS(VALUE1_0C)		, THIS(COMMAND1_0D)		, THIS(VOLUME_0C)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][12])		, &cb_ins_table_command1_0C	},
	{	0x16 , 0x0e , THIS(COMMAND1_0C)		, THIS(VALUE1_0D)		, THIS(COMMAND1_0E)		, THIS(VOLUME_0D)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][13])		, &cb_ins_table_command1_0D	},
	{	0x16 , 0x0f , THIS(COMMAND1_0D)		, THIS(VALUE1_0E)		, THIS(COMMAND1_0F)		, THIS(VOLUME_0E)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][14])		, &cb_ins_table_command1_0E	},
	{	0x16 , 0x10 , THIS(COMMAND1_0E)		, THIS(VALUE1_0F)		, THIS(COMMAND1_00)		, THIS(VOLUME_0F)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[0][15])		, &cb_ins_table_command1_0F	},
	{	0x1c , 0x01 , THIS(VALUE2_0F)		, THIS(TRANSPOSE_00)	, THIS(VALUE2_01)		, THIS(COMMAND2_00)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][0]) 		, &cb_ins_table_value2_00	},
	{	0x1c , 0x02 , THIS(VALUE2_00)		, THIS(TRANSPOSE_01)	, THIS(VALUE2_02)		, THIS(COMMAND2_01)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][1]) 		, &cb_ins_table_value2_01	},
	{	0x1c , 0x03 , THIS(VALUE2_01)		, THIS(TRANSPOSE_02)	, THIS(VALUE2_03)		, THIS(COMMAND2_02)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][2]) 		, &cb_ins_table_value2_02	},
	{	0x1c , 0x04 , THIS(VALUE2_02)		, THIS(TRANSPOSE_03)	, THIS(VALUE2_04)		, THIS(COMMAND2_03)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][3]) 		, &cb_ins_table_value2_03	},
	{	0x1c , 0x05 , THIS(VALUE2_03)		, THIS(TRANSPOSE_04)	, THIS(VALUE2_05)		, THIS(COMMAND2_04)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][4]) 		, &cb_ins_table_value2_04	},
	{	0x1c , 0x06 , THIS(VALUE2_04)		, THIS(TRANSPOSE_05)	, THIS(VALUE2_06)		, THIS(COMMAND2_05)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][5]) 		, &cb_ins_table_value2_05	},
	{	0x1c , 0x07 , THIS(VALUE2_05)		, THIS(TRANSPOSE_06)	, THIS(VALUE2_07)		, THIS(COMMAND2_06)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][6]) 		, &cb_ins_table_value2_06	},
	{	0x1c , 0x08 , THIS(VALUE2_06)		, THIS(TRANSPOSE_07)	, THIS(VALUE2_08)		, THIS(COMMAND2_07)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][7]) 		, &cb_ins_table_value2_07	},
	{	0x1c , 0x09 , THIS(VALUE2_07)		, THIS(TRANSPOSE_08)	, THIS(VALUE2_09)		, THIS(COMMAND2_08)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][8]) 		, &cb_ins_table_value2_08	},
	{	0x1c , 0x0a , THIS(VALUE2_08)		, THIS(TRANSPOSE_09)	, THIS(VALUE2_0A)		, THIS(COMMAND2_09)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][9]) 		, &cb_ins_table_value2_09	},
	{	0x1c , 0x0b , THIS(VALUE2_09)		, THIS(TRANSPOSE_0A)	, THIS(VALUE2_0B)		, THIS(COMMAND2_0A)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][10])		, &cb_ins_table_value2_0A	},
	{	0x1c , 0x0c , THIS(VALUE2_0A)		, THIS(TRANSPOSE_0B)	, THIS(VALUE2_0C)		, THIS(COMMAND2_0B)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][11])		, &cb_ins_table_value2_0B	},
	{	0x1c , 0x0d , THIS(VALUE2_0B)		, THIS(TRANSPOSE_0C)	, THIS(VALUE2_0D)		, THIS(COMMAND2_0C)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][12])		, &cb_ins_table_value2_0C	},
	{	0x1c , 0x0e , THIS(VALUE2_0C)		, THIS(TRANSPOSE_0D)	, THIS(VALUE2_0E)		, THIS(COMMAND2_0D)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][13])		, &cb_ins_table_value2_0D	},
	{	0x1c , 0x0f , THIS(VALUE2_0D)		, THIS(TRANSPOSE_0E)	, THIS(VALUE2_0F)		, THIS(COMMAND2_0E)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][14])		, &cb_ins_table_value2_0E	},
	{	0x1c , 0x10 , THIS(VALUE2_0E)		, THIS(TRANSPOSE_0F)	, THIS(VALUE2_00)		, THIS(COMMAND2_0F)		, &CACHE_HEXADECIMAL_TWOTILES	, INSTRUM(TABLE.VALUE[1][15])		, &cb_ins_table_value2_0F	},
	{	0x1b , 0x01 , THIS(COMMAND2_0F)		, THIS(VALUE2_00)		, THIS(COMMAND2_01)		, THIS(VALUE1_00)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][0]) 		, &cb_ins_table_command2_00	},
	{	0x1b , 0x02 , THIS(COMMAND2_00)		, THIS(VALUE2_01)		, THIS(COMMAND2_02)		, THIS(VALUE1_01)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][1]) 		, &cb_ins_table_command2_01	},
	{	0x1b , 0x03 , THIS(COMMAND2_01)		, THIS(VALUE2_02)		, THIS(COMMAND2_03)		, THIS(VALUE1_02)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][2]) 		, &cb_ins_table_command2_02	},
	{	0x1b , 0x04 , THIS(COMMAND2_02)		, THIS(VALUE2_03)		, THIS(COMMAND2_04)		, THIS(VALUE1_03)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][3]) 		, &cb_ins_table_command2_03	},
	{	0x1b , 0x05 , THIS(COMMAND2_03)		, THIS(VALUE2_04)		, THIS(COMMAND2_05)		, THIS(VALUE1_04)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][4]) 		, &cb_ins_table_command2_04	},
	{	0x1b , 0x06 , THIS(COMMAND2_04)		, THIS(VALUE2_05)		, THIS(COMMAND2_06)		, THIS(VALUE1_05)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][5]) 		, &cb_ins_table_command2_05	},
	{	0x1b , 0x07 , THIS(COMMAND2_05)		, THIS(VALUE2_06)		, THIS(COMMAND2_07)		, THIS(VALUE1_06)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][6]) 		, &cb_ins_table_command2_06	},
	{	0x1b , 0x08 , THIS(COMMAND2_06)		, THIS(VALUE2_07)		, THIS(COMMAND2_08)		, THIS(VALUE1_07)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][7]) 		, &cb_ins_table_command2_07	},
	{	0x1b , 0x09 , THIS(COMMAND2_07)		, THIS(VALUE2_08)		, THIS(COMMAND2_09)		, THIS(VALUE1_08)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][8]) 		, &cb_ins_table_command2_08	},
	{	0x1b , 0x0a , THIS(COMMAND2_08)		, THIS(VALUE2_09)		, THIS(COMMAND2_0A)		, THIS(VALUE1_09)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][9]) 		, &cb_ins_table_command2_09	},
	{	0x1b , 0x0b , THIS(COMMAND2_09)		, THIS(VALUE2_0A)		, THIS(COMMAND2_0B)		, THIS(VALUE1_0A)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][10])		, &cb_ins_table_command2_0A	},
	{	0x1b , 0x0c , THIS(COMMAND2_0A)		, THIS(VALUE2_0B)		, THIS(COMMAND2_0C)		, THIS(VALUE1_0B)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][11])		, &cb_ins_table_command2_0B	},
	{	0x1b , 0x0d , THIS(COMMAND2_0B)		, THIS(VALUE2_0C)		, THIS(COMMAND2_0D)		, THIS(VALUE1_0C)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][12])		, &cb_ins_table_command2_0C	},
	{	0x1b , 0x0e , THIS(COMMAND2_0C)		, THIS(VALUE2_0D)		, THIS(COMMAND2_0E)		, THIS(VALUE1_0D)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][13])		, &cb_ins_table_command2_0D	},
	{	0x1b , 0x0f , THIS(COMMAND2_0D)		, THIS(VALUE2_0E)		, THIS(COMMAND2_0F)		, THIS(VALUE1_0E)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][14])		, &cb_ins_table_command2_0E	},
	{	0x1b , 0x10 , THIS(COMMAND2_0E)		, THIS(VALUE2_0F)		, THIS(COMMAND2_00)		, THIS(VALUE1_0F)		, &CACHE_COMMANDS				, INSTRUM(TABLE.COMMAND[1][15])		, &cb_ins_table_command2_0F	},
	CONTROL_TERMINATOR
};
#undef THIS 
#undef INSTRUM

//**************************//
// TRACKER Screen CONTROLS  //
//**************************//
#define THIS(a) &CHAN0_CONTROLS[CONTROL_CHANNEL0_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[0].a))
const Control CHAN0_CONTROLS[CONTROL_CHANNEL0_0_MAX] = { 
	//  x	 y		up						right					down					left					cache							var									callback		
	{	0x01 , 0x04 , THIS(CHAN0KEY_0F)		, THIS(CHAN0INS_00)		, THIS(CHAN0KEY_01)		, THIS(CHAN0VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch0_key_00 }, 
	{	0x01 , 0x05 , THIS(CHAN0KEY_00)		, THIS(CHAN0INS_01)		, THIS(CHAN0KEY_02)		, THIS(CHAN0VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch0_key_01 },
	{	0x01 , 0x06 , THIS(CHAN0KEY_01)		, THIS(CHAN0INS_02)		, THIS(CHAN0KEY_03)		, THIS(CHAN0VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch0_key_02 },
	{	0x01 , 0x07 , THIS(CHAN0KEY_02)		, THIS(CHAN0INS_03)		, THIS(CHAN0KEY_04)		, THIS(CHAN0VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch0_key_03 },
	{	0x01 , 0x08 , THIS(CHAN0KEY_03)		, THIS(CHAN0INS_04)		, THIS(CHAN0KEY_05)		, THIS(CHAN0VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch0_key_04 },
	{	0x01 , 0x09 , THIS(CHAN0KEY_04)		, THIS(CHAN0INS_05)		, THIS(CHAN0KEY_06)		, THIS(CHAN0VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch0_key_05 },
	{	0x01 , 0x0a , THIS(CHAN0KEY_05)		, THIS(CHAN0INS_06)		, THIS(CHAN0KEY_07)		, THIS(CHAN0VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch0_key_06 },
	{	0x01 , 0x0b , THIS(CHAN0KEY_06)		, THIS(CHAN0INS_07)		, THIS(CHAN0KEY_08)		, THIS(CHAN0VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch0_key_07 },
	{	0x01 , 0x0c , THIS(CHAN0KEY_07)		, THIS(CHAN0INS_08)		, THIS(CHAN0KEY_09)		, THIS(CHAN0VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch0_key_08 },
	{	0x01 , 0x0d , THIS(CHAN0KEY_08)		, THIS(CHAN0INS_09)		, THIS(CHAN0KEY_0A)		, THIS(CHAN0VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch0_key_09 },
	{	0x01 , 0x0e , THIS(CHAN0KEY_09)		, THIS(CHAN0INS_0A)		, THIS(CHAN0KEY_0B)		, THIS(CHAN0VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch0_key_0A },
	{	0x01 , 0x0f , THIS(CHAN0KEY_0A)		, THIS(CHAN0INS_0B)		, THIS(CHAN0KEY_0C)		, THIS(CHAN0VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch0_key_0B },
	{	0x01 , 0x10 , THIS(CHAN0KEY_0B)		, THIS(CHAN0INS_0C)		, THIS(CHAN0KEY_0D)		, THIS(CHAN0VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch0_key_0C },
	{	0x01 , 0x11 , THIS(CHAN0KEY_0C)		, THIS(CHAN0INS_0D)		, THIS(CHAN0KEY_0E)		, THIS(CHAN0VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch0_key_0D },
	{	0x01 , 0x12 , THIS(CHAN0KEY_0D)		, THIS(CHAN0INS_0E)		, THIS(CHAN0KEY_0F)		, THIS(CHAN0VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch0_key_0E },
	{	0x01 , 0x13 , THIS(CHAN0KEY_0E)		, THIS(CHAN0INS_0F)		, THIS(CHAN0KEY_00)		, THIS(CHAN0VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch0_key_0F },
	{	0x04 , 0x04 , THIS(CHAN0INS_0F)		, THIS(CHAN0VOL_00)		, THIS(CHAN0INS_01)		, THIS(CHAN0KEY_00)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[0])					, &cb_ch0_ins_00 },
	{	0x04 , 0x05 , THIS(CHAN0INS_00)		, THIS(CHAN0VOL_01)		, THIS(CHAN0INS_02)		, THIS(CHAN0KEY_01)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[1])					, &cb_ch0_ins_01 },
	{	0x04 , 0x06 , THIS(CHAN0INS_01)		, THIS(CHAN0VOL_02)		, THIS(CHAN0INS_03)		, THIS(CHAN0KEY_02)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[2])					, &cb_ch0_ins_02 },
	{	0x04 , 0x07 , THIS(CHAN0INS_02)		, THIS(CHAN0VOL_03)		, THIS(CHAN0INS_04)		, THIS(CHAN0KEY_03)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[3])					, &cb_ch0_ins_03 },
	{	0x04 , 0x08 , THIS(CHAN0INS_03)		, THIS(CHAN0VOL_04)		, THIS(CHAN0INS_05)		, THIS(CHAN0KEY_04)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[4])					, &cb_ch0_ins_04 },
	{	0x04 , 0x09 , THIS(CHAN0INS_04)		, THIS(CHAN0VOL_05)		, THIS(CHAN0INS_06)		, THIS(CHAN0KEY_05)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[5])					, &cb_ch0_ins_05 },
	{	0x04 , 0x0a , THIS(CHAN0INS_05)		, THIS(CHAN0VOL_06)		, THIS(CHAN0INS_07)		, THIS(CHAN0KEY_06)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[6])					, &cb_ch0_ins_06 },
	{	0x04 , 0x0b , THIS(CHAN0INS_06)		, THIS(CHAN0VOL_07)		, THIS(CHAN0INS_08)		, THIS(CHAN0KEY_07)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[7])					, &cb_ch0_ins_07 },
	{	0x04 , 0x0c , THIS(CHAN0INS_07)		, THIS(CHAN0VOL_08)		, THIS(CHAN0INS_09)		, THIS(CHAN0KEY_08)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[8])					, &cb_ch0_ins_08 },
	{	0x04 , 0x0d , THIS(CHAN0INS_08)		, THIS(CHAN0VOL_09)		, THIS(CHAN0INS_0A)		, THIS(CHAN0KEY_09)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[9])					, &cb_ch0_ins_09 },
	{	0x04 , 0x0e , THIS(CHAN0INS_09)		, THIS(CHAN0VOL_0A)		, THIS(CHAN0INS_0B)		, THIS(CHAN0KEY_0A)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[10])					, &cb_ch0_ins_0A },
	{	0x04 , 0x0f , THIS(CHAN0INS_0A)		, THIS(CHAN0VOL_0B)		, THIS(CHAN0INS_0C)		, THIS(CHAN0KEY_0B)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[11])					, &cb_ch0_ins_0B },
	{	0x04 , 0x10 , THIS(CHAN0INS_0B)		, THIS(CHAN0VOL_0C)		, THIS(CHAN0INS_0D)		, THIS(CHAN0KEY_0C)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[12])					, &cb_ch0_ins_0C },
	{	0x04 , 0x11 , THIS(CHAN0INS_0C)		, THIS(CHAN0VOL_0D)		, THIS(CHAN0INS_0E)		, THIS(CHAN0KEY_0D)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[13])					, &cb_ch0_ins_0D },
	{	0x04 , 0x12 , THIS(CHAN0INS_0D)		, THIS(CHAN0VOL_0E)		, THIS(CHAN0INS_0F)		, THIS(CHAN0KEY_0E)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[14])					, &cb_ch0_ins_0E },
	{	0x04 , 0x13 , THIS(CHAN0INS_0E)		, THIS(CHAN0VOL_0F)		, THIS(CHAN0INS_00)		, THIS(CHAN0KEY_0F)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[15])					, &cb_ch0_ins_0F },
	{	0x06 , 0x04 , THIS(CHAN0VOL_0F)		, THIS(CHAN0CMD_00)		, THIS(CHAN0VOL_01)		, THIS(CHAN0INS_00)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[0])					, &cb_ch0_vol_00 },
	{	0x06 , 0x05 , THIS(CHAN0VOL_00)		, THIS(CHAN0CMD_01)		, THIS(CHAN0VOL_02)		, THIS(CHAN0INS_01)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[1])					, &cb_ch0_vol_01 },
	{	0x06 , 0x06 , THIS(CHAN0VOL_01)		, THIS(CHAN0CMD_02)		, THIS(CHAN0VOL_03)		, THIS(CHAN0INS_02)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[2])					, &cb_ch0_vol_02 },
	{	0x06 , 0x07 , THIS(CHAN0VOL_02)		, THIS(CHAN0CMD_03)		, THIS(CHAN0VOL_04)		, THIS(CHAN0INS_03)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[3])					, &cb_ch0_vol_03 },
	{	0x06 , 0x08 , THIS(CHAN0VOL_03)		, THIS(CHAN0CMD_04)		, THIS(CHAN0VOL_05)		, THIS(CHAN0INS_04)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[4])					, &cb_ch0_vol_04 },
	{	0x06 , 0x09 , THIS(CHAN0VOL_04)		, THIS(CHAN0CMD_05)		, THIS(CHAN0VOL_06)		, THIS(CHAN0INS_05)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[5])					, &cb_ch0_vol_05 },
	{	0x06 , 0x0a , THIS(CHAN0VOL_05)		, THIS(CHAN0CMD_06)		, THIS(CHAN0VOL_07)		, THIS(CHAN0INS_06)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[6])					, &cb_ch0_vol_06 },
	{	0x06 , 0x0b , THIS(CHAN0VOL_06)		, THIS(CHAN0CMD_07)		, THIS(CHAN0VOL_08)		, THIS(CHAN0INS_07)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[7])					, &cb_ch0_vol_07 },
	{	0x06 , 0x0c , THIS(CHAN0VOL_07)		, THIS(CHAN0CMD_08)		, THIS(CHAN0VOL_09)		, THIS(CHAN0INS_08)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[8])					, &cb_ch0_vol_08 },
	{	0x06 , 0x0d , THIS(CHAN0VOL_08)		, THIS(CHAN0CMD_09)		, THIS(CHAN0VOL_0A)		, THIS(CHAN0INS_09)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[9])					, &cb_ch0_vol_09 },
	{	0x06 , 0x0e , THIS(CHAN0VOL_09)		, THIS(CHAN0CMD_0A)		, THIS(CHAN0VOL_0B)		, THIS(CHAN0INS_0A)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[10])					, &cb_ch0_vol_0A },
	{	0x06 , 0x0f , THIS(CHAN0VOL_0A)		, THIS(CHAN0CMD_0B)		, THIS(CHAN0VOL_0C)		, THIS(CHAN0INS_0B)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[11])					, &cb_ch0_vol_0B },
	{	0x06 , 0x10 , THIS(CHAN0VOL_0B)		, THIS(CHAN0CMD_0C)		, THIS(CHAN0VOL_0D)		, THIS(CHAN0INS_0C)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[12])					, &cb_ch0_vol_0C },
	{	0x06 , 0x11 , THIS(CHAN0VOL_0C)		, THIS(CHAN0CMD_0D)		, THIS(CHAN0VOL_0E)		, THIS(CHAN0INS_0D)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[13])					, &cb_ch0_vol_0D },
	{	0x06 , 0x12 , THIS(CHAN0VOL_0D)		, THIS(CHAN0CMD_0E)		, THIS(CHAN0VOL_0F)		, THIS(CHAN0INS_0E)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[14])					, &cb_ch0_vol_0E },
	{	0x06 , 0x13 , THIS(CHAN0VOL_0E)		, THIS(CHAN0CMD_0F)		, THIS(CHAN0VOL_00)		, THIS(CHAN0INS_0F)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[15])					, &cb_ch0_vol_0F },
	{	0x07 , 0x04 , THIS(CHAN0CMD_0F)		, THIS(CHAN0VAL_00)		, THIS(CHAN0CMD_01)		, THIS(CHAN0VOL_00)		, &CACHE_COMMANDS				, THISVAR(CMD[0])					, &cb_ch0_cmd_00 },
	{	0x07 , 0x05 , THIS(CHAN0CMD_00)		, THIS(CHAN0VAL_01)		, THIS(CHAN0CMD_02)		, THIS(CHAN0VOL_01)		, &CACHE_COMMANDS				, THISVAR(CMD[1])					, &cb_ch0_cmd_01 },
	{	0x07 , 0x06 , THIS(CHAN0CMD_01)		, THIS(CHAN0VAL_02)		, THIS(CHAN0CMD_03)		, THIS(CHAN0VOL_02)		, &CACHE_COMMANDS				, THISVAR(CMD[2])					, &cb_ch0_cmd_02 },
	{	0x07 , 0x07 , THIS(CHAN0CMD_02)		, THIS(CHAN0VAL_03)		, THIS(CHAN0CMD_04)		, THIS(CHAN0VOL_03)		, &CACHE_COMMANDS				, THISVAR(CMD[3])					, &cb_ch0_cmd_03 },
	{	0x07 , 0x08 , THIS(CHAN0CMD_03)		, THIS(CHAN0VAL_04)		, THIS(CHAN0CMD_05)		, THIS(CHAN0VOL_04)		, &CACHE_COMMANDS				, THISVAR(CMD[4])					, &cb_ch0_cmd_04 },
	{	0x07 , 0x09 , THIS(CHAN0CMD_04)		, THIS(CHAN0VAL_05)		, THIS(CHAN0CMD_06)		, THIS(CHAN0VOL_05)		, &CACHE_COMMANDS				, THISVAR(CMD[5])					, &cb_ch0_cmd_05 },
	{	0x07 , 0x0a , THIS(CHAN0CMD_05)		, THIS(CHAN0VAL_06)		, THIS(CHAN0CMD_07)		, THIS(CHAN0VOL_06)		, &CACHE_COMMANDS				, THISVAR(CMD[6])					, &cb_ch0_cmd_06 },
	{	0x07 , 0x0b , THIS(CHAN0CMD_06)		, THIS(CHAN0VAL_07)		, THIS(CHAN0CMD_08)		, THIS(CHAN0VOL_07)		, &CACHE_COMMANDS				, THISVAR(CMD[7])					, &cb_ch0_cmd_07 },
	{	0x07 , 0x0c , THIS(CHAN0CMD_07)		, THIS(CHAN0VAL_08)		, THIS(CHAN0CMD_09)		, THIS(CHAN0VOL_08)		, &CACHE_COMMANDS				, THISVAR(CMD[8])					, &cb_ch0_cmd_08 },
	{	0x07 , 0x0d , THIS(CHAN0CMD_08)		, THIS(CHAN0VAL_09)		, THIS(CHAN0CMD_0A)		, THIS(CHAN0VOL_09)		, &CACHE_COMMANDS				, THISVAR(CMD[9])					, &cb_ch0_cmd_09 },
	{	0x07 , 0x0e , THIS(CHAN0CMD_09)		, THIS(CHAN0VAL_0A)		, THIS(CHAN0CMD_0B)		, THIS(CHAN0VOL_0A)		, &CACHE_COMMANDS				, THISVAR(CMD[10])					, &cb_ch0_cmd_0A },
	{	0x07 , 0x0f , THIS(CHAN0CMD_0A)		, THIS(CHAN0VAL_0B)		, THIS(CHAN0CMD_0C)		, THIS(CHAN0VOL_0B)		, &CACHE_COMMANDS				, THISVAR(CMD[11])					, &cb_ch0_cmd_0B },
	{	0x07 , 0x10 , THIS(CHAN0CMD_0B)		, THIS(CHAN0VAL_0C)		, THIS(CHAN0CMD_0D)		, THIS(CHAN0VOL_0C)		, &CACHE_COMMANDS				, THISVAR(CMD[12])					, &cb_ch0_cmd_0C },
	{	0x07 , 0x11 , THIS(CHAN0CMD_0C)		, THIS(CHAN0VAL_0D)		, THIS(CHAN0CMD_0E)		, THIS(CHAN0VOL_0D)		, &CACHE_COMMANDS				, THISVAR(CMD[13])					, &cb_ch0_cmd_0D },
	{	0x07 , 0x12 , THIS(CHAN0CMD_0D)		, THIS(CHAN0VAL_0E)		, THIS(CHAN0CMD_0F)		, THIS(CHAN0VOL_0E)		, &CACHE_COMMANDS				, THISVAR(CMD[14])					, &cb_ch0_cmd_0E },
	{	0x07 , 0x13 , THIS(CHAN0CMD_0E)		, THIS(CHAN0VAL_0F)		, THIS(CHAN0CMD_00)		, THIS(CHAN0VOL_0F)		, &CACHE_COMMANDS				, THISVAR(CMD[15])					, &cb_ch0_cmd_0F },
	{	0x08 , 0x04 , THIS(CHAN0VAL_0F)		, THIS(CHAN0KEY_00)		, THIS(CHAN0VAL_01)		, THIS(CHAN0CMD_00)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[0])					, &cb_ch0_val_00 },
	{	0x08 , 0x05 , THIS(CHAN0VAL_00)		, THIS(CHAN0KEY_01)		, THIS(CHAN0VAL_02)		, THIS(CHAN0CMD_01)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[1])					, &cb_ch0_val_01 },
	{	0x08 , 0x06 , THIS(CHAN0VAL_01)		, THIS(CHAN0KEY_02)		, THIS(CHAN0VAL_03)		, THIS(CHAN0CMD_02)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[2])					, &cb_ch0_val_02 },
	{	0x08 , 0x07 , THIS(CHAN0VAL_02)		, THIS(CHAN0KEY_03)		, THIS(CHAN0VAL_04)		, THIS(CHAN0CMD_03)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[3])					, &cb_ch0_val_03 },
	{	0x08 , 0x08 , THIS(CHAN0VAL_03)		, THIS(CHAN0KEY_04)		, THIS(CHAN0VAL_05)		, THIS(CHAN0CMD_04)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[4])					, &cb_ch0_val_04 },
	{	0x08 , 0x09 , THIS(CHAN0VAL_04)		, THIS(CHAN0KEY_05)		, THIS(CHAN0VAL_06)		, THIS(CHAN0CMD_05)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[5])					, &cb_ch0_val_05 },
	{	0x08 , 0x0a , THIS(CHAN0VAL_05)		, THIS(CHAN0KEY_06)		, THIS(CHAN0VAL_07)		, THIS(CHAN0CMD_06)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[6])					, &cb_ch0_val_06 },
	{	0x08 , 0x0b , THIS(CHAN0VAL_06)		, THIS(CHAN0KEY_07)		, THIS(CHAN0VAL_08)		, THIS(CHAN0CMD_07)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[7])					, &cb_ch0_val_07 },
	{	0x08 , 0x0c , THIS(CHAN0VAL_07)		, THIS(CHAN0KEY_08)		, THIS(CHAN0VAL_09)		, THIS(CHAN0CMD_08)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[8])					, &cb_ch0_val_08 },
	{	0x08 , 0x0d , THIS(CHAN0VAL_08)		, THIS(CHAN0KEY_09)		, THIS(CHAN0VAL_0A)		, THIS(CHAN0CMD_09)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[9])					, &cb_ch0_val_09 },
	{	0x08 , 0x0e , THIS(CHAN0VAL_09)		, THIS(CHAN0KEY_0A)		, THIS(CHAN0VAL_0B)		, THIS(CHAN0CMD_0A)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[10])					, &cb_ch0_val_0A },
	{	0x08 , 0x0f , THIS(CHAN0VAL_0A)		, THIS(CHAN0KEY_0B)		, THIS(CHAN0VAL_0C)		, THIS(CHAN0CMD_0B)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[11])					, &cb_ch0_val_0B },
	{	0x08 , 0x10 , THIS(CHAN0VAL_0B)		, THIS(CHAN0KEY_0C)		, THIS(CHAN0VAL_0D)		, THIS(CHAN0CMD_0C)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[12])					, &cb_ch0_val_0C },
	{	0x08 , 0x11 , THIS(CHAN0VAL_0C)		, THIS(CHAN0KEY_0D)		, THIS(CHAN0VAL_0E)		, THIS(CHAN0CMD_0D)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[13])					, &cb_ch0_val_0D },
	{	0x08 , 0x12 , THIS(CHAN0VAL_0D)		, THIS(CHAN0KEY_0E)		, THIS(CHAN0VAL_0F)		, THIS(CHAN0CMD_0E)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[14])					, &cb_ch0_val_0E },
	{	0x08 , 0x13 , THIS(CHAN0VAL_0E)		, THIS(CHAN0KEY_0F)		, THIS(CHAN0VAL_00)		, THIS(CHAN0CMD_0F)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[15])					, &cb_ch0_val_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN1_CONTROLS[CONTROL_CHANNEL1_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[1].a))
	{	0x0b , 0x04 , THIS(CHAN1KEY_0F)		, THIS(CHAN1INS_00)		, THIS(CHAN1KEY_01)		, THIS(CHAN1VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch1_key_00 },
	{	0x0b , 0x05 , THIS(CHAN1KEY_00)		, THIS(CHAN1INS_01)		, THIS(CHAN1KEY_02)		, THIS(CHAN1VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch1_key_01 },
	{	0x0b , 0x06 , THIS(CHAN1KEY_01)		, THIS(CHAN1INS_02)		, THIS(CHAN1KEY_03)		, THIS(CHAN1VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch1_key_02 },
	{	0x0b , 0x07 , THIS(CHAN1KEY_02)		, THIS(CHAN1INS_03)		, THIS(CHAN1KEY_04)		, THIS(CHAN1VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch1_key_03 },
	{	0x0b , 0x08 , THIS(CHAN1KEY_03)		, THIS(CHAN1INS_04)		, THIS(CHAN1KEY_05)		, THIS(CHAN1VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch1_key_04 },
	{	0x0b , 0x09 , THIS(CHAN1KEY_04)		, THIS(CHAN1INS_05)		, THIS(CHAN1KEY_06)		, THIS(CHAN1VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch1_key_05 },
	{	0x0b , 0x0a , THIS(CHAN1KEY_05)		, THIS(CHAN1INS_06)		, THIS(CHAN1KEY_07)		, THIS(CHAN1VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch1_key_06 },
	{	0x0b , 0x0b , THIS(CHAN1KEY_06)		, THIS(CHAN1INS_07)		, THIS(CHAN1KEY_08)		, THIS(CHAN1VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch1_key_07 },
	{	0x0b , 0x0c , THIS(CHAN1KEY_07)		, THIS(CHAN1INS_08)		, THIS(CHAN1KEY_09)		, THIS(CHAN1VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch1_key_08 },
	{	0x0b , 0x0d , THIS(CHAN1KEY_08)		, THIS(CHAN1INS_09)		, THIS(CHAN1KEY_0A)		, THIS(CHAN1VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch1_key_09 },
	{	0x0b , 0x0e , THIS(CHAN1KEY_09)		, THIS(CHAN1INS_0A)		, THIS(CHAN1KEY_0B)		, THIS(CHAN1VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch1_key_0A },
	{	0x0b , 0x0f , THIS(CHAN1KEY_0A)		, THIS(CHAN1INS_0B)		, THIS(CHAN1KEY_0C)		, THIS(CHAN1VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch1_key_0B },
	{	0x0b , 0x10 , THIS(CHAN1KEY_0B)		, THIS(CHAN1INS_0C)		, THIS(CHAN1KEY_0D)		, THIS(CHAN1VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch1_key_0C },
	{	0x0b , 0x11 , THIS(CHAN1KEY_0C)		, THIS(CHAN1INS_0D)		, THIS(CHAN1KEY_0E)		, THIS(CHAN1VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch1_key_0D },
	{	0x0b , 0x12 , THIS(CHAN1KEY_0D)		, THIS(CHAN1INS_0E)		, THIS(CHAN1KEY_0F)		, THIS(CHAN1VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch1_key_0E },
	{	0x0b , 0x13 , THIS(CHAN1KEY_0E)		, THIS(CHAN1INS_0F)		, THIS(CHAN1KEY_00)		, THIS(CHAN1VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch1_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN2_CONTROLS[CONTROL_CHANNEL2_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[2].a))
	{	0x0f , 0x04 , THIS(CHAN2KEY_0F)		, THIS(CHAN2INS_00)		, THIS(CHAN2KEY_01)		, THIS(CHAN2VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch2_key_00 },
	{	0x0f , 0x05 , THIS(CHAN2KEY_00)		, THIS(CHAN2INS_01)		, THIS(CHAN2KEY_02)		, THIS(CHAN2VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch2_key_01 },
	{	0x0f , 0x06 , THIS(CHAN2KEY_01)		, THIS(CHAN2INS_02)		, THIS(CHAN2KEY_03)		, THIS(CHAN2VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch2_key_02 },
	{	0x0f , 0x07 , THIS(CHAN2KEY_02)		, THIS(CHAN2INS_03)		, THIS(CHAN2KEY_04)		, THIS(CHAN2VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch2_key_03 },
	{	0x0f , 0x08 , THIS(CHAN2KEY_03)		, THIS(CHAN2INS_04)		, THIS(CHAN2KEY_05)		, THIS(CHAN2VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch2_key_04 },
	{	0x0f , 0x09 , THIS(CHAN2KEY_04)		, THIS(CHAN2INS_05)		, THIS(CHAN2KEY_06)		, THIS(CHAN2VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch2_key_05 },
	{	0x0f , 0x0a , THIS(CHAN2KEY_05)		, THIS(CHAN2INS_06)		, THIS(CHAN2KEY_07)		, THIS(CHAN2VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch2_key_06 },
	{	0x0f , 0x0b , THIS(CHAN2KEY_06)		, THIS(CHAN2INS_07)		, THIS(CHAN2KEY_08)		, THIS(CHAN2VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch2_key_07 },
	{	0x0f , 0x0c , THIS(CHAN2KEY_07)		, THIS(CHAN2INS_08)		, THIS(CHAN2KEY_09)		, THIS(CHAN2VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch2_key_08 },
	{	0x0f , 0x0d , THIS(CHAN2KEY_08)		, THIS(CHAN2INS_09)		, THIS(CHAN2KEY_0A)		, THIS(CHAN2VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch2_key_09 },
	{	0x0f , 0x0e , THIS(CHAN2KEY_09)		, THIS(CHAN2INS_0A)		, THIS(CHAN2KEY_0B)		, THIS(CHAN2VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch2_key_0A },
	{	0x0f , 0x0f , THIS(CHAN2KEY_0A)		, THIS(CHAN2INS_0B)		, THIS(CHAN2KEY_0C)		, THIS(CHAN2VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch2_key_0B },
	{	0x0f , 0x10 , THIS(CHAN2KEY_0B)		, THIS(CHAN2INS_0C)		, THIS(CHAN2KEY_0D)		, THIS(CHAN2VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch2_key_0C },
	{	0x0f , 0x11 , THIS(CHAN2KEY_0C)		, THIS(CHAN2INS_0D)		, THIS(CHAN2KEY_0E)		, THIS(CHAN2VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch2_key_0D },
	{	0x0f , 0x12 , THIS(CHAN2KEY_0D)		, THIS(CHAN2INS_0E)		, THIS(CHAN2KEY_0F)		, THIS(CHAN2VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch2_key_0E },
	{	0x0f , 0x13 , THIS(CHAN2KEY_0E)		, THIS(CHAN2INS_0F)		, THIS(CHAN2KEY_00)		, THIS(CHAN2VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch2_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN3_CONTROLS[CONTROL_CHANNEL3_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[3].a))
	{	0x13 , 0x04 , THIS(CHAN3KEY_0F)		, THIS(CHAN3INS_00)		, THIS(CHAN3KEY_01)		, THIS(CHAN3VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch3_key_00 },
	{	0x13 , 0x05 , THIS(CHAN3KEY_00)		, THIS(CHAN3INS_01)		, THIS(CHAN3KEY_02)		, THIS(CHAN3VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch3_key_01 },
	{	0x13 , 0x06 , THIS(CHAN3KEY_01)		, THIS(CHAN3INS_02)		, THIS(CHAN3KEY_03)		, THIS(CHAN3VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch3_key_02 },
	{	0x13 , 0x07 , THIS(CHAN3KEY_02)		, THIS(CHAN3INS_03)		, THIS(CHAN3KEY_04)		, THIS(CHAN3VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch3_key_03 },
	{	0x13 , 0x08 , THIS(CHAN3KEY_03)		, THIS(CHAN3INS_04)		, THIS(CHAN3KEY_05)		, THIS(CHAN3VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch3_key_04 },
	{	0x13 , 0x09 , THIS(CHAN3KEY_04)		, THIS(CHAN3INS_05)		, THIS(CHAN3KEY_06)		, THIS(CHAN3VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch3_key_05 },
	{	0x13 , 0x0a , THIS(CHAN3KEY_05)		, THIS(CHAN3INS_06)		, THIS(CHAN3KEY_07)		, THIS(CHAN3VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch3_key_06 },
	{	0x13 , 0x0b , THIS(CHAN3KEY_06)		, THIS(CHAN3INS_07)		, THIS(CHAN3KEY_08)		, THIS(CHAN3VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch3_key_07 },
	{	0x13 , 0x0c , THIS(CHAN3KEY_07)		, THIS(CHAN3INS_08)		, THIS(CHAN3KEY_09)		, THIS(CHAN3VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch3_key_08 },
	{	0x13 , 0x0d , THIS(CHAN3KEY_08)		, THIS(CHAN3INS_09)		, THIS(CHAN3KEY_0A)		, THIS(CHAN3VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch3_key_09 },
	{	0x13 , 0x0e , THIS(CHAN3KEY_09)		, THIS(CHAN3INS_0A)		, THIS(CHAN3KEY_0B)		, THIS(CHAN3VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch3_key_0A },
	{	0x13 , 0x0f , THIS(CHAN3KEY_0A)		, THIS(CHAN3INS_0B)		, THIS(CHAN3KEY_0C)		, THIS(CHAN3VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch3_key_0B },
	{	0x13 , 0x10 , THIS(CHAN3KEY_0B)		, THIS(CHAN3INS_0C)		, THIS(CHAN3KEY_0D)		, THIS(CHAN3VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch3_key_0C },
	{	0x13 , 0x11 , THIS(CHAN3KEY_0C)		, THIS(CHAN3INS_0D)		, THIS(CHAN3KEY_0E)		, THIS(CHAN3VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch3_key_0D },
	{	0x13 , 0x12 , THIS(CHAN3KEY_0D)		, THIS(CHAN3INS_0E)		, THIS(CHAN3KEY_0F)		, THIS(CHAN3VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch3_key_0E },
	{	0x13 , 0x13 , THIS(CHAN3KEY_0E)		, THIS(CHAN3INS_0F)		, THIS(CHAN3KEY_00)		, THIS(CHAN3VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch3_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN4_CONTROLS[CONTROL_CHANNEL4_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[4].a))
	{	0x17 , 0x04 , THIS(CHAN4KEY_0F)		, THIS(CHAN4INS_00)		, THIS(CHAN4KEY_01)		, THIS(CHAN4VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch4_key_00 },
	{	0x17 , 0x05 , THIS(CHAN4KEY_00)		, THIS(CHAN4INS_01)		, THIS(CHAN4KEY_02)		, THIS(CHAN4VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch4_key_01 },
	{	0x17 , 0x06 , THIS(CHAN4KEY_01)		, THIS(CHAN4INS_02)		, THIS(CHAN4KEY_03)		, THIS(CHAN4VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch4_key_02 },
	{	0x17 , 0x07 , THIS(CHAN4KEY_02)		, THIS(CHAN4INS_03)		, THIS(CHAN4KEY_04)		, THIS(CHAN4VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch4_key_03 },
	{	0x17 , 0x08 , THIS(CHAN4KEY_03)		, THIS(CHAN4INS_04)		, THIS(CHAN4KEY_05)		, THIS(CHAN4VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch4_key_04 },
	{	0x17 , 0x09 , THIS(CHAN4KEY_04)		, THIS(CHAN4INS_05)		, THIS(CHAN4KEY_06)		, THIS(CHAN4VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch4_key_05 },
	{	0x17 , 0x0a , THIS(CHAN4KEY_05)		, THIS(CHAN4INS_06)		, THIS(CHAN4KEY_07)		, THIS(CHAN4VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch4_key_06 },
	{	0x17 , 0x0b , THIS(CHAN4KEY_06)		, THIS(CHAN4INS_07)		, THIS(CHAN4KEY_08)		, THIS(CHAN4VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch4_key_07 },
	{	0x17 , 0x0c , THIS(CHAN4KEY_07)		, THIS(CHAN4INS_08)		, THIS(CHAN4KEY_09)		, THIS(CHAN4VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch4_key_08 },
	{	0x17 , 0x0d , THIS(CHAN4KEY_08)		, THIS(CHAN4INS_09)		, THIS(CHAN4KEY_0A)		, THIS(CHAN4VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch4_key_09 },
	{	0x17 , 0x0e , THIS(CHAN4KEY_09)		, THIS(CHAN4INS_0A)		, THIS(CHAN4KEY_0B)		, THIS(CHAN4VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch4_key_0A },
	{	0x17 , 0x0f , THIS(CHAN4KEY_0A)		, THIS(CHAN4INS_0B)		, THIS(CHAN4KEY_0C)		, THIS(CHAN4VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch4_key_0B },
	{	0x17 , 0x10 , THIS(CHAN4KEY_0B)		, THIS(CHAN4INS_0C)		, THIS(CHAN4KEY_0D)		, THIS(CHAN4VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch4_key_0C },
	{	0x17 , 0x11 , THIS(CHAN4KEY_0C)		, THIS(CHAN4INS_0D)		, THIS(CHAN4KEY_0E)		, THIS(CHAN4VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch4_key_0D },
	{	0x17 , 0x12 , THIS(CHAN4KEY_0D)		, THIS(CHAN4INS_0E)		, THIS(CHAN4KEY_0F)		, THIS(CHAN4VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch4_key_0E },
	{	0x17 , 0x13 , THIS(CHAN4KEY_0E)		, THIS(CHAN4INS_0F)		, THIS(CHAN4KEY_00)		, THIS(CHAN4VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch4_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN5_CONTROLS[CONTROL_CHANNEL5_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[5].a))
	{	0x1b , 0x04 , THIS(CHAN5KEY_0F)		, THIS(CHAN5INS_00)		, THIS(CHAN5KEY_01)		, THIS(CHAN5VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch5_key_00 },
	{	0x1b , 0x05 , THIS(CHAN5KEY_00)		, THIS(CHAN5INS_01)		, THIS(CHAN5KEY_02)		, THIS(CHAN5VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch5_key_01 },
	{	0x1b , 0x06 , THIS(CHAN5KEY_01)		, THIS(CHAN5INS_02)		, THIS(CHAN5KEY_03)		, THIS(CHAN5VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch5_key_02 },
	{	0x1b , 0x07 , THIS(CHAN5KEY_02)		, THIS(CHAN5INS_03)		, THIS(CHAN5KEY_04)		, THIS(CHAN5VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch5_key_03 },
	{	0x1b , 0x08 , THIS(CHAN5KEY_03)		, THIS(CHAN5INS_04)		, THIS(CHAN5KEY_05)		, THIS(CHAN5VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch5_key_04 },
	{	0x1b , 0x09 , THIS(CHAN5KEY_04)		, THIS(CHAN5INS_05)		, THIS(CHAN5KEY_06)		, THIS(CHAN5VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch5_key_05 },
	{	0x1b , 0x0a , THIS(CHAN5KEY_05)		, THIS(CHAN5INS_06)		, THIS(CHAN5KEY_07)		, THIS(CHAN5VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch5_key_06 },
	{	0x1b , 0x0b , THIS(CHAN5KEY_06)		, THIS(CHAN5INS_07)		, THIS(CHAN5KEY_08)		, THIS(CHAN5VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch5_key_07 },
	{	0x1b , 0x0c , THIS(CHAN5KEY_07)		, THIS(CHAN5INS_08)		, THIS(CHAN5KEY_09)		, THIS(CHAN5VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch5_key_08 },
	{	0x1b , 0x0d , THIS(CHAN5KEY_08)		, THIS(CHAN5INS_09)		, THIS(CHAN5KEY_0A)		, THIS(CHAN5VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch5_key_09 },
	{	0x1b , 0x0e , THIS(CHAN5KEY_09)		, THIS(CHAN5INS_0A)		, THIS(CHAN5KEY_0B)		, THIS(CHAN5VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch5_key_0A },
	{	0x1b , 0x0f , THIS(CHAN5KEY_0A)		, THIS(CHAN5INS_0B)		, THIS(CHAN5KEY_0C)		, THIS(CHAN5VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch5_key_0B },
	{	0x1b , 0x10 , THIS(CHAN5KEY_0B)		, THIS(CHAN5INS_0C)		, THIS(CHAN5KEY_0D)		, THIS(CHAN5VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch5_key_0C },
	{	0x1b , 0x11 , THIS(CHAN5KEY_0C)		, THIS(CHAN5INS_0D)		, THIS(CHAN5KEY_0E)		, THIS(CHAN5VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch5_key_0D },
	{	0x1b , 0x12 , THIS(CHAN5KEY_0D)		, THIS(CHAN5INS_0E)		, THIS(CHAN5KEY_0F)		, THIS(CHAN5VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch5_key_0E },
	{	0x1b , 0x13 , THIS(CHAN5KEY_0E)		, THIS(CHAN5INS_0F)		, THIS(CHAN5KEY_00)		, THIS(CHAN5VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch5_key_0F },
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN1_CONTROLS[CONTROL_CHANNEL1_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[1].a))
const Control CHAN1_CONTROLS[CONTROL_CHANNEL1_0_MAX] = { 
	//  x	 y		up						right					down					left					cache							var									callback		
	{	0x05 , 0x04 , THIS(CHAN1KEY_0F)		, THIS(CHAN1INS_00)		, THIS(CHAN1KEY_01)		, THIS(CHAN1VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch1_key_00 }, 
	{	0x05 , 0x05 , THIS(CHAN1KEY_00)		, THIS(CHAN1INS_01)		, THIS(CHAN1KEY_02)		, THIS(CHAN1VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])		 			, &cb_ch1_key_01 }, 
	{	0x05 , 0x06 , THIS(CHAN1KEY_01)		, THIS(CHAN1INS_02)		, THIS(CHAN1KEY_03)		, THIS(CHAN1VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])		 			, &cb_ch1_key_02 }, 
	{	0x05 , 0x07 , THIS(CHAN1KEY_02)		, THIS(CHAN1INS_03)		, THIS(CHAN1KEY_04)		, THIS(CHAN1VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])		 			, &cb_ch1_key_03 }, 
	{	0x05 , 0x08 , THIS(CHAN1KEY_03)		, THIS(CHAN1INS_04)		, THIS(CHAN1KEY_05)		, THIS(CHAN1VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])		 			, &cb_ch1_key_04 }, 
	{	0x05 , 0x09 , THIS(CHAN1KEY_04)		, THIS(CHAN1INS_05)		, THIS(CHAN1KEY_06)		, THIS(CHAN1VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])		 			, &cb_ch1_key_05 }, 
	{	0x05 , 0x0a , THIS(CHAN1KEY_05)		, THIS(CHAN1INS_06)		, THIS(CHAN1KEY_07)		, THIS(CHAN1VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])		 			, &cb_ch1_key_06 }, 
	{	0x05 , 0x0b , THIS(CHAN1KEY_06)		, THIS(CHAN1INS_07)		, THIS(CHAN1KEY_08)		, THIS(CHAN1VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])		 			, &cb_ch1_key_07 }, 
	{	0x05 , 0x0c , THIS(CHAN1KEY_07)		, THIS(CHAN1INS_08)		, THIS(CHAN1KEY_09)		, THIS(CHAN1VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])		 			, &cb_ch1_key_08 }, 
	{	0x05 , 0x0d , THIS(CHAN1KEY_08)		, THIS(CHAN1INS_09)		, THIS(CHAN1KEY_0A)		, THIS(CHAN1VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])		 			, &cb_ch1_key_09 }, 
	{	0x05 , 0x0e , THIS(CHAN1KEY_09)		, THIS(CHAN1INS_0A)		, THIS(CHAN1KEY_0B)		, THIS(CHAN1VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])	 				, &cb_ch1_key_0A }, 
	{	0x05 , 0x0f , THIS(CHAN1KEY_0A)		, THIS(CHAN1INS_0B)		, THIS(CHAN1KEY_0C)		, THIS(CHAN1VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])	 				, &cb_ch1_key_0B }, 
	{	0x05 , 0x10 , THIS(CHAN1KEY_0B)		, THIS(CHAN1INS_0C)		, THIS(CHAN1KEY_0D)		, THIS(CHAN1VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])	 				, &cb_ch1_key_0C }, 
	{	0x05 , 0x11 , THIS(CHAN1KEY_0C)		, THIS(CHAN1INS_0D)		, THIS(CHAN1KEY_0E)		, THIS(CHAN1VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])	 				, &cb_ch1_key_0D }, 
	{	0x05 , 0x12 , THIS(CHAN1KEY_0D)		, THIS(CHAN1INS_0E)		, THIS(CHAN1KEY_0F)		, THIS(CHAN1VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])	 				, &cb_ch1_key_0E }, 
	{	0x05 , 0x13 , THIS(CHAN1KEY_0E)		, THIS(CHAN1INS_0F)		, THIS(CHAN1KEY_00)		, THIS(CHAN1VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])	 				, &cb_ch1_key_0F }, 
	{	0x08 , 0x04 , THIS(CHAN1INS_0F)		, THIS(CHAN1VOL_00)		, THIS(CHAN1INS_01)		, THIS(CHAN1KEY_00)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[0])					, &cb_ch1_ins_00 }, 
	{	0x08 , 0x05 , THIS(CHAN1INS_00)		, THIS(CHAN1VOL_01)		, THIS(CHAN1INS_02)		, THIS(CHAN1KEY_01)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[1])					, &cb_ch1_ins_01 }, 
	{	0x08 , 0x06 , THIS(CHAN1INS_01)		, THIS(CHAN1VOL_02)		, THIS(CHAN1INS_03)		, THIS(CHAN1KEY_02)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[2])					, &cb_ch1_ins_02 }, 
	{	0x08 , 0x07 , THIS(CHAN1INS_02)		, THIS(CHAN1VOL_03)		, THIS(CHAN1INS_04)		, THIS(CHAN1KEY_03)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[3])					, &cb_ch1_ins_03 }, 
	{	0x08 , 0x08 , THIS(CHAN1INS_03)		, THIS(CHAN1VOL_04)		, THIS(CHAN1INS_05)		, THIS(CHAN1KEY_04)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[4])					, &cb_ch1_ins_04 }, 
	{	0x08 , 0x09 , THIS(CHAN1INS_04)		, THIS(CHAN1VOL_05)		, THIS(CHAN1INS_06)		, THIS(CHAN1KEY_05)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[5])					, &cb_ch1_ins_05 }, 
	{	0x08 , 0x0a , THIS(CHAN1INS_05)		, THIS(CHAN1VOL_06)		, THIS(CHAN1INS_07)		, THIS(CHAN1KEY_06)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[6])					, &cb_ch1_ins_06 }, 
	{	0x08 , 0x0b , THIS(CHAN1INS_06)		, THIS(CHAN1VOL_07)		, THIS(CHAN1INS_08)		, THIS(CHAN1KEY_07)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[7])					, &cb_ch1_ins_07 }, 
	{	0x08 , 0x0c , THIS(CHAN1INS_07)		, THIS(CHAN1VOL_08)		, THIS(CHAN1INS_09)		, THIS(CHAN1KEY_08)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[8])					, &cb_ch1_ins_08 }, 
	{	0x08 , 0x0d , THIS(CHAN1INS_08)		, THIS(CHAN1VOL_09)		, THIS(CHAN1INS_0A)		, THIS(CHAN1KEY_09)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[9])					, &cb_ch1_ins_09 }, 
	{	0x08 , 0x0e , THIS(CHAN1INS_09)		, THIS(CHAN1VOL_0A)		, THIS(CHAN1INS_0B)		, THIS(CHAN1KEY_0A)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[10])					, &cb_ch1_ins_0A }, 
	{	0x08 , 0x0f , THIS(CHAN1INS_0A)		, THIS(CHAN1VOL_0B)		, THIS(CHAN1INS_0C)		, THIS(CHAN1KEY_0B)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[11])					, &cb_ch1_ins_0B }, 
	{	0x08 , 0x10 , THIS(CHAN1INS_0B)		, THIS(CHAN1VOL_0C)		, THIS(CHAN1INS_0D)		, THIS(CHAN1KEY_0C)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[12])					, &cb_ch1_ins_0C }, 
	{	0x08 , 0x11 , THIS(CHAN1INS_0C)		, THIS(CHAN1VOL_0D)		, THIS(CHAN1INS_0E)		, THIS(CHAN1KEY_0D)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[13])					, &cb_ch1_ins_0D }, 
	{	0x08 , 0x12 , THIS(CHAN1INS_0D)		, THIS(CHAN1VOL_0E)		, THIS(CHAN1INS_0F)		, THIS(CHAN1KEY_0E)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[14])					, &cb_ch1_ins_0E }, 
	{	0x08 , 0x13 , THIS(CHAN1INS_0E)		, THIS(CHAN1VOL_0F)		, THIS(CHAN1INS_00)		, THIS(CHAN1KEY_0F)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[15])					, &cb_ch1_ins_0F }, 
	{	0x0a , 0x04 , THIS(CHAN1VOL_0F)		, THIS(CHAN1CMD_00)		, THIS(CHAN1VOL_01)		, THIS(CHAN1INS_00)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[0])					, &cb_ch1_vol_00 }, 
	{	0x0a , 0x05 , THIS(CHAN1VOL_00)		, THIS(CHAN1CMD_01)		, THIS(CHAN1VOL_02)		, THIS(CHAN1INS_01)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[1])					, &cb_ch1_vol_01 }, 
	{	0x0a , 0x06 , THIS(CHAN1VOL_01)		, THIS(CHAN1CMD_02)		, THIS(CHAN1VOL_03)		, THIS(CHAN1INS_02)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[2])					, &cb_ch1_vol_02 }, 
	{	0x0a , 0x07 , THIS(CHAN1VOL_02)		, THIS(CHAN1CMD_03)		, THIS(CHAN1VOL_04)		, THIS(CHAN1INS_03)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[3])					, &cb_ch1_vol_03 }, 
	{	0x0a , 0x08 , THIS(CHAN1VOL_03)		, THIS(CHAN1CMD_04)		, THIS(CHAN1VOL_05)		, THIS(CHAN1INS_04)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[4])					, &cb_ch1_vol_04 }, 
	{	0x0a , 0x09 , THIS(CHAN1VOL_04)		, THIS(CHAN1CMD_05)		, THIS(CHAN1VOL_06)		, THIS(CHAN1INS_05)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[5])					, &cb_ch1_vol_05 }, 
	{	0x0a , 0x0a , THIS(CHAN1VOL_05)		, THIS(CHAN1CMD_06)		, THIS(CHAN1VOL_07)		, THIS(CHAN1INS_06)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[6])					, &cb_ch1_vol_06 }, 
	{	0x0a , 0x0b , THIS(CHAN1VOL_06)		, THIS(CHAN1CMD_07)		, THIS(CHAN1VOL_08)		, THIS(CHAN1INS_07)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[7])					, &cb_ch1_vol_07 }, 
	{	0x0a , 0x0c , THIS(CHAN1VOL_07)		, THIS(CHAN1CMD_08)		, THIS(CHAN1VOL_09)		, THIS(CHAN1INS_08)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[8])					, &cb_ch1_vol_08 }, 
	{	0x0a , 0x0d , THIS(CHAN1VOL_08)		, THIS(CHAN1CMD_09)		, THIS(CHAN1VOL_0A)		, THIS(CHAN1INS_09)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[9])					, &cb_ch1_vol_09 }, 
	{	0x0a , 0x0e , THIS(CHAN1VOL_09)		, THIS(CHAN1CMD_0A)		, THIS(CHAN1VOL_0B)		, THIS(CHAN1INS_0A)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[10])					, &cb_ch1_vol_0A }, 
	{	0x0a , 0x0f , THIS(CHAN1VOL_0A)		, THIS(CHAN1CMD_0B)		, THIS(CHAN1VOL_0C)		, THIS(CHAN1INS_0B)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[11])					, &cb_ch1_vol_0B }, 
	{	0x0a , 0x10 , THIS(CHAN1VOL_0B)		, THIS(CHAN1CMD_0C)		, THIS(CHAN1VOL_0D)		, THIS(CHAN1INS_0C)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[12])					, &cb_ch1_vol_0C }, 
	{	0x0a , 0x11 , THIS(CHAN1VOL_0C)		, THIS(CHAN1CMD_0D)		, THIS(CHAN1VOL_0E)		, THIS(CHAN1INS_0D)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[13])					, &cb_ch1_vol_0D }, 
	{	0x0a , 0x12 , THIS(CHAN1VOL_0D)		, THIS(CHAN1CMD_0E)		, THIS(CHAN1VOL_0F)		, THIS(CHAN1INS_0E)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[14])					, &cb_ch1_vol_0E }, 
	{	0x0a , 0x13 , THIS(CHAN1VOL_0E)		, THIS(CHAN1CMD_0F)		, THIS(CHAN1VOL_00)		, THIS(CHAN1INS_0F)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[15])					, &cb_ch1_vol_0F }, 
	{	0x0b , 0x04 , THIS(CHAN1CMD_0F)		, THIS(CHAN1VAL_00)		, THIS(CHAN1CMD_01)		, THIS(CHAN1VOL_00)		, &CACHE_COMMANDS				, THISVAR(CMD[0])					, &cb_ch1_cmd_00 }, 
	{	0x0b , 0x05 , THIS(CHAN1CMD_00)		, THIS(CHAN1VAL_01)		, THIS(CHAN1CMD_02)		, THIS(CHAN1VOL_01)		, &CACHE_COMMANDS				, THISVAR(CMD[1])					, &cb_ch1_cmd_01 }, 
	{	0x0b , 0x06 , THIS(CHAN1CMD_01)		, THIS(CHAN1VAL_02)		, THIS(CHAN1CMD_03)		, THIS(CHAN1VOL_02)		, &CACHE_COMMANDS				, THISVAR(CMD[2])					, &cb_ch1_cmd_02 }, 
	{	0x0b , 0x07 , THIS(CHAN1CMD_02)		, THIS(CHAN1VAL_03)		, THIS(CHAN1CMD_04)		, THIS(CHAN1VOL_03)		, &CACHE_COMMANDS				, THISVAR(CMD[3])					, &cb_ch1_cmd_03 }, 
	{	0x0b , 0x08 , THIS(CHAN1CMD_03)		, THIS(CHAN1VAL_04)		, THIS(CHAN1CMD_05)		, THIS(CHAN1VOL_04)		, &CACHE_COMMANDS				, THISVAR(CMD[4])					, &cb_ch1_cmd_04 }, 
	{	0x0b , 0x09 , THIS(CHAN1CMD_04)		, THIS(CHAN1VAL_05)		, THIS(CHAN1CMD_06)		, THIS(CHAN1VOL_05)		, &CACHE_COMMANDS				, THISVAR(CMD[5])					, &cb_ch1_cmd_05 }, 
	{	0x0b , 0x0a , THIS(CHAN1CMD_05)		, THIS(CHAN1VAL_06)		, THIS(CHAN1CMD_07)		, THIS(CHAN1VOL_06)		, &CACHE_COMMANDS				, THISVAR(CMD[6])					, &cb_ch1_cmd_06 }, 
	{	0x0b , 0x0b , THIS(CHAN1CMD_06)		, THIS(CHAN1VAL_07)		, THIS(CHAN1CMD_08)		, THIS(CHAN1VOL_07)		, &CACHE_COMMANDS				, THISVAR(CMD[7])					, &cb_ch1_cmd_07 }, 
	{	0x0b , 0x0c , THIS(CHAN1CMD_07)		, THIS(CHAN1VAL_08)		, THIS(CHAN1CMD_09)		, THIS(CHAN1VOL_08)		, &CACHE_COMMANDS				, THISVAR(CMD[8])					, &cb_ch1_cmd_08 }, 
	{	0x0b , 0x0d , THIS(CHAN1CMD_08)		, THIS(CHAN1VAL_09)		, THIS(CHAN1CMD_0A)		, THIS(CHAN1VOL_09)		, &CACHE_COMMANDS				, THISVAR(CMD[9])					, &cb_ch1_cmd_09 }, 
	{	0x0b , 0x0e , THIS(CHAN1CMD_09)		, THIS(CHAN1VAL_0A)		, THIS(CHAN1CMD_0B)		, THIS(CHAN1VOL_0A)		, &CACHE_COMMANDS				, THISVAR(CMD[10])					, &cb_ch1_cmd_0A }, 
	{	0x0b , 0x0f , THIS(CHAN1CMD_0A)		, THIS(CHAN1VAL_0B)		, THIS(CHAN1CMD_0C)		, THIS(CHAN1VOL_0B)		, &CACHE_COMMANDS				, THISVAR(CMD[11])					, &cb_ch1_cmd_0B }, 
	{	0x0b , 0x10 , THIS(CHAN1CMD_0B)		, THIS(CHAN1VAL_0C)		, THIS(CHAN1CMD_0D)		, THIS(CHAN1VOL_0C)		, &CACHE_COMMANDS				, THISVAR(CMD[12])					, &cb_ch1_cmd_0C }, 
	{	0x0b , 0x11 , THIS(CHAN1CMD_0C)		, THIS(CHAN1VAL_0D)		, THIS(CHAN1CMD_0E)		, THIS(CHAN1VOL_0D)		, &CACHE_COMMANDS				, THISVAR(CMD[13])					, &cb_ch1_cmd_0D }, 
	{	0x0b , 0x12 , THIS(CHAN1CMD_0D)		, THIS(CHAN1VAL_0E)		, THIS(CHAN1CMD_0F)		, THIS(CHAN1VOL_0E)		, &CACHE_COMMANDS				, THISVAR(CMD[14])					, &cb_ch1_cmd_0E }, 
	{	0x0b , 0x13 , THIS(CHAN1CMD_0E)		, THIS(CHAN1VAL_0F)		, THIS(CHAN1CMD_00)		, THIS(CHAN1VOL_0F)		, &CACHE_COMMANDS				, THISVAR(CMD[15])					, &cb_ch1_cmd_0F }, 
	{	0x0c , 0x04 , THIS(CHAN1VAL_0F)		, THIS(CHAN1KEY_00)		, THIS(CHAN1VAL_01)		, THIS(CHAN1CMD_00)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[0])					, &cb_ch1_val_00 }, 
	{	0x0c , 0x05 , THIS(CHAN1VAL_00)		, THIS(CHAN1KEY_01)		, THIS(CHAN1VAL_02)		, THIS(CHAN1CMD_01)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[1])					, &cb_ch1_val_01 }, 
	{	0x0c , 0x06 , THIS(CHAN1VAL_01)		, THIS(CHAN1KEY_02)		, THIS(CHAN1VAL_03)		, THIS(CHAN1CMD_02)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[2])					, &cb_ch1_val_02 }, 
	{	0x0c , 0x07 , THIS(CHAN1VAL_02)		, THIS(CHAN1KEY_03)		, THIS(CHAN1VAL_04)		, THIS(CHAN1CMD_03)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[3])					, &cb_ch1_val_03 }, 
	{	0x0c , 0x08 , THIS(CHAN1VAL_03)		, THIS(CHAN1KEY_04)		, THIS(CHAN1VAL_05)		, THIS(CHAN1CMD_04)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[4])					, &cb_ch1_val_04 }, 
	{	0x0c , 0x09 , THIS(CHAN1VAL_04)		, THIS(CHAN1KEY_05)		, THIS(CHAN1VAL_06)		, THIS(CHAN1CMD_05)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[5])					, &cb_ch1_val_05 }, 
	{	0x0c , 0x0a , THIS(CHAN1VAL_05)		, THIS(CHAN1KEY_06)		, THIS(CHAN1VAL_07)		, THIS(CHAN1CMD_06)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[6])					, &cb_ch1_val_06 }, 
	{	0x0c , 0x0b , THIS(CHAN1VAL_06)		, THIS(CHAN1KEY_07)		, THIS(CHAN1VAL_08)		, THIS(CHAN1CMD_07)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[7])					, &cb_ch1_val_07 }, 
	{	0x0c , 0x0c , THIS(CHAN1VAL_07)		, THIS(CHAN1KEY_08)		, THIS(CHAN1VAL_09)		, THIS(CHAN1CMD_08)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[8])					, &cb_ch1_val_08 }, 
	{	0x0c , 0x0d , THIS(CHAN1VAL_08)		, THIS(CHAN1KEY_09)		, THIS(CHAN1VAL_0A)		, THIS(CHAN1CMD_09)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[9])					, &cb_ch1_val_09 }, 
	{	0x0c , 0x0e , THIS(CHAN1VAL_09)		, THIS(CHAN1KEY_0A)		, THIS(CHAN1VAL_0B)		, THIS(CHAN1CMD_0A)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[10])					, &cb_ch1_val_0A }, 
	{	0x0c , 0x0f , THIS(CHAN1VAL_0A)		, THIS(CHAN1KEY_0B)		, THIS(CHAN1VAL_0C)		, THIS(CHAN1CMD_0B)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[11])					, &cb_ch1_val_0B }, 
	{	0x0c , 0x10 , THIS(CHAN1VAL_0B)		, THIS(CHAN1KEY_0C)		, THIS(CHAN1VAL_0D)		, THIS(CHAN1CMD_0C)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[12])					, &cb_ch1_val_0C }, 
	{	0x0c , 0x11 , THIS(CHAN1VAL_0C)		, THIS(CHAN1KEY_0D)		, THIS(CHAN1VAL_0E)		, THIS(CHAN1CMD_0D)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[13])					, &cb_ch1_val_0D }, 
	{	0x0c , 0x12 , THIS(CHAN1VAL_0D)		, THIS(CHAN1KEY_0E)		, THIS(CHAN1VAL_0F)		, THIS(CHAN1CMD_0E)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[14])					, &cb_ch1_val_0E }, 
	{	0x0c , 0x13 , THIS(CHAN1VAL_0E)		, THIS(CHAN1KEY_0F)		, THIS(CHAN1VAL_00)		, THIS(CHAN1CMD_0F)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[15])					, &cb_ch1_val_0F }, 
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN0_CONTROLS[CONTROL_CHANNEL0_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[0].a))
	{	0x01 , 0x04 , THIS(CHAN0KEY_0F)		, THIS(CHAN0INS_00)		, THIS(CHAN0KEY_01)		, THIS(CHAN0VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch0_key_00 },
	{	0x01 , 0x05 , THIS(CHAN0KEY_00)		, THIS(CHAN0INS_01)		, THIS(CHAN0KEY_02)		, THIS(CHAN0VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch0_key_01 },
	{	0x01 , 0x06 , THIS(CHAN0KEY_01)		, THIS(CHAN0INS_02)		, THIS(CHAN0KEY_03)		, THIS(CHAN0VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch0_key_02 },
	{	0x01 , 0x07 , THIS(CHAN0KEY_02)		, THIS(CHAN0INS_03)		, THIS(CHAN0KEY_04)		, THIS(CHAN0VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch0_key_03 },
	{	0x01 , 0x08 , THIS(CHAN0KEY_03)		, THIS(CHAN0INS_04)		, THIS(CHAN0KEY_05)		, THIS(CHAN0VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch0_key_04 },
	{	0x01 , 0x09 , THIS(CHAN0KEY_04)		, THIS(CHAN0INS_05)		, THIS(CHAN0KEY_06)		, THIS(CHAN0VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch0_key_05 },
	{	0x01 , 0x0a , THIS(CHAN0KEY_05)		, THIS(CHAN0INS_06)		, THIS(CHAN0KEY_07)		, THIS(CHAN0VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch0_key_06 },
	{	0x01 , 0x0b , THIS(CHAN0KEY_06)		, THIS(CHAN0INS_07)		, THIS(CHAN0KEY_08)		, THIS(CHAN0VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch0_key_07 },
	{	0x01 , 0x0c , THIS(CHAN0KEY_07)		, THIS(CHAN0INS_08)		, THIS(CHAN0KEY_09)		, THIS(CHAN0VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch0_key_08 },
	{	0x01 , 0x0d , THIS(CHAN0KEY_08)		, THIS(CHAN0INS_09)		, THIS(CHAN0KEY_0A)		, THIS(CHAN0VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch0_key_09 },
	{	0x01 , 0x0e , THIS(CHAN0KEY_09)		, THIS(CHAN0INS_0A)		, THIS(CHAN0KEY_0B)		, THIS(CHAN0VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch0_key_0A },
	{	0x01 , 0x0f , THIS(CHAN0KEY_0A)		, THIS(CHAN0INS_0B)		, THIS(CHAN0KEY_0C)		, THIS(CHAN0VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch0_key_0B },
	{	0x01 , 0x10 , THIS(CHAN0KEY_0B)		, THIS(CHAN0INS_0C)		, THIS(CHAN0KEY_0D)		, THIS(CHAN0VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch0_key_0C },
	{	0x01 , 0x11 , THIS(CHAN0KEY_0C)		, THIS(CHAN0INS_0D)		, THIS(CHAN0KEY_0E)		, THIS(CHAN0VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch0_key_0D },
	{	0x01 , 0x12 , THIS(CHAN0KEY_0D)		, THIS(CHAN0INS_0E)		, THIS(CHAN0KEY_0F)		, THIS(CHAN0VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch0_key_0E },
	{	0x01 , 0x13 , THIS(CHAN0KEY_0E)		, THIS(CHAN0INS_0F)		, THIS(CHAN0KEY_00)		, THIS(CHAN0VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch0_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN2_CONTROLS[CONTROL_CHANNEL2_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[2].a))
	{	0x0f , 0x04 , THIS(CHAN2KEY_0F)		, THIS(CHAN2INS_00)		, THIS(CHAN2KEY_01)		, THIS(CHAN2VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch2_key_00 },
	{	0x0f , 0x05 , THIS(CHAN2KEY_00)		, THIS(CHAN2INS_01)		, THIS(CHAN2KEY_02)		, THIS(CHAN2VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch2_key_01 },
	{	0x0f , 0x06 , THIS(CHAN2KEY_01)		, THIS(CHAN2INS_02)		, THIS(CHAN2KEY_03)		, THIS(CHAN2VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch2_key_02 },
	{	0x0f , 0x07 , THIS(CHAN2KEY_02)		, THIS(CHAN2INS_03)		, THIS(CHAN2KEY_04)		, THIS(CHAN2VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch2_key_03 },
	{	0x0f , 0x08 , THIS(CHAN2KEY_03)		, THIS(CHAN2INS_04)		, THIS(CHAN2KEY_05)		, THIS(CHAN2VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch2_key_04 },
	{	0x0f , 0x09 , THIS(CHAN2KEY_04)		, THIS(CHAN2INS_05)		, THIS(CHAN2KEY_06)		, THIS(CHAN2VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch2_key_05 },
	{	0x0f , 0x0a , THIS(CHAN2KEY_05)		, THIS(CHAN2INS_06)		, THIS(CHAN2KEY_07)		, THIS(CHAN2VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch2_key_06 },
	{	0x0f , 0x0b , THIS(CHAN2KEY_06)		, THIS(CHAN2INS_07)		, THIS(CHAN2KEY_08)		, THIS(CHAN2VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch2_key_07 },
	{	0x0f , 0x0c , THIS(CHAN2KEY_07)		, THIS(CHAN2INS_08)		, THIS(CHAN2KEY_09)		, THIS(CHAN2VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch2_key_08 },
	{	0x0f , 0x0d , THIS(CHAN2KEY_08)		, THIS(CHAN2INS_09)		, THIS(CHAN2KEY_0A)		, THIS(CHAN2VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch2_key_09 },
	{	0x0f , 0x0e , THIS(CHAN2KEY_09)		, THIS(CHAN2INS_0A)		, THIS(CHAN2KEY_0B)		, THIS(CHAN2VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch2_key_0A },
	{	0x0f , 0x0f , THIS(CHAN2KEY_0A)		, THIS(CHAN2INS_0B)		, THIS(CHAN2KEY_0C)		, THIS(CHAN2VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch2_key_0B },
	{	0x0f , 0x10 , THIS(CHAN2KEY_0B)		, THIS(CHAN2INS_0C)		, THIS(CHAN2KEY_0D)		, THIS(CHAN2VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch2_key_0C },
	{	0x0f , 0x11 , THIS(CHAN2KEY_0C)		, THIS(CHAN2INS_0D)		, THIS(CHAN2KEY_0E)		, THIS(CHAN2VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch2_key_0D },
	{	0x0f , 0x12 , THIS(CHAN2KEY_0D)		, THIS(CHAN2INS_0E)		, THIS(CHAN2KEY_0F)		, THIS(CHAN2VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch2_key_0E },
	{	0x0f , 0x13 , THIS(CHAN2KEY_0E)		, THIS(CHAN2INS_0F)		, THIS(CHAN2KEY_00)		, THIS(CHAN2VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch2_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN3_CONTROLS[CONTROL_CHANNEL3_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[3].a))
	{	0x13 , 0x04 , THIS(CHAN3KEY_0F)		, THIS(CHAN3INS_00)		, THIS(CHAN3KEY_01)		, THIS(CHAN3VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch3_key_00 },
	{	0x13 , 0x05 , THIS(CHAN3KEY_00)		, THIS(CHAN3INS_01)		, THIS(CHAN3KEY_02)		, THIS(CHAN3VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch3_key_01 },
	{	0x13 , 0x06 , THIS(CHAN3KEY_01)		, THIS(CHAN3INS_02)		, THIS(CHAN3KEY_03)		, THIS(CHAN3VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch3_key_02 },
	{	0x13 , 0x07 , THIS(CHAN3KEY_02)		, THIS(CHAN3INS_03)		, THIS(CHAN3KEY_04)		, THIS(CHAN3VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch3_key_03 },
	{	0x13 , 0x08 , THIS(CHAN3KEY_03)		, THIS(CHAN3INS_04)		, THIS(CHAN3KEY_05)		, THIS(CHAN3VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch3_key_04 },
	{	0x13 , 0x09 , THIS(CHAN3KEY_04)		, THIS(CHAN3INS_05)		, THIS(CHAN3KEY_06)		, THIS(CHAN3VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch3_key_05 },
	{	0x13 , 0x0a , THIS(CHAN3KEY_05)		, THIS(CHAN3INS_06)		, THIS(CHAN3KEY_07)		, THIS(CHAN3VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch3_key_06 },
	{	0x13 , 0x0b , THIS(CHAN3KEY_06)		, THIS(CHAN3INS_07)		, THIS(CHAN3KEY_08)		, THIS(CHAN3VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch3_key_07 },
	{	0x13 , 0x0c , THIS(CHAN3KEY_07)		, THIS(CHAN3INS_08)		, THIS(CHAN3KEY_09)		, THIS(CHAN3VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch3_key_08 },
	{	0x13 , 0x0d , THIS(CHAN3KEY_08)		, THIS(CHAN3INS_09)		, THIS(CHAN3KEY_0A)		, THIS(CHAN3VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch3_key_09 },
	{	0x13 , 0x0e , THIS(CHAN3KEY_09)		, THIS(CHAN3INS_0A)		, THIS(CHAN3KEY_0B)		, THIS(CHAN3VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch3_key_0A },
	{	0x13 , 0x0f , THIS(CHAN3KEY_0A)		, THIS(CHAN3INS_0B)		, THIS(CHAN3KEY_0C)		, THIS(CHAN3VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch3_key_0B },
	{	0x13 , 0x10 , THIS(CHAN3KEY_0B)		, THIS(CHAN3INS_0C)		, THIS(CHAN3KEY_0D)		, THIS(CHAN3VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch3_key_0C },
	{	0x13 , 0x11 , THIS(CHAN3KEY_0C)		, THIS(CHAN3INS_0D)		, THIS(CHAN3KEY_0E)		, THIS(CHAN3VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch3_key_0D },
	{	0x13 , 0x12 , THIS(CHAN3KEY_0D)		, THIS(CHAN3INS_0E)		, THIS(CHAN3KEY_0F)		, THIS(CHAN3VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch3_key_0E },
	{	0x13 , 0x13 , THIS(CHAN3KEY_0E)		, THIS(CHAN3INS_0F)		, THIS(CHAN3KEY_00)		, THIS(CHAN3VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch3_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN4_CONTROLS[CONTROL_CHANNEL4_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[4].a))
	{	0x17 , 0x04 , THIS(CHAN4KEY_0F)		, THIS(CHAN4INS_00)		, THIS(CHAN4KEY_01)		, THIS(CHAN4VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch4_key_00 },
	{	0x17 , 0x05 , THIS(CHAN4KEY_00)		, THIS(CHAN4INS_01)		, THIS(CHAN4KEY_02)		, THIS(CHAN4VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch4_key_01 },
	{	0x17 , 0x06 , THIS(CHAN4KEY_01)		, THIS(CHAN4INS_02)		, THIS(CHAN4KEY_03)		, THIS(CHAN4VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch4_key_02 },
	{	0x17 , 0x07 , THIS(CHAN4KEY_02)		, THIS(CHAN4INS_03)		, THIS(CHAN4KEY_04)		, THIS(CHAN4VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch4_key_03 },
	{	0x17 , 0x08 , THIS(CHAN4KEY_03)		, THIS(CHAN4INS_04)		, THIS(CHAN4KEY_05)		, THIS(CHAN4VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch4_key_04 },
	{	0x17 , 0x09 , THIS(CHAN4KEY_04)		, THIS(CHAN4INS_05)		, THIS(CHAN4KEY_06)		, THIS(CHAN4VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch4_key_05 },
	{	0x17 , 0x0a , THIS(CHAN4KEY_05)		, THIS(CHAN4INS_06)		, THIS(CHAN4KEY_07)		, THIS(CHAN4VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch4_key_06 },
	{	0x17 , 0x0b , THIS(CHAN4KEY_06)		, THIS(CHAN4INS_07)		, THIS(CHAN4KEY_08)		, THIS(CHAN4VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch4_key_07 },
	{	0x17 , 0x0c , THIS(CHAN4KEY_07)		, THIS(CHAN4INS_08)		, THIS(CHAN4KEY_09)		, THIS(CHAN4VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch4_key_08 },
	{	0x17 , 0x0d , THIS(CHAN4KEY_08)		, THIS(CHAN4INS_09)		, THIS(CHAN4KEY_0A)		, THIS(CHAN4VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch4_key_09 },
	{	0x17 , 0x0e , THIS(CHAN4KEY_09)		, THIS(CHAN4INS_0A)		, THIS(CHAN4KEY_0B)		, THIS(CHAN4VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch4_key_0A },
	{	0x17 , 0x0f , THIS(CHAN4KEY_0A)		, THIS(CHAN4INS_0B)		, THIS(CHAN4KEY_0C)		, THIS(CHAN4VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch4_key_0B },
	{	0x17 , 0x10 , THIS(CHAN4KEY_0B)		, THIS(CHAN4INS_0C)		, THIS(CHAN4KEY_0D)		, THIS(CHAN4VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch4_key_0C },
	{	0x17 , 0x11 , THIS(CHAN4KEY_0C)		, THIS(CHAN4INS_0D)		, THIS(CHAN4KEY_0E)		, THIS(CHAN4VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch4_key_0D },
	{	0x17 , 0x12 , THIS(CHAN4KEY_0D)		, THIS(CHAN4INS_0E)		, THIS(CHAN4KEY_0F)		, THIS(CHAN4VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch4_key_0E },
	{	0x17 , 0x13 , THIS(CHAN4KEY_0E)		, THIS(CHAN4INS_0F)		, THIS(CHAN4KEY_00)		, THIS(CHAN4VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch4_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN5_CONTROLS[CONTROL_CHANNEL5_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[5].a))
	{	0x1b , 0x04 , THIS(CHAN5KEY_0F)		, THIS(CHAN5INS_00)		, THIS(CHAN5KEY_01)		, THIS(CHAN5VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch5_key_00 },
	{	0x1b , 0x05 , THIS(CHAN5KEY_00)		, THIS(CHAN5INS_01)		, THIS(CHAN5KEY_02)		, THIS(CHAN5VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch5_key_01 },
	{	0x1b , 0x06 , THIS(CHAN5KEY_01)		, THIS(CHAN5INS_02)		, THIS(CHAN5KEY_03)		, THIS(CHAN5VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch5_key_02 },
	{	0x1b , 0x07 , THIS(CHAN5KEY_02)		, THIS(CHAN5INS_03)		, THIS(CHAN5KEY_04)		, THIS(CHAN5VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch5_key_03 },
	{	0x1b , 0x08 , THIS(CHAN5KEY_03)		, THIS(CHAN5INS_04)		, THIS(CHAN5KEY_05)		, THIS(CHAN5VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch5_key_04 },
	{	0x1b , 0x09 , THIS(CHAN5KEY_04)		, THIS(CHAN5INS_05)		, THIS(CHAN5KEY_06)		, THIS(CHAN5VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch5_key_05 },
	{	0x1b , 0x0a , THIS(CHAN5KEY_05)		, THIS(CHAN5INS_06)		, THIS(CHAN5KEY_07)		, THIS(CHAN5VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch5_key_06 },
	{	0x1b , 0x0b , THIS(CHAN5KEY_06)		, THIS(CHAN5INS_07)		, THIS(CHAN5KEY_08)		, THIS(CHAN5VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch5_key_07 },
	{	0x1b , 0x0c , THIS(CHAN5KEY_07)		, THIS(CHAN5INS_08)		, THIS(CHAN5KEY_09)		, THIS(CHAN5VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch5_key_08 },
	{	0x1b , 0x0d , THIS(CHAN5KEY_08)		, THIS(CHAN5INS_09)		, THIS(CHAN5KEY_0A)		, THIS(CHAN5VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch5_key_09 },
	{	0x1b , 0x0e , THIS(CHAN5KEY_09)		, THIS(CHAN5INS_0A)		, THIS(CHAN5KEY_0B)		, THIS(CHAN5VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch5_key_0A },
	{	0x1b , 0x0f , THIS(CHAN5KEY_0A)		, THIS(CHAN5INS_0B)		, THIS(CHAN5KEY_0C)		, THIS(CHAN5VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch5_key_0B },
	{	0x1b , 0x10 , THIS(CHAN5KEY_0B)		, THIS(CHAN5INS_0C)		, THIS(CHAN5KEY_0D)		, THIS(CHAN5VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch5_key_0C },
	{	0x1b , 0x11 , THIS(CHAN5KEY_0C)		, THIS(CHAN5INS_0D)		, THIS(CHAN5KEY_0E)		, THIS(CHAN5VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch5_key_0D },
	{	0x1b , 0x12 , THIS(CHAN5KEY_0D)		, THIS(CHAN5INS_0E)		, THIS(CHAN5KEY_0F)		, THIS(CHAN5VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch5_key_0E },
	{	0x1b , 0x13 , THIS(CHAN5KEY_0E)		, THIS(CHAN5INS_0F)		, THIS(CHAN5KEY_00)		, THIS(CHAN5VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch5_key_0F }, 
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN2_CONTROLS[CONTROL_CHANNEL2_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[2].a))
const Control CHAN2_CONTROLS[CONTROL_CHANNEL2_0_MAX] = { 
	//  x	 y		up						right					down					left					cache							var									callback		
	{	0x09 , 0x04 , THIS(CHAN2KEY_0F)		, THIS(CHAN2INS_00)		, THIS(CHAN2KEY_01)		, THIS(CHAN2VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch2_key_00 },
	{	0x09 , 0x05 , THIS(CHAN2KEY_00)		, THIS(CHAN2INS_01)		, THIS(CHAN2KEY_02)		, THIS(CHAN2VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch2_key_01 },
	{	0x09 , 0x06 , THIS(CHAN2KEY_01)		, THIS(CHAN2INS_02)		, THIS(CHAN2KEY_03)		, THIS(CHAN2VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch2_key_02 },
	{	0x09 , 0x07 , THIS(CHAN2KEY_02)		, THIS(CHAN2INS_03)		, THIS(CHAN2KEY_04)		, THIS(CHAN2VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch2_key_03 },
	{	0x09 , 0x08 , THIS(CHAN2KEY_03)		, THIS(CHAN2INS_04)		, THIS(CHAN2KEY_05)		, THIS(CHAN2VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch2_key_04 },
	{	0x09 , 0x09 , THIS(CHAN2KEY_04)		, THIS(CHAN2INS_05)		, THIS(CHAN2KEY_06)		, THIS(CHAN2VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch2_key_05 },
	{	0x09 , 0x0a , THIS(CHAN2KEY_05)		, THIS(CHAN2INS_06)		, THIS(CHAN2KEY_07)		, THIS(CHAN2VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch2_key_06 },
	{	0x09 , 0x0b , THIS(CHAN2KEY_06)		, THIS(CHAN2INS_07)		, THIS(CHAN2KEY_08)		, THIS(CHAN2VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch2_key_07 },
	{	0x09 , 0x0c , THIS(CHAN2KEY_07)		, THIS(CHAN2INS_08)		, THIS(CHAN2KEY_09)		, THIS(CHAN2VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch2_key_08 },
	{	0x09 , 0x0d , THIS(CHAN2KEY_08)		, THIS(CHAN2INS_09)		, THIS(CHAN2KEY_0A)		, THIS(CHAN2VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch2_key_09 },
	{	0x09 , 0x0e , THIS(CHAN2KEY_09)		, THIS(CHAN2INS_0A)		, THIS(CHAN2KEY_0B)		, THIS(CHAN2VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch2_key_0A },
	{	0x09 , 0x0f , THIS(CHAN2KEY_0A)		, THIS(CHAN2INS_0B)		, THIS(CHAN2KEY_0C)		, THIS(CHAN2VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch2_key_0B },
	{	0x09 , 0x10 , THIS(CHAN2KEY_0B)		, THIS(CHAN2INS_0C)		, THIS(CHAN2KEY_0D)		, THIS(CHAN2VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch2_key_0C },
	{	0x09 , 0x11 , THIS(CHAN2KEY_0C)		, THIS(CHAN2INS_0D)		, THIS(CHAN2KEY_0E)		, THIS(CHAN2VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch2_key_0D },
	{	0x09 , 0x12 , THIS(CHAN2KEY_0D)		, THIS(CHAN2INS_0E)		, THIS(CHAN2KEY_0F)		, THIS(CHAN2VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch2_key_0E },
	{	0x09 , 0x13 , THIS(CHAN2KEY_0E)		, THIS(CHAN2INS_0F)		, THIS(CHAN2KEY_00)		, THIS(CHAN2VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch2_key_0F },
	{	0x0c , 0x04 , THIS(CHAN2INS_0F)		, THIS(CHAN2VOL_00)		, THIS(CHAN2INS_01)		, THIS(CHAN2KEY_00)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[0])					, &cb_ch2_ins_00 }, 
	{	0x0c , 0x05 , THIS(CHAN2INS_00)		, THIS(CHAN2VOL_01)		, THIS(CHAN2INS_02)		, THIS(CHAN2KEY_01)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[1])					, &cb_ch2_ins_01 }, 
	{	0x0c , 0x06 , THIS(CHAN2INS_01)		, THIS(CHAN2VOL_02)		, THIS(CHAN2INS_03)		, THIS(CHAN2KEY_02)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[2])					, &cb_ch2_ins_02 }, 
	{	0x0c , 0x07 , THIS(CHAN2INS_02)		, THIS(CHAN2VOL_03)		, THIS(CHAN2INS_04)		, THIS(CHAN2KEY_03)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[3])					, &cb_ch2_ins_03 }, 
	{	0x0c , 0x08 , THIS(CHAN2INS_03)		, THIS(CHAN2VOL_04)		, THIS(CHAN2INS_05)		, THIS(CHAN2KEY_04)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[4])					, &cb_ch2_ins_04 }, 
	{	0x0c , 0x09 , THIS(CHAN2INS_04)		, THIS(CHAN2VOL_05)		, THIS(CHAN2INS_06)		, THIS(CHAN2KEY_05)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[5])					, &cb_ch2_ins_05 }, 
	{	0x0c , 0x0a , THIS(CHAN2INS_05)		, THIS(CHAN2VOL_06)		, THIS(CHAN2INS_07)		, THIS(CHAN2KEY_06)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[6])					, &cb_ch2_ins_06 }, 
	{	0x0c , 0x0b , THIS(CHAN2INS_06)		, THIS(CHAN2VOL_07)		, THIS(CHAN2INS_08)		, THIS(CHAN2KEY_07)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[7])					, &cb_ch2_ins_07 }, 
	{	0x0c , 0x0c , THIS(CHAN2INS_07)		, THIS(CHAN2VOL_08)		, THIS(CHAN2INS_09)		, THIS(CHAN2KEY_08)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[8])					, &cb_ch2_ins_08 }, 
	{	0x0c , 0x0d , THIS(CHAN2INS_08)		, THIS(CHAN2VOL_09)		, THIS(CHAN2INS_0A)		, THIS(CHAN2KEY_09)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[9])					, &cb_ch2_ins_09 }, 
	{	0x0c , 0x0e , THIS(CHAN2INS_09)		, THIS(CHAN2VOL_0A)		, THIS(CHAN2INS_0B)		, THIS(CHAN2KEY_0A)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[10])					, &cb_ch2_ins_0A }, 
	{	0x0c , 0x0f , THIS(CHAN2INS_0A)		, THIS(CHAN2VOL_0B)		, THIS(CHAN2INS_0C)		, THIS(CHAN2KEY_0B)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[11])					, &cb_ch2_ins_0B }, 
	{	0x0c , 0x10 , THIS(CHAN2INS_0B)		, THIS(CHAN2VOL_0C)		, THIS(CHAN2INS_0D)		, THIS(CHAN2KEY_0C)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[12])					, &cb_ch2_ins_0C }, 
	{	0x0c , 0x11 , THIS(CHAN2INS_0C)		, THIS(CHAN2VOL_0D)		, THIS(CHAN2INS_0E)		, THIS(CHAN2KEY_0D)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[13])					, &cb_ch2_ins_0D }, 
	{	0x0c , 0x12 , THIS(CHAN2INS_0D)		, THIS(CHAN2VOL_0E)		, THIS(CHAN2INS_0F)		, THIS(CHAN2KEY_0E)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[14])					, &cb_ch2_ins_0E }, 
	{	0x0c , 0x13 , THIS(CHAN2INS_0E)		, THIS(CHAN2VOL_0F)		, THIS(CHAN2INS_00)		, THIS(CHAN2KEY_0F)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[15])					, &cb_ch2_ins_0F },
	{	0x0e , 0x04 , THIS(CHAN2VOL_0F)		, THIS(CHAN2CMD_00)		, THIS(CHAN2VOL_01)		, THIS(CHAN2INS_00)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[0])					, &cb_ch2_vol_00 }, 
	{	0x0e , 0x05 , THIS(CHAN2VOL_00)		, THIS(CHAN2CMD_01)		, THIS(CHAN2VOL_02)		, THIS(CHAN2INS_01)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[1])					, &cb_ch2_vol_01 }, 
	{	0x0e , 0x06 , THIS(CHAN2VOL_01)		, THIS(CHAN2CMD_02)		, THIS(CHAN2VOL_03)		, THIS(CHAN2INS_02)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[2])					, &cb_ch2_vol_02 }, 
	{	0x0e , 0x07 , THIS(CHAN2VOL_02)		, THIS(CHAN2CMD_03)		, THIS(CHAN2VOL_04)		, THIS(CHAN2INS_03)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[3])					, &cb_ch2_vol_03 }, 
	{	0x0e , 0x08 , THIS(CHAN2VOL_03)		, THIS(CHAN2CMD_04)		, THIS(CHAN2VOL_05)		, THIS(CHAN2INS_04)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[4])					, &cb_ch2_vol_04 }, 
	{	0x0e , 0x09 , THIS(CHAN2VOL_04)		, THIS(CHAN2CMD_05)		, THIS(CHAN2VOL_06)		, THIS(CHAN2INS_05)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[5])					, &cb_ch2_vol_05 }, 
	{	0x0e , 0x0a , THIS(CHAN2VOL_05)		, THIS(CHAN2CMD_06)		, THIS(CHAN2VOL_07)		, THIS(CHAN2INS_06)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[6])					, &cb_ch2_vol_06 }, 
	{	0x0e , 0x0b , THIS(CHAN2VOL_06)		, THIS(CHAN2CMD_07)		, THIS(CHAN2VOL_08)		, THIS(CHAN2INS_07)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[7])					, &cb_ch2_vol_07 }, 
	{	0x0e , 0x0c , THIS(CHAN2VOL_07)		, THIS(CHAN2CMD_08)		, THIS(CHAN2VOL_09)		, THIS(CHAN2INS_08)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[8])					, &cb_ch2_vol_08 }, 
	{	0x0e , 0x0d , THIS(CHAN2VOL_08)		, THIS(CHAN2CMD_09)		, THIS(CHAN2VOL_0A)		, THIS(CHAN2INS_09)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[9])					, &cb_ch2_vol_09 }, 
	{	0x0e , 0x0e , THIS(CHAN2VOL_09)		, THIS(CHAN2CMD_0A)		, THIS(CHAN2VOL_0B)		, THIS(CHAN2INS_0A)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[10])					, &cb_ch2_vol_0A }, 
	{	0x0e , 0x0f , THIS(CHAN2VOL_0A)		, THIS(CHAN2CMD_0B)		, THIS(CHAN2VOL_0C)		, THIS(CHAN2INS_0B)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[11])					, &cb_ch2_vol_0B }, 
	{	0x0e , 0x10 , THIS(CHAN2VOL_0B)		, THIS(CHAN2CMD_0C)		, THIS(CHAN2VOL_0D)		, THIS(CHAN2INS_0C)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[12])					, &cb_ch2_vol_0C }, 
	{	0x0e , 0x11 , THIS(CHAN2VOL_0C)		, THIS(CHAN2CMD_0D)		, THIS(CHAN2VOL_0E)		, THIS(CHAN2INS_0D)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[13])					, &cb_ch2_vol_0D }, 
	{	0x0e , 0x12 , THIS(CHAN2VOL_0D)		, THIS(CHAN2CMD_0E)		, THIS(CHAN2VOL_0F)		, THIS(CHAN2INS_0E)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[14])					, &cb_ch2_vol_0E }, 
	{	0x0e , 0x13 , THIS(CHAN2VOL_0E)		, THIS(CHAN2CMD_0F)		, THIS(CHAN2VOL_00)		, THIS(CHAN2INS_0F)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[15])					, &cb_ch2_vol_0F }, 
	{	0x0f , 0x04 , THIS(CHAN2CMD_0F)		, THIS(CHAN2VAL_00)		, THIS(CHAN2CMD_01)		, THIS(CHAN2VOL_00)		, &CACHE_COMMANDS				, THISVAR(CMD[0])					, &cb_ch2_cmd_00 }, 
	{	0x0f , 0x05 , THIS(CHAN2CMD_00)		, THIS(CHAN2VAL_01)		, THIS(CHAN2CMD_02)		, THIS(CHAN2VOL_01)		, &CACHE_COMMANDS				, THISVAR(CMD[1])					, &cb_ch2_cmd_01 }, 
	{	0x0f , 0x06 , THIS(CHAN2CMD_01)		, THIS(CHAN2VAL_02)		, THIS(CHAN2CMD_03)		, THIS(CHAN2VOL_02)		, &CACHE_COMMANDS				, THISVAR(CMD[2])					, &cb_ch2_cmd_02 }, 
	{	0x0f , 0x07 , THIS(CHAN2CMD_02)		, THIS(CHAN2VAL_03)		, THIS(CHAN2CMD_04)		, THIS(CHAN2VOL_03)		, &CACHE_COMMANDS				, THISVAR(CMD[3])					, &cb_ch2_cmd_03 }, 
	{	0x0f , 0x08 , THIS(CHAN2CMD_03)		, THIS(CHAN2VAL_04)		, THIS(CHAN2CMD_05)		, THIS(CHAN2VOL_04)		, &CACHE_COMMANDS				, THISVAR(CMD[4])					, &cb_ch2_cmd_04 }, 
	{	0x0f , 0x09 , THIS(CHAN2CMD_04)		, THIS(CHAN2VAL_05)		, THIS(CHAN2CMD_06)		, THIS(CHAN2VOL_05)		, &CACHE_COMMANDS				, THISVAR(CMD[5])					, &cb_ch2_cmd_05 }, 
	{	0x0f , 0x0a , THIS(CHAN2CMD_05)		, THIS(CHAN2VAL_06)		, THIS(CHAN2CMD_07)		, THIS(CHAN2VOL_06)		, &CACHE_COMMANDS				, THISVAR(CMD[6])					, &cb_ch2_cmd_06 }, 
	{	0x0f , 0x0b , THIS(CHAN2CMD_06)		, THIS(CHAN2VAL_07)		, THIS(CHAN2CMD_08)		, THIS(CHAN2VOL_07)		, &CACHE_COMMANDS				, THISVAR(CMD[7])					, &cb_ch2_cmd_07 }, 
	{	0x0f , 0x0c , THIS(CHAN2CMD_07)		, THIS(CHAN2VAL_08)		, THIS(CHAN2CMD_09)		, THIS(CHAN2VOL_08)		, &CACHE_COMMANDS				, THISVAR(CMD[8])					, &cb_ch2_cmd_08 }, 
	{	0x0f , 0x0d , THIS(CHAN2CMD_08)		, THIS(CHAN2VAL_09)		, THIS(CHAN2CMD_0A)		, THIS(CHAN2VOL_09)		, &CACHE_COMMANDS				, THISVAR(CMD[9])					, &cb_ch2_cmd_09 }, 
	{	0x0f , 0x0e , THIS(CHAN2CMD_09)		, THIS(CHAN2VAL_0A)		, THIS(CHAN2CMD_0B)		, THIS(CHAN2VOL_0A)		, &CACHE_COMMANDS				, THISVAR(CMD[10])					, &cb_ch2_cmd_0A }, 
	{	0x0f , 0x0f , THIS(CHAN2CMD_0A)		, THIS(CHAN2VAL_0B)		, THIS(CHAN2CMD_0C)		, THIS(CHAN2VOL_0B)		, &CACHE_COMMANDS				, THISVAR(CMD[11])					, &cb_ch2_cmd_0B }, 
	{	0x0f , 0x10 , THIS(CHAN2CMD_0B)		, THIS(CHAN2VAL_0C)		, THIS(CHAN2CMD_0D)		, THIS(CHAN2VOL_0C)		, &CACHE_COMMANDS				, THISVAR(CMD[12])					, &cb_ch2_cmd_0C }, 
	{	0x0f , 0x11 , THIS(CHAN2CMD_0C)		, THIS(CHAN2VAL_0D)		, THIS(CHAN2CMD_0E)		, THIS(CHAN2VOL_0D)		, &CACHE_COMMANDS				, THISVAR(CMD[13])					, &cb_ch2_cmd_0D }, 
	{	0x0f , 0x12 , THIS(CHAN2CMD_0D)		, THIS(CHAN2VAL_0E)		, THIS(CHAN2CMD_0F)		, THIS(CHAN2VOL_0E)		, &CACHE_COMMANDS				, THISVAR(CMD[14])					, &cb_ch2_cmd_0E }, 
	{	0x0f , 0x13 , THIS(CHAN2CMD_0E)		, THIS(CHAN2VAL_0F)		, THIS(CHAN2CMD_00)		, THIS(CHAN2VOL_0F)		, &CACHE_COMMANDS				, THISVAR(CMD[15])					, &cb_ch2_cmd_0F }, 
	{	0x10 , 0x04 , THIS(CHAN2VAL_0F)		, THIS(CHAN2KEY_00)		, THIS(CHAN2VAL_01)		, THIS(CHAN2CMD_00)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[0])					, &cb_ch2_val_00 }, 
	{	0x10 , 0x05 , THIS(CHAN2VAL_00)		, THIS(CHAN2KEY_01)		, THIS(CHAN2VAL_02)		, THIS(CHAN2CMD_01)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[1])					, &cb_ch2_val_01 }, 
	{	0x10 , 0x06 , THIS(CHAN2VAL_01)		, THIS(CHAN2KEY_02)		, THIS(CHAN2VAL_03)		, THIS(CHAN2CMD_02)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[2])					, &cb_ch2_val_02 }, 
	{	0x10 , 0x07 , THIS(CHAN2VAL_02)		, THIS(CHAN2KEY_03)		, THIS(CHAN2VAL_04)		, THIS(CHAN2CMD_03)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[3])					, &cb_ch2_val_03 }, 
	{	0x10 , 0x08 , THIS(CHAN2VAL_03)		, THIS(CHAN2KEY_04)		, THIS(CHAN2VAL_05)		, THIS(CHAN2CMD_04)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[4])					, &cb_ch2_val_04 }, 
	{	0x10 , 0x09 , THIS(CHAN2VAL_04)		, THIS(CHAN2KEY_05)		, THIS(CHAN2VAL_06)		, THIS(CHAN2CMD_05)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[5])					, &cb_ch2_val_05 }, 
	{	0x10 , 0x0a , THIS(CHAN2VAL_05)		, THIS(CHAN2KEY_06)		, THIS(CHAN2VAL_07)		, THIS(CHAN2CMD_06)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[6])					, &cb_ch2_val_06 }, 
	{	0x10 , 0x0b , THIS(CHAN2VAL_06)		, THIS(CHAN2KEY_07)		, THIS(CHAN2VAL_08)		, THIS(CHAN2CMD_07)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[7])					, &cb_ch2_val_07 }, 
	{	0x10 , 0x0c , THIS(CHAN2VAL_07)		, THIS(CHAN2KEY_08)		, THIS(CHAN2VAL_09)		, THIS(CHAN2CMD_08)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[8])					, &cb_ch2_val_08 }, 
	{	0x10 , 0x0d , THIS(CHAN2VAL_08)		, THIS(CHAN2KEY_09)		, THIS(CHAN2VAL_0A)		, THIS(CHAN2CMD_09)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[9])					, &cb_ch2_val_09 }, 
	{	0x10 , 0x0e , THIS(CHAN2VAL_09)		, THIS(CHAN2KEY_0A)		, THIS(CHAN2VAL_0B)		, THIS(CHAN2CMD_0A)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[10])					, &cb_ch2_val_0A }, 
	{	0x10 , 0x0f , THIS(CHAN2VAL_0A)		, THIS(CHAN2KEY_0B)		, THIS(CHAN2VAL_0C)		, THIS(CHAN2CMD_0B)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[11])					, &cb_ch2_val_0B }, 
	{	0x10 , 0x10 , THIS(CHAN2VAL_0B)		, THIS(CHAN2KEY_0C)		, THIS(CHAN2VAL_0D)		, THIS(CHAN2CMD_0C)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[12])					, &cb_ch2_val_0C }, 
	{	0x10 , 0x11 , THIS(CHAN2VAL_0C)		, THIS(CHAN2KEY_0D)		, THIS(CHAN2VAL_0E)		, THIS(CHAN2CMD_0D)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[13])					, &cb_ch2_val_0D }, 
	{	0x10 , 0x12 , THIS(CHAN2VAL_0D)		, THIS(CHAN2KEY_0E)		, THIS(CHAN2VAL_0F)		, THIS(CHAN2CMD_0E)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[14])					, &cb_ch2_val_0E }, 
	{	0x10 , 0x13 , THIS(CHAN2VAL_0E)		, THIS(CHAN2KEY_0F)		, THIS(CHAN2VAL_00)		, THIS(CHAN2CMD_0F)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[15])					, &cb_ch2_val_0F }, 
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN0_CONTROLS[CONTROL_CHANNEL0_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[0].a))
	{	0x01 , 0x04 , THIS(CHAN0KEY_0F)		, THIS(CHAN0INS_00)		, THIS(CHAN0KEY_01)		, THIS(CHAN0VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch0_key_00 },
	{	0x01 , 0x05 , THIS(CHAN0KEY_00)		, THIS(CHAN0INS_01)		, THIS(CHAN0KEY_02)		, THIS(CHAN0VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch0_key_01 },
	{	0x01 , 0x06 , THIS(CHAN0KEY_01)		, THIS(CHAN0INS_02)		, THIS(CHAN0KEY_03)		, THIS(CHAN0VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch0_key_02 },
	{	0x01 , 0x07 , THIS(CHAN0KEY_02)		, THIS(CHAN0INS_03)		, THIS(CHAN0KEY_04)		, THIS(CHAN0VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch0_key_03 },
	{	0x01 , 0x08 , THIS(CHAN0KEY_03)		, THIS(CHAN0INS_04)		, THIS(CHAN0KEY_05)		, THIS(CHAN0VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch0_key_04 },
	{	0x01 , 0x09 , THIS(CHAN0KEY_04)		, THIS(CHAN0INS_05)		, THIS(CHAN0KEY_06)		, THIS(CHAN0VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch0_key_05 },
	{	0x01 , 0x0a , THIS(CHAN0KEY_05)		, THIS(CHAN0INS_06)		, THIS(CHAN0KEY_07)		, THIS(CHAN0VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch0_key_06 },
	{	0x01 , 0x0b , THIS(CHAN0KEY_06)		, THIS(CHAN0INS_07)		, THIS(CHAN0KEY_08)		, THIS(CHAN0VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch0_key_07 },
	{	0x01 , 0x0c , THIS(CHAN0KEY_07)		, THIS(CHAN0INS_08)		, THIS(CHAN0KEY_09)		, THIS(CHAN0VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch0_key_08 },
	{	0x01 , 0x0d , THIS(CHAN0KEY_08)		, THIS(CHAN0INS_09)		, THIS(CHAN0KEY_0A)		, THIS(CHAN0VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch0_key_09 },
	{	0x01 , 0x0e , THIS(CHAN0KEY_09)		, THIS(CHAN0INS_0A)		, THIS(CHAN0KEY_0B)		, THIS(CHAN0VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch0_key_0A },
	{	0x01 , 0x0f , THIS(CHAN0KEY_0A)		, THIS(CHAN0INS_0B)		, THIS(CHAN0KEY_0C)		, THIS(CHAN0VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch0_key_0B },
	{	0x01 , 0x10 , THIS(CHAN0KEY_0B)		, THIS(CHAN0INS_0C)		, THIS(CHAN0KEY_0D)		, THIS(CHAN0VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch0_key_0C },
	{	0x01 , 0x11 , THIS(CHAN0KEY_0C)		, THIS(CHAN0INS_0D)		, THIS(CHAN0KEY_0E)		, THIS(CHAN0VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch0_key_0D },
	{	0x01 , 0x12 , THIS(CHAN0KEY_0D)		, THIS(CHAN0INS_0E)		, THIS(CHAN0KEY_0F)		, THIS(CHAN0VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch0_key_0E },
	{	0x01 , 0x13 , THIS(CHAN0KEY_0E)		, THIS(CHAN0INS_0F)		, THIS(CHAN0KEY_00)		, THIS(CHAN0VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch0_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN1_CONTROLS[CONTROL_CHANNEL1_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[1].a))
	{	0x05 , 0x04 , THIS(CHAN1KEY_0F)		, THIS(CHAN1INS_00)		, THIS(CHAN1KEY_01)		, THIS(CHAN1VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch1_key_00 },
	{	0x05 , 0x05 , THIS(CHAN1KEY_00)		, THIS(CHAN1INS_01)		, THIS(CHAN1KEY_02)		, THIS(CHAN1VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch1_key_01 },
	{	0x05 , 0x06 , THIS(CHAN1KEY_01)		, THIS(CHAN1INS_02)		, THIS(CHAN1KEY_03)		, THIS(CHAN1VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch1_key_02 },
	{	0x05 , 0x07 , THIS(CHAN1KEY_02)		, THIS(CHAN1INS_03)		, THIS(CHAN1KEY_04)		, THIS(CHAN1VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch1_key_03 },
	{	0x05 , 0x08 , THIS(CHAN1KEY_03)		, THIS(CHAN1INS_04)		, THIS(CHAN1KEY_05)		, THIS(CHAN1VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch1_key_04 },
	{	0x05 , 0x09 , THIS(CHAN1KEY_04)		, THIS(CHAN1INS_05)		, THIS(CHAN1KEY_06)		, THIS(CHAN1VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch1_key_05 },
	{	0x05 , 0x0a , THIS(CHAN1KEY_05)		, THIS(CHAN1INS_06)		, THIS(CHAN1KEY_07)		, THIS(CHAN1VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch1_key_06 },
	{	0x05 , 0x0b , THIS(CHAN1KEY_06)		, THIS(CHAN1INS_07)		, THIS(CHAN1KEY_08)		, THIS(CHAN1VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch1_key_07 },
	{	0x05 , 0x0c , THIS(CHAN1KEY_07)		, THIS(CHAN1INS_08)		, THIS(CHAN1KEY_09)		, THIS(CHAN1VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch1_key_08 },
	{	0x05 , 0x0d , THIS(CHAN1KEY_08)		, THIS(CHAN1INS_09)		, THIS(CHAN1KEY_0A)		, THIS(CHAN1VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch1_key_09 },
	{	0x05 , 0x0e , THIS(CHAN1KEY_09)		, THIS(CHAN1INS_0A)		, THIS(CHAN1KEY_0B)		, THIS(CHAN1VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch1_key_0A },
	{	0x05 , 0x0f , THIS(CHAN1KEY_0A)		, THIS(CHAN1INS_0B)		, THIS(CHAN1KEY_0C)		, THIS(CHAN1VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch1_key_0B },
	{	0x05 , 0x10 , THIS(CHAN1KEY_0B)		, THIS(CHAN1INS_0C)		, THIS(CHAN1KEY_0D)		, THIS(CHAN1VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch1_key_0C },
	{	0x05 , 0x11 , THIS(CHAN1KEY_0C)		, THIS(CHAN1INS_0D)		, THIS(CHAN1KEY_0E)		, THIS(CHAN1VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch1_key_0D },
	{	0x05 , 0x12 , THIS(CHAN1KEY_0D)		, THIS(CHAN1INS_0E)		, THIS(CHAN1KEY_0F)		, THIS(CHAN1VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch1_key_0E },
	{	0x05 , 0x13 , THIS(CHAN1KEY_0E)		, THIS(CHAN1INS_0F)		, THIS(CHAN1KEY_00)		, THIS(CHAN1VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch1_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN3_CONTROLS[CONTROL_CHANNEL3_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[3].a))
	{	0x13 , 0x04 , THIS(CHAN3KEY_0F)		, THIS(CHAN3INS_00)		, THIS(CHAN3KEY_01)		, THIS(CHAN3VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch3_key_00 },
	{	0x13 , 0x05 , THIS(CHAN3KEY_00)		, THIS(CHAN3INS_01)		, THIS(CHAN3KEY_02)		, THIS(CHAN3VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch3_key_01 },
	{	0x13 , 0x06 , THIS(CHAN3KEY_01)		, THIS(CHAN3INS_02)		, THIS(CHAN3KEY_03)		, THIS(CHAN3VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch3_key_02 },
	{	0x13 , 0x07 , THIS(CHAN3KEY_02)		, THIS(CHAN3INS_03)		, THIS(CHAN3KEY_04)		, THIS(CHAN3VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch3_key_03 },
	{	0x13 , 0x08 , THIS(CHAN3KEY_03)		, THIS(CHAN3INS_04)		, THIS(CHAN3KEY_05)		, THIS(CHAN3VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch3_key_04 },
	{	0x13 , 0x09 , THIS(CHAN3KEY_04)		, THIS(CHAN3INS_05)		, THIS(CHAN3KEY_06)		, THIS(CHAN3VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch3_key_05 },
	{	0x13 , 0x0a , THIS(CHAN3KEY_05)		, THIS(CHAN3INS_06)		, THIS(CHAN3KEY_07)		, THIS(CHAN3VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch3_key_06 },
	{	0x13 , 0x0b , THIS(CHAN3KEY_06)		, THIS(CHAN3INS_07)		, THIS(CHAN3KEY_08)		, THIS(CHAN3VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch3_key_07 },
	{	0x13 , 0x0c , THIS(CHAN3KEY_07)		, THIS(CHAN3INS_08)		, THIS(CHAN3KEY_09)		, THIS(CHAN3VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch3_key_08 },
	{	0x13 , 0x0d , THIS(CHAN3KEY_08)		, THIS(CHAN3INS_09)		, THIS(CHAN3KEY_0A)		, THIS(CHAN3VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch3_key_09 },
	{	0x13 , 0x0e , THIS(CHAN3KEY_09)		, THIS(CHAN3INS_0A)		, THIS(CHAN3KEY_0B)		, THIS(CHAN3VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch3_key_0A },
	{	0x13 , 0x0f , THIS(CHAN3KEY_0A)		, THIS(CHAN3INS_0B)		, THIS(CHAN3KEY_0C)		, THIS(CHAN3VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch3_key_0B },
	{	0x13 , 0x10 , THIS(CHAN3KEY_0B)		, THIS(CHAN3INS_0C)		, THIS(CHAN3KEY_0D)		, THIS(CHAN3VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch3_key_0C },
	{	0x13 , 0x11 , THIS(CHAN3KEY_0C)		, THIS(CHAN3INS_0D)		, THIS(CHAN3KEY_0E)		, THIS(CHAN3VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch3_key_0D },
	{	0x13 , 0x12 , THIS(CHAN3KEY_0D)		, THIS(CHAN3INS_0E)		, THIS(CHAN3KEY_0F)		, THIS(CHAN3VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch3_key_0E },
	{	0x13 , 0x13 , THIS(CHAN3KEY_0E)		, THIS(CHAN3INS_0F)		, THIS(CHAN3KEY_00)		, THIS(CHAN3VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch3_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN4_CONTROLS[CONTROL_CHANNEL4_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[4].a))
	{	0x17 , 0x04 , THIS(CHAN4KEY_0F)		, THIS(CHAN4INS_00)		, THIS(CHAN4KEY_01)		, THIS(CHAN4VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch4_key_00 },
	{	0x17 , 0x05 , THIS(CHAN4KEY_00)		, THIS(CHAN4INS_01)		, THIS(CHAN4KEY_02)		, THIS(CHAN4VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch4_key_01 },
	{	0x17 , 0x06 , THIS(CHAN4KEY_01)		, THIS(CHAN4INS_02)		, THIS(CHAN4KEY_03)		, THIS(CHAN4VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch4_key_02 },
	{	0x17 , 0x07 , THIS(CHAN4KEY_02)		, THIS(CHAN4INS_03)		, THIS(CHAN4KEY_04)		, THIS(CHAN4VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch4_key_03 },
	{	0x17 , 0x08 , THIS(CHAN4KEY_03)		, THIS(CHAN4INS_04)		, THIS(CHAN4KEY_05)		, THIS(CHAN4VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch4_key_04 },
	{	0x17 , 0x09 , THIS(CHAN4KEY_04)		, THIS(CHAN4INS_05)		, THIS(CHAN4KEY_06)		, THIS(CHAN4VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch4_key_05 },
	{	0x17 , 0x0a , THIS(CHAN4KEY_05)		, THIS(CHAN4INS_06)		, THIS(CHAN4KEY_07)		, THIS(CHAN4VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch4_key_06 },
	{	0x17 , 0x0b , THIS(CHAN4KEY_06)		, THIS(CHAN4INS_07)		, THIS(CHAN4KEY_08)		, THIS(CHAN4VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch4_key_07 },
	{	0x17 , 0x0c , THIS(CHAN4KEY_07)		, THIS(CHAN4INS_08)		, THIS(CHAN4KEY_09)		, THIS(CHAN4VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch4_key_08 },
	{	0x17 , 0x0d , THIS(CHAN4KEY_08)		, THIS(CHAN4INS_09)		, THIS(CHAN4KEY_0A)		, THIS(CHAN4VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch4_key_09 },
	{	0x17 , 0x0e , THIS(CHAN4KEY_09)		, THIS(CHAN4INS_0A)		, THIS(CHAN4KEY_0B)		, THIS(CHAN4VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch4_key_0A },
	{	0x17 , 0x0f , THIS(CHAN4KEY_0A)		, THIS(CHAN4INS_0B)		, THIS(CHAN4KEY_0C)		, THIS(CHAN4VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch4_key_0B },
	{	0x17 , 0x10 , THIS(CHAN4KEY_0B)		, THIS(CHAN4INS_0C)		, THIS(CHAN4KEY_0D)		, THIS(CHAN4VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch4_key_0C },
	{	0x17 , 0x11 , THIS(CHAN4KEY_0C)		, THIS(CHAN4INS_0D)		, THIS(CHAN4KEY_0E)		, THIS(CHAN4VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch4_key_0D },
	{	0x17 , 0x12 , THIS(CHAN4KEY_0D)		, THIS(CHAN4INS_0E)		, THIS(CHAN4KEY_0F)		, THIS(CHAN4VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch4_key_0E },
	{	0x17 , 0x13 , THIS(CHAN4KEY_0E)		, THIS(CHAN4INS_0F)		, THIS(CHAN4KEY_00)		, THIS(CHAN4VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch4_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN5_CONTROLS[CONTROL_CHANNEL5_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[5].a))
	{	0x1b , 0x04 , THIS(CHAN5KEY_0F)		, THIS(CHAN5INS_00)		, THIS(CHAN5KEY_01)		, THIS(CHAN5VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch5_key_00 },
	{	0x1b , 0x05 , THIS(CHAN5KEY_00)		, THIS(CHAN5INS_01)		, THIS(CHAN5KEY_02)		, THIS(CHAN5VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch5_key_01 },
	{	0x1b , 0x06 , THIS(CHAN5KEY_01)		, THIS(CHAN5INS_02)		, THIS(CHAN5KEY_03)		, THIS(CHAN5VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch5_key_02 },
	{	0x1b , 0x07 , THIS(CHAN5KEY_02)		, THIS(CHAN5INS_03)		, THIS(CHAN5KEY_04)		, THIS(CHAN5VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch5_key_03 },
	{	0x1b , 0x08 , THIS(CHAN5KEY_03)		, THIS(CHAN5INS_04)		, THIS(CHAN5KEY_05)		, THIS(CHAN5VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch5_key_04 },
	{	0x1b , 0x09 , THIS(CHAN5KEY_04)		, THIS(CHAN5INS_05)		, THIS(CHAN5KEY_06)		, THIS(CHAN5VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch5_key_05 },
	{	0x1b , 0x0a , THIS(CHAN5KEY_05)		, THIS(CHAN5INS_06)		, THIS(CHAN5KEY_07)		, THIS(CHAN5VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch5_key_06 },
	{	0x1b , 0x0b , THIS(CHAN5KEY_06)		, THIS(CHAN5INS_07)		, THIS(CHAN5KEY_08)		, THIS(CHAN5VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch5_key_07 },
	{	0x1b , 0x0c , THIS(CHAN5KEY_07)		, THIS(CHAN5INS_08)		, THIS(CHAN5KEY_09)		, THIS(CHAN5VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch5_key_08 },
	{	0x1b , 0x0d , THIS(CHAN5KEY_08)		, THIS(CHAN5INS_09)		, THIS(CHAN5KEY_0A)		, THIS(CHAN5VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch5_key_09 },
	{	0x1b , 0x0e , THIS(CHAN5KEY_09)		, THIS(CHAN5INS_0A)		, THIS(CHAN5KEY_0B)		, THIS(CHAN5VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch5_key_0A },
	{	0x1b , 0x0f , THIS(CHAN5KEY_0A)		, THIS(CHAN5INS_0B)		, THIS(CHAN5KEY_0C)		, THIS(CHAN5VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch5_key_0B },
	{	0x1b , 0x10 , THIS(CHAN5KEY_0B)		, THIS(CHAN5INS_0C)		, THIS(CHAN5KEY_0D)		, THIS(CHAN5VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch5_key_0C },
	{	0x1b , 0x11 , THIS(CHAN5KEY_0C)		, THIS(CHAN5INS_0D)		, THIS(CHAN5KEY_0E)		, THIS(CHAN5VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch5_key_0D },
	{	0x1b , 0x12 , THIS(CHAN5KEY_0D)		, THIS(CHAN5INS_0E)		, THIS(CHAN5KEY_0F)		, THIS(CHAN5VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch5_key_0E },
	{	0x1b , 0x13 , THIS(CHAN5KEY_0E)		, THIS(CHAN5INS_0F)		, THIS(CHAN5KEY_00)		, THIS(CHAN5VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch5_key_0F },
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS

#define THISVAR(a) ((u8*)&(VAR_CELLS[3].a))
#define THIS(a) &CHAN3_CONTROLS[CONTROL_CHANNEL3_0_##a]
const Control CHAN3_CONTROLS[CONTROL_CHANNEL3_0_MAX] = { 
	//  x	 y		up						right					down					left					cache							var									callback		
	{	0x0d , 0x04 , THIS(CHAN3KEY_0F)		, THIS(CHAN3INS_00)		, THIS(CHAN3KEY_01)		, THIS(CHAN3VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch3_key_00 },
	{	0x0d , 0x05 , THIS(CHAN3KEY_00)		, THIS(CHAN3INS_01)		, THIS(CHAN3KEY_02)		, THIS(CHAN3VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch3_key_01 },
	{	0x0d , 0x06 , THIS(CHAN3KEY_01)		, THIS(CHAN3INS_02)		, THIS(CHAN3KEY_03)		, THIS(CHAN3VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch3_key_02 },
	{	0x0d , 0x07 , THIS(CHAN3KEY_02)		, THIS(CHAN3INS_03)		, THIS(CHAN3KEY_04)		, THIS(CHAN3VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch3_key_03 },
	{	0x0d , 0x08 , THIS(CHAN3KEY_03)		, THIS(CHAN3INS_04)		, THIS(CHAN3KEY_05)		, THIS(CHAN3VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch3_key_04 },
	{	0x0d , 0x09 , THIS(CHAN3KEY_04)		, THIS(CHAN3INS_05)		, THIS(CHAN3KEY_06)		, THIS(CHAN3VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch3_key_05 },
	{	0x0d , 0x0a , THIS(CHAN3KEY_05)		, THIS(CHAN3INS_06)		, THIS(CHAN3KEY_07)		, THIS(CHAN3VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch3_key_06 },
	{	0x0d , 0x0b , THIS(CHAN3KEY_06)		, THIS(CHAN3INS_07)		, THIS(CHAN3KEY_08)		, THIS(CHAN3VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch3_key_07 },
	{	0x0d , 0x0c , THIS(CHAN3KEY_07)		, THIS(CHAN3INS_08)		, THIS(CHAN3KEY_09)		, THIS(CHAN3VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch3_key_08 },
	{	0x0d , 0x0d , THIS(CHAN3KEY_08)		, THIS(CHAN3INS_09)		, THIS(CHAN3KEY_0A)		, THIS(CHAN3VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch3_key_09 },
	{	0x0d , 0x0e , THIS(CHAN3KEY_09)		, THIS(CHAN3INS_0A)		, THIS(CHAN3KEY_0B)		, THIS(CHAN3VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch3_key_0A },
	{	0x0d , 0x0f , THIS(CHAN3KEY_0A)		, THIS(CHAN3INS_0B)		, THIS(CHAN3KEY_0C)		, THIS(CHAN3VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch3_key_0B },
	{	0x0d , 0x10 , THIS(CHAN3KEY_0B)		, THIS(CHAN3INS_0C)		, THIS(CHAN3KEY_0D)		, THIS(CHAN3VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch3_key_0C },
	{	0x0d , 0x11 , THIS(CHAN3KEY_0C)		, THIS(CHAN3INS_0D)		, THIS(CHAN3KEY_0E)		, THIS(CHAN3VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch3_key_0D },
	{	0x0d , 0x12 , THIS(CHAN3KEY_0D)		, THIS(CHAN3INS_0E)		, THIS(CHAN3KEY_0F)		, THIS(CHAN3VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch3_key_0E },
	{	0x0d , 0x13 , THIS(CHAN3KEY_0E)		, THIS(CHAN3INS_0F)		, THIS(CHAN3KEY_00)		, THIS(CHAN3VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch3_key_0F },
	{	0x10 , 0x04 , THIS(CHAN3INS_0F)		, THIS(CHAN3VOL_00)		, THIS(CHAN3INS_01)		, THIS(CHAN3KEY_00)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[0])					, &cb_ch3_ins_00 }, 
	{	0x10 , 0x05 , THIS(CHAN3INS_00)		, THIS(CHAN3VOL_01)		, THIS(CHAN3INS_02)		, THIS(CHAN3KEY_01)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[1])					, &cb_ch3_ins_01 }, 
	{	0x10 , 0x06 , THIS(CHAN3INS_01)		, THIS(CHAN3VOL_02)		, THIS(CHAN3INS_03)		, THIS(CHAN3KEY_02)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[2])					, &cb_ch3_ins_02 }, 
	{	0x10 , 0x07 , THIS(CHAN3INS_02)		, THIS(CHAN3VOL_03)		, THIS(CHAN3INS_04)		, THIS(CHAN3KEY_03)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[3])					, &cb_ch3_ins_03 }, 
	{	0x10 , 0x08 , THIS(CHAN3INS_03)		, THIS(CHAN3VOL_04)		, THIS(CHAN3INS_05)		, THIS(CHAN3KEY_04)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[4])					, &cb_ch3_ins_04 }, 
	{	0x10 , 0x09 , THIS(CHAN3INS_04)		, THIS(CHAN3VOL_05)		, THIS(CHAN3INS_06)		, THIS(CHAN3KEY_05)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[5])					, &cb_ch3_ins_05 }, 
	{	0x10 , 0x0a , THIS(CHAN3INS_05)		, THIS(CHAN3VOL_06)		, THIS(CHAN3INS_07)		, THIS(CHAN3KEY_06)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[6])					, &cb_ch3_ins_06 }, 
	{	0x10 , 0x0b , THIS(CHAN3INS_06)		, THIS(CHAN3VOL_07)		, THIS(CHAN3INS_08)		, THIS(CHAN3KEY_07)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[7])					, &cb_ch3_ins_07 }, 
	{	0x10 , 0x0c , THIS(CHAN3INS_07)		, THIS(CHAN3VOL_08)		, THIS(CHAN3INS_09)		, THIS(CHAN3KEY_08)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[8])					, &cb_ch3_ins_08 }, 
	{	0x10 , 0x0d , THIS(CHAN3INS_08)		, THIS(CHAN3VOL_09)		, THIS(CHAN3INS_0A)		, THIS(CHAN3KEY_09)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[9])					, &cb_ch3_ins_09 }, 
	{	0x10 , 0x0e , THIS(CHAN3INS_09)		, THIS(CHAN3VOL_0A)		, THIS(CHAN3INS_0B)		, THIS(CHAN3KEY_0A)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[10])					, &cb_ch3_ins_0A }, 
	{	0x10 , 0x0f , THIS(CHAN3INS_0A)		, THIS(CHAN3VOL_0B)		, THIS(CHAN3INS_0C)		, THIS(CHAN3KEY_0B)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[11])					, &cb_ch3_ins_0B }, 
	{	0x10 , 0x10 , THIS(CHAN3INS_0B)		, THIS(CHAN3VOL_0C)		, THIS(CHAN3INS_0D)		, THIS(CHAN3KEY_0C)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[12])					, &cb_ch3_ins_0C }, 
	{	0x10 , 0x11 , THIS(CHAN3INS_0C)		, THIS(CHAN3VOL_0D)		, THIS(CHAN3INS_0E)		, THIS(CHAN3KEY_0D)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[13])					, &cb_ch3_ins_0D }, 
	{	0x10 , 0x12 , THIS(CHAN3INS_0D)		, THIS(CHAN3VOL_0E)		, THIS(CHAN3INS_0F)		, THIS(CHAN3KEY_0E)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[14])					, &cb_ch3_ins_0E }, 
	{	0x10 , 0x13 , THIS(CHAN3INS_0E)		, THIS(CHAN3VOL_0F)		, THIS(CHAN3INS_00)		, THIS(CHAN3KEY_0F)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[15])					, &cb_ch3_ins_0F }, 
	{	0x12 , 0x04 , THIS(CHAN3VOL_0F)		, THIS(CHAN3CMD_00)		, THIS(CHAN3VOL_01)		, THIS(CHAN3INS_00)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[0])					, &cb_ch3_vol_00 }, 
	{	0x12 , 0x05 , THIS(CHAN3VOL_00)		, THIS(CHAN3CMD_01)		, THIS(CHAN3VOL_02)		, THIS(CHAN3INS_01)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[1])					, &cb_ch3_vol_01 }, 
	{	0x12 , 0x06 , THIS(CHAN3VOL_01)		, THIS(CHAN3CMD_02)		, THIS(CHAN3VOL_03)		, THIS(CHAN3INS_02)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[2])					, &cb_ch3_vol_02 }, 
	{	0x12 , 0x07 , THIS(CHAN3VOL_02)		, THIS(CHAN3CMD_03)		, THIS(CHAN3VOL_04)		, THIS(CHAN3INS_03)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[3])					, &cb_ch3_vol_03 }, 
	{	0x12 , 0x08 , THIS(CHAN3VOL_03)		, THIS(CHAN3CMD_04)		, THIS(CHAN3VOL_05)		, THIS(CHAN3INS_04)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[4])					, &cb_ch3_vol_04 }, 
	{	0x12 , 0x09 , THIS(CHAN3VOL_04)		, THIS(CHAN3CMD_05)		, THIS(CHAN3VOL_06)		, THIS(CHAN3INS_05)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[5])					, &cb_ch3_vol_05 }, 
	{	0x12 , 0x0a , THIS(CHAN3VOL_05)		, THIS(CHAN3CMD_06)		, THIS(CHAN3VOL_07)		, THIS(CHAN3INS_06)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[6])					, &cb_ch3_vol_06 }, 
	{	0x12 , 0x0b , THIS(CHAN3VOL_06)		, THIS(CHAN3CMD_07)		, THIS(CHAN3VOL_08)		, THIS(CHAN3INS_07)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[7])					, &cb_ch3_vol_07 }, 
	{	0x12 , 0x0c , THIS(CHAN3VOL_07)		, THIS(CHAN3CMD_08)		, THIS(CHAN3VOL_09)		, THIS(CHAN3INS_08)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[8])					, &cb_ch3_vol_08 }, 
	{	0x12 , 0x0d , THIS(CHAN3VOL_08)		, THIS(CHAN3CMD_09)		, THIS(CHAN3VOL_0A)		, THIS(CHAN3INS_09)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[9])					, &cb_ch3_vol_09 }, 
	{	0x12 , 0x0e , THIS(CHAN3VOL_09)		, THIS(CHAN3CMD_0A)		, THIS(CHAN3VOL_0B)		, THIS(CHAN3INS_0A)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[10])					, &cb_ch3_vol_0A }, 
	{	0x12 , 0x0f , THIS(CHAN3VOL_0A)		, THIS(CHAN3CMD_0B)		, THIS(CHAN3VOL_0C)		, THIS(CHAN3INS_0B)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[11])					, &cb_ch3_vol_0B }, 
	{	0x12 , 0x10 , THIS(CHAN3VOL_0B)		, THIS(CHAN3CMD_0C)		, THIS(CHAN3VOL_0D)		, THIS(CHAN3INS_0C)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[12])					, &cb_ch3_vol_0C }, 
	{	0x12 , 0x11 , THIS(CHAN3VOL_0C)		, THIS(CHAN3CMD_0D)		, THIS(CHAN3VOL_0E)		, THIS(CHAN3INS_0D)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[13])					, &cb_ch3_vol_0D }, 
	{	0x12 , 0x12 , THIS(CHAN3VOL_0D)		, THIS(CHAN3CMD_0E)		, THIS(CHAN3VOL_0F)		, THIS(CHAN3INS_0E)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[14])					, &cb_ch3_vol_0E }, 
	{	0x12 , 0x13 , THIS(CHAN3VOL_0E)		, THIS(CHAN3CMD_0F)		, THIS(CHAN3VOL_00)		, THIS(CHAN3INS_0F)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[15])					, &cb_ch3_vol_0F }, 
	{	0x13 , 0x04 , THIS(CHAN3CMD_0F)		, THIS(CHAN3VAL_00)		, THIS(CHAN3CMD_01)		, THIS(CHAN3VOL_00)		, &CACHE_COMMANDS				, THISVAR(CMD[0])					, &cb_ch3_cmd_00 }, 
	{	0x13 , 0x05 , THIS(CHAN3CMD_00)		, THIS(CHAN3VAL_01)		, THIS(CHAN3CMD_02)		, THIS(CHAN3VOL_01)		, &CACHE_COMMANDS				, THISVAR(CMD[1])					, &cb_ch3_cmd_01 }, 
	{	0x13 , 0x06 , THIS(CHAN3CMD_01)		, THIS(CHAN3VAL_02)		, THIS(CHAN3CMD_03)		, THIS(CHAN3VOL_02)		, &CACHE_COMMANDS				, THISVAR(CMD[2])					, &cb_ch3_cmd_02 }, 
	{	0x13 , 0x07 , THIS(CHAN3CMD_02)		, THIS(CHAN3VAL_03)		, THIS(CHAN3CMD_04)		, THIS(CHAN3VOL_03)		, &CACHE_COMMANDS				, THISVAR(CMD[3])					, &cb_ch3_cmd_03 }, 
	{	0x13 , 0x08 , THIS(CHAN3CMD_03)		, THIS(CHAN3VAL_04)		, THIS(CHAN3CMD_05)		, THIS(CHAN3VOL_04)		, &CACHE_COMMANDS				, THISVAR(CMD[4])					, &cb_ch3_cmd_04 }, 
	{	0x13 , 0x09 , THIS(CHAN3CMD_04)		, THIS(CHAN3VAL_05)		, THIS(CHAN3CMD_06)		, THIS(CHAN3VOL_05)		, &CACHE_COMMANDS				, THISVAR(CMD[5])					, &cb_ch3_cmd_05 }, 
	{	0x13 , 0x0a , THIS(CHAN3CMD_05)		, THIS(CHAN3VAL_06)		, THIS(CHAN3CMD_07)		, THIS(CHAN3VOL_06)		, &CACHE_COMMANDS				, THISVAR(CMD[6])					, &cb_ch3_cmd_06 }, 
	{	0x13 , 0x0b , THIS(CHAN3CMD_06)		, THIS(CHAN3VAL_07)		, THIS(CHAN3CMD_08)		, THIS(CHAN3VOL_07)		, &CACHE_COMMANDS				, THISVAR(CMD[7])					, &cb_ch3_cmd_07 }, 
	{	0x13 , 0x0c , THIS(CHAN3CMD_07)		, THIS(CHAN3VAL_08)		, THIS(CHAN3CMD_09)		, THIS(CHAN3VOL_08)		, &CACHE_COMMANDS				, THISVAR(CMD[8])					, &cb_ch3_cmd_08 }, 
	{	0x13 , 0x0d , THIS(CHAN3CMD_08)		, THIS(CHAN3VAL_09)		, THIS(CHAN3CMD_0A)		, THIS(CHAN3VOL_09)		, &CACHE_COMMANDS				, THISVAR(CMD[9])					, &cb_ch3_cmd_09 }, 
	{	0x13 , 0x0e , THIS(CHAN3CMD_09)		, THIS(CHAN3VAL_0A)		, THIS(CHAN3CMD_0B)		, THIS(CHAN3VOL_0A)		, &CACHE_COMMANDS				, THISVAR(CMD[10])					, &cb_ch3_cmd_0A }, 
	{	0x13 , 0x0f , THIS(CHAN3CMD_0A)		, THIS(CHAN3VAL_0B)		, THIS(CHAN3CMD_0C)		, THIS(CHAN3VOL_0B)		, &CACHE_COMMANDS				, THISVAR(CMD[11])					, &cb_ch3_cmd_0B }, 
	{	0x13 , 0x10 , THIS(CHAN3CMD_0B)		, THIS(CHAN3VAL_0C)		, THIS(CHAN3CMD_0D)		, THIS(CHAN3VOL_0C)		, &CACHE_COMMANDS				, THISVAR(CMD[12])					, &cb_ch3_cmd_0C }, 
	{	0x13 , 0x11 , THIS(CHAN3CMD_0C)		, THIS(CHAN3VAL_0D)		, THIS(CHAN3CMD_0E)		, THIS(CHAN3VOL_0D)		, &CACHE_COMMANDS				, THISVAR(CMD[13])					, &cb_ch3_cmd_0D }, 
	{	0x13 , 0x12 , THIS(CHAN3CMD_0D)		, THIS(CHAN3VAL_0E)		, THIS(CHAN3CMD_0F)		, THIS(CHAN3VOL_0E)		, &CACHE_COMMANDS				, THISVAR(CMD[14])					, &cb_ch3_cmd_0E }, 
	{	0x13 , 0x13 , THIS(CHAN3CMD_0E)		, THIS(CHAN3VAL_0F)		, THIS(CHAN3CMD_00)		, THIS(CHAN3VOL_0F)		, &CACHE_COMMANDS				, THISVAR(CMD[15])					, &cb_ch3_cmd_0F }, 
	{	0x14 , 0x04 , THIS(CHAN3VAL_0F)		, THIS(CHAN3KEY_00)		, THIS(CHAN3VAL_01)		, THIS(CHAN3CMD_00)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[0])					, &cb_ch3_val_00 }, 
	{	0x14 , 0x05 , THIS(CHAN3VAL_00)		, THIS(CHAN3KEY_01)		, THIS(CHAN3VAL_02)		, THIS(CHAN3CMD_01)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[1])					, &cb_ch3_val_01 }, 
	{	0x14 , 0x06 , THIS(CHAN3VAL_01)		, THIS(CHAN3KEY_02)		, THIS(CHAN3VAL_03)		, THIS(CHAN3CMD_02)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[2])					, &cb_ch3_val_02 }, 
	{	0x14 , 0x07 , THIS(CHAN3VAL_02)		, THIS(CHAN3KEY_03)		, THIS(CHAN3VAL_04)		, THIS(CHAN3CMD_03)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[3])					, &cb_ch3_val_03 }, 
	{	0x14 , 0x08 , THIS(CHAN3VAL_03)		, THIS(CHAN3KEY_04)		, THIS(CHAN3VAL_05)		, THIS(CHAN3CMD_04)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[4])					, &cb_ch3_val_04 }, 
	{	0x14 , 0x09 , THIS(CHAN3VAL_04)		, THIS(CHAN3KEY_05)		, THIS(CHAN3VAL_06)		, THIS(CHAN3CMD_05)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[5])					, &cb_ch3_val_05 }, 
	{	0x14 , 0x0a , THIS(CHAN3VAL_05)		, THIS(CHAN3KEY_06)		, THIS(CHAN3VAL_07)		, THIS(CHAN3CMD_06)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[6])					, &cb_ch3_val_06 }, 
	{	0x14 , 0x0b , THIS(CHAN3VAL_06)		, THIS(CHAN3KEY_07)		, THIS(CHAN3VAL_08)		, THIS(CHAN3CMD_07)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[7])					, &cb_ch3_val_07 }, 
	{	0x14 , 0x0c , THIS(CHAN3VAL_07)		, THIS(CHAN3KEY_08)		, THIS(CHAN3VAL_09)		, THIS(CHAN3CMD_08)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[8])					, &cb_ch3_val_08 }, 
	{	0x14 , 0x0d , THIS(CHAN3VAL_08)		, THIS(CHAN3KEY_09)		, THIS(CHAN3VAL_0A)		, THIS(CHAN3CMD_09)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[9])					, &cb_ch3_val_09 }, 
	{	0x14 , 0x0e , THIS(CHAN3VAL_09)		, THIS(CHAN3KEY_0A)		, THIS(CHAN3VAL_0B)		, THIS(CHAN3CMD_0A)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[10])					, &cb_ch3_val_0A }, 
	{	0x14 , 0x0f , THIS(CHAN3VAL_0A)		, THIS(CHAN3KEY_0B)		, THIS(CHAN3VAL_0C)		, THIS(CHAN3CMD_0B)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[11])					, &cb_ch3_val_0B }, 
	{	0x14 , 0x10 , THIS(CHAN3VAL_0B)		, THIS(CHAN3KEY_0C)		, THIS(CHAN3VAL_0D)		, THIS(CHAN3CMD_0C)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[12])					, &cb_ch3_val_0C }, 
	{	0x14 , 0x11 , THIS(CHAN3VAL_0C)		, THIS(CHAN3KEY_0D)		, THIS(CHAN3VAL_0E)		, THIS(CHAN3CMD_0D)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[13])					, &cb_ch3_val_0D }, 
	{	0x14 , 0x12 , THIS(CHAN3VAL_0D)		, THIS(CHAN3KEY_0E)		, THIS(CHAN3VAL_0F)		, THIS(CHAN3CMD_0E)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[14])					, &cb_ch3_val_0E }, 
	{	0x14 , 0x13 , THIS(CHAN3VAL_0E)		, THIS(CHAN3KEY_0F)		, THIS(CHAN3VAL_00)		, THIS(CHAN3CMD_0F)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[15])					, &cb_ch3_val_0F }, 
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN0_CONTROLS[CONTROL_CHANNEL0_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[0].a))
	{	0x01 , 0x04 , THIS(CHAN0KEY_0F)		, THIS(CHAN0INS_00)		, THIS(CHAN0KEY_01)		, THIS(CHAN0VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch0_key_00 },
	{	0x01 , 0x05 , THIS(CHAN0KEY_00)		, THIS(CHAN0INS_01)		, THIS(CHAN0KEY_02)		, THIS(CHAN0VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch0_key_01 },
	{	0x01 , 0x06 , THIS(CHAN0KEY_01)		, THIS(CHAN0INS_02)		, THIS(CHAN0KEY_03)		, THIS(CHAN0VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch0_key_02 },
	{	0x01 , 0x07 , THIS(CHAN0KEY_02)		, THIS(CHAN0INS_03)		, THIS(CHAN0KEY_04)		, THIS(CHAN0VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch0_key_03 },
	{	0x01 , 0x08 , THIS(CHAN0KEY_03)		, THIS(CHAN0INS_04)		, THIS(CHAN0KEY_05)		, THIS(CHAN0VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch0_key_04 },
	{	0x01 , 0x09 , THIS(CHAN0KEY_04)		, THIS(CHAN0INS_05)		, THIS(CHAN0KEY_06)		, THIS(CHAN0VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch0_key_05 },
	{	0x01 , 0x0a , THIS(CHAN0KEY_05)		, THIS(CHAN0INS_06)		, THIS(CHAN0KEY_07)		, THIS(CHAN0VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch0_key_06 },
	{	0x01 , 0x0b , THIS(CHAN0KEY_06)		, THIS(CHAN0INS_07)		, THIS(CHAN0KEY_08)		, THIS(CHAN0VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch0_key_07 },
	{	0x01 , 0x0c , THIS(CHAN0KEY_07)		, THIS(CHAN0INS_08)		, THIS(CHAN0KEY_09)		, THIS(CHAN0VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch0_key_08 },
	{	0x01 , 0x0d , THIS(CHAN0KEY_08)		, THIS(CHAN0INS_09)		, THIS(CHAN0KEY_0A)		, THIS(CHAN0VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch0_key_09 },
	{	0x01 , 0x0e , THIS(CHAN0KEY_09)		, THIS(CHAN0INS_0A)		, THIS(CHAN0KEY_0B)		, THIS(CHAN0VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch0_key_0A },
	{	0x01 , 0x0f , THIS(CHAN0KEY_0A)		, THIS(CHAN0INS_0B)		, THIS(CHAN0KEY_0C)		, THIS(CHAN0VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch0_key_0B },
	{	0x01 , 0x10 , THIS(CHAN0KEY_0B)		, THIS(CHAN0INS_0C)		, THIS(CHAN0KEY_0D)		, THIS(CHAN0VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch0_key_0C },
	{	0x01 , 0x11 , THIS(CHAN0KEY_0C)		, THIS(CHAN0INS_0D)		, THIS(CHAN0KEY_0E)		, THIS(CHAN0VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch0_key_0D },
	{	0x01 , 0x12 , THIS(CHAN0KEY_0D)		, THIS(CHAN0INS_0E)		, THIS(CHAN0KEY_0F)		, THIS(CHAN0VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch0_key_0E },
	{	0x01 , 0x13 , THIS(CHAN0KEY_0E)		, THIS(CHAN0INS_0F)		, THIS(CHAN0KEY_00)		, THIS(CHAN0VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch0_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN1_CONTROLS[CONTROL_CHANNEL1_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[1].a))
	{	0x05 , 0x04 , THIS(CHAN1KEY_0F)		, THIS(CHAN1INS_00)		, THIS(CHAN1KEY_01)		, THIS(CHAN1VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch1_key_00 },
	{	0x05 , 0x05 , THIS(CHAN1KEY_00)		, THIS(CHAN1INS_01)		, THIS(CHAN1KEY_02)		, THIS(CHAN1VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch1_key_01 },
	{	0x05 , 0x06 , THIS(CHAN1KEY_01)		, THIS(CHAN1INS_02)		, THIS(CHAN1KEY_03)		, THIS(CHAN1VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch1_key_02 },
	{	0x05 , 0x07 , THIS(CHAN1KEY_02)		, THIS(CHAN1INS_03)		, THIS(CHAN1KEY_04)		, THIS(CHAN1VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch1_key_03 },
	{	0x05 , 0x08 , THIS(CHAN1KEY_03)		, THIS(CHAN1INS_04)		, THIS(CHAN1KEY_05)		, THIS(CHAN1VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch1_key_04 },
	{	0x05 , 0x09 , THIS(CHAN1KEY_04)		, THIS(CHAN1INS_05)		, THIS(CHAN1KEY_06)		, THIS(CHAN1VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch1_key_05 },
	{	0x05 , 0x0a , THIS(CHAN1KEY_05)		, THIS(CHAN1INS_06)		, THIS(CHAN1KEY_07)		, THIS(CHAN1VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch1_key_06 },
	{	0x05 , 0x0b , THIS(CHAN1KEY_06)		, THIS(CHAN1INS_07)		, THIS(CHAN1KEY_08)		, THIS(CHAN1VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch1_key_07 },
	{	0x05 , 0x0c , THIS(CHAN1KEY_07)		, THIS(CHAN1INS_08)		, THIS(CHAN1KEY_09)		, THIS(CHAN1VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch1_key_08 },
	{	0x05 , 0x0d , THIS(CHAN1KEY_08)		, THIS(CHAN1INS_09)		, THIS(CHAN1KEY_0A)		, THIS(CHAN1VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch1_key_09 },
	{	0x05 , 0x0e , THIS(CHAN1KEY_09)		, THIS(CHAN1INS_0A)		, THIS(CHAN1KEY_0B)		, THIS(CHAN1VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch1_key_0A },
	{	0x05 , 0x0f , THIS(CHAN1KEY_0A)		, THIS(CHAN1INS_0B)		, THIS(CHAN1KEY_0C)		, THIS(CHAN1VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch1_key_0B },
	{	0x05 , 0x10 , THIS(CHAN1KEY_0B)		, THIS(CHAN1INS_0C)		, THIS(CHAN1KEY_0D)		, THIS(CHAN1VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch1_key_0C },
	{	0x05 , 0x11 , THIS(CHAN1KEY_0C)		, THIS(CHAN1INS_0D)		, THIS(CHAN1KEY_0E)		, THIS(CHAN1VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch1_key_0D },
	{	0x05 , 0x12 , THIS(CHAN1KEY_0D)		, THIS(CHAN1INS_0E)		, THIS(CHAN1KEY_0F)		, THIS(CHAN1VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch1_key_0E },
	{	0x05 , 0x13 , THIS(CHAN1KEY_0E)		, THIS(CHAN1INS_0F)		, THIS(CHAN1KEY_00)		, THIS(CHAN1VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch1_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN2_CONTROLS[CONTROL_CHANNEL2_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[2].a))
	{	0x09 , 0x04 , THIS(CHAN2KEY_0F)		, THIS(CHAN2INS_00)		, THIS(CHAN2KEY_01)		, THIS(CHAN2VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch2_key_00 },
	{	0x09 , 0x05 , THIS(CHAN2KEY_00)		, THIS(CHAN2INS_01)		, THIS(CHAN2KEY_02)		, THIS(CHAN2VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch2_key_01 },
	{	0x09 , 0x06 , THIS(CHAN2KEY_01)		, THIS(CHAN2INS_02)		, THIS(CHAN2KEY_03)		, THIS(CHAN2VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch2_key_02 },
	{	0x09 , 0x07 , THIS(CHAN2KEY_02)		, THIS(CHAN2INS_03)		, THIS(CHAN2KEY_04)		, THIS(CHAN2VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch2_key_03 },
	{	0x09 , 0x08 , THIS(CHAN2KEY_03)		, THIS(CHAN2INS_04)		, THIS(CHAN2KEY_05)		, THIS(CHAN2VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch2_key_04 },
	{	0x09 , 0x09 , THIS(CHAN2KEY_04)		, THIS(CHAN2INS_05)		, THIS(CHAN2KEY_06)		, THIS(CHAN2VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch2_key_05 },
	{	0x09 , 0x0a , THIS(CHAN2KEY_05)		, THIS(CHAN2INS_06)		, THIS(CHAN2KEY_07)		, THIS(CHAN2VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch2_key_06 },
	{	0x09 , 0x0b , THIS(CHAN2KEY_06)		, THIS(CHAN2INS_07)		, THIS(CHAN2KEY_08)		, THIS(CHAN2VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch2_key_07 },
	{	0x09 , 0x0c , THIS(CHAN2KEY_07)		, THIS(CHAN2INS_08)		, THIS(CHAN2KEY_09)		, THIS(CHAN2VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch2_key_08 },
	{	0x09 , 0x0d , THIS(CHAN2KEY_08)		, THIS(CHAN2INS_09)		, THIS(CHAN2KEY_0A)		, THIS(CHAN2VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch2_key_09 },
	{	0x09 , 0x0e , THIS(CHAN2KEY_09)		, THIS(CHAN2INS_0A)		, THIS(CHAN2KEY_0B)		, THIS(CHAN2VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch2_key_0A },
	{	0x09 , 0x0f , THIS(CHAN2KEY_0A)		, THIS(CHAN2INS_0B)		, THIS(CHAN2KEY_0C)		, THIS(CHAN2VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch2_key_0B },
	{	0x09 , 0x10 , THIS(CHAN2KEY_0B)		, THIS(CHAN2INS_0C)		, THIS(CHAN2KEY_0D)		, THIS(CHAN2VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch2_key_0C },
	{	0x09 , 0x11 , THIS(CHAN2KEY_0C)		, THIS(CHAN2INS_0D)		, THIS(CHAN2KEY_0E)		, THIS(CHAN2VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch2_key_0D },
	{	0x09 , 0x12 , THIS(CHAN2KEY_0D)		, THIS(CHAN2INS_0E)		, THIS(CHAN2KEY_0F)		, THIS(CHAN2VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch2_key_0E },
	{	0x09 , 0x13 , THIS(CHAN2KEY_0E)		, THIS(CHAN2INS_0F)		, THIS(CHAN2KEY_00)		, THIS(CHAN2VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch2_key_0F },
#undef THIS
#undef THISVAR

#define THIS(a) &CHAN4_CONTROLS[CONTROL_CHANNEL4_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[4].a))
	{	0x17 , 0x04 , THIS(CHAN4KEY_0F)		, THIS(CHAN4INS_00)		, THIS(CHAN4KEY_01)		, THIS(CHAN4VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch4_key_00 },
	{	0x17 , 0x05 , THIS(CHAN4KEY_00)		, THIS(CHAN4INS_01)		, THIS(CHAN4KEY_02)		, THIS(CHAN4VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch4_key_01 },
	{	0x17 , 0x06 , THIS(CHAN4KEY_01)		, THIS(CHAN4INS_02)		, THIS(CHAN4KEY_03)		, THIS(CHAN4VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch4_key_02 },
	{	0x17 , 0x07 , THIS(CHAN4KEY_02)		, THIS(CHAN4INS_03)		, THIS(CHAN4KEY_04)		, THIS(CHAN4VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch4_key_03 },
	{	0x17 , 0x08 , THIS(CHAN4KEY_03)		, THIS(CHAN4INS_04)		, THIS(CHAN4KEY_05)		, THIS(CHAN4VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch4_key_04 },
	{	0x17 , 0x09 , THIS(CHAN4KEY_04)		, THIS(CHAN4INS_05)		, THIS(CHAN4KEY_06)		, THIS(CHAN4VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch4_key_05 },
	{	0x17 , 0x0a , THIS(CHAN4KEY_05)		, THIS(CHAN4INS_06)		, THIS(CHAN4KEY_07)		, THIS(CHAN4VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch4_key_06 },
	{	0x17 , 0x0b , THIS(CHAN4KEY_06)		, THIS(CHAN4INS_07)		, THIS(CHAN4KEY_08)		, THIS(CHAN4VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch4_key_07 },
	{	0x17 , 0x0c , THIS(CHAN4KEY_07)		, THIS(CHAN4INS_08)		, THIS(CHAN4KEY_09)		, THIS(CHAN4VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch4_key_08 },
	{	0x17 , 0x0d , THIS(CHAN4KEY_08)		, THIS(CHAN4INS_09)		, THIS(CHAN4KEY_0A)		, THIS(CHAN4VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch4_key_09 },
	{	0x17 , 0x0e , THIS(CHAN4KEY_09)		, THIS(CHAN4INS_0A)		, THIS(CHAN4KEY_0B)		, THIS(CHAN4VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch4_key_0A },
	{	0x17 , 0x0f , THIS(CHAN4KEY_0A)		, THIS(CHAN4INS_0B)		, THIS(CHAN4KEY_0C)		, THIS(CHAN4VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch4_key_0B },
	{	0x17 , 0x10 , THIS(CHAN4KEY_0B)		, THIS(CHAN4INS_0C)		, THIS(CHAN4KEY_0D)		, THIS(CHAN4VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch4_key_0C },
	{	0x17 , 0x11 , THIS(CHAN4KEY_0C)		, THIS(CHAN4INS_0D)		, THIS(CHAN4KEY_0E)		, THIS(CHAN4VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch4_key_0D },
	{	0x17 , 0x12 , THIS(CHAN4KEY_0D)		, THIS(CHAN4INS_0E)		, THIS(CHAN4KEY_0F)		, THIS(CHAN4VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch4_key_0E },
	{	0x17 , 0x13 , THIS(CHAN4KEY_0E)		, THIS(CHAN4INS_0F)		, THIS(CHAN4KEY_00)		, THIS(CHAN4VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch4_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN5_CONTROLS[CONTROL_CHANNEL5_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[5].a))
	{	0x1b , 0x04 , THIS(CHAN5KEY_0F)		, THIS(CHAN5INS_00)		, THIS(CHAN5KEY_01)		, THIS(CHAN5VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch5_key_00 },
	{	0x1b , 0x05 , THIS(CHAN5KEY_00)		, THIS(CHAN5INS_01)		, THIS(CHAN5KEY_02)		, THIS(CHAN5VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch5_key_01 },
	{	0x1b , 0x06 , THIS(CHAN5KEY_01)		, THIS(CHAN5INS_02)		, THIS(CHAN5KEY_03)		, THIS(CHAN5VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch5_key_02 },
	{	0x1b , 0x07 , THIS(CHAN5KEY_02)		, THIS(CHAN5INS_03)		, THIS(CHAN5KEY_04)		, THIS(CHAN5VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch5_key_03 },
	{	0x1b , 0x08 , THIS(CHAN5KEY_03)		, THIS(CHAN5INS_04)		, THIS(CHAN5KEY_05)		, THIS(CHAN5VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch5_key_04 },
	{	0x1b , 0x09 , THIS(CHAN5KEY_04)		, THIS(CHAN5INS_05)		, THIS(CHAN5KEY_06)		, THIS(CHAN5VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch5_key_05 },
	{	0x1b , 0x0a , THIS(CHAN5KEY_05)		, THIS(CHAN5INS_06)		, THIS(CHAN5KEY_07)		, THIS(CHAN5VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch5_key_06 },
	{	0x1b , 0x0b , THIS(CHAN5KEY_06)		, THIS(CHAN5INS_07)		, THIS(CHAN5KEY_08)		, THIS(CHAN5VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch5_key_07 },
	{	0x1b , 0x0c , THIS(CHAN5KEY_07)		, THIS(CHAN5INS_08)		, THIS(CHAN5KEY_09)		, THIS(CHAN5VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch5_key_08 },
	{	0x1b , 0x0d , THIS(CHAN5KEY_08)		, THIS(CHAN5INS_09)		, THIS(CHAN5KEY_0A)		, THIS(CHAN5VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch5_key_09 },
	{	0x1b , 0x0e , THIS(CHAN5KEY_09)		, THIS(CHAN5INS_0A)		, THIS(CHAN5KEY_0B)		, THIS(CHAN5VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch5_key_0A },
	{	0x1b , 0x0f , THIS(CHAN5KEY_0A)		, THIS(CHAN5INS_0B)		, THIS(CHAN5KEY_0C)		, THIS(CHAN5VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch5_key_0B },
	{	0x1b , 0x10 , THIS(CHAN5KEY_0B)		, THIS(CHAN5INS_0C)		, THIS(CHAN5KEY_0D)		, THIS(CHAN5VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch5_key_0C },
	{	0x1b , 0x11 , THIS(CHAN5KEY_0C)		, THIS(CHAN5INS_0D)		, THIS(CHAN5KEY_0E)		, THIS(CHAN5VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch5_key_0D },
	{	0x1b , 0x12 , THIS(CHAN5KEY_0D)		, THIS(CHAN5INS_0E)		, THIS(CHAN5KEY_0F)		, THIS(CHAN5VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch5_key_0E },
	{	0x1b , 0x13 , THIS(CHAN5KEY_0E)		, THIS(CHAN5INS_0F)		, THIS(CHAN5KEY_00)		, THIS(CHAN5VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch5_key_0F },
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN4_CONTROLS[CONTROL_CHANNEL4_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[4].a))
const Control CHAN4_CONTROLS[CONTROL_CHANNEL4_0_MAX] = { 
	//  x	 y		up						right					down					left					cache							var									callback		
	{	0x11 , 0x04 , THIS(CHAN4KEY_0F)		, THIS(CHAN4INS_00)		, THIS(CHAN4KEY_01)		, THIS(CHAN4VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0]) 					, &cb_ch4_key_00 }, 
	{	0x11 , 0x05 , THIS(CHAN4KEY_00)		, THIS(CHAN4INS_01)		, THIS(CHAN4KEY_02)		, THIS(CHAN4VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1]) 					, &cb_ch4_key_01 }, 
	{	0x11 , 0x06 , THIS(CHAN4KEY_01)		, THIS(CHAN4INS_02)		, THIS(CHAN4KEY_03)		, THIS(CHAN4VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2]) 					, &cb_ch4_key_02 }, 
	{	0x11 , 0x07 , THIS(CHAN4KEY_02)		, THIS(CHAN4INS_03)		, THIS(CHAN4KEY_04)		, THIS(CHAN4VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3]) 					, &cb_ch4_key_03 }, 
	{	0x11 , 0x08 , THIS(CHAN4KEY_03)		, THIS(CHAN4INS_04)		, THIS(CHAN4KEY_05)		, THIS(CHAN4VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4]) 					, &cb_ch4_key_04 }, 
	{	0x11 , 0x09 , THIS(CHAN4KEY_04)		, THIS(CHAN4INS_05)		, THIS(CHAN4KEY_06)		, THIS(CHAN4VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5]) 					, &cb_ch4_key_05 }, 
	{	0x11 , 0x0a , THIS(CHAN4KEY_05)		, THIS(CHAN4INS_06)		, THIS(CHAN4KEY_07)		, THIS(CHAN4VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6]) 					, &cb_ch4_key_06 }, 
	{	0x11 , 0x0b , THIS(CHAN4KEY_06)		, THIS(CHAN4INS_07)		, THIS(CHAN4KEY_08)		, THIS(CHAN4VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7]) 					, &cb_ch4_key_07 }, 
	{	0x11 , 0x0c , THIS(CHAN4KEY_07)		, THIS(CHAN4INS_08)		, THIS(CHAN4KEY_09)		, THIS(CHAN4VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8]) 					, &cb_ch4_key_08 }, 
	{	0x11 , 0x0d , THIS(CHAN4KEY_08)		, THIS(CHAN4INS_09)		, THIS(CHAN4KEY_0A)		, THIS(CHAN4VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9]) 					, &cb_ch4_key_09 }, 
	{	0x11 , 0x0e , THIS(CHAN4KEY_09)		, THIS(CHAN4INS_0A)		, THIS(CHAN4KEY_0B)		, THIS(CHAN4VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch4_key_0A }, 
	{	0x11 , 0x0f , THIS(CHAN4KEY_0A)		, THIS(CHAN4INS_0B)		, THIS(CHAN4KEY_0C)		, THIS(CHAN4VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch4_key_0B }, 
	{	0x11 , 0x10 , THIS(CHAN4KEY_0B)		, THIS(CHAN4INS_0C)		, THIS(CHAN4KEY_0D)		, THIS(CHAN4VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch4_key_0C }, 
	{	0x11 , 0x11 , THIS(CHAN4KEY_0C)		, THIS(CHAN4INS_0D)		, THIS(CHAN4KEY_0E)		, THIS(CHAN4VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch4_key_0D }, 
	{	0x11 , 0x12 , THIS(CHAN4KEY_0D)		, THIS(CHAN4INS_0E)		, THIS(CHAN4KEY_0F)		, THIS(CHAN4VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch4_key_0E }, 
	{	0x11 , 0x13 , THIS(CHAN4KEY_0E)		, THIS(CHAN4INS_0F)		, THIS(CHAN4KEY_00)		, THIS(CHAN4VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch4_key_0F }, 
	{	0x14 , 0x04 , THIS(CHAN4INS_0F)		, THIS(CHAN4VOL_00)		, THIS(CHAN4INS_01)		, THIS(CHAN4KEY_00)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[0]) 					, &cb_ch4_ins_00 }, 
	{	0x14 , 0x05 , THIS(CHAN4INS_00)		, THIS(CHAN4VOL_01)		, THIS(CHAN4INS_02)		, THIS(CHAN4KEY_01)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[1]) 					, &cb_ch4_ins_01 }, 
	{	0x14 , 0x06 , THIS(CHAN4INS_01)		, THIS(CHAN4VOL_02)		, THIS(CHAN4INS_03)		, THIS(CHAN4KEY_02)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[2]) 					, &cb_ch4_ins_02 }, 
	{	0x14 , 0x07 , THIS(CHAN4INS_02)		, THIS(CHAN4VOL_03)		, THIS(CHAN4INS_04)		, THIS(CHAN4KEY_03)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[3]) 					, &cb_ch4_ins_03 }, 
	{	0x14 , 0x08 , THIS(CHAN4INS_03)		, THIS(CHAN4VOL_04)		, THIS(CHAN4INS_05)		, THIS(CHAN4KEY_04)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[4]) 					, &cb_ch4_ins_04 }, 
	{	0x14 , 0x09 , THIS(CHAN4INS_04)		, THIS(CHAN4VOL_05)		, THIS(CHAN4INS_06)		, THIS(CHAN4KEY_05)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[5]) 					, &cb_ch4_ins_05 }, 
	{	0x14 , 0x0a , THIS(CHAN4INS_05)		, THIS(CHAN4VOL_06)		, THIS(CHAN4INS_07)		, THIS(CHAN4KEY_06)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[6]) 					, &cb_ch4_ins_06 }, 
	{	0x14 , 0x0b , THIS(CHAN4INS_06)		, THIS(CHAN4VOL_07)		, THIS(CHAN4INS_08)		, THIS(CHAN4KEY_07)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[7]) 					, &cb_ch4_ins_07 }, 
	{	0x14 , 0x0c , THIS(CHAN4INS_07)		, THIS(CHAN4VOL_08)		, THIS(CHAN4INS_09)		, THIS(CHAN4KEY_08)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[8]) 					, &cb_ch4_ins_08 }, 
	{	0x14 , 0x0d , THIS(CHAN4INS_08)		, THIS(CHAN4VOL_09)		, THIS(CHAN4INS_0A)		, THIS(CHAN4KEY_09)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[9]) 					, &cb_ch4_ins_09 }, 
	{	0x14 , 0x0e , THIS(CHAN4INS_09)		, THIS(CHAN4VOL_0A)		, THIS(CHAN4INS_0B)		, THIS(CHAN4KEY_0A)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[10])					, &cb_ch4_ins_0A }, 
	{	0x14 , 0x0f , THIS(CHAN4INS_0A)		, THIS(CHAN4VOL_0B)		, THIS(CHAN4INS_0C)		, THIS(CHAN4KEY_0B)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[11])					, &cb_ch4_ins_0B }, 
	{	0x14 , 0x10 , THIS(CHAN4INS_0B)		, THIS(CHAN4VOL_0C)		, THIS(CHAN4INS_0D)		, THIS(CHAN4KEY_0C)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[12])					, &cb_ch4_ins_0C }, 
	{	0x14 , 0x11 , THIS(CHAN4INS_0C)		, THIS(CHAN4VOL_0D)		, THIS(CHAN4INS_0E)		, THIS(CHAN4KEY_0D)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[13])					, &cb_ch4_ins_0D }, 
	{	0x14 , 0x12 , THIS(CHAN4INS_0D)		, THIS(CHAN4VOL_0E)		, THIS(CHAN4INS_0F)		, THIS(CHAN4KEY_0E)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[14])					, &cb_ch4_ins_0E }, 
	{	0x14 , 0x13 , THIS(CHAN4INS_0E)		, THIS(CHAN4VOL_0F)		, THIS(CHAN4INS_00)		, THIS(CHAN4KEY_0F)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[15])					, &cb_ch4_ins_0F }, 
	{	0x16 , 0x04 , THIS(CHAN4VOL_0F)		, THIS(CHAN4CMD_00)		, THIS(CHAN4VOL_01)		, THIS(CHAN4INS_00)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[0]) 					, &cb_ch4_vol_00 }, 
	{	0x16 , 0x05 , THIS(CHAN4VOL_00)		, THIS(CHAN4CMD_01)		, THIS(CHAN4VOL_02)		, THIS(CHAN4INS_01)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[1]) 					, &cb_ch4_vol_01 }, 
	{	0x16 , 0x06 , THIS(CHAN4VOL_01)		, THIS(CHAN4CMD_02)		, THIS(CHAN4VOL_03)		, THIS(CHAN4INS_02)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[2]) 					, &cb_ch4_vol_02 }, 
	{	0x16 , 0x07 , THIS(CHAN4VOL_02)		, THIS(CHAN4CMD_03)		, THIS(CHAN4VOL_04)		, THIS(CHAN4INS_03)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[3]) 					, &cb_ch4_vol_03 }, 
	{	0x16 , 0x08 , THIS(CHAN4VOL_03)		, THIS(CHAN4CMD_04)		, THIS(CHAN4VOL_05)		, THIS(CHAN4INS_04)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[4]) 					, &cb_ch4_vol_04 }, 
	{	0x16 , 0x09 , THIS(CHAN4VOL_04)		, THIS(CHAN4CMD_05)		, THIS(CHAN4VOL_06)		, THIS(CHAN4INS_05)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[5]) 					, &cb_ch4_vol_05 }, 
	{	0x16 , 0x0a , THIS(CHAN4VOL_05)		, THIS(CHAN4CMD_06)		, THIS(CHAN4VOL_07)		, THIS(CHAN4INS_06)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[6]) 					, &cb_ch4_vol_06 }, 
	{	0x16 , 0x0b , THIS(CHAN4VOL_06)		, THIS(CHAN4CMD_07)		, THIS(CHAN4VOL_08)		, THIS(CHAN4INS_07)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[7]) 					, &cb_ch4_vol_07 }, 
	{	0x16 , 0x0c , THIS(CHAN4VOL_07)		, THIS(CHAN4CMD_08)		, THIS(CHAN4VOL_09)		, THIS(CHAN4INS_08)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[8]) 					, &cb_ch4_vol_08 }, 
	{	0x16 , 0x0d , THIS(CHAN4VOL_08)		, THIS(CHAN4CMD_09)		, THIS(CHAN4VOL_0A)		, THIS(CHAN4INS_09)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[9]) 					, &cb_ch4_vol_09 }, 
	{	0x16 , 0x0e , THIS(CHAN4VOL_09)		, THIS(CHAN4CMD_0A)		, THIS(CHAN4VOL_0B)		, THIS(CHAN4INS_0A)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[10])					, &cb_ch4_vol_0A }, 
	{	0x16 , 0x0f , THIS(CHAN4VOL_0A)		, THIS(CHAN4CMD_0B)		, THIS(CHAN4VOL_0C)		, THIS(CHAN4INS_0B)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[11])					, &cb_ch4_vol_0B }, 
	{	0x16 , 0x10 , THIS(CHAN4VOL_0B)		, THIS(CHAN4CMD_0C)		, THIS(CHAN4VOL_0D)		, THIS(CHAN4INS_0C)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[12])					, &cb_ch4_vol_0C }, 
	{	0x16 , 0x11 , THIS(CHAN4VOL_0C)		, THIS(CHAN4CMD_0D)		, THIS(CHAN4VOL_0E)		, THIS(CHAN4INS_0D)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[13])					, &cb_ch4_vol_0D }, 
	{	0x16 , 0x12 , THIS(CHAN4VOL_0D)		, THIS(CHAN4CMD_0E)		, THIS(CHAN4VOL_0F)		, THIS(CHAN4INS_0E)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[14])					, &cb_ch4_vol_0E }, 
	{	0x16 , 0x13 , THIS(CHAN4VOL_0E)		, THIS(CHAN4CMD_0F)		, THIS(CHAN4VOL_00)		, THIS(CHAN4INS_0F)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[15])					, &cb_ch4_vol_0F }, 
	{	0x17 , 0x04 , THIS(CHAN4CMD_0F)		, THIS(CHAN4VAL_00)		, THIS(CHAN4CMD_01)		, THIS(CHAN4VOL_00)		, &CACHE_COMMANDS				, THISVAR(CMD[0]) 					, &cb_ch4_cmd_00 }, 
	{	0x17 , 0x05 , THIS(CHAN4CMD_00)		, THIS(CHAN4VAL_01)		, THIS(CHAN4CMD_02)		, THIS(CHAN4VOL_01)		, &CACHE_COMMANDS				, THISVAR(CMD[1]) 					, &cb_ch4_cmd_01 }, 
	{	0x17 , 0x06 , THIS(CHAN4CMD_01)		, THIS(CHAN4VAL_02)		, THIS(CHAN4CMD_03)		, THIS(CHAN4VOL_02)		, &CACHE_COMMANDS				, THISVAR(CMD[2]) 					, &cb_ch4_cmd_02 }, 
	{	0x17 , 0x07 , THIS(CHAN4CMD_02)		, THIS(CHAN4VAL_03)		, THIS(CHAN4CMD_04)		, THIS(CHAN4VOL_03)		, &CACHE_COMMANDS				, THISVAR(CMD[3]) 					, &cb_ch4_cmd_03 }, 
	{	0x17 , 0x08 , THIS(CHAN4CMD_03)		, THIS(CHAN4VAL_04)		, THIS(CHAN4CMD_05)		, THIS(CHAN4VOL_04)		, &CACHE_COMMANDS				, THISVAR(CMD[4]) 					, &cb_ch4_cmd_04 }, 
	{	0x17 , 0x09 , THIS(CHAN4CMD_04)		, THIS(CHAN4VAL_05)		, THIS(CHAN4CMD_06)		, THIS(CHAN4VOL_05)		, &CACHE_COMMANDS				, THISVAR(CMD[5]) 					, &cb_ch4_cmd_05 }, 
	{	0x17 , 0x0a , THIS(CHAN4CMD_05)		, THIS(CHAN4VAL_06)		, THIS(CHAN4CMD_07)		, THIS(CHAN4VOL_06)		, &CACHE_COMMANDS				, THISVAR(CMD[6]) 					, &cb_ch4_cmd_06 }, 
	{	0x17 , 0x0b , THIS(CHAN4CMD_06)		, THIS(CHAN4VAL_07)		, THIS(CHAN4CMD_08)		, THIS(CHAN4VOL_07)		, &CACHE_COMMANDS				, THISVAR(CMD[7]) 					, &cb_ch4_cmd_07 }, 
	{	0x17 , 0x0c , THIS(CHAN4CMD_07)		, THIS(CHAN4VAL_08)		, THIS(CHAN4CMD_09)		, THIS(CHAN4VOL_08)		, &CACHE_COMMANDS				, THISVAR(CMD[8]) 					, &cb_ch4_cmd_08 }, 
	{	0x17 , 0x0d , THIS(CHAN4CMD_08)		, THIS(CHAN4VAL_09)		, THIS(CHAN4CMD_0A)		, THIS(CHAN4VOL_09)		, &CACHE_COMMANDS				, THISVAR(CMD[9]) 					, &cb_ch4_cmd_09 }, 
	{	0x17 , 0x0e , THIS(CHAN4CMD_09)		, THIS(CHAN4VAL_0A)		, THIS(CHAN4CMD_0B)		, THIS(CHAN4VOL_0A)		, &CACHE_COMMANDS				, THISVAR(CMD[10])					, &cb_ch4_cmd_0A }, 
	{	0x17 , 0x0f , THIS(CHAN4CMD_0A)		, THIS(CHAN4VAL_0B)		, THIS(CHAN4CMD_0C)		, THIS(CHAN4VOL_0B)		, &CACHE_COMMANDS				, THISVAR(CMD[11])					, &cb_ch4_cmd_0B }, 
	{	0x17 , 0x10 , THIS(CHAN4CMD_0B)		, THIS(CHAN4VAL_0C)		, THIS(CHAN4CMD_0D)		, THIS(CHAN4VOL_0C)		, &CACHE_COMMANDS				, THISVAR(CMD[12])					, &cb_ch4_cmd_0C }, 
	{	0x17 , 0x11 , THIS(CHAN4CMD_0C)		, THIS(CHAN4VAL_0D)		, THIS(CHAN4CMD_0E)		, THIS(CHAN4VOL_0D)		, &CACHE_COMMANDS				, THISVAR(CMD[13])					, &cb_ch4_cmd_0D }, 
	{	0x17 , 0x12 , THIS(CHAN4CMD_0D)		, THIS(CHAN4VAL_0E)		, THIS(CHAN4CMD_0F)		, THIS(CHAN4VOL_0E)		, &CACHE_COMMANDS				, THISVAR(CMD[14])					, &cb_ch4_cmd_0E }, 
	{	0x17 , 0x13 , THIS(CHAN4CMD_0E)		, THIS(CHAN4VAL_0F)		, THIS(CHAN4CMD_00)		, THIS(CHAN4VOL_0F)		, &CACHE_COMMANDS				, THISVAR(CMD[15])					, &cb_ch4_cmd_0F }, 
	{	0x18 , 0x04 , THIS(CHAN4VAL_0F)		, THIS(CHAN4KEY_00)		, THIS(CHAN4VAL_01)		, THIS(CHAN4CMD_00)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[0]) 					, &cb_ch4_val_00 }, 
	{	0x18 , 0x05 , THIS(CHAN4VAL_00)		, THIS(CHAN4KEY_01)		, THIS(CHAN4VAL_02)		, THIS(CHAN4CMD_01)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[1]) 					, &cb_ch4_val_01 }, 
	{	0x18 , 0x06 , THIS(CHAN4VAL_01)		, THIS(CHAN4KEY_02)		, THIS(CHAN4VAL_03)		, THIS(CHAN4CMD_02)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[2]) 					, &cb_ch4_val_02 }, 
	{	0x18 , 0x07 , THIS(CHAN4VAL_02)		, THIS(CHAN4KEY_03)		, THIS(CHAN4VAL_04)		, THIS(CHAN4CMD_03)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[3]) 					, &cb_ch4_val_03 }, 
	{	0x18 , 0x08 , THIS(CHAN4VAL_03)		, THIS(CHAN4KEY_04)		, THIS(CHAN4VAL_05)		, THIS(CHAN4CMD_04)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[4]) 					, &cb_ch4_val_04 }, 
	{	0x18 , 0x09 , THIS(CHAN4VAL_04)		, THIS(CHAN4KEY_05)		, THIS(CHAN4VAL_06)		, THIS(CHAN4CMD_05)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[5]) 					, &cb_ch4_val_05 }, 
	{	0x18 , 0x0a , THIS(CHAN4VAL_05)		, THIS(CHAN4KEY_06)		, THIS(CHAN4VAL_07)		, THIS(CHAN4CMD_06)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[6]) 					, &cb_ch4_val_06 }, 
	{	0x18 , 0x0b , THIS(CHAN4VAL_06)		, THIS(CHAN4KEY_07)		, THIS(CHAN4VAL_08)		, THIS(CHAN4CMD_07)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[7]) 					, &cb_ch4_val_07 }, 
	{	0x18 , 0x0c , THIS(CHAN4VAL_07)		, THIS(CHAN4KEY_08)		, THIS(CHAN4VAL_09)		, THIS(CHAN4CMD_08)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[8]) 					, &cb_ch4_val_08 }, 
	{	0x18 , 0x0d , THIS(CHAN4VAL_08)		, THIS(CHAN4KEY_09)		, THIS(CHAN4VAL_0A)		, THIS(CHAN4CMD_09)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[9]) 					, &cb_ch4_val_09 }, 
	{	0x18 , 0x0e , THIS(CHAN4VAL_09)		, THIS(CHAN4KEY_0A)		, THIS(CHAN4VAL_0B)		, THIS(CHAN4CMD_0A)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[10])					, &cb_ch4_val_0A }, 
	{	0x18 , 0x0f , THIS(CHAN4VAL_0A)		, THIS(CHAN4KEY_0B)		, THIS(CHAN4VAL_0C)		, THIS(CHAN4CMD_0B)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[11])					, &cb_ch4_val_0B }, 
	{	0x18 , 0x10 , THIS(CHAN4VAL_0B)		, THIS(CHAN4KEY_0C)		, THIS(CHAN4VAL_0D)		, THIS(CHAN4CMD_0C)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[12])					, &cb_ch4_val_0C }, 
	{	0x18 , 0x11 , THIS(CHAN4VAL_0C)		, THIS(CHAN4KEY_0D)		, THIS(CHAN4VAL_0E)		, THIS(CHAN4CMD_0D)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[13])					, &cb_ch4_val_0D }, 
	{	0x18 , 0x12 , THIS(CHAN4VAL_0D)		, THIS(CHAN4KEY_0E)		, THIS(CHAN4VAL_0F)		, THIS(CHAN4CMD_0E)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[14])					, &cb_ch4_val_0E }, 
	{	0x18 , 0x13 , THIS(CHAN4VAL_0E)		, THIS(CHAN4KEY_0F)		, THIS(CHAN4VAL_00)		, THIS(CHAN4CMD_0F)		, &CACHE_HEXADECIMAL_INSTRUMENT	, THISVAR(VAL[15])					, &cb_ch4_val_0F }, 
	
	/*-------------------------------------------------------------------------------*/
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN0_CONTROLS[CONTROL_CHANNEL0_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[0].a))
	{	0x01 , 0x04 , THIS(CHAN0KEY_0F)		, THIS(CHAN0INS_00)		, THIS(CHAN0KEY_01)		, THIS(CHAN0VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch0_key_00 }, 
	{	0x01 , 0x05 , THIS(CHAN0KEY_00)		, THIS(CHAN0INS_01)		, THIS(CHAN0KEY_02)		, THIS(CHAN0VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch0_key_01 }, 
	{	0x01 , 0x06 , THIS(CHAN0KEY_01)		, THIS(CHAN0INS_02)		, THIS(CHAN0KEY_03)		, THIS(CHAN0VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch0_key_02 }, 
	{	0x01 , 0x07 , THIS(CHAN0KEY_02)		, THIS(CHAN0INS_03)		, THIS(CHAN0KEY_04)		, THIS(CHAN0VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch0_key_03 }, 
	{	0x01 , 0x08 , THIS(CHAN0KEY_03)		, THIS(CHAN0INS_04)		, THIS(CHAN0KEY_05)		, THIS(CHAN0VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch0_key_04 }, 
	{	0x01 , 0x09 , THIS(CHAN0KEY_04)		, THIS(CHAN0INS_05)		, THIS(CHAN0KEY_06)		, THIS(CHAN0VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch0_key_05 }, 
	{	0x01 , 0x0a , THIS(CHAN0KEY_05)		, THIS(CHAN0INS_06)		, THIS(CHAN0KEY_07)		, THIS(CHAN0VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch0_key_06 }, 
	{	0x01 , 0x0b , THIS(CHAN0KEY_06)		, THIS(CHAN0INS_07)		, THIS(CHAN0KEY_08)		, THIS(CHAN0VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch0_key_07 }, 
	{	0x01 , 0x0c , THIS(CHAN0KEY_07)		, THIS(CHAN0INS_08)		, THIS(CHAN0KEY_09)		, THIS(CHAN0VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch0_key_08 }, 
	{	0x01 , 0x0d , THIS(CHAN0KEY_08)		, THIS(CHAN0INS_09)		, THIS(CHAN0KEY_0A)		, THIS(CHAN0VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch0_key_09 }, 
	{	0x01 , 0x0e , THIS(CHAN0KEY_09)		, THIS(CHAN0INS_0A)		, THIS(CHAN0KEY_0B)		, THIS(CHAN0VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch0_key_0A }, 
	{	0x01 , 0x0f , THIS(CHAN0KEY_0A)		, THIS(CHAN0INS_0B)		, THIS(CHAN0KEY_0C)		, THIS(CHAN0VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch0_key_0B }, 
	{	0x01 , 0x10 , THIS(CHAN0KEY_0B)		, THIS(CHAN0INS_0C)		, THIS(CHAN0KEY_0D)		, THIS(CHAN0VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch0_key_0C }, 
	{	0x01 , 0x11 , THIS(CHAN0KEY_0C)		, THIS(CHAN0INS_0D)		, THIS(CHAN0KEY_0E)		, THIS(CHAN0VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch0_key_0D }, 
	{	0x01 , 0x12 , THIS(CHAN0KEY_0D)		, THIS(CHAN0INS_0E)		, THIS(CHAN0KEY_0F)		, THIS(CHAN0VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch0_key_0E }, 
	{	0x01 , 0x13 , THIS(CHAN0KEY_0E)		, THIS(CHAN0INS_0F)		, THIS(CHAN0KEY_00)		, THIS(CHAN0VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch0_key_0F }, 
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN1_CONTROLS[CONTROL_CHANNEL1_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[1].a))
	{	0x05 , 0x04 , THIS(CHAN1KEY_0F)		, THIS(CHAN1INS_00)		, THIS(CHAN1KEY_01)		, THIS(CHAN1VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch1_key_00 },
	{	0x05 , 0x05 , THIS(CHAN1KEY_00)		, THIS(CHAN1INS_01)		, THIS(CHAN1KEY_02)		, THIS(CHAN1VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch1_key_01 },
	{	0x05 , 0x06 , THIS(CHAN1KEY_01)		, THIS(CHAN1INS_02)		, THIS(CHAN1KEY_03)		, THIS(CHAN1VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch1_key_02 },
	{	0x05 , 0x07 , THIS(CHAN1KEY_02)		, THIS(CHAN1INS_03)		, THIS(CHAN1KEY_04)		, THIS(CHAN1VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch1_key_03 },
	{	0x05 , 0x08 , THIS(CHAN1KEY_03)		, THIS(CHAN1INS_04)		, THIS(CHAN1KEY_05)		, THIS(CHAN1VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch1_key_04 },
	{	0x05 , 0x09 , THIS(CHAN1KEY_04)		, THIS(CHAN1INS_05)		, THIS(CHAN1KEY_06)		, THIS(CHAN1VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch1_key_05 },
	{	0x05 , 0x0a , THIS(CHAN1KEY_05)		, THIS(CHAN1INS_06)		, THIS(CHAN1KEY_07)		, THIS(CHAN1VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch1_key_06 },
	{	0x05 , 0x0b , THIS(CHAN1KEY_06)		, THIS(CHAN1INS_07)		, THIS(CHAN1KEY_08)		, THIS(CHAN1VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch1_key_07 },
	{	0x05 , 0x0c , THIS(CHAN1KEY_07)		, THIS(CHAN1INS_08)		, THIS(CHAN1KEY_09)		, THIS(CHAN1VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch1_key_08 },
	{	0x05 , 0x0d , THIS(CHAN1KEY_08)		, THIS(CHAN1INS_09)		, THIS(CHAN1KEY_0A)		, THIS(CHAN1VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch1_key_09 },
	{	0x05 , 0x0e , THIS(CHAN1KEY_09)		, THIS(CHAN1INS_0A)		, THIS(CHAN1KEY_0B)		, THIS(CHAN1VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch1_key_0A },
	{	0x05 , 0x0f , THIS(CHAN1KEY_0A)		, THIS(CHAN1INS_0B)		, THIS(CHAN1KEY_0C)		, THIS(CHAN1VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch1_key_0B },
	{	0x05 , 0x10 , THIS(CHAN1KEY_0B)		, THIS(CHAN1INS_0C)		, THIS(CHAN1KEY_0D)		, THIS(CHAN1VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch1_key_0C },
	{	0x05 , 0x11 , THIS(CHAN1KEY_0C)		, THIS(CHAN1INS_0D)		, THIS(CHAN1KEY_0E)		, THIS(CHAN1VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch1_key_0D },
	{	0x05 , 0x12 , THIS(CHAN1KEY_0D)		, THIS(CHAN1INS_0E)		, THIS(CHAN1KEY_0F)		, THIS(CHAN1VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch1_key_0E },
	{	0x05 , 0x13 , THIS(CHAN1KEY_0E)		, THIS(CHAN1INS_0F)		, THIS(CHAN1KEY_00)		, THIS(CHAN1VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch1_key_0F },
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN2_CONTROLS[CONTROL_CHANNEL2_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[2].a))
	{	0x09 , 0x04 , THIS(CHAN2KEY_0F)		, THIS(CHAN2INS_00)		, THIS(CHAN2KEY_01)		, THIS(CHAN2VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch2_key_00 }, 
	{	0x09 , 0x05 , THIS(CHAN2KEY_00)		, THIS(CHAN2INS_01)		, THIS(CHAN2KEY_02)		, THIS(CHAN2VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch2_key_01 }, 
	{	0x09 , 0x06 , THIS(CHAN2KEY_01)		, THIS(CHAN2INS_02)		, THIS(CHAN2KEY_03)		, THIS(CHAN2VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch2_key_02 }, 
	{	0x09 , 0x07 , THIS(CHAN2KEY_02)		, THIS(CHAN2INS_03)		, THIS(CHAN2KEY_04)		, THIS(CHAN2VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch2_key_03 }, 
	{	0x09 , 0x08 , THIS(CHAN2KEY_03)		, THIS(CHAN2INS_04)		, THIS(CHAN2KEY_05)		, THIS(CHAN2VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch2_key_04 }, 
	{	0x09 , 0x09 , THIS(CHAN2KEY_04)		, THIS(CHAN2INS_05)		, THIS(CHAN2KEY_06)		, THIS(CHAN2VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch2_key_05 }, 
	{	0x09 , 0x0a , THIS(CHAN2KEY_05)		, THIS(CHAN2INS_06)		, THIS(CHAN2KEY_07)		, THIS(CHAN2VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch2_key_06 }, 
	{	0x09 , 0x0b , THIS(CHAN2KEY_06)		, THIS(CHAN2INS_07)		, THIS(CHAN2KEY_08)		, THIS(CHAN2VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch2_key_07 }, 
	{	0x09 , 0x0c , THIS(CHAN2KEY_07)		, THIS(CHAN2INS_08)		, THIS(CHAN2KEY_09)		, THIS(CHAN2VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch2_key_08 }, 
	{	0x09 , 0x0d , THIS(CHAN2KEY_08)		, THIS(CHAN2INS_09)		, THIS(CHAN2KEY_0A)		, THIS(CHAN2VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch2_key_09 }, 
	{	0x09 , 0x0e , THIS(CHAN2KEY_09)		, THIS(CHAN2INS_0A)		, THIS(CHAN2KEY_0B)		, THIS(CHAN2VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch2_key_0A }, 
	{	0x09 , 0x0f , THIS(CHAN2KEY_0A)		, THIS(CHAN2INS_0B)		, THIS(CHAN2KEY_0C)		, THIS(CHAN2VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch2_key_0B }, 
	{	0x09 , 0x10 , THIS(CHAN2KEY_0B)		, THIS(CHAN2INS_0C)		, THIS(CHAN2KEY_0D)		, THIS(CHAN2VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch2_key_0C }, 
	{	0x09 , 0x11 , THIS(CHAN2KEY_0C)		, THIS(CHAN2INS_0D)		, THIS(CHAN2KEY_0E)		, THIS(CHAN2VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch2_key_0D }, 
	{	0x09 , 0x12 , THIS(CHAN2KEY_0D)		, THIS(CHAN2INS_0E)		, THIS(CHAN2KEY_0F)		, THIS(CHAN2VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch2_key_0E }, 
	{	0x09 , 0x13 , THIS(CHAN2KEY_0E)		, THIS(CHAN2INS_0F)		, THIS(CHAN2KEY_00)		, THIS(CHAN2VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch2_key_0F }, 
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN3_CONTROLS[CONTROL_CHANNEL3_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[3].a))
	{	0x0d , 0x04 , THIS(CHAN3KEY_0F)		, THIS(CHAN3INS_00)		, THIS(CHAN3KEY_01)		, THIS(CHAN3VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch3_key_00 }, 
	{	0x0d , 0x05 , THIS(CHAN3KEY_00)		, THIS(CHAN3INS_01)		, THIS(CHAN3KEY_02)		, THIS(CHAN3VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch3_key_01 }, 
	{	0x0d , 0x06 , THIS(CHAN3KEY_01)		, THIS(CHAN3INS_02)		, THIS(CHAN3KEY_03)		, THIS(CHAN3VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch3_key_02 }, 
	{	0x0d , 0x07 , THIS(CHAN3KEY_02)		, THIS(CHAN3INS_03)		, THIS(CHAN3KEY_04)		, THIS(CHAN3VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch3_key_03 }, 
	{	0x0d , 0x08 , THIS(CHAN3KEY_03)		, THIS(CHAN3INS_04)		, THIS(CHAN3KEY_05)		, THIS(CHAN3VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch3_key_04 }, 
	{	0x0d , 0x09 , THIS(CHAN3KEY_04)		, THIS(CHAN3INS_05)		, THIS(CHAN3KEY_06)		, THIS(CHAN3VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch3_key_05 }, 
	{	0x0d , 0x0a , THIS(CHAN3KEY_05)		, THIS(CHAN3INS_06)		, THIS(CHAN3KEY_07)		, THIS(CHAN3VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch3_key_06 }, 
	{	0x0d , 0x0b , THIS(CHAN3KEY_06)		, THIS(CHAN3INS_07)		, THIS(CHAN3KEY_08)		, THIS(CHAN3VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch3_key_07 }, 
	{	0x0d , 0x0c , THIS(CHAN3KEY_07)		, THIS(CHAN3INS_08)		, THIS(CHAN3KEY_09)		, THIS(CHAN3VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch3_key_08 }, 
	{	0x0d , 0x0d , THIS(CHAN3KEY_08)		, THIS(CHAN3INS_09)		, THIS(CHAN3KEY_0A)		, THIS(CHAN3VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch3_key_09 }, 
	{	0x0d , 0x0e , THIS(CHAN3KEY_09)		, THIS(CHAN3INS_0A)		, THIS(CHAN3KEY_0B)		, THIS(CHAN3VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch3_key_0A }, 
	{	0x0d , 0x0f , THIS(CHAN3KEY_0A)		, THIS(CHAN3INS_0B)		, THIS(CHAN3KEY_0C)		, THIS(CHAN3VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch3_key_0B }, 
	{	0x0d , 0x10 , THIS(CHAN3KEY_0B)		, THIS(CHAN3INS_0C)		, THIS(CHAN3KEY_0D)		, THIS(CHAN3VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch3_key_0C }, 
	{	0x0d , 0x11 , THIS(CHAN3KEY_0C)		, THIS(CHAN3INS_0D)		, THIS(CHAN3KEY_0E)		, THIS(CHAN3VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch3_key_0D }, 
	{	0x0d , 0x12 , THIS(CHAN3KEY_0D)		, THIS(CHAN3INS_0E)		, THIS(CHAN3KEY_0F)		, THIS(CHAN3VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch3_key_0E }, 
	{	0x0d , 0x13 , THIS(CHAN3KEY_0E)		, THIS(CHAN3INS_0F)		, THIS(CHAN3KEY_00)		, THIS(CHAN3VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch3_key_0F }, 
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN5_CONTROLS[CONTROL_CHANNEL5_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[5].a))
	{	0x1b , 0x04 , THIS(CHAN5KEY_0F)		, THIS(CHAN5INS_00)		, THIS(CHAN5KEY_01)		, THIS(CHAN5VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch5_key_00 },
	{	0x1b , 0x05 , THIS(CHAN5KEY_00)		, THIS(CHAN5INS_01)		, THIS(CHAN5KEY_02)		, THIS(CHAN5VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch5_key_01 },
	{	0x1b , 0x06 , THIS(CHAN5KEY_01)		, THIS(CHAN5INS_02)		, THIS(CHAN5KEY_03)		, THIS(CHAN5VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch5_key_02 },
	{	0x1b , 0x07 , THIS(CHAN5KEY_02)		, THIS(CHAN5INS_03)		, THIS(CHAN5KEY_04)		, THIS(CHAN5VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch5_key_03 },
	{	0x1b , 0x08 , THIS(CHAN5KEY_03)		, THIS(CHAN5INS_04)		, THIS(CHAN5KEY_05)		, THIS(CHAN5VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch5_key_04 },
	{	0x1b , 0x09 , THIS(CHAN5KEY_04)		, THIS(CHAN5INS_05)		, THIS(CHAN5KEY_06)		, THIS(CHAN5VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch5_key_05 },
	{	0x1b , 0x0a , THIS(CHAN5KEY_05)		, THIS(CHAN5INS_06)		, THIS(CHAN5KEY_07)		, THIS(CHAN5VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch5_key_06 },
	{	0x1b , 0x0b , THIS(CHAN5KEY_06)		, THIS(CHAN5INS_07)		, THIS(CHAN5KEY_08)		, THIS(CHAN5VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch5_key_07 },
	{	0x1b , 0x0c , THIS(CHAN5KEY_07)		, THIS(CHAN5INS_08)		, THIS(CHAN5KEY_09)		, THIS(CHAN5VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch5_key_08 },
	{	0x1b , 0x0d , THIS(CHAN5KEY_08)		, THIS(CHAN5INS_09)		, THIS(CHAN5KEY_0A)		, THIS(CHAN5VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch5_key_09 },
	{	0x1b , 0x0e , THIS(CHAN5KEY_09)		, THIS(CHAN5INS_0A)		, THIS(CHAN5KEY_0B)		, THIS(CHAN5VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch5_key_0A },
	{	0x1b , 0x0f , THIS(CHAN5KEY_0A)		, THIS(CHAN5INS_0B)		, THIS(CHAN5KEY_0C)		, THIS(CHAN5VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch5_key_0B },
	{	0x1b , 0x10 , THIS(CHAN5KEY_0B)		, THIS(CHAN5INS_0C)		, THIS(CHAN5KEY_0D)		, THIS(CHAN5VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch5_key_0C },
	{	0x1b , 0x11 , THIS(CHAN5KEY_0C)		, THIS(CHAN5INS_0D)		, THIS(CHAN5KEY_0E)		, THIS(CHAN5VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch5_key_0D },
	{	0x1b , 0x12 , THIS(CHAN5KEY_0D)		, THIS(CHAN5INS_0E)		, THIS(CHAN5KEY_0F)		, THIS(CHAN5VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch5_key_0E },
	{	0x1b , 0x13 , THIS(CHAN5KEY_0E)		, THIS(CHAN5INS_0F)		, THIS(CHAN5KEY_00)		, THIS(CHAN5VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch5_key_0F },
	CONTROL_TERMINATOR
};
#undef THISVAR
#undef THIS

#define THIS(a) &CHAN5_CONTROLS[CONTROL_CHANNEL5_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[5].a))
const Control CHAN5_CONTROLS[CONTROL_CHANNEL5_0_MAX] = { 
	//  x	 y		up						right					down					left					cache							var									callback		
	{	0x15 , 0x04 , THIS(CHAN5KEY_0F)		, THIS(CHAN5INS_00)		, THIS(CHAN5KEY_01)		, THIS(CHAN5VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch5_key_00 }, 
	{	0x15 , 0x05 , THIS(CHAN5KEY_00)		, THIS(CHAN5INS_01)		, THIS(CHAN5KEY_02)		, THIS(CHAN5VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch5_key_01 }, 
	{	0x15 , 0x06 , THIS(CHAN5KEY_01)		, THIS(CHAN5INS_02)		, THIS(CHAN5KEY_03)		, THIS(CHAN5VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch5_key_02 }, 
	{	0x15 , 0x07 , THIS(CHAN5KEY_02)		, THIS(CHAN5INS_03)		, THIS(CHAN5KEY_04)		, THIS(CHAN5VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch5_key_03 }, 
	{	0x15 , 0x08 , THIS(CHAN5KEY_03)		, THIS(CHAN5INS_04)		, THIS(CHAN5KEY_05)		, THIS(CHAN5VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch5_key_04 }, 
	{	0x15 , 0x09 , THIS(CHAN5KEY_04)		, THIS(CHAN5INS_05)		, THIS(CHAN5KEY_06)		, THIS(CHAN5VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch5_key_05 }, 
	{	0x15 , 0x0a , THIS(CHAN5KEY_05)		, THIS(CHAN5INS_06)		, THIS(CHAN5KEY_07)		, THIS(CHAN5VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch5_key_06 }, 
	{	0x15 , 0x0b , THIS(CHAN5KEY_06)		, THIS(CHAN5INS_07)		, THIS(CHAN5KEY_08)		, THIS(CHAN5VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch5_key_07 }, 
	{	0x15 , 0x0c , THIS(CHAN5KEY_07)		, THIS(CHAN5INS_08)		, THIS(CHAN5KEY_09)		, THIS(CHAN5VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch5_key_08 }, 
	{	0x15 , 0x0d , THIS(CHAN5KEY_08)		, THIS(CHAN5INS_09)		, THIS(CHAN5KEY_0A)		, THIS(CHAN5VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch5_key_09 }, 
	{	0x15 , 0x0e , THIS(CHAN5KEY_09)		, THIS(CHAN5INS_0A)		, THIS(CHAN5KEY_0B)		, THIS(CHAN5VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch5_key_0A }, 
	{	0x15 , 0x0f , THIS(CHAN5KEY_0A)		, THIS(CHAN5INS_0B)		, THIS(CHAN5KEY_0C)		, THIS(CHAN5VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch5_key_0B }, 
	{	0x15 , 0x10 , THIS(CHAN5KEY_0B)		, THIS(CHAN5INS_0C)		, THIS(CHAN5KEY_0D)		, THIS(CHAN5VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch5_key_0C }, 
	{	0x15 , 0x11 , THIS(CHAN5KEY_0C)		, THIS(CHAN5INS_0D)		, THIS(CHAN5KEY_0E)		, THIS(CHAN5VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch5_key_0D }, 
	{	0x15 , 0x12 , THIS(CHAN5KEY_0D)		, THIS(CHAN5INS_0E)		, THIS(CHAN5KEY_0F)		, THIS(CHAN5VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch5_key_0E }, 
	{	0x15 , 0x13 , THIS(CHAN5KEY_0E)		, THIS(CHAN5INS_0F)		, THIS(CHAN5KEY_00)		, THIS(CHAN5VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch5_key_0F }, 
	{	0x18 , 0x04 , THIS(CHAN5INS_0F)		, THIS(CHAN5VOL_00)		, THIS(CHAN5INS_01)		, THIS(CHAN5KEY_00)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[0])					, &cb_ch5_ins_00 }, 
	{	0x18 , 0x05 , THIS(CHAN5INS_00)		, THIS(CHAN5VOL_01)		, THIS(CHAN5INS_02)		, THIS(CHAN5KEY_01)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[1])					, &cb_ch5_ins_01 }, 
	{	0x18 , 0x06 , THIS(CHAN5INS_01)		, THIS(CHAN5VOL_02)		, THIS(CHAN5INS_03)		, THIS(CHAN5KEY_02)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[2])					, &cb_ch5_ins_02 }, 
	{	0x18 , 0x07 , THIS(CHAN5INS_02)		, THIS(CHAN5VOL_03)		, THIS(CHAN5INS_04)		, THIS(CHAN5KEY_03)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[3])					, &cb_ch5_ins_03 }, 
	{	0x18 , 0x08 , THIS(CHAN5INS_03)		, THIS(CHAN5VOL_04)		, THIS(CHAN5INS_05)		, THIS(CHAN5KEY_04)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[4])					, &cb_ch5_ins_04 }, 
	{	0x18 , 0x09 , THIS(CHAN5INS_04)		, THIS(CHAN5VOL_05)		, THIS(CHAN5INS_06)		, THIS(CHAN5KEY_05)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[5])					, &cb_ch5_ins_05 }, 
	{	0x18 , 0x0a , THIS(CHAN5INS_05)		, THIS(CHAN5VOL_06)		, THIS(CHAN5INS_07)		, THIS(CHAN5KEY_06)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[6])					, &cb_ch5_ins_06 }, 
	{	0x18 , 0x0b , THIS(CHAN5INS_06)		, THIS(CHAN5VOL_07)		, THIS(CHAN5INS_08)		, THIS(CHAN5KEY_07)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[7])					, &cb_ch5_ins_07 }, 
	{	0x18 , 0x0c , THIS(CHAN5INS_07)		, THIS(CHAN5VOL_08)		, THIS(CHAN5INS_09)		, THIS(CHAN5KEY_08)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[8])					, &cb_ch5_ins_08 }, 
	{	0x18 , 0x0d , THIS(CHAN5INS_08)		, THIS(CHAN5VOL_09)		, THIS(CHAN5INS_0A)		, THIS(CHAN5KEY_09)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[9])					, &cb_ch5_ins_09 }, 
	{	0x18 , 0x0e , THIS(CHAN5INS_09)		, THIS(CHAN5VOL_0A)		, THIS(CHAN5INS_0B)		, THIS(CHAN5KEY_0A)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[10])					, &cb_ch5_ins_0A }, 
	{	0x18 , 0x0f , THIS(CHAN5INS_0A)		, THIS(CHAN5VOL_0B)		, THIS(CHAN5INS_0C)		, THIS(CHAN5KEY_0B)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[11])					, &cb_ch5_ins_0B }, 
	{	0x18 , 0x10 , THIS(CHAN5INS_0B)		, THIS(CHAN5VOL_0C)		, THIS(CHAN5INS_0D)		, THIS(CHAN5KEY_0C)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[12])					, &cb_ch5_ins_0C }, 
	{	0x18 , 0x11 , THIS(CHAN5INS_0C)		, THIS(CHAN5VOL_0D)		, THIS(CHAN5INS_0E)		, THIS(CHAN5KEY_0D)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[13])					, &cb_ch5_ins_0D }, 
	{	0x18 , 0x12 , THIS(CHAN5INS_0D)		, THIS(CHAN5VOL_0E)		, THIS(CHAN5INS_0F)		, THIS(CHAN5KEY_0E)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[14])					, &cb_ch5_ins_0E }, 
	{	0x18 , 0x13 , THIS(CHAN5INS_0E)		, THIS(CHAN5VOL_0F)		, THIS(CHAN5INS_00)		, THIS(CHAN5KEY_0F)		, &CACHE_HEXADECIMAL_TWOTILES	, THISVAR(INS[15])					, &cb_ch5_ins_0F }, 
	{	0x1a , 0x04 , THIS(CHAN5VOL_0F)		, THIS(CHAN5CMD_00)		, THIS(CHAN5VOL_01)		, THIS(CHAN5INS_00)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[0])					, &cb_ch5_vol_00 }, 
	{	0x1a , 0x05 , THIS(CHAN5VOL_00)		, THIS(CHAN5CMD_01)		, THIS(CHAN5VOL_02)		, THIS(CHAN5INS_01)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[1])					, &cb_ch5_vol_01 }, 
	{	0x1a , 0x06 , THIS(CHAN5VOL_01)		, THIS(CHAN5CMD_02)		, THIS(CHAN5VOL_03)		, THIS(CHAN5INS_02)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[2])					, &cb_ch5_vol_02 }, 
	{	0x1a , 0x07 , THIS(CHAN5VOL_02)		, THIS(CHAN5CMD_03)		, THIS(CHAN5VOL_04)		, THIS(CHAN5INS_03)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[3])					, &cb_ch5_vol_03 }, 
	{	0x1a , 0x08 , THIS(CHAN5VOL_03)		, THIS(CHAN5CMD_04)		, THIS(CHAN5VOL_05)		, THIS(CHAN5INS_04)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[4])					, &cb_ch5_vol_04 }, 
	{	0x1a , 0x09 , THIS(CHAN5VOL_04)		, THIS(CHAN5CMD_05)		, THIS(CHAN5VOL_06)		, THIS(CHAN5INS_05)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[5])					, &cb_ch5_vol_05 }, 
	{	0x1a , 0x0a , THIS(CHAN5VOL_05)		, THIS(CHAN5CMD_06)		, THIS(CHAN5VOL_07)		, THIS(CHAN5INS_06)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[6])					, &cb_ch5_vol_06 }, 
	{	0x1a , 0x0b , THIS(CHAN5VOL_06)		, THIS(CHAN5CMD_07)		, THIS(CHAN5VOL_08)		, THIS(CHAN5INS_07)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[7])					, &cb_ch5_vol_07 }, 
	{	0x1a , 0x0c , THIS(CHAN5VOL_07)		, THIS(CHAN5CMD_08)		, THIS(CHAN5VOL_09)		, THIS(CHAN5INS_08)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[8])					, &cb_ch5_vol_08 }, 
	{	0x1a , 0x0d , THIS(CHAN5VOL_08)		, THIS(CHAN5CMD_09)		, THIS(CHAN5VOL_0A)		, THIS(CHAN5INS_09)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[9])					, &cb_ch5_vol_09 }, 
	{	0x1a , 0x0e , THIS(CHAN5VOL_09)		, THIS(CHAN5CMD_0A)		, THIS(CHAN5VOL_0B)		, THIS(CHAN5INS_0A)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[10])					, &cb_ch5_vol_0A }, 
	{	0x1a , 0x0f , THIS(CHAN5VOL_0A)		, THIS(CHAN5CMD_0B)		, THIS(CHAN5VOL_0C)		, THIS(CHAN5INS_0B)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[11])					, &cb_ch5_vol_0B }, 
	{	0x1a , 0x10 , THIS(CHAN5VOL_0B)		, THIS(CHAN5CMD_0C)		, THIS(CHAN5VOL_0D)		, THIS(CHAN5INS_0C)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[12])					, &cb_ch5_vol_0C }, 
	{	0x1a , 0x11 , THIS(CHAN5VOL_0C)		, THIS(CHAN5CMD_0D)		, THIS(CHAN5VOL_0E)		, THIS(CHAN5INS_0D)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[13])					, &cb_ch5_vol_0D }, 
	{	0x1a , 0x12 , THIS(CHAN5VOL_0D)		, THIS(CHAN5CMD_0E)		, THIS(CHAN5VOL_0F)		, THIS(CHAN5INS_0E)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[14])					, &cb_ch5_vol_0E }, 
	{	0x1a , 0x13 , THIS(CHAN5VOL_0E)		, THIS(CHAN5CMD_0F)		, THIS(CHAN5VOL_00)		, THIS(CHAN5INS_0F)		, &CACHE_HEXADECIMAL			, THISVAR(VOL[15])					, &cb_ch5_vol_0F }, 
	{	0x1b , 0x04 , THIS(CHAN5CMD_0F)		, THIS(CHAN5VAL_00)		, THIS(CHAN5CMD_01)		, THIS(CHAN5VOL_00)		, &CACHE_COMMANDS				, THISVAR(CMD[0])					, &cb_ch5_cmd_00 }, 
	{	0x1b , 0x05 , THIS(CHAN5CMD_00)		, THIS(CHAN5VAL_01)		, THIS(CHAN5CMD_02)		, THIS(CHAN5VOL_01)		, &CACHE_COMMANDS				, THISVAR(CMD[1])					, &cb_ch5_cmd_01 }, 
	{	0x1b , 0x06 , THIS(CHAN5CMD_01)		, THIS(CHAN5VAL_02)		, THIS(CHAN5CMD_03)		, THIS(CHAN5VOL_02)		, &CACHE_COMMANDS				, THISVAR(CMD[2])					, &cb_ch5_cmd_02 }, 
	{	0x1b , 0x07 , THIS(CHAN5CMD_02)		, THIS(CHAN5VAL_03)		, THIS(CHAN5CMD_04)		, THIS(CHAN5VOL_03)		, &CACHE_COMMANDS				, THISVAR(CMD[3])					, &cb_ch5_cmd_03 }, 
	{	0x1b , 0x08 , THIS(CHAN5CMD_03)		, THIS(CHAN5VAL_04)		, THIS(CHAN5CMD_05)		, THIS(CHAN5VOL_04)		, &CACHE_COMMANDS				, THISVAR(CMD[4])					, &cb_ch5_cmd_04 }, 
	{	0x1b , 0x09 , THIS(CHAN5CMD_04)		, THIS(CHAN5VAL_05)		, THIS(CHAN5CMD_06)		, THIS(CHAN5VOL_05)		, &CACHE_COMMANDS				, THISVAR(CMD[5])					, &cb_ch5_cmd_05 }, 
	{	0x1b , 0x0a , THIS(CHAN5CMD_05)		, THIS(CHAN5VAL_06)		, THIS(CHAN5CMD_07)		, THIS(CHAN5VOL_06)		, &CACHE_COMMANDS				, THISVAR(CMD[6])					, &cb_ch5_cmd_06 }, 
	{	0x1b , 0x0b , THIS(CHAN5CMD_06)		, THIS(CHAN5VAL_07)		, THIS(CHAN5CMD_08)		, THIS(CHAN5VOL_07)		, &CACHE_COMMANDS				, THISVAR(CMD[7])					, &cb_ch5_cmd_07 }, 
	{	0x1b , 0x0c , THIS(CHAN5CMD_07)		, THIS(CHAN5VAL_08)		, THIS(CHAN5CMD_09)		, THIS(CHAN5VOL_08)		, &CACHE_COMMANDS				, THISVAR(CMD[8])					, &cb_ch5_cmd_08 }, 
	{	0x1b , 0x0d , THIS(CHAN5CMD_08)		, THIS(CHAN5VAL_09)		, THIS(CHAN5CMD_0A)		, THIS(CHAN5VOL_09)		, &CACHE_COMMANDS				, THISVAR(CMD[9])					, &cb_ch5_cmd_09 }, 
	{	0x1b , 0x0e , THIS(CHAN5CMD_09)		, THIS(CHAN5VAL_0A)		, THIS(CHAN5CMD_0B)		, THIS(CHAN5VOL_0A)		, &CACHE_COMMANDS				, THISVAR(CMD[10])					, &cb_ch5_cmd_0A }, 
	{	0x1b , 0x0f , THIS(CHAN5CMD_0A)		, THIS(CHAN5VAL_0B)		, THIS(CHAN5CMD_0C)		, THIS(CHAN5VOL_0B)		, &CACHE_COMMANDS				, THISVAR(CMD[11])					, &cb_ch5_cmd_0B }, 
	{	0x1b , 0x10 , THIS(CHAN5CMD_0B)		, THIS(CHAN5VAL_0C)		, THIS(CHAN5CMD_0D)		, THIS(CHAN5VOL_0C)		, &CACHE_COMMANDS				, THISVAR(CMD[12])					, &cb_ch5_cmd_0C }, 
	{	0x1b , 0x11 , THIS(CHAN5CMD_0C)		, THIS(CHAN5VAL_0D)		, THIS(CHAN5CMD_0E)		, THIS(CHAN5VOL_0D)		, &CACHE_COMMANDS				, THISVAR(CMD[13])					, &cb_ch5_cmd_0D }, 
	{	0x1b , 0x12 , THIS(CHAN5CMD_0D)		, THIS(CHAN5VAL_0E)		, THIS(CHAN5CMD_0F)		, THIS(CHAN5VOL_0E)		, &CACHE_COMMANDS				, THISVAR(CMD[14])					, &cb_ch5_cmd_0E }, 
	{	0x1b , 0x13 , THIS(CHAN5CMD_0E)		, THIS(CHAN5VAL_0F)		, THIS(CHAN5CMD_00)		, THIS(CHAN5VOL_0F)		, &CACHE_COMMANDS				, THISVAR(CMD[15])					, &cb_ch5_cmd_0F }, 
	{	0x1c , 0x04 , THIS(CHAN5VAL_0F)		, THIS(CHAN5KEY_00)		, THIS(CHAN5VAL_01)		, THIS(CHAN5CMD_00)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[0])					, &cb_ch5_val_00 }, 
	{	0x1c , 0x05 , THIS(CHAN5VAL_00)		, THIS(CHAN5KEY_01)		, THIS(CHAN5VAL_02)		, THIS(CHAN5CMD_01)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[1])					, &cb_ch5_val_01 }, 
	{	0x1c , 0x06 , THIS(CHAN5VAL_01)		, THIS(CHAN5KEY_02)		, THIS(CHAN5VAL_03)		, THIS(CHAN5CMD_02)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[2])					, &cb_ch5_val_02 }, 
	{	0x1c , 0x07 , THIS(CHAN5VAL_02)		, THIS(CHAN5KEY_03)		, THIS(CHAN5VAL_04)		, THIS(CHAN5CMD_03)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[3])					, &cb_ch5_val_03 }, 
	{	0x1c , 0x08 , THIS(CHAN5VAL_03)		, THIS(CHAN5KEY_04)		, THIS(CHAN5VAL_05)		, THIS(CHAN5CMD_04)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[4])					, &cb_ch5_val_04 }, 
	{	0x1c , 0x09 , THIS(CHAN5VAL_04)		, THIS(CHAN5KEY_05)		, THIS(CHAN5VAL_06)		, THIS(CHAN5CMD_05)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[5])					, &cb_ch5_val_05 }, 
	{	0x1c , 0x0a , THIS(CHAN5VAL_05)		, THIS(CHAN5KEY_06)		, THIS(CHAN5VAL_07)		, THIS(CHAN5CMD_06)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[6])					, &cb_ch5_val_06 }, 
	{	0x1c , 0x0b , THIS(CHAN5VAL_06)		, THIS(CHAN5KEY_07)		, THIS(CHAN5VAL_08)		, THIS(CHAN5CMD_07)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[7])					, &cb_ch5_val_07 }, 
	{	0x1c , 0x0c , THIS(CHAN5VAL_07)		, THIS(CHAN5KEY_08)		, THIS(CHAN5VAL_09)		, THIS(CHAN5CMD_08)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[8])					, &cb_ch5_val_08 }, 
	{	0x1c , 0x0d , THIS(CHAN5VAL_08)		, THIS(CHAN5KEY_09)		, THIS(CHAN5VAL_0A)		, THIS(CHAN5CMD_09)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[9])					, &cb_ch5_val_09 }, 
	{	0x1c , 0x0e , THIS(CHAN5VAL_09)		, THIS(CHAN5KEY_0A)		, THIS(CHAN5VAL_0B)		, THIS(CHAN5CMD_0A)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[10])					, &cb_ch5_val_0A }, 
	{	0x1c , 0x0f , THIS(CHAN5VAL_0A)		, THIS(CHAN5KEY_0B)		, THIS(CHAN5VAL_0C)		, THIS(CHAN5CMD_0B)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[11])					, &cb_ch5_val_0B }, 
	{	0x1c , 0x10 , THIS(CHAN5VAL_0B)		, THIS(CHAN5KEY_0C)		, THIS(CHAN5VAL_0D)		, THIS(CHAN5CMD_0C)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[12])					, &cb_ch5_val_0C }, 
	{	0x1c , 0x11 , THIS(CHAN5VAL_0C)		, THIS(CHAN5KEY_0D)		, THIS(CHAN5VAL_0E)		, THIS(CHAN5CMD_0D)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[13])					, &cb_ch5_val_0D }, 
	{	0x1c , 0x12 , THIS(CHAN5VAL_0D)		, THIS(CHAN5KEY_0E)		, THIS(CHAN5VAL_0F)		, THIS(CHAN5CMD_0E)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[14])					, &cb_ch5_val_0E }, 
	{	0x1c , 0x13 , THIS(CHAN5VAL_0E)		, THIS(CHAN5KEY_0F)		, THIS(CHAN5VAL_00)		, THIS(CHAN5CMD_0F)		, &CACHE_HEXADECIMAL_INSTRUMENT , THISVAR(VAL[15])					, &cb_ch5_val_0F }, 
#undef THIS
#undef THISVAR

#define THIS(a) &CHAN0_CONTROLS[CONTROL_CHANNEL0_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[0].a))
	{	0x01 , 0x04 , THIS(CHAN0KEY_0F)		, THIS(CHAN0INS_00)		, THIS(CHAN0KEY_01)		, THIS(CHAN0VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch0_key_00 }, 
	{	0x01 , 0x05 , THIS(CHAN0KEY_00)		, THIS(CHAN0INS_01)		, THIS(CHAN0KEY_02)		, THIS(CHAN0VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch0_key_01 }, 
	{	0x01 , 0x06 , THIS(CHAN0KEY_01)		, THIS(CHAN0INS_02)		, THIS(CHAN0KEY_03)		, THIS(CHAN0VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch0_key_02 }, 
	{	0x01 , 0x07 , THIS(CHAN0KEY_02)		, THIS(CHAN0INS_03)		, THIS(CHAN0KEY_04)		, THIS(CHAN0VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch0_key_03 }, 
	{	0x01 , 0x08 , THIS(CHAN0KEY_03)		, THIS(CHAN0INS_04)		, THIS(CHAN0KEY_05)		, THIS(CHAN0VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch0_key_04 }, 
	{	0x01 , 0x09 , THIS(CHAN0KEY_04)		, THIS(CHAN0INS_05)		, THIS(CHAN0KEY_06)		, THIS(CHAN0VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch0_key_05 }, 
	{	0x01 , 0x0a , THIS(CHAN0KEY_05)		, THIS(CHAN0INS_06)		, THIS(CHAN0KEY_07)		, THIS(CHAN0VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch0_key_06 }, 
	{	0x01 , 0x0b , THIS(CHAN0KEY_06)		, THIS(CHAN0INS_07)		, THIS(CHAN0KEY_08)		, THIS(CHAN0VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch0_key_07 }, 
	{	0x01 , 0x0c , THIS(CHAN0KEY_07)		, THIS(CHAN0INS_08)		, THIS(CHAN0KEY_09)		, THIS(CHAN0VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch0_key_08 }, 
	{	0x01 , 0x0d , THIS(CHAN0KEY_08)		, THIS(CHAN0INS_09)		, THIS(CHAN0KEY_0A)		, THIS(CHAN0VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch0_key_09 }, 
	{	0x01 , 0x0e , THIS(CHAN0KEY_09)		, THIS(CHAN0INS_0A)		, THIS(CHAN0KEY_0B)		, THIS(CHAN0VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch0_key_0A }, 
	{	0x01 , 0x0f , THIS(CHAN0KEY_0A)		, THIS(CHAN0INS_0B)		, THIS(CHAN0KEY_0C)		, THIS(CHAN0VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch0_key_0B }, 
	{	0x01 , 0x10 , THIS(CHAN0KEY_0B)		, THIS(CHAN0INS_0C)		, THIS(CHAN0KEY_0D)		, THIS(CHAN0VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch0_key_0C }, 
	{	0x01 , 0x11 , THIS(CHAN0KEY_0C)		, THIS(CHAN0INS_0D)		, THIS(CHAN0KEY_0E)		, THIS(CHAN0VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch0_key_0D }, 
	{	0x01 , 0x12 , THIS(CHAN0KEY_0D)		, THIS(CHAN0INS_0E)		, THIS(CHAN0KEY_0F)		, THIS(CHAN0VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch0_key_0E }, 
	{	0x01 , 0x13 , THIS(CHAN0KEY_0E)		, THIS(CHAN0INS_0F)		, THIS(CHAN0KEY_00)		, THIS(CHAN0VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch0_key_0F }, 
#undef THIS
#undef THISVAR

#define THIS(a) &CHAN1_CONTROLS[CONTROL_CHANNEL1_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[1].a))
	{	0x05 , 0x04 , THIS(CHAN1KEY_0F)		, THIS(CHAN1INS_00)		, THIS(CHAN1KEY_01)		, THIS(CHAN1VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch1_key_00 }, 
	{	0x05 , 0x05 , THIS(CHAN1KEY_00)		, THIS(CHAN1INS_01)		, THIS(CHAN1KEY_02)		, THIS(CHAN1VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch1_key_01 }, 
	{	0x05 , 0x06 , THIS(CHAN1KEY_01)		, THIS(CHAN1INS_02)		, THIS(CHAN1KEY_03)		, THIS(CHAN1VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch1_key_02 }, 
	{	0x05 , 0x07 , THIS(CHAN1KEY_02)		, THIS(CHAN1INS_03)		, THIS(CHAN1KEY_04)		, THIS(CHAN1VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch1_key_03 }, 
	{	0x05 , 0x08 , THIS(CHAN1KEY_03)		, THIS(CHAN1INS_04)		, THIS(CHAN1KEY_05)		, THIS(CHAN1VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch1_key_04 }, 
	{	0x05 , 0x09 , THIS(CHAN1KEY_04)		, THIS(CHAN1INS_05)		, THIS(CHAN1KEY_06)		, THIS(CHAN1VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch1_key_05 }, 
	{	0x05 , 0x0a , THIS(CHAN1KEY_05)		, THIS(CHAN1INS_06)		, THIS(CHAN1KEY_07)		, THIS(CHAN1VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch1_key_06 }, 
	{	0x05 , 0x0b , THIS(CHAN1KEY_06)		, THIS(CHAN1INS_07)		, THIS(CHAN1KEY_08)		, THIS(CHAN1VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch1_key_07 }, 
	{	0x05 , 0x0c , THIS(CHAN1KEY_07)		, THIS(CHAN1INS_08)		, THIS(CHAN1KEY_09)		, THIS(CHAN1VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch1_key_08 }, 
	{	0x05 , 0x0d , THIS(CHAN1KEY_08)		, THIS(CHAN1INS_09)		, THIS(CHAN1KEY_0A)		, THIS(CHAN1VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch1_key_09 }, 
	{	0x05 , 0x0e , THIS(CHAN1KEY_09)		, THIS(CHAN1INS_0A)		, THIS(CHAN1KEY_0B)		, THIS(CHAN1VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch1_key_0A }, 
	{	0x05 , 0x0f , THIS(CHAN1KEY_0A)		, THIS(CHAN1INS_0B)		, THIS(CHAN1KEY_0C)		, THIS(CHAN1VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch1_key_0B }, 
	{	0x05 , 0x10 , THIS(CHAN1KEY_0B)		, THIS(CHAN1INS_0C)		, THIS(CHAN1KEY_0D)		, THIS(CHAN1VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch1_key_0C }, 
	{	0x05 , 0x11 , THIS(CHAN1KEY_0C)		, THIS(CHAN1INS_0D)		, THIS(CHAN1KEY_0E)		, THIS(CHAN1VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch1_key_0D }, 
	{	0x05 , 0x12 , THIS(CHAN1KEY_0D)		, THIS(CHAN1INS_0E)		, THIS(CHAN1KEY_0F)		, THIS(CHAN1VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch1_key_0E }, 
	{	0x05 , 0x13 , THIS(CHAN1KEY_0E)		, THIS(CHAN1INS_0F)		, THIS(CHAN1KEY_00)		, THIS(CHAN1VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch1_key_0F }, 
#undef THIS
#undef THISVAR

#define THIS(a) &CHAN2_CONTROLS[CONTROL_CHANNEL2_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[2].a))
	{	0x09 , 0x04 , THIS(CHAN2KEY_0F)		, THIS(CHAN2INS_00)		, THIS(CHAN2KEY_01)		, THIS(CHAN2VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch2_key_00 }, 
	{	0x09 , 0x05 , THIS(CHAN2KEY_00)		, THIS(CHAN2INS_01)		, THIS(CHAN2KEY_02)		, THIS(CHAN2VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch2_key_01 },
	{	0x09 , 0x06 , THIS(CHAN2KEY_01)		, THIS(CHAN2INS_02)		, THIS(CHAN2KEY_03)		, THIS(CHAN2VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch2_key_02 },
	{	0x09 , 0x07 , THIS(CHAN2KEY_02)		, THIS(CHAN2INS_03)		, THIS(CHAN2KEY_04)		, THIS(CHAN2VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch2_key_03 },
	{	0x09 , 0x08 , THIS(CHAN2KEY_03)		, THIS(CHAN2INS_04)		, THIS(CHAN2KEY_05)		, THIS(CHAN2VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch2_key_04 },
	{	0x09 , 0x09 , THIS(CHAN2KEY_04)		, THIS(CHAN2INS_05)		, THIS(CHAN2KEY_06)		, THIS(CHAN2VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch2_key_05 },
	{	0x09 , 0x0a , THIS(CHAN2KEY_05)		, THIS(CHAN2INS_06)		, THIS(CHAN2KEY_07)		, THIS(CHAN2VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch2_key_06 },
	{	0x09 , 0x0b , THIS(CHAN2KEY_06)		, THIS(CHAN2INS_07)		, THIS(CHAN2KEY_08)		, THIS(CHAN2VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch2_key_07 },
	{	0x09 , 0x0c , THIS(CHAN2KEY_07)		, THIS(CHAN2INS_08)		, THIS(CHAN2KEY_09)		, THIS(CHAN2VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch2_key_08 },
	{	0x09 , 0x0d , THIS(CHAN2KEY_08)		, THIS(CHAN2INS_09)		, THIS(CHAN2KEY_0A)		, THIS(CHAN2VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch2_key_09 },
	{	0x09 , 0x0e , THIS(CHAN2KEY_09)		, THIS(CHAN2INS_0A)		, THIS(CHAN2KEY_0B)		, THIS(CHAN2VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch2_key_0A },
	{	0x09 , 0x0f , THIS(CHAN2KEY_0A)		, THIS(CHAN2INS_0B)		, THIS(CHAN2KEY_0C)		, THIS(CHAN2VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch2_key_0B },
	{	0x09 , 0x10 , THIS(CHAN2KEY_0B)		, THIS(CHAN2INS_0C)		, THIS(CHAN2KEY_0D)		, THIS(CHAN2VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch2_key_0C },
	{	0x09 , 0x11 , THIS(CHAN2KEY_0C)		, THIS(CHAN2INS_0D)		, THIS(CHAN2KEY_0E)		, THIS(CHAN2VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch2_key_0D },
	{	0x09 , 0x12 , THIS(CHAN2KEY_0D)		, THIS(CHAN2INS_0E)		, THIS(CHAN2KEY_0F)		, THIS(CHAN2VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch2_key_0E },
	{	0x09 , 0x13 , THIS(CHAN2KEY_0E)		, THIS(CHAN2INS_0F)		, THIS(CHAN2KEY_00)		, THIS(CHAN2VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch2_key_0F },
#undef THIS
#undef THISVAR

#define THIS(a) &CHAN3_CONTROLS[CONTROL_CHANNEL3_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[3].a))
	{	0x0d , 0x04 , THIS(CHAN3KEY_0F)		, THIS(CHAN3INS_00)		, THIS(CHAN3KEY_01)		, THIS(CHAN3VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch3_key_00 }, 
	{	0x0d , 0x05 , THIS(CHAN3KEY_00)		, THIS(CHAN3INS_01)		, THIS(CHAN3KEY_02)		, THIS(CHAN3VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch3_key_01 }, 
	{	0x0d , 0x06 , THIS(CHAN3KEY_01)		, THIS(CHAN3INS_02)		, THIS(CHAN3KEY_03)		, THIS(CHAN3VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch3_key_02 }, 
	{	0x0d , 0x07 , THIS(CHAN3KEY_02)		, THIS(CHAN3INS_03)		, THIS(CHAN3KEY_04)		, THIS(CHAN3VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch3_key_03 }, 
	{	0x0d , 0x08 , THIS(CHAN3KEY_03)		, THIS(CHAN3INS_04)		, THIS(CHAN3KEY_05)		, THIS(CHAN3VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch3_key_04 }, 
	{	0x0d , 0x09 , THIS(CHAN3KEY_04)		, THIS(CHAN3INS_05)		, THIS(CHAN3KEY_06)		, THIS(CHAN3VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch3_key_05 }, 
	{	0x0d , 0x0a , THIS(CHAN3KEY_05)		, THIS(CHAN3INS_06)		, THIS(CHAN3KEY_07)		, THIS(CHAN3VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch3_key_06 }, 
	{	0x0d , 0x0b , THIS(CHAN3KEY_06)		, THIS(CHAN3INS_07)		, THIS(CHAN3KEY_08)		, THIS(CHAN3VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch3_key_07 }, 
	{	0x0d , 0x0c , THIS(CHAN3KEY_07)		, THIS(CHAN3INS_08)		, THIS(CHAN3KEY_09)		, THIS(CHAN3VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch3_key_08 }, 
	{	0x0d , 0x0d , THIS(CHAN3KEY_08)		, THIS(CHAN3INS_09)		, THIS(CHAN3KEY_0A)		, THIS(CHAN3VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch3_key_09 }, 
	{	0x0d , 0x0e , THIS(CHAN3KEY_09)		, THIS(CHAN3INS_0A)		, THIS(CHAN3KEY_0B)		, THIS(CHAN3VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch3_key_0A }, 
	{	0x0d , 0x0f , THIS(CHAN3KEY_0A)		, THIS(CHAN3INS_0B)		, THIS(CHAN3KEY_0C)		, THIS(CHAN3VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch3_key_0B }, 
	{	0x0d , 0x10 , THIS(CHAN3KEY_0B)		, THIS(CHAN3INS_0C)		, THIS(CHAN3KEY_0D)		, THIS(CHAN3VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch3_key_0C }, 
	{	0x0d , 0x11 , THIS(CHAN3KEY_0C)		, THIS(CHAN3INS_0D)		, THIS(CHAN3KEY_0E)		, THIS(CHAN3VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch3_key_0D }, 
	{	0x0d , 0x12 , THIS(CHAN3KEY_0D)		, THIS(CHAN3INS_0E)		, THIS(CHAN3KEY_0F)		, THIS(CHAN3VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch3_key_0E }, 
	{	0x0d , 0x13 , THIS(CHAN3KEY_0E)		, THIS(CHAN3INS_0F)		, THIS(CHAN3KEY_00)		, THIS(CHAN3VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch3_key_0F }, 
#undef THIS
#undef THISVAR

#define THIS(a) &CHAN4_CONTROLS[CONTROL_CHANNEL4_0_##a]
#define THISVAR(a) ((u8*)&(VAR_CELLS[4].a))
	{	0x11 , 0x04 , THIS(CHAN4KEY_0F)		, THIS(CHAN4INS_00)		, THIS(CHAN4KEY_01)		, THIS(CHAN4VAL_00)		, &CACHE_NOTES					, THISVAR(KEY[0])					, &cb_ch4_key_00 }, 
	{	0x11 , 0x05 , THIS(CHAN4KEY_00)		, THIS(CHAN4INS_01)		, THIS(CHAN4KEY_02)		, THIS(CHAN4VAL_01)		, &CACHE_NOTES					, THISVAR(KEY[1])					, &cb_ch4_key_01 }, 
	{	0x11 , 0x06 , THIS(CHAN4KEY_01)		, THIS(CHAN4INS_02)		, THIS(CHAN4KEY_03)		, THIS(CHAN4VAL_02)		, &CACHE_NOTES					, THISVAR(KEY[2])					, &cb_ch4_key_02 }, 
	{	0x11 , 0x07 , THIS(CHAN4KEY_02)		, THIS(CHAN4INS_03)		, THIS(CHAN4KEY_04)		, THIS(CHAN4VAL_03)		, &CACHE_NOTES					, THISVAR(KEY[3])					, &cb_ch4_key_03 }, 
	{	0x11 , 0x08 , THIS(CHAN4KEY_03)		, THIS(CHAN4INS_04)		, THIS(CHAN4KEY_05)		, THIS(CHAN4VAL_04)		, &CACHE_NOTES					, THISVAR(KEY[4])					, &cb_ch4_key_04 }, 
	{	0x11 , 0x09 , THIS(CHAN4KEY_04)		, THIS(CHAN4INS_05)		, THIS(CHAN4KEY_06)		, THIS(CHAN4VAL_05)		, &CACHE_NOTES					, THISVAR(KEY[5])					, &cb_ch4_key_05 }, 
	{	0x11 , 0x0a , THIS(CHAN4KEY_05)		, THIS(CHAN4INS_06)		, THIS(CHAN4KEY_07)		, THIS(CHAN4VAL_06)		, &CACHE_NOTES					, THISVAR(KEY[6])					, &cb_ch4_key_06 }, 
	{	0x11 , 0x0b , THIS(CHAN4KEY_06)		, THIS(CHAN4INS_07)		, THIS(CHAN4KEY_08)		, THIS(CHAN4VAL_07)		, &CACHE_NOTES					, THISVAR(KEY[7])					, &cb_ch4_key_07 }, 
	{	0x11 , 0x0c , THIS(CHAN4KEY_07)		, THIS(CHAN4INS_08)		, THIS(CHAN4KEY_09)		, THIS(CHAN4VAL_08)		, &CACHE_NOTES					, THISVAR(KEY[8])					, &cb_ch4_key_08 }, 
	{	0x11 , 0x0d , THIS(CHAN4KEY_08)		, THIS(CHAN4INS_09)		, THIS(CHAN4KEY_0A)		, THIS(CHAN4VAL_09)		, &CACHE_NOTES					, THISVAR(KEY[9])					, &cb_ch4_key_09 }, 
	{	0x11 , 0x0e , THIS(CHAN4KEY_09)		, THIS(CHAN4INS_0A)		, THIS(CHAN4KEY_0B)		, THIS(CHAN4VAL_0A)		, &CACHE_NOTES					, THISVAR(KEY[10])					, &cb_ch4_key_0A }, 
	{	0x11 , 0x0f , THIS(CHAN4KEY_0A)		, THIS(CHAN4INS_0B)		, THIS(CHAN4KEY_0C)		, THIS(CHAN4VAL_0B)		, &CACHE_NOTES					, THISVAR(KEY[11])					, &cb_ch4_key_0B }, 
	{	0x11 , 0x10 , THIS(CHAN4KEY_0B)		, THIS(CHAN4INS_0C)		, THIS(CHAN4KEY_0D)		, THIS(CHAN4VAL_0C)		, &CACHE_NOTES					, THISVAR(KEY[12])					, &cb_ch4_key_0C }, 
	{	0x11 , 0x11 , THIS(CHAN4KEY_0C)		, THIS(CHAN4INS_0D)		, THIS(CHAN4KEY_0E)		, THIS(CHAN4VAL_0D)		, &CACHE_NOTES					, THISVAR(KEY[13])					, &cb_ch4_key_0D }, 
	{	0x11 , 0x12 , THIS(CHAN4KEY_0D)		, THIS(CHAN4INS_0E)		, THIS(CHAN4KEY_0F)		, THIS(CHAN4VAL_0E)		, &CACHE_NOTES					, THISVAR(KEY[14])					, &cb_ch4_key_0E }, 
	{	0x11 , 0x13 , THIS(CHAN4KEY_0E)		, THIS(CHAN4INS_0F)		, THIS(CHAN4KEY_00)		, THIS(CHAN4VAL_0F)		, &CACHE_NOTES					, THISVAR(KEY[15])					, &cb_ch4_key_0F }, 
	CONTROL_TERMINATOR
};
#undef THIS
#undef THISVAR
