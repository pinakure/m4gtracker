#include "../live.hpp"
#include "../../debug.hpp"
#include "../../data/data.hpp"
#include "../../kernel/key/key.hpp"
#include "../../kernel/gpu/gpu.hpp"
#include "../../kernel/sys/sys.hpp"
#include "../../kernel/spu/sequencer.hpp"
#include "../../kernel/spu/mixer.hpp"
#include "../../kernel/sram/sram.hpp"
#include "../../screens/songedit.hpp"

MEM_IN_EWRAM u8 		Performance::vars[ CONTROL_LIVE1_MAX ];	
MEM_IN_EWRAM LiveTable	Performance::LEFT;
MEM_IN_EWRAM LiveTable	Performance::RIGHT;
MEM_IN_EWRAM u8 		Performance::RETRIG;
MEM_IN_EWRAM u8 		Performance::SPEED /*4*/;
MEM_IN_EWRAM u8	 		Performance::QUANTIZE /*3*/;
MEM_IN_EWRAM u8			Performance::LOCK;

const Display LIVE1_DISPLAYS[LIVE1_DISPLAY_MAX] = { 
//					{ x		, y		, invert	, cache					, var								 , active		, redraw  	},
/* 	STATUS_LOCK	*/	{ 0x18	, 0x01	, false		, &CACHE_LOCKED			, (u8*)&( Live::PERFORM.LOCK		), false		, false 	},
/* 	STATUS_FREE	*/	{ 0x13	, 0x01	, true 		, &CACHE_FREE			, (u8*)&( Live::PERFORM.LOCK		), false		, false 	},
/* 	SELECT		*/	{ 0x13	, 0x11	, false		, &CACHE_SELECT			, (u8*)&( VAR_INPUT.SELECT			), false		, false 	},
/* 	START		*/	{ 0x09	, 0x00	, false		, &CACHE_START			, (u8*)&( VAR_INPUT.START			), false		, false 	},
/* 	RIGHT_A		*/	{ 0x1c	, 0x06	, false		, &CACHE_A				, (u8*)&( VAR_INPUT.A				), false		, false 	},
/* 	LEFT_A		*/	{ 0x01	, 0x06	, false		, &CACHE_A				, (u8*)&( VAR_INPUT.A				), false		, false 	},
/* 	RIGHT_B		*/	{ 0x1c	, 0x07	, false		, &CACHE_B				, (u8*)&( VAR_INPUT.B				), false		, false 	},
/* 	LEFT_B		*/	{ 0x01	, 0x07	, false		, &CACHE_B				, (u8*)&( VAR_INPUT.B				), false		, false 	},
/* 	RIGHT_UP	*/	{ 0x1c	, 0x08	, false		, &CACHE_UP				, (u8*)&( VAR_INPUT.UP				), false		, false 	},
/* 	LEFT_UP		*/	{ 0x01	, 0x08	, false		, &CACHE_UP				, (u8*)&( VAR_INPUT.UP				), false		, false 	},
/* 	RIGHT_RIGHT	*/	{ 0x1c	, 0x09	, false		, &CACHE_RIGHT			, (u8*)&( VAR_INPUT.RIGHT			), false		, false 	},
/* 	LEFT_RIGHT	*/	{ 0x01	, 0x09	, false		, &CACHE_RIGHT			, (u8*)&( VAR_INPUT.RIGHT			), false		, false 	},
/* 	RIGHT_DOWN	*/	{ 0x1c	, 0x0a	, false		, &CACHE_DOWN			, (u8*)&( VAR_INPUT.DOWN			), false		, false 	},
/* 	LEFT_DOWN	*/	{ 0x01	, 0x0a	, false		, &CACHE_DOWN			, (u8*)&( VAR_INPUT.DOWN			), false		, false 	},
/* 	RIGHT_LEFT	*/	{ 0x1c	, 0x0b	, false		, &CACHE_LEFT			, (u8*)&( VAR_INPUT.LEFT			), false		, false 	},
/* 	LEFT_LEFT	*/	{ 0x01	, 0x0b	, false		, &CACHE_LEFT			, (u8*)&( VAR_INPUT.LEFT			), false		, false 	},
/* 	RIGHT_L		*/	{ 0x1c	, 0x0c	, false		, &CACHE_L				, (u8*)&( VAR_INPUT.L				), false		, false 	},
/* 	LEFT_L		*/	{ 0x01	, 0x0c	, false		, &CACHE_L				, (u8*)&( VAR_INPUT.L				), false		, false 	},
/* 	RIGHT_R		*/	{ 0x1c	, 0x0d	, false		, &CACHE_R				, (u8*)&( VAR_INPUT.R				), false		, false 	},
/* 	LEFT_R		*/	{ 0x01	, 0x0d	, false		, &CACHE_R				, (u8*)&( VAR_INPUT.R				), false		, false 	},
DISPLAY_TERMINATOR
};

const Callback cb_live_retrig		= { modify1BIT		, EVENT_KEYDOWN_B 		, &Live::PERFORM.RETRIG		, NULL };
const Callback cb_live_speed	 	= { modify8BIT		, EVENT_MODIFY_B 		, &Live::PERFORM.SPEED		, NULL };
const Callback cb_live_quantize1	= { modify3BIT		, EVENT_MODIFY_B 		, &Live::PERFORM.QUANTIZE	, NULL };

#define CB_LIVE_LEFT(n, i, m)		const Callback cb_live_left_##n##_0##i  = { m, EVENT_MODIFY_B, &Live::PERFORM.LEFT.n[ i ]	, NULL }
#define CB_LIVE_RIGHT(n, i, m)		const Callback cb_live_right_##n##_0##i = { m, EVENT_MODIFY_B, &Live::PERFORM.RIGHT.n[ i ]	, NULL }

CB_LIVE_LEFT(VAL, 0, modifyValue);
CB_LIVE_LEFT(VAL, 1, modifyValue);
CB_LIVE_LEFT(VAL, 2, modifyValue);
CB_LIVE_LEFT(VAL, 3, modifyValue);
CB_LIVE_LEFT(VAL, 4, modifyValue);
CB_LIVE_LEFT(VAL, 5, modifyValue);
CB_LIVE_LEFT(VAL, 6, modifyValue);
CB_LIVE_LEFT(VAL, 7, modifyValue);

CB_LIVE_LEFT(VOL, 0, modifyVolume);
CB_LIVE_LEFT(VOL, 1, modifyVolume);
CB_LIVE_LEFT(VOL, 2, modifyVolume);
CB_LIVE_LEFT(VOL, 3, modifyVolume);
CB_LIVE_LEFT(VOL, 4, modifyVolume);
CB_LIVE_LEFT(VOL, 5, modifyVolume);
CB_LIVE_LEFT(VOL, 6, modifyVolume);
CB_LIVE_LEFT(VOL, 7, modifyVolume);

CB_LIVE_LEFT(INS, 0, modifyInst);
CB_LIVE_LEFT(INS, 1, modifyInst);
CB_LIVE_LEFT(INS, 2, modifyInst);
CB_LIVE_LEFT(INS, 3, modifyInst);
CB_LIVE_LEFT(INS, 4, modifyInst);
CB_LIVE_LEFT(INS, 5, modifyInst);
CB_LIVE_LEFT(INS, 6, modifyInst);
CB_LIVE_LEFT(INS, 7, modifyInst);

CB_LIVE_LEFT(CHAN, 0, modify6VAL);
CB_LIVE_LEFT(CHAN, 1, modify6VAL);
CB_LIVE_LEFT(CHAN, 2, modify6VAL);
CB_LIVE_LEFT(CHAN, 3, modify6VAL);
CB_LIVE_LEFT(CHAN, 4, modify6VAL);
CB_LIVE_LEFT(CHAN, 5, modify6VAL);
CB_LIVE_LEFT(CHAN, 6, modify6VAL);
CB_LIVE_LEFT(CHAN, 7, modify6VAL);

CB_LIVE_LEFT(KEY, 0, modifyNote);
CB_LIVE_LEFT(KEY, 1, modifyNote);
CB_LIVE_LEFT(KEY, 2, modifyNote);
CB_LIVE_LEFT(KEY, 3, modifyNote);
CB_LIVE_LEFT(KEY, 4, modifyNote);
CB_LIVE_LEFT(KEY, 5, modifyNote);
CB_LIVE_LEFT(KEY, 6, modifyNote);
CB_LIVE_LEFT(KEY, 7, modifyNote);

CB_LIVE_LEFT(CMD, 0, modifyCommand);
CB_LIVE_LEFT(CMD, 1, modifyCommand);
CB_LIVE_LEFT(CMD, 2, modifyCommand);
CB_LIVE_LEFT(CMD, 3, modifyCommand);
CB_LIVE_LEFT(CMD, 4, modifyCommand);
CB_LIVE_LEFT(CMD, 5, modifyCommand);
CB_LIVE_LEFT(CMD, 6, modifyCommand);
CB_LIVE_LEFT(CMD, 7, modifyCommand);

CB_LIVE_RIGHT(VAL, 0, modifyValue);
CB_LIVE_RIGHT(VAL, 1, modifyValue);
CB_LIVE_RIGHT(VAL, 2, modifyValue);
CB_LIVE_RIGHT(VAL, 3, modifyValue);
CB_LIVE_RIGHT(VAL, 4, modifyValue);
CB_LIVE_RIGHT(VAL, 5, modifyValue);
CB_LIVE_RIGHT(VAL, 6, modifyValue);
CB_LIVE_RIGHT(VAL, 7, modifyValue);

CB_LIVE_RIGHT(CMD, 0, modifyCommand);
CB_LIVE_RIGHT(CMD, 1, modifyCommand);
CB_LIVE_RIGHT(CMD, 2, modifyCommand);
CB_LIVE_RIGHT(CMD, 3, modifyCommand);
CB_LIVE_RIGHT(CMD, 4, modifyCommand);
CB_LIVE_RIGHT(CMD, 5, modifyCommand);
CB_LIVE_RIGHT(CMD, 6, modifyCommand);
CB_LIVE_RIGHT(CMD, 7, modifyCommand);

CB_LIVE_RIGHT(CHAN, 0, modify6VAL);
CB_LIVE_RIGHT(CHAN, 1, modify6VAL);
CB_LIVE_RIGHT(CHAN, 2, modify6VAL);
CB_LIVE_RIGHT(CHAN, 3, modify6VAL);
CB_LIVE_RIGHT(CHAN, 4, modify6VAL);
CB_LIVE_RIGHT(CHAN, 5, modify6VAL);
CB_LIVE_RIGHT(CHAN, 6, modify6VAL);
CB_LIVE_RIGHT(CHAN, 7, modify6VAL);

CB_LIVE_RIGHT(INS, 0, modifyInst);
CB_LIVE_RIGHT(INS, 1, modifyInst);
CB_LIVE_RIGHT(INS, 2, modifyInst);
CB_LIVE_RIGHT(INS, 3, modifyInst);
CB_LIVE_RIGHT(INS, 4, modifyInst);
CB_LIVE_RIGHT(INS, 5, modifyInst);
CB_LIVE_RIGHT(INS, 6, modifyInst);
CB_LIVE_RIGHT(INS, 7, modifyInst);

CB_LIVE_RIGHT(VOL, 0, modifyVolume);
CB_LIVE_RIGHT(VOL, 1, modifyVolume);
CB_LIVE_RIGHT(VOL, 2, modifyVolume);
CB_LIVE_RIGHT(VOL, 3, modifyVolume);
CB_LIVE_RIGHT(VOL, 4, modifyVolume);
CB_LIVE_RIGHT(VOL, 5, modifyVolume);
CB_LIVE_RIGHT(VOL, 6, modifyVolume);
CB_LIVE_RIGHT(VOL, 7, modifyVolume);

CB_LIVE_RIGHT(KEY, 0, modifyNote);
CB_LIVE_RIGHT(KEY, 1, modifyNote);
CB_LIVE_RIGHT(KEY, 2, modifyNote);
CB_LIVE_RIGHT(KEY, 3, modifyNote);
CB_LIVE_RIGHT(KEY, 4, modifyNote);
CB_LIVE_RIGHT(KEY, 5, modifyNote);
CB_LIVE_RIGHT(KEY, 6, modifyNote);
CB_LIVE_RIGHT(KEY, 7, modifyNote);
#undef CB_LIVE_LEFT
#undef CB_LIVE_RIGHT

#define CTL( a ) &LIVE1_CONTROLS[ CONTROL_LIVE1_##a ]
#define VAR( a ) ( ( u8* )&( Live::PERFORM.a ) )
const Control LIVE1_CONTROLS[ CONTROL_LIVE1_MAX ] = { 
//  { x	   , y	  , up						  , right				  , down				  , left				  , cache						, var					  , callback				},
	{ 0x0c , 0x0f , CTL( VAL_LEFT_07		) , NULL				  , CTL( SPEED			) , NULL				  , &CACHE_CHECK				, VAR( RETRIG			) , &cb_live_retrig 		},
	{ 0x0c , 0x10 , CTL( RETRIG				) , NULL				  , CTL( QUANTIZE1		) , NULL				  , &CACHE_HEXADECIMAL_DOUBLE	, VAR( SPEED			) , &cb_live_speed 			},
	{ 0x0c , 0x11 , CTL( SPEED				) , NULL				  , CTL( VAL_LEFT_00	) , NULL				  , &CACHE_QUANTIZES			, VAR( QUANTIZE			) , &cb_live_quantize1		},
	{ 0x0b , 0x06 , CTL( QUANTIZE1			) , CTL( KEY_RIGHT_00	) , CTL( VAL_LEFT_01	) , CTL( CMD_LEFT_00	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.VAL   [ 0 ] ) , &cb_live_left_VAL_00	},
	{ 0x0b , 0x07 , CTL( VAL_LEFT_00		) , CTL( KEY_RIGHT_01	) , CTL( VAL_LEFT_02	) , CTL( CMD_LEFT_01	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.VAL   [ 1 ] ) , &cb_live_left_VAL_01	},
	{ 0x0b , 0x08 , CTL( VAL_LEFT_01		) , CTL( KEY_RIGHT_02	) , CTL( VAL_LEFT_03	) , CTL( CMD_LEFT_02	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.VAL   [ 2 ] ) , &cb_live_left_VAL_02	},
	{ 0x0b , 0x09 , CTL( VAL_LEFT_02		) , CTL( KEY_RIGHT_03	) , CTL( VAL_LEFT_04	) , CTL( CMD_LEFT_03	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.VAL   [ 3 ] ) , &cb_live_left_VAL_03	},
	{ 0x0b , 0x0a , CTL( VAL_LEFT_03		) , CTL( KEY_RIGHT_04	) , CTL( VAL_LEFT_05	) , CTL( CMD_LEFT_04	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.VAL   [ 4 ] ) , &cb_live_left_VAL_04	},
	{ 0x0b , 0x0b , CTL( VAL_LEFT_04		) , CTL( KEY_RIGHT_05	) , CTL( VAL_LEFT_06	) , CTL( CMD_LEFT_05	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.VAL   [ 5 ] ) , &cb_live_left_VAL_05	},
	{ 0x0b , 0x0c , CTL( VAL_LEFT_05		) , CTL( KEY_RIGHT_06	) , CTL( VAL_LEFT_07	) , CTL( CMD_LEFT_06	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.VAL   [ 6 ] ) , &cb_live_left_VAL_06	},
	{ 0x0b , 0x0d , CTL( VAL_LEFT_06		) , CTL( KEY_RIGHT_07	) , CTL( RETRIG			) , CTL( CMD_LEFT_07	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.VAL   [ 7 ] ) , &cb_live_left_VAL_07	},
	{ 0x09 , 0x06 , CTL( QUANTIZE1			) , CTL( CMD_LEFT_00	) , CTL( VOL_LEFT_01	) , CTL( CHAN_LEFT_00	) , &CACHE_HEXADECIMAL			, VAR( LEFT.VOL   [ 0 ] ) , &cb_live_left_VOL_00	},
	{ 0x09 , 0x07 , CTL( VOL_LEFT_00		) , CTL( CMD_LEFT_01	) , CTL( VOL_LEFT_02	) , CTL( CHAN_LEFT_01	) , &CACHE_HEXADECIMAL			, VAR( LEFT.VOL   [ 1 ] ) , &cb_live_left_VOL_01	},
	{ 0x09 , 0x08 , CTL( VOL_LEFT_01		) , CTL( CMD_LEFT_02	) , CTL( VOL_LEFT_03	) , CTL( CHAN_LEFT_02	) , &CACHE_HEXADECIMAL			, VAR( LEFT.VOL   [ 2 ] ) , &cb_live_left_VOL_02	},
	{ 0x09 , 0x09 , CTL( VOL_LEFT_02		) , CTL( CMD_LEFT_03	) , CTL( VOL_LEFT_04	) , CTL( CHAN_LEFT_03	) , &CACHE_HEXADECIMAL			, VAR( LEFT.VOL   [ 3 ] ) , &cb_live_left_VOL_03	},
	{ 0x09 , 0x0a , CTL( VOL_LEFT_03		) , CTL( CMD_LEFT_04	) , CTL( VOL_LEFT_05	) , CTL( CHAN_LEFT_04	) , &CACHE_HEXADECIMAL			, VAR( LEFT.VOL   [ 4 ] ) , &cb_live_left_VOL_04	},
	{ 0x09 , 0x0b , CTL( VOL_LEFT_04		) , CTL( CMD_LEFT_05	) , CTL( VOL_LEFT_06	) , CTL( CHAN_LEFT_05	) , &CACHE_HEXADECIMAL			, VAR( LEFT.VOL   [ 5 ] ) , &cb_live_left_VOL_05	},
	{ 0x09 , 0x0c , CTL( VOL_LEFT_05		) , CTL( CMD_LEFT_06	) , CTL( VOL_LEFT_07	) , CTL( CHAN_LEFT_06	) , &CACHE_HEXADECIMAL			, VAR( LEFT.VOL   [ 6 ] ) , &cb_live_left_VOL_06	},
	{ 0x09 , 0x0d , CTL( VOL_LEFT_06		) , CTL( CMD_LEFT_07	) , CTL( RETRIG			) , CTL( CHAN_LEFT_07	) , &CACHE_HEXADECIMAL			, VAR( LEFT.VOL   [ 7 ] ) , &cb_live_left_VOL_07	},
	{ 0x06 , 0x06 , CTL( QUANTIZE1			) , CTL( CHAN_LEFT_00	) , CTL( INS_LEFT_01	) , CTL( KEY_LEFT_00	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.INS   [ 0 ] ) , &cb_live_left_INS_00	},
	{ 0x06 , 0x07 , CTL( INS_LEFT_00		) , CTL( CHAN_LEFT_01	) , CTL( INS_LEFT_02	) , CTL( KEY_LEFT_01	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.INS   [ 1 ] ) , &cb_live_left_INS_01	},
	{ 0x06 , 0x08 , CTL( INS_LEFT_01		) , CTL( CHAN_LEFT_02	) , CTL( INS_LEFT_03	) , CTL( KEY_LEFT_02	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.INS   [ 2 ] ) , &cb_live_left_INS_02	},
	{ 0x06 , 0x09 , CTL( INS_LEFT_02		) , CTL( CHAN_LEFT_03	) , CTL( INS_LEFT_04	) , CTL( KEY_LEFT_03	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.INS   [ 3 ] ) , &cb_live_left_INS_03	},
	{ 0x06 , 0x0a , CTL( INS_LEFT_03		) , CTL( CHAN_LEFT_04	) , CTL( INS_LEFT_05	) , CTL( KEY_LEFT_04	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.INS   [ 4 ] ) , &cb_live_left_INS_04	},
	{ 0x06 , 0x0b , CTL( INS_LEFT_04		) , CTL( CHAN_LEFT_05	) , CTL( INS_LEFT_06	) , CTL( KEY_LEFT_05	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.INS   [ 5 ] ) , &cb_live_left_INS_05	},
	{ 0x06 , 0x0c , CTL( INS_LEFT_05		) , CTL( CHAN_LEFT_06	) , CTL( INS_LEFT_07	) , CTL( KEY_LEFT_06	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.INS   [ 6 ] ) , &cb_live_left_INS_06	},
	{ 0x06 , 0x0d , CTL( INS_LEFT_06		) , CTL( CHAN_LEFT_07	) , CTL( RETRIG			) , CTL( KEY_LEFT_07	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( LEFT.INS   [ 7 ] ) , &cb_live_left_INS_07	},
	{ 0x08 , 0x06 , CTL( QUANTIZE1			) , CTL( VOL_LEFT_00	) , CTL( CHAN_LEFT_01	) , CTL( INS_LEFT_00	) , &CACHE_CHANNELS				, VAR( LEFT.CHAN  [ 0 ] ) , &cb_live_left_CHAN_00 	},
	{ 0x08 , 0x07 , CTL( CHAN_LEFT_00		) , CTL( VOL_LEFT_01	) , CTL( CHAN_LEFT_02	) , CTL( INS_LEFT_01	) , &CACHE_CHANNELS				, VAR( LEFT.CHAN  [ 1 ] ) , &cb_live_left_CHAN_01 	},
	{ 0x08 , 0x08 , CTL( CHAN_LEFT_01		) , CTL( VOL_LEFT_02	) , CTL( CHAN_LEFT_03	) , CTL( INS_LEFT_02	) , &CACHE_CHANNELS				, VAR( LEFT.CHAN  [ 2 ] ) , &cb_live_left_CHAN_02 	},
	{ 0x08 , 0x09 , CTL( CHAN_LEFT_02		) , CTL( VOL_LEFT_03	) , CTL( CHAN_LEFT_04	) , CTL( INS_LEFT_03	) , &CACHE_CHANNELS				, VAR( LEFT.CHAN  [ 3 ] ) , &cb_live_left_CHAN_03 	},
	{ 0x08 , 0x0a , CTL( CHAN_LEFT_03		) , CTL( VOL_LEFT_04	) , CTL( CHAN_LEFT_05	) , CTL( INS_LEFT_04	) , &CACHE_CHANNELS				, VAR( LEFT.CHAN  [ 4 ] ) , &cb_live_left_CHAN_04 	},
	{ 0x08 , 0x0b , CTL( CHAN_LEFT_04		) , CTL( VOL_LEFT_05	) , CTL( CHAN_LEFT_06	) , CTL( INS_LEFT_05	) , &CACHE_CHANNELS				, VAR( LEFT.CHAN  [ 5 ] ) , &cb_live_left_CHAN_05 	},
	{ 0x08 , 0x0c , CTL( CHAN_LEFT_05		) , CTL( VOL_LEFT_06	) , CTL( CHAN_LEFT_07	) , CTL( INS_LEFT_06	) , &CACHE_CHANNELS				, VAR( LEFT.CHAN  [ 6 ] ) , &cb_live_left_CHAN_06 	},
	{ 0x08 , 0x0d , CTL( CHAN_LEFT_06		) , CTL( VOL_LEFT_07	) , CTL( RETRIG			) , CTL( INS_LEFT_07	) , &CACHE_CHANNELS				, VAR( LEFT.CHAN  [ 7 ] ) , &cb_live_left_CHAN_07 	},
	{ 0x03 , 0x06 , CTL( QUANTIZE1			) , CTL( INS_LEFT_00	) , CTL( KEY_LEFT_01	) , CTL( VAL_RIGHT_00	) , &CACHE_NOTES				, VAR( LEFT.KEY   [ 0 ] ) , &cb_live_left_KEY_00 	},
	{ 0x03 , 0x07 , CTL( KEY_LEFT_00		) , CTL( INS_LEFT_01	) , CTL( KEY_LEFT_02	) , CTL( VAL_RIGHT_01	) , &CACHE_NOTES				, VAR( LEFT.KEY   [ 1 ] ) , &cb_live_left_KEY_01	},
	{ 0x03 , 0x08 , CTL( KEY_LEFT_01		) , CTL( INS_LEFT_02	) , CTL( KEY_LEFT_03	) , CTL( VAL_RIGHT_02	) , &CACHE_NOTES				, VAR( LEFT.KEY   [ 2 ] ) , &cb_live_left_KEY_02	},
	{ 0x03 , 0x09 , CTL( KEY_LEFT_02		) , CTL( INS_LEFT_03	) , CTL( KEY_LEFT_04	) , CTL( VAL_RIGHT_03	) , &CACHE_NOTES				, VAR( LEFT.KEY   [ 3 ] ) , &cb_live_left_KEY_03	},
	{ 0x03 , 0x0a , CTL( KEY_LEFT_03		) , CTL( INS_LEFT_04	) , CTL( KEY_LEFT_05	) , CTL( VAL_RIGHT_04	) , &CACHE_NOTES				, VAR( LEFT.KEY   [ 4 ] ) , &cb_live_left_KEY_04	},
	{ 0x03 , 0x0b , CTL( KEY_LEFT_04		) , CTL( INS_LEFT_05	) , CTL( KEY_LEFT_06	) , CTL( VAL_RIGHT_05	) , &CACHE_NOTES				, VAR( LEFT.KEY   [ 5 ] ) , &cb_live_left_KEY_05	},
	{ 0x03 , 0x0c , CTL( KEY_LEFT_05		) , CTL( INS_LEFT_06	) , CTL( KEY_LEFT_07	) , CTL( VAL_RIGHT_06	) , &CACHE_NOTES				, VAR( LEFT.KEY   [ 6 ] ) , &cb_live_left_KEY_06	},
	{ 0x03 , 0x0d , CTL( KEY_LEFT_06		) , CTL( INS_LEFT_07	) , CTL( RETRIG			) , CTL( VAL_RIGHT_07	) , &CACHE_NOTES				, VAR( LEFT.KEY   [ 7 ] ) , &cb_live_left_KEY_07	},
	{ 0x0a , 0x06 , CTL( QUANTIZE1			) , CTL( VAL_LEFT_00	) , CTL( CMD_LEFT_01	) , CTL( VOL_LEFT_00	) , &CACHE_COMMANDS				, VAR( LEFT.CMD   [ 0 ] ) , &cb_live_left_CMD_00	},
	{ 0x0a , 0x07 , CTL( CMD_LEFT_00		) , CTL( VAL_LEFT_01	) , CTL( CMD_LEFT_02	) , CTL( VOL_LEFT_01	) , &CACHE_COMMANDS				, VAR( LEFT.CMD   [ 1 ] ) , &cb_live_left_CMD_01	},
	{ 0x0a , 0x08 , CTL( CMD_LEFT_01		) , CTL( VAL_LEFT_02	) , CTL( CMD_LEFT_03	) , CTL( VOL_LEFT_02	) , &CACHE_COMMANDS				, VAR( LEFT.CMD   [ 2 ] ) , &cb_live_left_CMD_02	},
	{ 0x0a , 0x09 , CTL( CMD_LEFT_02		) , CTL( VAL_LEFT_03	) , CTL( CMD_LEFT_04	) , CTL( VOL_LEFT_03	) , &CACHE_COMMANDS				, VAR( LEFT.CMD   [ 3 ] ) , &cb_live_left_CMD_03	},
	{ 0x0a , 0x0a , CTL( CMD_LEFT_03		) , CTL( VAL_LEFT_04	) , CTL( CMD_LEFT_05	) , CTL( VOL_LEFT_04	) , &CACHE_COMMANDS				, VAR( LEFT.CMD   [ 4 ] ) , &cb_live_left_CMD_04	},
	{ 0x0a , 0x0b , CTL( CMD_LEFT_04		) , CTL( VAL_LEFT_05	) , CTL( CMD_LEFT_06	) , CTL( VOL_LEFT_05	) , &CACHE_COMMANDS				, VAR( LEFT.CMD   [ 5 ] ) , &cb_live_left_CMD_05	},
	{ 0x0a , 0x0c , CTL( CMD_LEFT_05		) , CTL( VAL_LEFT_06	) , CTL( CMD_LEFT_07	) , CTL( VOL_LEFT_06	) , &CACHE_COMMANDS				, VAR( LEFT.CMD   [ 6 ] ) , &cb_live_left_CMD_06	},
	{ 0x0a , 0x0d , CTL( CMD_LEFT_06		) , CTL( VAL_LEFT_07	) , CTL( RETRIG			) , CTL( VOL_LEFT_07	) , &CACHE_COMMANDS				, VAR( LEFT.CMD   [ 7 ] ) , &cb_live_left_CMD_07	},
	{ 0x19 , 0x06 , CTL( VAL_RIGHT_07		) , CTL( KEY_LEFT_00	) , CTL( VAL_RIGHT_01	) , CTL( CMD_RIGHT_00	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.VAL  [ 0 ] ) , &cb_live_right_VAL_00	},
	{ 0x19 , 0x07 , CTL( VAL_RIGHT_00		) , CTL( KEY_LEFT_01	) , CTL( VAL_RIGHT_02	) , CTL( CMD_RIGHT_01	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.VAL  [ 1 ] ) , &cb_live_right_VAL_01	},
	{ 0x19 , 0x08 , CTL( VAL_RIGHT_01		) , CTL( KEY_LEFT_02	) , CTL( VAL_RIGHT_03	) , CTL( CMD_RIGHT_02	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.VAL  [ 2 ] ) , &cb_live_right_VAL_02	},
	{ 0x19 , 0x09 , CTL( VAL_RIGHT_02		) , CTL( KEY_LEFT_03	) , CTL( VAL_RIGHT_04	) , CTL( CMD_RIGHT_03	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.VAL  [ 3 ] ) , &cb_live_right_VAL_03	},
	{ 0x19 , 0x0a , CTL( VAL_RIGHT_03		) , CTL( KEY_LEFT_04	) , CTL( VAL_RIGHT_05	) , CTL( CMD_RIGHT_04	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.VAL  [ 4 ] ) , &cb_live_right_VAL_04	},
	{ 0x19 , 0x0b , CTL( VAL_RIGHT_04		) , CTL( KEY_LEFT_05	) , CTL( VAL_RIGHT_06	) , CTL( CMD_RIGHT_05	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.VAL  [ 5 ] ) , &cb_live_right_VAL_05	},
	{ 0x19 , 0x0c , CTL( VAL_RIGHT_05		) , CTL( KEY_LEFT_06	) , CTL( VAL_RIGHT_07	) , CTL( CMD_RIGHT_06	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.VAL  [ 6 ] ) , &cb_live_right_VAL_06	},
	{ 0x19 , 0x0d , CTL( VAL_RIGHT_06		) , CTL( KEY_LEFT_07	) , CTL( VAL_RIGHT_00	) , CTL( CMD_RIGHT_07	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.VAL  [ 7 ] ) , &cb_live_right_VAL_07	},
	{ 0x18 , 0x06 , CTL( CMD_RIGHT_07		) , CTL( VAL_RIGHT_00	) , CTL( CMD_RIGHT_01	) , CTL( VOL_RIGHT_00	) , &CACHE_COMMANDS				, VAR( RIGHT.CMD  [ 0 ] ) , &cb_live_right_CMD_00	},
	{ 0x18 , 0x07 , CTL( CMD_RIGHT_00		) , CTL( VAL_RIGHT_01	) , CTL( CMD_RIGHT_02	) , CTL( VOL_RIGHT_01	) , &CACHE_COMMANDS				, VAR( RIGHT.CMD  [ 1 ] ) , &cb_live_right_CMD_01	},
	{ 0x18 , 0x08 , CTL( CMD_RIGHT_01		) , CTL( VAL_RIGHT_02	) , CTL( CMD_RIGHT_03	) , CTL( VOL_RIGHT_02	) , &CACHE_COMMANDS				, VAR( RIGHT.CMD  [ 2 ] ) , &cb_live_right_CMD_02	},
	{ 0x18 , 0x09 , CTL( CMD_RIGHT_02		) , CTL( VAL_RIGHT_03	) , CTL( CMD_RIGHT_04	) , CTL( VOL_RIGHT_03	) , &CACHE_COMMANDS				, VAR( RIGHT.CMD  [ 3 ] ) , &cb_live_right_CMD_03	},
	{ 0x18 , 0x0a , CTL( CMD_RIGHT_03		) , CTL( VAL_RIGHT_04	) , CTL( CMD_RIGHT_05	) , CTL( VOL_RIGHT_04	) , &CACHE_COMMANDS				, VAR( RIGHT.CMD  [ 4 ] ) , &cb_live_right_CMD_04	},
	{ 0x18 , 0x0b , CTL( CMD_RIGHT_04		) , CTL( VAL_RIGHT_05	) , CTL( CMD_RIGHT_06	) , CTL( VOL_RIGHT_05	) , &CACHE_COMMANDS				, VAR( RIGHT.CMD  [ 5 ] ) , &cb_live_right_CMD_05	},
	{ 0x18 , 0x0c , CTL( CMD_RIGHT_05		) , CTL( VAL_RIGHT_06	) , CTL( CMD_RIGHT_07	) , CTL( VOL_RIGHT_06	) , &CACHE_COMMANDS				, VAR( RIGHT.CMD  [ 6 ] ) , &cb_live_right_CMD_06	},
	{ 0x18 , 0x0d , CTL( CMD_RIGHT_06		) , CTL( VAL_RIGHT_07	) , CTL( CMD_RIGHT_00	) , CTL( VOL_RIGHT_07	) , &CACHE_COMMANDS				, VAR( RIGHT.CMD  [ 7 ] ) , &cb_live_right_CMD_07	},
	{ 0x16 , 0x06 , CTL( CHAN_RIGHT_07		) , CTL( VOL_RIGHT_00	) , CTL( CHAN_RIGHT_01	) , CTL( INS_RIGHT_00	) , &CACHE_CHANNELS				, VAR( RIGHT.CHAN [ 0 ] ) , &cb_live_right_CHAN_00 	},
	{ 0x16 , 0x07 , CTL( CHAN_RIGHT_00		) , CTL( VOL_RIGHT_01	) , CTL( CHAN_RIGHT_02	) , CTL( INS_RIGHT_01	) , &CACHE_CHANNELS				, VAR( RIGHT.CHAN [ 1 ] ) , &cb_live_right_CHAN_01 	},
	{ 0x16 , 0x08 , CTL( CHAN_RIGHT_01		) , CTL( VOL_RIGHT_02	) , CTL( CHAN_RIGHT_03	) , CTL( INS_RIGHT_02	) , &CACHE_CHANNELS				, VAR( RIGHT.CHAN [ 2 ] ) , &cb_live_right_CHAN_02 	},
	{ 0x16 , 0x09 , CTL( CHAN_RIGHT_02		) , CTL( VOL_RIGHT_03	) , CTL( CHAN_RIGHT_04	) , CTL( INS_RIGHT_03	) , &CACHE_CHANNELS				, VAR( RIGHT.CHAN [ 3 ] ) , &cb_live_right_CHAN_03 	},
	{ 0x16 , 0x0a , CTL( CHAN_RIGHT_03		) , CTL( VOL_RIGHT_04	) , CTL( CHAN_RIGHT_05	) , CTL( INS_RIGHT_04	) , &CACHE_CHANNELS				, VAR( RIGHT.CHAN [ 4 ] ) , &cb_live_right_CHAN_04 	},
	{ 0x16 , 0x0b , CTL( CHAN_RIGHT_04		) , CTL( VOL_RIGHT_05	) , CTL( CHAN_RIGHT_06	) , CTL( INS_RIGHT_05	) , &CACHE_CHANNELS				, VAR( RIGHT.CHAN [ 5 ] ) , &cb_live_right_CHAN_05 	},
	{ 0x16 , 0x0c , CTL( CHAN_RIGHT_05		) , CTL( VOL_RIGHT_06	) , CTL( CHAN_RIGHT_07	) , CTL( INS_RIGHT_06	) , &CACHE_CHANNELS				, VAR( RIGHT.CHAN [ 6 ] ) , &cb_live_right_CHAN_06 	},
	{ 0x16 , 0x0d , CTL( CHAN_RIGHT_06		) , CTL( VOL_RIGHT_07	) , CTL( CHAN_RIGHT_00	) , CTL( INS_RIGHT_07	) , &CACHE_CHANNELS				, VAR( RIGHT.CHAN [ 7 ] ) , &cb_live_right_CHAN_07 	},
	{ 0x14 , 0x06 , CTL( INS_RIGHT_07		) , CTL( CHAN_RIGHT_00	) , CTL( INS_RIGHT_01	) , CTL( KEY_RIGHT_00	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.INS  [ 0 ] ) , &cb_live_right_INS_00	},
	{ 0x14 , 0x07 , CTL( INS_RIGHT_00		) , CTL( CHAN_RIGHT_01	) , CTL( INS_RIGHT_02	) , CTL( KEY_RIGHT_01	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.INS  [ 1 ] ) , &cb_live_right_INS_01	},
	{ 0x14 , 0x08 , CTL( INS_RIGHT_01		) , CTL( CHAN_RIGHT_02	) , CTL( INS_RIGHT_03	) , CTL( KEY_RIGHT_02	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.INS  [ 2 ] ) , &cb_live_right_INS_02	},
	{ 0x14 , 0x09 , CTL( INS_RIGHT_02		) , CTL( CHAN_RIGHT_03	) , CTL( INS_RIGHT_04	) , CTL( KEY_RIGHT_03	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.INS  [ 3 ] ) , &cb_live_right_INS_03	},
	{ 0x14 , 0x0a , CTL( INS_RIGHT_03		) , CTL( CHAN_RIGHT_04	) , CTL( INS_RIGHT_05	) , CTL( KEY_RIGHT_04	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.INS  [ 4 ] ) , &cb_live_right_INS_04	},
	{ 0x14 , 0x0b , CTL( INS_RIGHT_04		) , CTL( CHAN_RIGHT_05	) , CTL( INS_RIGHT_06	) , CTL( KEY_RIGHT_05	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.INS  [ 5 ] ) , &cb_live_right_INS_05	},
	{ 0x14 , 0x0c , CTL( INS_RIGHT_05		) , CTL( CHAN_RIGHT_06	) , CTL( INS_RIGHT_07	) , CTL( KEY_RIGHT_06	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.INS  [ 6 ] ) , &cb_live_right_INS_06	},
	{ 0x14 , 0x0d , CTL( INS_RIGHT_06		) , CTL( CHAN_RIGHT_07	) , CTL( INS_RIGHT_00	) , CTL( KEY_RIGHT_07	) , &CACHE_HEXADECIMAL_TWOTILES	, VAR( RIGHT.INS  [ 7 ] ) , &cb_live_right_INS_07	},
	{ 0x17 , 0x06 , CTL( VOL_RIGHT_07		) , CTL( CMD_RIGHT_00	) , CTL( VOL_RIGHT_01	) , CTL( CHAN_RIGHT_00	) , &CACHE_HEXADECIMAL			, VAR( RIGHT.VOL  [ 0 ] ) , &cb_live_right_VOL_00	},
	{ 0x17 , 0x07 , CTL( VOL_RIGHT_00		) , CTL( CMD_RIGHT_01	) , CTL( VOL_RIGHT_02	) , CTL( CHAN_RIGHT_01	) , &CACHE_HEXADECIMAL			, VAR( RIGHT.VOL  [ 1 ] ) , &cb_live_right_VOL_01	},
	{ 0x17 , 0x08 , CTL( VOL_RIGHT_01		) , CTL( CMD_RIGHT_02	) , CTL( VOL_RIGHT_03	) , CTL( CHAN_RIGHT_02	) , &CACHE_HEXADECIMAL			, VAR( RIGHT.VOL  [ 2 ] ) , &cb_live_right_VOL_02	},
	{ 0x17 , 0x09 , CTL( VOL_RIGHT_02		) , CTL( CMD_RIGHT_03	) , CTL( VOL_RIGHT_04	) , CTL( CHAN_RIGHT_03	) , &CACHE_HEXADECIMAL			, VAR( RIGHT.VOL  [ 3 ] ) , &cb_live_right_VOL_03	},
	{ 0x17 , 0x0a , CTL( VOL_RIGHT_03		) , CTL( CMD_RIGHT_04	) , CTL( VOL_RIGHT_05	) , CTL( CHAN_RIGHT_04	) , &CACHE_HEXADECIMAL			, VAR( RIGHT.VOL  [ 4 ] ) , &cb_live_right_VOL_04	},
	{ 0x17 , 0x0b , CTL( VOL_RIGHT_04		) , CTL( CMD_RIGHT_05	) , CTL( VOL_RIGHT_06	) , CTL( CHAN_RIGHT_05	) , &CACHE_HEXADECIMAL			, VAR( RIGHT.VOL  [ 5 ] ) , &cb_live_right_VOL_05	},
	{ 0x17 , 0x0c , CTL( VOL_RIGHT_05		) , CTL( CMD_RIGHT_06	) , CTL( VOL_RIGHT_07	) , CTL( CHAN_RIGHT_06	) , &CACHE_HEXADECIMAL			, VAR( RIGHT.VOL  [ 6 ] ) , &cb_live_right_VOL_06	},
	{ 0x17 , 0x0d , CTL( VOL_RIGHT_06		) , CTL( CMD_RIGHT_07	) , CTL( VOL_RIGHT_00	) , CTL( CHAN_RIGHT_07	) , &CACHE_HEXADECIMAL			, VAR( RIGHT.VOL  [ 7 ] ) , &cb_live_right_VOL_07	},
	{ 0x11 , 0x06 , CTL( KEY_RIGHT_07		) , CTL( INS_RIGHT_00	) , CTL( KEY_RIGHT_01	) , CTL( VAL_LEFT_00  	) , &CACHE_NOTES				, VAR( RIGHT.KEY  [ 0 ] ) , &cb_live_right_KEY_00 	},
	{ 0x11 , 0x07 , CTL( KEY_RIGHT_00		) , CTL( INS_RIGHT_01	) , CTL( KEY_RIGHT_02	) , CTL( VAL_LEFT_01  	) , &CACHE_NOTES				, VAR( RIGHT.KEY  [ 1 ] ) , &cb_live_right_KEY_01	},
	{ 0x11 , 0x08 , CTL( KEY_RIGHT_01		) , CTL( INS_RIGHT_02	) , CTL( KEY_RIGHT_03	) , CTL( VAL_LEFT_02  	) , &CACHE_NOTES				, VAR( RIGHT.KEY  [ 2 ] ) , &cb_live_right_KEY_02	},
	{ 0x11 , 0x09 , CTL( KEY_RIGHT_02		) , CTL( INS_RIGHT_03	) , CTL( KEY_RIGHT_04	) , CTL( VAL_LEFT_03  	) , &CACHE_NOTES				, VAR( RIGHT.KEY  [ 3 ] ) , &cb_live_right_KEY_03	},
	{ 0x11 , 0x0a , CTL( KEY_RIGHT_03		) , CTL( INS_RIGHT_04	) , CTL( KEY_RIGHT_05	) , CTL( VAL_LEFT_04  	) , &CACHE_NOTES				, VAR( RIGHT.KEY  [ 4 ] ) , &cb_live_right_KEY_04	},
	{ 0x11 , 0x0b , CTL( KEY_RIGHT_04		) , CTL( INS_RIGHT_05	) , CTL( KEY_RIGHT_06	) , CTL( VAL_LEFT_05  	) , &CACHE_NOTES				, VAR( RIGHT.KEY  [ 5 ] ) , &cb_live_right_KEY_05	},
	{ 0x11 , 0x0c , CTL( KEY_RIGHT_05		) , CTL( INS_RIGHT_06	) , CTL( KEY_RIGHT_07	) , CTL( VAL_LEFT_06  	) , &CACHE_NOTES				, VAR( RIGHT.KEY  [ 6 ] ) , &cb_live_right_KEY_06	},
	{ 0x11 , 0x0d , CTL( KEY_RIGHT_06		) , CTL( INS_RIGHT_07	) , CTL( KEY_RIGHT_00	) , CTL( VAL_LEFT_07  	) , &CACHE_NOTES				, VAR( RIGHT.KEY  [ 7 ] ) , &cb_live_right_KEY_07	},
	CONTROL_TERMINATOR
};
#undef VAR
#undef CTL

void Performance::update(  ){
	if(KEYDOWN_START){
		RegionHandler::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&RegionHandler::region->displays[LIVE1_DISPLAY_STATUS_FREE])&0x0fffffff);
		Live::PERFORM.LOCK ^= 1;
		if(!Sequencer::playing){
			if(Live::PERFORM.LOCK) Mixer::start();
			else Mixer::stop();
		}
		RegionHandler::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&RegionHandler::region->displays[LIVE1_DISPLAY_STATUS_LOCKED])&0x0fffffff);
		Sys::forceNoInput();
	}
	
	if( !Live::PERFORM.LOCK ) return;
	
	if(KEYDOWN_START || KEYUP_START){
		VAR_INPUT.START = KEYDOWN_START ? 1 : 0;
		RegionHandler::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&RegionHandler::region->displays[LIVE1_DISPLAY_START])&0x0fffffff);
	}

	if(KEYDOWN_SELECT || KEYUP_SELECT){
		VAR_INPUT.SELECT = KEYDOWN_SELECT ? 1 : 0;
		RegionHandler::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&RegionHandler::region->displays[LIVE1_DISPLAY_SELECT])&0x0fffffff);
	}
	
	
	#define MONITOR(a)	if( KEYDOWN_##a || KEYUP_##a ) {		\
							VAR_INPUT.a = KEYDOWN_##a ? 1 : 0;	\
							RegionHandler::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&RegionHandler::region->displays[LIVE1_DISPLAY_RIGHT_##a])&0x0fffffff); \
							RegionHandler::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&RegionHandler::region->displays[LIVE1_DISPLAY_LEFT_##a ])&0x0fffffff); \
						}
	MONITOR(A);
	MONITOR(B);
	MONITOR(L);
	MONITOR(R);

	MONITOR(UP) 
	else MONITOR(RIGHT) 
	else MONITOR(DOWN) 
	else MONITOR(LEFT);
	
	#undef MONITOR
	
	LiveTable *t = VAR_INPUT.SELECT ? &RIGHT : &LEFT;
	
	int index = 0;
	if( KEYDOWN_A 		) index = 1;
	if( KEYDOWN_B 		) index = 2;
	if( KEYDOWN_UP 		) index = 3;
	if( KEYDOWN_RIGHT 	) index = 4;
	if( KEYDOWN_DOWN 	) index = 5;
	if( KEYDOWN_LEFT 	) index = 6;
	if( KEYDOWN_L 		) index = 7;
	if( KEYDOWN_R 		) index = 8;
		
		
		
	if( index ){
		index--;// index is used as boolean and index simultaneously, so we intendendly not using index=0 previously
		u8 channel_index  = t->CHAN[ index ];
		Channel *channel = &VAR_CHANNEL[ channel_index ];
		
		if( t->KEY[ index ] ) {
			channel->key  	= t->KEY[ index ];
			channel->volume = t->VOL[ index ];
			channel->inst 	= t->INS[ index ];			
			channel->value 	= t->VAL[ index ];
			channel->cmd 	= t->CMD[ index ];
			channel->retrig = true;
			channel->reset	= true;
			channel->trigger( channel );
		} 
	} 
	// If quantize is on, signals must be sent fixing their tick to desired quant time
}	

static u8 iterator;

void Performance::clear(){
	for( iterator=0; iterator<LIVE_TABLE_LENGTH; iterator++ ){
		LEFT.KEY	[ iterator ] = 0;
		LEFT.INS	[ iterator ] = 0;
		LEFT.CHAN	[ iterator ] = 0;
		LEFT.VOL	[ iterator ] = 0;
		LEFT.CMD	[ iterator ] = 0;
		LEFT.VAL	[ iterator ] = 0;
		RIGHT.KEY	[ iterator ] = 0;
		RIGHT.INS	[ iterator ] = 0;
		RIGHT.CHAN	[ iterator ] = 0;
		RIGHT.VOL	[ iterator ] = 0;
		RIGHT.CMD	[ iterator ] = 0;
		RIGHT.VAL	[ iterator ] = 0;
	}
}

void Performance::seek(){
	Sram::seek		( DATA_BASE_ADDRESS 					); // Address =  0x80
	Sram::forward 	( SONG_DETAILS_SIZE * SONG_SLOT_COUNT 	); // Address += 0x020 * 6 = 0x0C0 + 0x080 = 0x140 ( 320 )
	Sram::forward 	( GROOVE_TABLE_SIZE * SONG_SLOT_COUNT	); // Address += 0x010 * 6 = 0x060 + 0x140 = 0x1A0 ( 416 )
	Sram::forward 	( LIVE_TABLE_SIZE	* Song::slot 		); // Adresss += 0x060 * slot : 96 bytes for live tables
	Sram::next();	
}

void Performance::read(){
	seek();
	// Load live tables 
	for( iterator=0; iterator<LIVE_TABLE_LENGTH; iterator++ ){									// 96
		LEFT.KEY	[ iterator ] = Sram::read();										// 88
		LEFT.INS	[ iterator ] = Sram::read();										// 80
		LEFT.CHAN	[ iterator ] = Sram::read();										// 72
		LEFT.VOL	[ iterator ] = Sram::read();										// 64
		LEFT.CMD	[ iterator ] = Sram::read();										// 56
		LEFT.VAL	[ iterator ] = Sram::read();										// 48
		RIGHT.KEY	[ iterator ] = Sram::read();										// 40
		RIGHT.INS	[ iterator ] = Sram::read();										// 32
		RIGHT.CHAN	[ iterator ] = Sram::read();										// 24
		RIGHT.VOL	[ iterator ] = Sram::read();										// 16
		RIGHT.CMD	[ iterator ] = Sram::read();										// 8
		RIGHT.VAL	[ iterator ] = Sram::read();										// 0
	}
}

void Performance::write(){
	seek();
	for( iterator=0; iterator<LIVE_TABLE_LENGTH; iterator++ ){
		Sram::write( LEFT.KEY	[ iterator ] );
		Sram::write( LEFT.INS	[ iterator ] );
		Sram::write( LEFT.CHAN	[ iterator ] );
		Sram::write( LEFT.VOL	[ iterator ] );
		Sram::write( LEFT.CMD	[ iterator ] );
		Sram::write( LEFT.VAL	[ iterator ] );
		Sram::write( RIGHT.KEY	[ iterator ] );
		Sram::write( RIGHT.INS	[ iterator ] );
		Sram::write( RIGHT.CHAN	[ iterator ] );
		Sram::write( RIGHT.VOL	[ iterator ] );
		Sram::write( RIGHT.CMD	[ iterator ] );
		Sram::write( RIGHT.VAL	[ iterator ] );
	}
}

void Performance::dispatchMessage( u32 msg, u32 pointer){
	
	switch( msg ){
		
		/* Navigation */
		case MESSAGE_NAVIGATE_LEFT	: RegionHandler::load( ((Region*)pointer)->left ); break;
		case MESSAGE_NAVIGATE_RIGHT	: RegionHandler::load( ((Region*)pointer)->right); break;
		case MESSAGE_NAVIGATE_DOWN	: RegionHandler::load( ((Region*)pointer)->down ); break;
		case MESSAGE_NAVIGATE_UP	: RegionHandler::load( ((Region*)pointer)->up   ); break;
		case MESSAGE_ENTER			: break;
		case MESSAGE_EXIT			: break;
	}
}
