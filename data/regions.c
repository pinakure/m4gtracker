/*

FILE DEPRECATED:
This regions must reside in each correspondent callback/xxx file

*/

#include "data.hpp"
#include "viewports.hpp"
#include "../modules/spu/mixer.hpp"
#include "../modules/mem/mem.hpp"


void updateCHANNEL0(RegionHandler* rh);
void updateCHANNEL1(RegionHandler* rh);
void updateCHANNEL2(RegionHandler* rh);
void updateCHANNEL3(RegionHandler* rh);
void updateCHANNEL4(RegionHandler* rh);
void updateCHANNEL5(RegionHandler* rh);
void updatePROGRESS(RegionHandler* rh);


#include "../callbacks/debug.hpp"
#include "../callbacks/ins.hpp"
#include "../callbacks/pat.hpp"
#include "../callbacks/trk.hpp"
#include "../callbacks/sng.hpp"
#include "../callbacks/hlp.hpp"
#include "../callbacks/snk.hpp"
#include "../callbacks/liv.hpp"
#include "../callbacks/cfg.hpp"


//----------------------------------------------------------------------------------------
//	REGIONS
//----------------------------------------------------------------------------------------
static u8 VARS_LIVE1			[CONTROL_LIVE1_MAX];	
static u8 VARS_LIVE2			[CONTROL_LIVE2_MAX];	
static u8 VARS_SNG				[CONTROL_SNG_MAX];	
static u8 VARS_INS_PWM			[CONTROL_INS_PWM_MAX];	
static u8 VARS_INS_WAV			[CONTROL_INS_WAV_MAX];	
static u8 VARS_INS_FMW			[CONTROL_INS_FMW_MAX];	
static u8 VARS_INS_SMP			[CONTROL_INS_SMP_MAX];	
static u8 VARS_PAT				[CONTROL_PAT_MAX];	
static u8 VARS_TABLE			[CONTROL_TABLE_MAX];	
static u8 VARS_CHAN0			[CONTROL_CHANNEL0_0_MAX];	
static u8 VARS_CHAN1			[CONTROL_CHANNEL1_0_MAX];	
static u8 VARS_CHAN2			[CONTROL_CHANNEL2_0_MAX];	
static u8 VARS_CHAN3			[CONTROL_CHANNEL3_0_MAX];	
static u8 VARS_CHAN4			[CONTROL_CHANNEL4_0_MAX];	
static u8 VARS_CHAN5			[CONTROL_CHANNEL5_0_MAX];	
static u8 VARS_LOOKNFEEL		[CONTROL_LOOKNFEEL_MAX];	
static u8 VARS_LINKMODE			[CONTROL_LINKMODE_MAX];	
static u8 VARS_BEHAVIOR			[CONTROL_BEHAVIOR_MAX];	
static u8 VARS_TRACKER			[CONTROL_TRACKER_MAX];	
static u8 VARS_MEMORY			[CONTROL_MEMORY_MAX];	
static u8 VARS_HLP				[1];
static u8 VARS_INS				[1];
static u8 VARS_VIS				[1];
static u8 VARS_TRK				[1];
static u8 VARS_CFG				[1];
static u8 VARS_SNK				[1];
static u8 VARS_COLOREDITOR		[1];
static u8 VARS_CHANNELMIXER		[1];
static u8 VARS_MEMORYTEST		[1];
static u8 VARS_MEMORYSONGMAP	[1];
static u8 VARS_LINKSTATUS		[1];
static u8 VARS_CREDITS			[1];
static u8 VARS_PROGRESS			[1];
static u8 VARS_HEADER0			[1];
static u8 VARS_HEADER1			[1];

// Maybe this can be used for display redraw controller vars

//										  { xadd , yadd	, x		, y     , width , heig	, up                 	, right					, down					, left				  	, map 	, dirty					, controls				, displays			, focus																		, controlVars		, updater				, viewport			name 
const Region REGION_MAP_1_LIVE2 		= { 0xFF , 0xFF	, 0x1e	, 0x00	, 0x1e	, 0x14	, &REGION_MAP_4_CFG		, &REGION_MAP_1_HLP		, &REGION_MAP_3_TRK		, &REGION_MAP_1_LIVE1	, 0x00	, &CACHE_DIRTY_MAP_1	, LIVE2_CONTROLS		, LIVE2_DISPLAYS	, ( Control* ) &( LIVE2_CONTROLS		[		 CONTROL_LIVE2_LIVEMODE ] ) , VARS_LIVE2 		, LiveSet::updatePiano	, NULL			}; /*LIVE2*/
const Region REGION_MAP_1_LIVE1 		= { 0xFF , 0xFF	, 0x00	, 0x00	, 0x1e	, 0x14	, &REGION_MAP_2_SNG		, &REGION_MAP_1_LIVE2	, &REGION_MAP_2_PAT		, &REGION_MAP_1_HLP		, 0x00	, &CACHE_DIRTY_MAP_1	, LIVE1_CONTROLS		, LIVE1_DISPLAYS	, ( Control* ) &( LIVE1_CONTROLS		[ 	  CONTROL_LIVE1_KEY_LEFT_00 ] ) , VARS_LIVE1 		, LiveSet::updateCmder	, NULL			}; /*LIVE1*/
const Region REGION_MAP_1_HLP 			= { 0xFF , 0xFF	, 0x00	, 0x14	, 0x1e	, 0x14	, &REGION_MAP_4_SNK		, &REGION_MAP_1_LIVE1	, &REGION_MAP_2_INS		, &REGION_MAP_1_LIVE2	, 0x00	, &CACHE_DIRTY_MAP_1	, NULL					, NULL				, NULL  											 						, VARS_HLP 			, Help::update			, NULL			}; /*HLP*/
const Region REGION_MAP_2_SNG 			= { 0xFF , 0xFF	, 0x1e	, 0x00	, 0x1e	, 0x14	, &REGION_MAP_2_PAT		, &REGION_MAP_4_CFG		, &REGION_MAP_1_LIVE1	, &REGION_MAP_4_SNK		, 0x01	, &CACHE_DIRTY_MAP_2	, SNG_CONTROLS			, SNG_DISPLAYS		, ( Control* ) &( SNG_CONTROLS			[ 			   CONTROL_SNG_LOAD ] ) , VARS_SNG 			, SongEdit::update		, NULL			}; /*SNG*/
const Region REGION_MAP_2_INS 			= { 0xFF , 0xFF	, 0x00	, 0x14	, 0x1e	, 0x14	, &REGION_MAP_1_HLP		, &REGION_MAP_2_PAT		, &REGION_MAP_4_SNK		, &REGION_MAP_3_TRK		, 0x01	, &CACHE_DIRTY_MAP_2	, NULL					, NULL				, NULL 																		, VARS_INS 			, NULL					, &VIEWPORT_INS	}; /*INS*/
const Region REGION_MAP_2_INS_WAV 		= { 0xFF , 0xFF	, 0x0e	, 0x2c	, 0x0e	, 0x14	, NULL					, NULL					, NULL					, NULL					, 0x01	, &CACHE_DIRTY_MAP_2	, INS_WAV_CONTROLS		, NULL				, ( Control* ) &( INS_WAV_CONTROLS		[ 		   CONTROL_INS_WAV_TYPE ] ) , VARS_INS_WAV 		, InstEdit::updateWav	, NULL			}; /*INS_WAV*/
const Region REGION_MAP_2_INS_FMW 		= { 0xFF , 0xFF	, 0x1c	, 0x2c	, 0x0e	, 0x14	, NULL					, NULL					, NULL					, NULL					, 0x01	, &CACHE_DIRTY_MAP_2	, INS_FMW_CONTROLS		, NULL				, ( Control* ) &( INS_FMW_CONTROLS		[ 		   CONTROL_INS_FMW_TYPE ] ) , VARS_INS_FMW 		, InstEdit::updateFmw	, NULL			}; /*INS_FMW*/
const Region REGION_MAP_2_INS_SMP 		= { 0xFF , 0xFF	, 0x2a	, 0x2c	, 0x0e	, 0x14	, NULL					, NULL					, NULL					, NULL					, 0x01	, &CACHE_DIRTY_MAP_2	, INS_SMP_CONTROLS		, NULL				, ( Control* ) &( INS_SMP_CONTROLS		[ 		   CONTROL_INS_SMP_TYPE ] ) , VARS_INS_SMP 		, InstEdit::updateSmp	, NULL			}; /*INS_SMP*/
const Region REGION_MAP_2_INS_PWM 		= { 0xFF , 0xFF	, 0x00	, 0x2c	, 0x0e	, 0x14	, NULL					, NULL					, NULL					, NULL					, 0x01	, &CACHE_DIRTY_MAP_2	, INS_PWM_CONTROLS		, NULL				, ( Control* ) &( INS_PWM_CONTROLS		[ 		   CONTROL_INS_PWM_TYPE ] ) , VARS_INS_PWM 		, InstEdit::updatePwm	, NULL			}; /*INS_PWM*/
const Region REGION_MAP_2_PAT 			= { 0xFF , 0xFF	, 0x00	, 0x00	, 0x1e	, 0x14	, &REGION_MAP_1_LIVE1	, &REGION_MAP_3_TRK		, &REGION_MAP_2_SNG		, &REGION_MAP_2_INS		, 0x01	, &CACHE_DIRTY_MAP_2	, PAT_CONTROLS			, PAT_DISPLAYS		, ( Control* ) &( PAT_CONTROLS			[ 							0x0 ] ) , VARS_PAT 			, PatEdit::update		, NULL			}; /*PAT*/
const Region REGION_MAP_2_TABLE 		= { 0xFF , 0xFF	, 0x1e	, 0x15	, 0x10	, 0x12	, NULL					, NULL					, NULL					, NULL					, 0x01	, NULL					, TABLE_CONTROLS		, TABLE_DISPLAYS	, ( Control* ) &( TABLE_CONTROLS 		[ 							0x0 ] ) , VARS_TABLE 		, Tracker::updateTable	, NULL			}; /*TABLE*/
const Region REGION_MAP_2_VIS 			= { 0xFF , 0xFF	, 0x1e	, 0x15	, 0x10	, 0x12	, NULL					, NULL					, NULL					, NULL					, 0x01	, &CACHE_DIRTY_MAP_2	, NULL					, VIS_DISPLAYS		, NULL 																		, VARS_VIS 			, Tracker::updateVis 	, NULL			}; /*VIS*/
const Region REGION_MAP_3_TRK 			= { 0xFF , 0xFF	, 0x00	, 0x00	, 0x1e	, 0x14	, &REGION_MAP_1_LIVE2	, &REGION_MAP_2_INS		, &REGION_MAP_4_CFG		, &REGION_MAP_2_PAT		, 0x02	, &CACHE_DIRTY_MAP_3	, NULL					, NULL				, NULL  																	, VARS_TRK			, NULL					, &VIEWPORT_TRK	}; /*TRK*/
const Region REGION_MAP_3_HEADER1 		= { 0xFF , 0xFF	, 0x00	, 0x3d	, 0x1e	, 0x03	, NULL					, NULL					, NULL					, NULL					, 0x02	, &CACHE_DIRTY_MAP_3	, NULL					, NULL				, NULL 																		, VARS_HEADER1 		, Tracker::updateHeader1, NULL			}; /*HEADER1*/
const Region REGION_MAP_3_HEADER0 		= { 0xFF , 0xFF	, 0x00	, 0x3a	, 0x1e	, 0x03	, NULL					, NULL					, NULL					, NULL					, 0x02	, &CACHE_DIRTY_MAP_3	, NULL					, NULL				, NULL 																		, VARS_HEADER0 		, Tracker::updateHeader2, NULL			}; /*HEADER0*/
const Region REGION_MAP_3_CHANNEL0 		= { 0x01 , 0x04	, 0x01	, 0x14	, 0x0a	, 0x11	, NULL					, NULL					, NULL					, NULL					, 0x02	, NULL					, CHAN0_CONTROLS		, CHAN0_DISPLAYS	, ( Control* ) &( CHAN0_CONTROLS		[ CONTROL_CHANNEL0_0_CHAN0KEY_00] ) , VARS_CHAN0 		, Tracker::globalUpdate	, NULL			}; /*CHANNEL0*/
const Region REGION_MAP_3_CHANNEL1 		= { 0x05 , 0x04	, 0x0a	, 0x14	, 0x0a	, 0x11	, NULL					, NULL					, NULL					, NULL					, 0x02	, NULL					, CHAN1_CONTROLS		, CHAN1_DISPLAYS	, ( Control* ) &( CHAN1_CONTROLS		[ CONTROL_CHANNEL1_0_CHAN1KEY_00] ) , VARS_CHAN1 		, Tracker::globalUpdate	, NULL			}; /*CHANNEL1*/
const Region REGION_MAP_3_CHANNEL2 		= { 0x09 , 0x04	, 0x14	, 0x14	, 0x0a	, 0x11	, NULL					, NULL					, NULL					, NULL					, 0x02	, NULL					, CHAN2_CONTROLS		, CHAN2_DISPLAYS	, ( Control* ) &( CHAN2_CONTROLS		[ CONTROL_CHANNEL2_0_CHAN2KEY_00] ) , VARS_CHAN2 		, Tracker::globalUpdate	, NULL			}; /*CHANNEL2*/
const Region REGION_MAP_3_CHANNEL3 		= { 0x0D , 0x04	, 0x1e	, 0x14	, 0x0a	, 0x11	, NULL					, NULL					, NULL					, NULL					, 0x02	, NULL					, CHAN3_CONTROLS		, CHAN3_DISPLAYS	, ( Control* ) &( CHAN3_CONTROLS		[ CONTROL_CHANNEL3_0_CHAN3KEY_00] ) , VARS_CHAN3 		, Tracker::globalUpdate	, NULL			}; /*CHANNEL3*/
const Region REGION_MAP_3_CHANNEL4 		= { 0x11 , 0x04	, 0x28	, 0x14	, 0x0a	, 0x11	, NULL					, NULL					, NULL					, NULL					, 0x02	, NULL					, CHAN4_CONTROLS		, CHAN4_DISPLAYS	, ( Control* ) &( CHAN4_CONTROLS		[ CONTROL_CHANNEL4_0_CHAN4KEY_00] ) , VARS_CHAN4 		, Tracker::globalUpdate	, NULL			}; /*CHANNEL4*/
const Region REGION_MAP_3_CHANNEL5 		= { 0x15 , 0x04	, 0x32	, 0x14	, 0x0a	, 0x11	, NULL					, NULL					, NULL					, NULL					, 0x02	, NULL					, CHAN5_CONTROLS		, CHAN5_DISPLAYS	, ( Control* ) &( CHAN5_CONTROLS		[ CONTROL_CHANNEL5_0_CHAN5KEY_00] ) , VARS_CHAN5 		, Tracker::globalUpdate	, NULL			}; /*CHANNEL5*/
const Region REGION_MAP_4_CFG 			= { 0xFF , 0xFF	, 0x00	, 0x00	, 0x1e	, 0x14	, &REGION_MAP_3_TRK		, &REGION_MAP_4_SNK		, &REGION_MAP_1_LIVE2	, &REGION_MAP_2_SNG		, 0x03	, &CACHE_DIRTY_MAP_4	, NULL					, NULL				, NULL 																		, VARS_CFG 			, Config::update		, &VIEWPORT_CFG	}; /*CFG*/
const Region REGION_MAP_4_SNK 			= { 0xFF , 0xFF	, 0x00	, 0x14	, 0x1e	, 0x14	, &REGION_MAP_2_INS		, &REGION_MAP_2_SNG		, &REGION_MAP_1_HLP		, &REGION_MAP_4_CFG		, 0x03	, NULL					, NULL					, NULL				, NULL 																		, VARS_SNK 			, SnakeGame::update		, NULL			}; /*SNK*/
const Region REGION_MAP_4_LOOKNFEEL 	= { 0xFF , 0xFF	, 0x1e	, 0x00	, 0x19	, 0x0a	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, LOOKNFEEL_CONTROLS	, NULL				, ( Control* ) &( LOOKNFEEL_CONTROLS	[ 							0x0 ] ) , VARS_LOOKNFEEL 	, LookNFeel::update		, NULL			}; /*LOOKNFEEL*/
const Region REGION_MAP_4_LINKMODE 		= { 0xFF , 0xFF	, 0x1e	, 0x0a	, 0x19	, 0x0a	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, LINKMODE_CONTROLS		, NULL				, ( Control* ) &( LINKMODE_CONTROLS		[ 							0x0 ] ) , VARS_LINKMODE 	, LinkMode::update 		, NULL			}; /*LINKMODE*/
const Region REGION_MAP_4_BEHAVIOR 		= { 0xFF , 0xFF	, 0x1e	, 0x14	, 0x19	, 0x0a	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, BEHAVIOR_CONTROLS		, NULL				, ( Control* ) &( BEHAVIOR_CONTROLS		[ 							0x0 ] ) , VARS_BEHAVIOR 	, Behavior::update		, NULL			}; /*BEHAVIOR*/
const Region REGION_MAP_4_TRACKER 		= { 0xFF , 0xFF	, 0x1e	, 0x1e	, 0x19	, 0x0a	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, TRACKER_CONTROLS		, NULL				, ( Control* ) &( TRACKER_CONTROLS		[ 							0x0 ] ) , VARS_TRACKER 		, Tracker::update		, NULL			}; /*TRACKER*/
const Region REGION_MAP_4_MEMORY 		= { 0xFF , 0xFF	, 0x1e	, 0x28	, 0x19	, 0x0a	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, MEMORY_CONTROLS		, NULL				, ( Control* ) &( MEMORY_CONTROLS		[ 							0x0 ] ) , VARS_MEMORY 		, Mem::update			, NULL			}; /*MEMORY*/
const Region REGION_MAP_4_COLOREDITOR 	= { 0xFF , 0xFF	, 0x00	, 0x28	, 0x12	, 0x04	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, NULL					, NULL				, NULL  																	, VARS_COLOREDITOR	, ColorEditor::update	, NULL			}; /*COLOREDITOR*/
const Region REGION_MAP_4_CHANNELMIXER 	= { 0xFF , 0xFF	, 0x00	, 0x34	, 0x12	, 0x04	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, NULL					, NULL				, NULL  																	, VARS_CHANNELMIXER , Mixer::update			, NULL			}; /*CHANNELMIXER*/
const Region REGION_MAP_4_MEMORYSONGMAP	= {	0xFF , 0xFF	, 0x00	, 0x38	, 0x12	, 0x04	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, NULL					, NULL				, NULL  																	, VARS_MEMORYSONGMAP, Config::memMap		, NULL			}; /*MEMORYSONGMAP*/
const Region REGION_MAP_4_MEMORYTEST 	= {	0xFF , 0xFF	, 0x00	, 0x3c	, 0x12	, 0x04	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, NULL					, NULL				, NULL  																	, VARS_MEMORYTEST 	, Debug::updateMemTest  , NULL			}; /*MEMORYTEST*/
const Region REGION_MAP_4_LINKSTATUS 	= {	0xFF , 0xFF	, 0x00	, 0x2c	, 0x12	, 0x04	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, NULL					, NULL				, NULL  																	, VARS_LINKSTATUS 	, LinkMode::monitor		, NULL			}; /*LINKSTATUS*/
const Region REGION_MAP_4_CREDITS 		= { 0xFF , 0xFF	, 0x00	, 0x30	, 0x12	, 0x04	, NULL					, NULL					, NULL					, NULL					, 0x03	, &CACHE_DIRTY_MAP_4	, NULL					, NULL				, NULL  																	, VARS_CREDITS 		, LookNFeel::credits	, NULL			}; /*CREDITS*/
const Region REGION_MAP_5_PROGRESS 		= { 0xFF , 0xFF	, 0x1c	, 0x00	, 0x0b	, 0x08	, NULL					, NULL					, NULL					, NULL					, 0x04	, NULL					, NULL					, NULL				, NULL  																	, VARS_PROGRESS 	, NULL					, NULL			}; /*PROGRESS*/
