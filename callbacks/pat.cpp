#include "pat.hpp"
#include "../data/data.hpp"
#include "../data/helpers.hpp"
#include "../modules/key/key.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/spu/mixer.hpp"
#include "../modules/clip/clip.hpp"
#include "debug.hpp"

#define COLUMN00	 5
#define COLUMN01	 9
#define COLUMN02	13
#define COLUMN03	17
#define COLUMN04	21
#define COLUMN05	25

const 	u8 	NUMBERS[16] 				= { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
const 	u8 	PatEdit::arrow_position[6] 	= { COLUMN00 - 2, COLUMN01 - 2, COLUMN02 - 2, COLUMN03 - 2, COLUMN04 - 2, COLUMN05 - 2 };
const 	u16 PatEdit::channel_symbols[6] = { 0x7035, 0x7035, 0x7036, 0x7037, 0x7038, 0x7039 };
u8 			PatEdit::bookmark_row 		= 0;
u8 			PatEdit::bookmarks[6] 		= {0,0,0,0,0,0};
bool 		PatEdit::solo_clean 		= false;
bool 		PatEdit::clean 				= false;
u16 		PatEdit::icon_time 			= 0;

#define CTL(a) 			&PAT_CONTROLS[CONTROL_PAT_##a]
#define VAR(index, a) 	((u8*)&(VAR_PATTERN[index].a))

const Callback cb_pat_solo_0 = { PatEdit::solo , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_solo_1 = { PatEdit::solo , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_solo_2 = { PatEdit::solo , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_solo_3 = { PatEdit::solo , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_solo_4 = { PatEdit::solo , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_solo_5 = { PatEdit::solo , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_mute_0 = { PatEdit::mute , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_mute_1 = { PatEdit::mute , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_mute_2 = { PatEdit::mute , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_mute_3 = { PatEdit::mute , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_mute_4 = { PatEdit::mute , EVENT_KEYUP_B	, NULL 	, NULL 	};
const Callback cb_pat_mute_5 = { PatEdit::mute , EVENT_KEYUP_B	, NULL 	, NULL 	};


void transposeCH0_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 0 ); }
void transposeCH1_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 1 ); }
void transposeCH2_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 2 ); }
void transposeCH3_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 3 ); }
void transposeCH4_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 4 ); }
void transposeCH5_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 5 ); }

void transposeCH0_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 0 ); }
void transposeCH1_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 1 ); }
void transposeCH2_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 2 ); }
void transposeCH3_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 3 ); }
void transposeCH4_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 4 ); }
void transposeCH5_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 5 ); }

void transposeCH0_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 0; PatEdit::bookmark_row = *(u8*)pointer;}
void transposeCH1_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 1; PatEdit::bookmark_row = *(u8*)pointer;}
void transposeCH2_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 2;	PatEdit::bookmark_row = *(u8*)pointer;}
void transposeCH3_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 3;	PatEdit::bookmark_row = *(u8*)pointer;}
void transposeCH4_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 4;	PatEdit::bookmark_row = *(u8*)pointer;}
void transposeCH5_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 5;	PatEdit::bookmark_row = *(u8*)pointer;}


#ifndef NSONGTRANSPOSE
//TRANSPOSE
#define CB_TRANSPOSE(c, a)																																				\
const Callback cb_transpose_focus_##c##_0##a = { transposeCH##c##_focus	, EVENT_FOCUS 		, (u32*)&NUMBERS[0x0##a] 				, NULL					 		 }; \
const Callback cb_transpose_paste_##c##_0##a = { transposeCH##c##_paste	, EVENT_KEYDOWN_B 	, &VAR_PATTERN[0x##c].TRANSPOSE[0x##a] 	, &cb_transpose_focus_##c##_0##a };	\
const Callback cb_transpose_##c##_0##a		 = { transposeCH##c##_alter	, EVENT_MODIFY_B 	, &VAR_PATTERN[0x##c].TRANSPOSE[0x##a] 	, &cb_transpose_paste_##c##_0##a };  
CB_TRANSPOSE(0, 0);		CB_TRANSPOSE(1, 0);		CB_TRANSPOSE(2, 0);		CB_TRANSPOSE(3, 0);		CB_TRANSPOSE(4, 0);		CB_TRANSPOSE(5, 0);
CB_TRANSPOSE(0, 1);		CB_TRANSPOSE(1, 1);		CB_TRANSPOSE(2, 1);		CB_TRANSPOSE(3, 1);		CB_TRANSPOSE(4, 1);		CB_TRANSPOSE(5, 1);
CB_TRANSPOSE(0, 2);		CB_TRANSPOSE(1, 2);		CB_TRANSPOSE(2, 2);		CB_TRANSPOSE(3, 2);		CB_TRANSPOSE(4, 2);		CB_TRANSPOSE(5, 2);
CB_TRANSPOSE(0, 3);		CB_TRANSPOSE(1, 3);		CB_TRANSPOSE(2, 3);		CB_TRANSPOSE(3, 3);		CB_TRANSPOSE(4, 3);		CB_TRANSPOSE(5, 3);
CB_TRANSPOSE(0, 4);		CB_TRANSPOSE(1, 4);		CB_TRANSPOSE(2, 4);		CB_TRANSPOSE(3, 4);		CB_TRANSPOSE(4, 4);		CB_TRANSPOSE(5, 4);
CB_TRANSPOSE(0, 5);		CB_TRANSPOSE(1, 5);		CB_TRANSPOSE(2, 5);		CB_TRANSPOSE(3, 5);		CB_TRANSPOSE(4, 5);		CB_TRANSPOSE(5, 5);
CB_TRANSPOSE(0, 6);		CB_TRANSPOSE(1, 6);		CB_TRANSPOSE(2, 6);		CB_TRANSPOSE(3, 6);		CB_TRANSPOSE(4, 6);		CB_TRANSPOSE(5, 6);
CB_TRANSPOSE(0, 7);		CB_TRANSPOSE(1, 7);		CB_TRANSPOSE(2, 7);		CB_TRANSPOSE(3, 7);		CB_TRANSPOSE(4, 7);		CB_TRANSPOSE(5, 7);
CB_TRANSPOSE(0, 8);		CB_TRANSPOSE(1, 8);		CB_TRANSPOSE(2, 8);		CB_TRANSPOSE(3, 8);		CB_TRANSPOSE(4, 8);		CB_TRANSPOSE(5, 8);
CB_TRANSPOSE(0, 9);		CB_TRANSPOSE(1, 9);		CB_TRANSPOSE(2, 9);		CB_TRANSPOSE(3, 9);		CB_TRANSPOSE(4, 9);		CB_TRANSPOSE(5, 9);
CB_TRANSPOSE(0, A);		CB_TRANSPOSE(1, A);		CB_TRANSPOSE(2, A);		CB_TRANSPOSE(3, A);		CB_TRANSPOSE(4, A);		CB_TRANSPOSE(5, A);
CB_TRANSPOSE(0, B);		CB_TRANSPOSE(1, B);		CB_TRANSPOSE(2, B);		CB_TRANSPOSE(3, B);		CB_TRANSPOSE(4, B);		CB_TRANSPOSE(5, B);
CB_TRANSPOSE(0, C);		CB_TRANSPOSE(1, C);		CB_TRANSPOSE(2, C);		CB_TRANSPOSE(3, C);		CB_TRANSPOSE(4, C);		CB_TRANSPOSE(5, C);
CB_TRANSPOSE(0, D);		CB_TRANSPOSE(1, D);		CB_TRANSPOSE(2, D);		CB_TRANSPOSE(3, D);		CB_TRANSPOSE(4, D);		CB_TRANSPOSE(5, D);
CB_TRANSPOSE(0, E);		CB_TRANSPOSE(1, E);		CB_TRANSPOSE(2, E);		CB_TRANSPOSE(3, E);		CB_TRANSPOSE(4, E);		CB_TRANSPOSE(5, E);
CB_TRANSPOSE(0, F);		CB_TRANSPOSE(1, F);		CB_TRANSPOSE(2, F);		CB_TRANSPOSE(3, F);		CB_TRANSPOSE(4, F);		CB_TRANSPOSE(5, F);
#undef CB_TRANSPOSE

#endif


const Control PAT_CONTROLS[ CONTROL_PAT_MAX ] = { 
//	{ x	 		, y		, up					, right					, down					, left					, cache							, var						, callback			},
	#ifdef NSONGTRANSPOSE
	{ COLUMN00 	, 0x04 	, CTL( SOLO_LEFT_02	  ) , CTL( PATTERNS_B_00  ) , CTL( PATTERNS_A_01  ) , CTL( PATTERNS_F_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  0 ]  ) , &cb_patterns_0_00	},
	{ COLUMN00 	, 0x05 	, CTL( PATTERNS_A_00  )	, CTL( PATTERNS_B_01  ) , CTL( PATTERNS_A_02  ) , CTL( PATTERNS_F_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  1 ]  ) , &cb_patterns_0_01	},
	{ COLUMN00 	, 0x06 	, CTL( PATTERNS_A_01  )	, CTL( PATTERNS_B_02  ) , CTL( PATTERNS_A_03  ) , CTL( PATTERNS_F_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  2 ]  ) , &cb_patterns_0_02	},
	{ COLUMN00 	, 0x07 	, CTL( PATTERNS_A_02  )	, CTL( PATTERNS_B_03  ) , CTL( PATTERNS_A_04  ) , CTL( PATTERNS_F_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  3 ]  ) , &cb_patterns_0_03	},
	{ COLUMN00 	, 0x08 	, CTL( PATTERNS_A_03  )	, CTL( PATTERNS_B_04  ) , CTL( PATTERNS_A_05  ) , CTL( PATTERNS_F_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  4 ]  ) , &cb_patterns_0_04	},
	{ COLUMN00 	, 0x09 	, CTL( PATTERNS_A_04  )	, CTL( PATTERNS_B_05  ) , CTL( PATTERNS_A_06  ) , CTL( PATTERNS_F_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  5 ]  ) , &cb_patterns_0_05	},
	{ COLUMN00 	, 0x0a 	, CTL( PATTERNS_A_05  )	, CTL( PATTERNS_B_06  ) , CTL( PATTERNS_A_07  ) , CTL( PATTERNS_F_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  6 ]  ) , &cb_patterns_0_06	},
	{ COLUMN00 	, 0x0b 	, CTL( PATTERNS_A_06  )	, CTL( PATTERNS_B_07  ) , CTL( PATTERNS_A_08  ) , CTL( PATTERNS_F_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  7 ]  ) , &cb_patterns_0_07	},
	{ COLUMN00 	, 0x0c 	, CTL( PATTERNS_A_07  )	, CTL( PATTERNS_B_08  ) , CTL( PATTERNS_A_09  ) , CTL( PATTERNS_F_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  8 ]  ) , &cb_patterns_0_08	},
	{ COLUMN00 	, 0x0d 	, CTL( PATTERNS_A_08  )	, CTL( PATTERNS_B_09  ) , CTL( PATTERNS_A_0A  ) , CTL( PATTERNS_F_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  9 ]  ) , &cb_patterns_0_09	},
	{ COLUMN00 	, 0x0e 	, CTL( PATTERNS_A_09  )	, CTL( PATTERNS_B_0A  ) , CTL( PATTERNS_A_0B  ) , CTL( PATTERNS_F_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 10 ]  ) , &cb_patterns_0_0A	},
	{ COLUMN00 	, 0x0f 	, CTL( PATTERNS_A_0A  )	, CTL( PATTERNS_B_0B  ) , CTL( PATTERNS_A_0C  ) , CTL( PATTERNS_F_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 11 ]  ) , &cb_patterns_0_0B	},
	{ COLUMN00 	, 0x10 	, CTL( PATTERNS_A_0B  )	, CTL( PATTERNS_B_0C  ) , CTL( PATTERNS_A_0D  ) , CTL( PATTERNS_F_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 12 ]  ) , &cb_patterns_0_0C	},
	{ COLUMN00 	, 0x11 	, CTL( PATTERNS_A_0C  )	, CTL( PATTERNS_B_0D  ) , CTL( PATTERNS_A_0E  ) , CTL( PATTERNS_F_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 13 ]  ) , &cb_patterns_0_0D	},
	{ COLUMN00 	, 0x12 	, CTL( PATTERNS_A_0D  )	, CTL( PATTERNS_B_0E  ) , CTL( PATTERNS_A_0F  ) , CTL( PATTERNS_F_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 14 ]  ) , &cb_patterns_0_0E	},
	{ COLUMN00 	, 0x13 	, CTL( PATTERNS_A_0E  )	, CTL( PATTERNS_B_0F  ) , CTL( SOLO_LEFT_00   ) , CTL( PATTERNS_F_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 15 ]  ) , &cb_patterns_0_0F	},
	{ COLUMN01 	, 0x04 	, CTL( SOLO_LEFT_02   ) , CTL( PATTERNS_C_00  ) , CTL( PATTERNS_B_01  ) , CTL( PATTERNS_A_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  0 ]  ) , &cb_patterns_1_00	},
	{ COLUMN01 	, 0x05 	, CTL( PATTERNS_B_00  )	, CTL( PATTERNS_C_01  ) , CTL( PATTERNS_B_02  ) , CTL( PATTERNS_A_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  1 ]  ) , &cb_patterns_1_01	},
	{ COLUMN01 	, 0x06 	, CTL( PATTERNS_B_01  )	, CTL( PATTERNS_C_02  ) , CTL( PATTERNS_B_03  ) , CTL( PATTERNS_A_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  2 ]  ) , &cb_patterns_1_02	},
	{ COLUMN01 	, 0x07 	, CTL( PATTERNS_B_02  )	, CTL( PATTERNS_C_03  ) , CTL( PATTERNS_B_04  ) , CTL( PATTERNS_A_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  3 ]  ) , &cb_patterns_1_03	},
	{ COLUMN01 	, 0x08 	, CTL( PATTERNS_B_03  )	, CTL( PATTERNS_C_04  ) , CTL( PATTERNS_B_05  ) , CTL( PATTERNS_A_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  4 ]  ) , &cb_patterns_1_04	},
	{ COLUMN01 	, 0x09 	, CTL( PATTERNS_B_04  )	, CTL( PATTERNS_C_05  ) , CTL( PATTERNS_B_06  ) , CTL( PATTERNS_A_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  5 ]  ) , &cb_patterns_1_05	},
	{ COLUMN01 	, 0x0a 	, CTL( PATTERNS_B_05  )	, CTL( PATTERNS_C_06  ) , CTL( PATTERNS_B_07  ) , CTL( PATTERNS_A_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  6 ]  ) , &cb_patterns_1_06	},
	{ COLUMN01 	, 0x0b 	, CTL( PATTERNS_B_06  )	, CTL( PATTERNS_C_07  ) , CTL( PATTERNS_B_08  ) , CTL( PATTERNS_A_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  7 ]  ) , &cb_patterns_1_07	},
	{ COLUMN01 	, 0x0c 	, CTL( PATTERNS_B_07  )	, CTL( PATTERNS_C_08  ) , CTL( PATTERNS_B_09  ) , CTL( PATTERNS_A_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  8 ]  ) , &cb_patterns_1_08	},
	{ COLUMN01 	, 0x0d 	, CTL( PATTERNS_B_08  )	, CTL( PATTERNS_C_09  ) , CTL( PATTERNS_B_0A  ) , CTL( PATTERNS_A_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  9 ]  ) , &cb_patterns_1_09	},
	{ COLUMN01 	, 0x0e 	, CTL( PATTERNS_B_09  )	, CTL( PATTERNS_C_0A  ) , CTL( PATTERNS_B_0B  ) , CTL( PATTERNS_A_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 10 ]  ) , &cb_patterns_1_0A	},
	{ COLUMN01 	, 0x0f 	, CTL( PATTERNS_B_0A  )	, CTL( PATTERNS_C_0B  ) , CTL( PATTERNS_B_0C  ) , CTL( PATTERNS_A_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 11 ]  ) , &cb_patterns_1_0B	},
	{ COLUMN01 	, 0x10 	, CTL( PATTERNS_B_0B  )	, CTL( PATTERNS_C_0C  ) , CTL( PATTERNS_B_0D  ) , CTL( PATTERNS_A_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 12 ]  ) , &cb_patterns_1_0C	},
	{ COLUMN01 	, 0x11 	, CTL( PATTERNS_B_0C  )	, CTL( PATTERNS_C_0D  ) , CTL( PATTERNS_B_0E  ) , CTL( PATTERNS_A_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 13 ]  ) , &cb_patterns_1_0D	},
	{ COLUMN01 	, 0x12 	, CTL( PATTERNS_B_0D  )	, CTL( PATTERNS_C_0E  ) , CTL( PATTERNS_B_0F  ) , CTL( PATTERNS_A_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 14 ]  ) , &cb_patterns_1_0E	},
	{ COLUMN01 	, 0x13 	, CTL( PATTERNS_B_0E  )	, CTL( PATTERNS_C_0F  ) , CTL( SOLO_LEFT_00   ) , CTL( PATTERNS_A_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 15 ]  ) , &cb_patterns_1_0F	},
	{ COLUMN02 	, 0x04 	, CTL( SOLO_LEFT_02   ) , CTL( PATTERNS_D_00  ) , CTL( PATTERNS_C_01  ) , CTL( PATTERNS_B_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  0 ]  ) , &cb_patterns_2_00	}, 
	{ COLUMN02 	, 0x05 	, CTL( PATTERNS_C_00  )	, CTL( PATTERNS_D_01  ) , CTL( PATTERNS_C_02  ) , CTL( PATTERNS_B_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  1 ]  ) , &cb_patterns_2_01	}, 
	{ COLUMN02 	, 0x06 	, CTL( PATTERNS_C_01  )	, CTL( PATTERNS_D_02  ) , CTL( PATTERNS_C_03  ) , CTL( PATTERNS_B_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  2 ]  ) , &cb_patterns_2_02	}, 
	{ COLUMN02 	, 0x07 	, CTL( PATTERNS_C_02  )	, CTL( PATTERNS_D_03  ) , CTL( PATTERNS_C_04  ) , CTL( PATTERNS_B_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  3 ]  ) , &cb_patterns_2_03	}, 
	{ COLUMN02 	, 0x08 	, CTL( PATTERNS_C_03  )	, CTL( PATTERNS_D_04  ) , CTL( PATTERNS_C_05  ) , CTL( PATTERNS_B_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  4 ]  ) , &cb_patterns_2_04	}, 
	{ COLUMN02 	, 0x09 	, CTL( PATTERNS_C_04  )	, CTL( PATTERNS_D_05  ) , CTL( PATTERNS_C_06  ) , CTL( PATTERNS_B_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  5 ]  ) , &cb_patterns_2_05	}, 
	{ COLUMN02 	, 0x0a 	, CTL( PATTERNS_C_05  )	, CTL( PATTERNS_D_06  ) , CTL( PATTERNS_C_07  ) , CTL( PATTERNS_B_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  6 ]  ) , &cb_patterns_2_06	}, 
	{ COLUMN02 	, 0x0b 	, CTL( PATTERNS_C_06  )	, CTL( PATTERNS_D_07  ) , CTL( PATTERNS_C_08  ) , CTL( PATTERNS_B_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  7 ]  ) , &cb_patterns_2_07	}, 
	{ COLUMN02 	, 0x0c 	, CTL( PATTERNS_C_07  )	, CTL( PATTERNS_D_08  ) , CTL( PATTERNS_C_09  ) , CTL( PATTERNS_B_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  8 ]  ) , &cb_patterns_2_08	}, 
	{ COLUMN02 	, 0x0d 	, CTL( PATTERNS_C_08  )	, CTL( PATTERNS_D_09  ) , CTL( PATTERNS_C_0A  ) , CTL( PATTERNS_B_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  9 ]  ) , &cb_patterns_2_09	}, 
	{ COLUMN02 	, 0x0e 	, CTL( PATTERNS_C_09  )	, CTL( PATTERNS_D_0A  ) , CTL( PATTERNS_C_0B  ) , CTL( PATTERNS_B_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 10 ]  ) , &cb_patterns_2_0A	}, 
	{ COLUMN02 	, 0x0f 	, CTL( PATTERNS_C_0A  )	, CTL( PATTERNS_D_0B  ) , CTL( PATTERNS_C_0C  ) , CTL( PATTERNS_B_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 11 ]  ) , &cb_patterns_2_0B	}, 
	{ COLUMN02 	, 0x10 	, CTL( PATTERNS_C_0B  )	, CTL( PATTERNS_D_0C  ) , CTL( PATTERNS_C_0D  ) , CTL( PATTERNS_B_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 12 ]  ) , &cb_patterns_2_0C	}, 
	{ COLUMN02 	, 0x11 	, CTL( PATTERNS_C_0C  )	, CTL( PATTERNS_D_0D  ) , CTL( PATTERNS_C_0E  ) , CTL( PATTERNS_B_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 13 ]  ) , &cb_patterns_2_0D	}, 
	{ COLUMN02 	, 0x12 	, CTL( PATTERNS_C_0D  )	, CTL( PATTERNS_D_0E  ) , CTL( PATTERNS_C_0F  ) , CTL( PATTERNS_B_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 14 ]  ) , &cb_patterns_2_0E	}, 
	{ COLUMN02 	, 0x13 	, CTL( PATTERNS_C_0E  )	, CTL( PATTERNS_D_0F  ) , CTL( SOLO_LEFT_00   ) , CTL( PATTERNS_B_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 15 ]  ) , &cb_patterns_2_0F	}, 
	{ COLUMN03 	, 0x04 	, CTL( MUTE_LEFT_02   ) , CTL( PATTERNS_E_00  ) , CTL( PATTERNS_D_01  ) , CTL( PATTERNS_C_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  0 ]  ) , &cb_patterns_3_00	},  
	{ COLUMN03 	, 0x05 	, CTL( PATTERNS_D_00  )	, CTL( PATTERNS_E_01  ) , CTL( PATTERNS_D_02  ) , CTL( PATTERNS_C_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  1 ]  ) , &cb_patterns_3_01	},  
	{ COLUMN03 	, 0x06 	, CTL( PATTERNS_D_01  )	, CTL( PATTERNS_E_02  ) , CTL( PATTERNS_D_03  ) , CTL( PATTERNS_C_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  2 ]  ) , &cb_patterns_3_02	},  
	{ COLUMN03 	, 0x07 	, CTL( PATTERNS_D_02  )	, CTL( PATTERNS_E_03  ) , CTL( PATTERNS_D_04  ) , CTL( PATTERNS_C_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  3 ]  ) , &cb_patterns_3_03	},  
	{ COLUMN03 	, 0x08 	, CTL( PATTERNS_D_03  )	, CTL( PATTERNS_E_04  ) , CTL( PATTERNS_D_05  ) , CTL( PATTERNS_C_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  4 ]  ) , &cb_patterns_3_04	},  
	{ COLUMN03 	, 0x09 	, CTL( PATTERNS_D_04  )	, CTL( PATTERNS_E_05  ) , CTL( PATTERNS_D_06  ) , CTL( PATTERNS_C_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  5 ]  ) , &cb_patterns_3_05	},  
	{ COLUMN03 	, 0x0a 	, CTL( PATTERNS_D_05  )	, CTL( PATTERNS_E_06  ) , CTL( PATTERNS_D_07  ) , CTL( PATTERNS_C_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  6 ]  ) , &cb_patterns_3_06	},  
	{ COLUMN03 	, 0x0b 	, CTL( PATTERNS_D_06  )	, CTL( PATTERNS_E_07  ) , CTL( PATTERNS_D_08  ) , CTL( PATTERNS_C_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  7 ]  ) , &cb_patterns_3_07	},  
	{ COLUMN03 	, 0x0c 	, CTL( PATTERNS_D_07  )	, CTL( PATTERNS_E_08  ) , CTL( PATTERNS_D_09  ) , CTL( PATTERNS_C_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  8 ]  ) , &cb_patterns_3_08	},  
	{ COLUMN03 	, 0x0d 	, CTL( PATTERNS_D_08  )	, CTL( PATTERNS_E_09  ) , CTL( PATTERNS_D_0A  ) , CTL( PATTERNS_C_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  9 ]  ) , &cb_patterns_3_09	},  
	{ COLUMN03 	, 0x0e 	, CTL( PATTERNS_D_09  )	, CTL( PATTERNS_E_0A  ) , CTL( PATTERNS_D_0B  ) , CTL( PATTERNS_C_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 10 ]  ) , &cb_patterns_3_0A	},  
	{ COLUMN03 	, 0x0f 	, CTL( PATTERNS_D_0A  )	, CTL( PATTERNS_E_0B  ) , CTL( PATTERNS_D_0C  ) , CTL( PATTERNS_C_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 11 ]  ) , &cb_patterns_3_0B	},  
	{ COLUMN03 	, 0x10 	, CTL( PATTERNS_D_0B  )	, CTL( PATTERNS_E_0C  ) , CTL( PATTERNS_D_0D  ) , CTL( PATTERNS_C_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 12 ]  ) , &cb_patterns_3_0C	},  
	{ COLUMN03 	, 0x11 	, CTL( PATTERNS_D_0C  )	, CTL( PATTERNS_E_0D  ) , CTL( PATTERNS_D_0E  ) , CTL( PATTERNS_C_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 13 ]  ) , &cb_patterns_3_0D	},  
	{ COLUMN03 	, 0x12 	, CTL( PATTERNS_D_0D  )	, CTL( PATTERNS_E_0E  ) , CTL( PATTERNS_D_0F  ) , CTL( PATTERNS_C_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 14 ]  ) , &cb_patterns_3_0E	},  
	{ COLUMN03 	, 0x13 	, CTL( PATTERNS_D_0E  )	, CTL( PATTERNS_E_0F  ) , CTL( MUTE_LEFT_00   ) , CTL( PATTERNS_C_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 15 ]  ) , &cb_patterns_3_0F	},  
	{ COLUMN04 	, 0x04 	, CTL( MUTE_RIGHT_02  )	, CTL( PATTERNS_F_00  ) , CTL( PATTERNS_E_01  ) , CTL( PATTERNS_D_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  0 ]  ) , &cb_patterns_4_00	},  
	{ COLUMN04 	, 0x05 	, CTL( PATTERNS_E_00  )	, CTL( PATTERNS_F_01  ) , CTL( PATTERNS_E_02  ) , CTL( PATTERNS_D_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  1 ]  ) , &cb_patterns_4_01	},  
	{ COLUMN04 	, 0x06 	, CTL( PATTERNS_E_01  )	, CTL( PATTERNS_F_02  ) , CTL( PATTERNS_E_03  ) , CTL( PATTERNS_D_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  2 ]  ) , &cb_patterns_4_02	},  
	{ COLUMN04 	, 0x07 	, CTL( PATTERNS_E_02  )	, CTL( PATTERNS_F_03  ) , CTL( PATTERNS_E_04  ) , CTL( PATTERNS_D_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  3 ]  ) , &cb_patterns_4_03	},  
	{ COLUMN04 	, 0x08 	, CTL( PATTERNS_E_03  )	, CTL( PATTERNS_F_04  ) , CTL( PATTERNS_E_05  ) , CTL( PATTERNS_D_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  4 ]  ) , &cb_patterns_4_04	},  
	{ COLUMN04 	, 0x09 	, CTL( PATTERNS_E_04  )	, CTL( PATTERNS_F_05  ) , CTL( PATTERNS_E_06  ) , CTL( PATTERNS_D_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  5 ]  ) , &cb_patterns_4_05	},  
	{ COLUMN04 	, 0x0a 	, CTL( PATTERNS_E_05  )	, CTL( PATTERNS_F_06  ) , CTL( PATTERNS_E_07  ) , CTL( PATTERNS_D_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  6 ]  ) , &cb_patterns_4_06	},  
	{ COLUMN04 	, 0x0b 	, CTL( PATTERNS_E_06  )	, CTL( PATTERNS_F_07  ) , CTL( PATTERNS_E_08  ) , CTL( PATTERNS_D_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  7 ]  ) , &cb_patterns_4_07	},  
	{ COLUMN04 	, 0x0c 	, CTL( PATTERNS_E_07  )	, CTL( PATTERNS_F_08  ) , CTL( PATTERNS_E_09  ) , CTL( PATTERNS_D_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  8 ]  ) , &cb_patterns_4_08	},  
	{ COLUMN04 	, 0x0d 	, CTL( PATTERNS_E_08  )	, CTL( PATTERNS_F_09  ) , CTL( PATTERNS_E_0A  ) , CTL( PATTERNS_D_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  9 ]  ) , &cb_patterns_4_09	},  
	{ COLUMN04 	, 0x0e 	, CTL( PATTERNS_E_09  )	, CTL( PATTERNS_F_0A  ) , CTL( PATTERNS_E_0B  ) , CTL( PATTERNS_D_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 10 ]  ) , &cb_patterns_4_0A	},  
	{ COLUMN04 	, 0x0f 	, CTL( PATTERNS_E_0A  )	, CTL( PATTERNS_F_0B  ) , CTL( PATTERNS_E_0C  ) , CTL( PATTERNS_D_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 11 ]  ) , &cb_patterns_4_0B	},  
	{ COLUMN04 	, 0x10 	, CTL( PATTERNS_E_0B  )	, CTL( PATTERNS_F_0C  ) , CTL( PATTERNS_E_0D  ) , CTL( PATTERNS_D_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 12 ]  ) , &cb_patterns_4_0C	},  
	{ COLUMN04 	, 0x11 	, CTL( PATTERNS_E_0C  )	, CTL( PATTERNS_F_0D  ) , CTL( PATTERNS_E_0E  ) , CTL( PATTERNS_D_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 13 ]  ) , &cb_patterns_4_0D	},  
	{ COLUMN04 	, 0x12 	, CTL( PATTERNS_E_0D  )	, CTL( PATTERNS_F_0E  ) , CTL( PATTERNS_E_0F  ) , CTL( PATTERNS_D_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 14 ]  ) , &cb_patterns_4_0E	},  
	{ COLUMN04 	, 0x13 	, CTL( PATTERNS_E_0E  )	, CTL( PATTERNS_F_0F  ) , CTL( MUTE_RIGHT_00  ) , CTL( PATTERNS_D_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 15 ]  ) , &cb_patterns_4_0F	},  
	{ COLUMN05 	, 0x04 	, CTL( SOLO_RIGHT_02  )	, CTL( PATTERNS_A_00  ) , CTL( PATTERNS_F_01  ) , CTL( PATTERNS_E_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  0 ]  ) , &cb_patterns_5_00	},  
	{ COLUMN05 	, 0x05 	, CTL( PATTERNS_F_00  )	, CTL( PATTERNS_A_01  ) , CTL( PATTERNS_F_02  ) , CTL( PATTERNS_E_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  1 ]  ) , &cb_patterns_5_01	},  
	{ COLUMN05 	, 0x06 	, CTL( PATTERNS_F_01  )	, CTL( PATTERNS_A_02  ) , CTL( PATTERNS_F_03  ) , CTL( PATTERNS_E_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  2 ]  ) , &cb_patterns_5_02	},  
	{ COLUMN05 	, 0x07 	, CTL( PATTERNS_F_02  )	, CTL( PATTERNS_A_03  ) , CTL( PATTERNS_F_04  ) , CTL( PATTERNS_E_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  3 ]  ) , &cb_patterns_5_03	},  
	{ COLUMN05 	, 0x08 	, CTL( PATTERNS_F_03  )	, CTL( PATTERNS_A_04  ) , CTL( PATTERNS_F_05  ) , CTL( PATTERNS_E_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  4 ]  ) , &cb_patterns_5_04	},  
	{ COLUMN05 	, 0x09 	, CTL( PATTERNS_F_04  )	, CTL( PATTERNS_A_05  ) , CTL( PATTERNS_F_06  ) , CTL( PATTERNS_E_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  5 ]  ) , &cb_patterns_5_05	},  
	{ COLUMN05 	, 0x0a 	, CTL( PATTERNS_F_05  )	, CTL( PATTERNS_A_06  ) , CTL( PATTERNS_F_07  ) , CTL( PATTERNS_E_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  6 ]  ) , &cb_patterns_5_06	},  
	{ COLUMN05 	, 0x0b 	, CTL( PATTERNS_F_06  )	, CTL( PATTERNS_A_07  ) , CTL( PATTERNS_F_08  ) , CTL( PATTERNS_E_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  7 ]  ) , &cb_patterns_5_07	},  
	{ COLUMN05 	, 0x0c 	, CTL( PATTERNS_F_07  )	, CTL( PATTERNS_A_08  ) , CTL( PATTERNS_F_09  ) , CTL( PATTERNS_E_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  8 ]  ) , &cb_patterns_5_08	},  
	{ COLUMN05 	, 0x0d 	, CTL( PATTERNS_F_08  )	, CTL( PATTERNS_A_09  ) , CTL( PATTERNS_F_0A  ) , CTL( PATTERNS_E_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  9 ]  ) , &cb_patterns_5_09	},  
	{ COLUMN05 	, 0x0e 	, CTL( PATTERNS_F_09  )	, CTL( PATTERNS_A_0A  ) , CTL( PATTERNS_F_0B  ) , CTL( PATTERNS_E_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 10 ]  ) , &cb_patterns_5_0A	},  
	{ COLUMN05 	, 0x0f 	, CTL( PATTERNS_F_0A  )	, CTL( PATTERNS_A_0B  ) , CTL( PATTERNS_F_0C  ) , CTL( PATTERNS_E_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 11 ]  ) , &cb_patterns_5_0B	},  
	{ COLUMN05 	, 0x10 	, CTL( PATTERNS_F_0B  )	, CTL( PATTERNS_A_0C  ) , CTL( PATTERNS_F_0D  ) , CTL( PATTERNS_E_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 12 ]  ) , &cb_patterns_5_0C	},  
	{ COLUMN05 	, 0x11 	, CTL( PATTERNS_F_0C  )	, CTL( PATTERNS_A_0D  ) , CTL( PATTERNS_F_0E  ) , CTL( PATTERNS_E_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 13 ]  ) , &cb_patterns_5_0D	},  
	{ COLUMN05 	, 0x12 	, CTL( PATTERNS_F_0D  )	, CTL( PATTERNS_A_0E  ) , CTL( PATTERNS_F_0F  ) , CTL( PATTERNS_E_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 14 ]  ) , &cb_patterns_5_0E	},  
	{ COLUMN05 	, 0x13 	, CTL( PATTERNS_F_0E  )	, CTL( PATTERNS_A_0F  ) , CTL( SOLO_RIGHT_00  ) , CTL( PATTERNS_E_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 15 ]  ) , &cb_patterns_5_0F	},  

	#endif
	#ifndef NSONGTRANSPOSE
	
//	{ x	 		, y		, up					, right					, down					, left					, cache							, var						, callback			},
	{ COLUMN00 	, 0x04 	, CTL( SOLO_LEFT_02	  ) , CTL( TRANSPOSE_B_00  ) , CTL( PATTERNS_A_01  ) , CTL( TRANSPOSE_A_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  0 ]  ) , &cb_patterns_0_00	},
	{ COLUMN00 	, 0x05 	, CTL( PATTERNS_A_00  )	, CTL( TRANSPOSE_B_01  ) , CTL( PATTERNS_A_02  ) , CTL( TRANSPOSE_A_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  1 ]  ) , &cb_patterns_0_01	},
	{ COLUMN00 	, 0x06 	, CTL( PATTERNS_A_01  )	, CTL( TRANSPOSE_B_02  ) , CTL( PATTERNS_A_03  ) , CTL( TRANSPOSE_A_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  2 ]  ) , &cb_patterns_0_02	},
	{ COLUMN00 	, 0x07 	, CTL( PATTERNS_A_02  )	, CTL( TRANSPOSE_B_03  ) , CTL( PATTERNS_A_04  ) , CTL( TRANSPOSE_A_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  3 ]  ) , &cb_patterns_0_03	},
	{ COLUMN00 	, 0x08 	, CTL( PATTERNS_A_03  )	, CTL( TRANSPOSE_B_04  ) , CTL( PATTERNS_A_05  ) , CTL( TRANSPOSE_A_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  4 ]  ) , &cb_patterns_0_04	},
	{ COLUMN00 	, 0x09 	, CTL( PATTERNS_A_04  )	, CTL( TRANSPOSE_B_05  ) , CTL( PATTERNS_A_06  ) , CTL( TRANSPOSE_A_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  5 ]  ) , &cb_patterns_0_05	},
	{ COLUMN00 	, 0x0a 	, CTL( PATTERNS_A_05  )	, CTL( TRANSPOSE_B_06  ) , CTL( PATTERNS_A_07  ) , CTL( TRANSPOSE_A_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  6 ]  ) , &cb_patterns_0_06	},
	{ COLUMN00 	, 0x0b 	, CTL( PATTERNS_A_06  )	, CTL( TRANSPOSE_B_07  ) , CTL( PATTERNS_A_08  ) , CTL( TRANSPOSE_A_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  7 ]  ) , &cb_patterns_0_07	},
	{ COLUMN00 	, 0x0c 	, CTL( PATTERNS_A_07  )	, CTL( TRANSPOSE_B_08  ) , CTL( PATTERNS_A_09  ) , CTL( TRANSPOSE_A_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  8 ]  ) , &cb_patterns_0_08	},
	{ COLUMN00 	, 0x0d 	, CTL( PATTERNS_A_08  )	, CTL( TRANSPOSE_B_09  ) , CTL( PATTERNS_A_0A  ) , CTL( TRANSPOSE_A_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[  9 ]  ) , &cb_patterns_0_09	},
	{ COLUMN00 	, 0x0e 	, CTL( PATTERNS_A_09  )	, CTL( TRANSPOSE_B_0A  ) , CTL( PATTERNS_A_0B  ) , CTL( TRANSPOSE_A_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 10 ]  ) , &cb_patterns_0_0A	},
	{ COLUMN00 	, 0x0f 	, CTL( PATTERNS_A_0A  )	, CTL( TRANSPOSE_B_0B  ) , CTL( PATTERNS_A_0C  ) , CTL( TRANSPOSE_A_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 11 ]  ) , &cb_patterns_0_0B	},
	{ COLUMN00 	, 0x10 	, CTL( PATTERNS_A_0B  )	, CTL( TRANSPOSE_B_0C  ) , CTL( PATTERNS_A_0D  ) , CTL( TRANSPOSE_A_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 12 ]  ) , &cb_patterns_0_0C	},
	{ COLUMN00 	, 0x11 	, CTL( PATTERNS_A_0C  )	, CTL( TRANSPOSE_B_0D  ) , CTL( PATTERNS_A_0E  ) , CTL( TRANSPOSE_A_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 13 ]  ) , &cb_patterns_0_0D	},
	{ COLUMN00 	, 0x12 	, CTL( PATTERNS_A_0D  )	, CTL( TRANSPOSE_B_0E  ) , CTL( PATTERNS_A_0F  ) , CTL( TRANSPOSE_A_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 14 ]  ) , &cb_patterns_0_0E	},
	{ COLUMN00 	, 0x13 	, CTL( PATTERNS_A_0E  )	, CTL( TRANSPOSE_B_0F  ) , CTL( SOLO_LEFT_00   ) , CTL( TRANSPOSE_A_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 0 , ORDER[ 15 ]  ) , &cb_patterns_0_0F	},
	{ COLUMN01 	, 0x04 	, CTL( SOLO_LEFT_02   ) , CTL( TRANSPOSE_C_00  ) , CTL( PATTERNS_B_01  ) , CTL( TRANSPOSE_B_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  0 ]  ) , &cb_patterns_1_00	},
	{ COLUMN01 	, 0x05 	, CTL( PATTERNS_B_00  )	, CTL( TRANSPOSE_C_01  ) , CTL( PATTERNS_B_02  ) , CTL( TRANSPOSE_B_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  1 ]  ) , &cb_patterns_1_01	},
	{ COLUMN01 	, 0x06 	, CTL( PATTERNS_B_01  )	, CTL( TRANSPOSE_C_02  ) , CTL( PATTERNS_B_03  ) , CTL( TRANSPOSE_B_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  2 ]  ) , &cb_patterns_1_02	},
	{ COLUMN01 	, 0x07 	, CTL( PATTERNS_B_02  )	, CTL( TRANSPOSE_C_03  ) , CTL( PATTERNS_B_04  ) , CTL( TRANSPOSE_B_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  3 ]  ) , &cb_patterns_1_03	},
	{ COLUMN01 	, 0x08 	, CTL( PATTERNS_B_03  )	, CTL( TRANSPOSE_C_04  ) , CTL( PATTERNS_B_05  ) , CTL( TRANSPOSE_B_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  4 ]  ) , &cb_patterns_1_04	},
	{ COLUMN01 	, 0x09 	, CTL( PATTERNS_B_04  )	, CTL( TRANSPOSE_C_05  ) , CTL( PATTERNS_B_06  ) , CTL( TRANSPOSE_B_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  5 ]  ) , &cb_patterns_1_05	},
	{ COLUMN01 	, 0x0a 	, CTL( PATTERNS_B_05  )	, CTL( TRANSPOSE_C_06  ) , CTL( PATTERNS_B_07  ) , CTL( TRANSPOSE_B_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  6 ]  ) , &cb_patterns_1_06	},
	{ COLUMN01 	, 0x0b 	, CTL( PATTERNS_B_06  )	, CTL( TRANSPOSE_C_07  ) , CTL( PATTERNS_B_08  ) , CTL( TRANSPOSE_B_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  7 ]  ) , &cb_patterns_1_07	},
	{ COLUMN01 	, 0x0c 	, CTL( PATTERNS_B_07  )	, CTL( TRANSPOSE_C_08  ) , CTL( PATTERNS_B_09  ) , CTL( TRANSPOSE_B_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  8 ]  ) , &cb_patterns_1_08	},
	{ COLUMN01 	, 0x0d 	, CTL( PATTERNS_B_08  )	, CTL( TRANSPOSE_C_09  ) , CTL( PATTERNS_B_0A  ) , CTL( TRANSPOSE_B_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[  9 ]  ) , &cb_patterns_1_09	},
	{ COLUMN01 	, 0x0e 	, CTL( PATTERNS_B_09  )	, CTL( TRANSPOSE_C_0A  ) , CTL( PATTERNS_B_0B  ) , CTL( TRANSPOSE_B_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 10 ]  ) , &cb_patterns_1_0A	},
	{ COLUMN01 	, 0x0f 	, CTL( PATTERNS_B_0A  )	, CTL( TRANSPOSE_C_0B  ) , CTL( PATTERNS_B_0C  ) , CTL( TRANSPOSE_B_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 11 ]  ) , &cb_patterns_1_0B	},
	{ COLUMN01 	, 0x10 	, CTL( PATTERNS_B_0B  )	, CTL( TRANSPOSE_C_0C  ) , CTL( PATTERNS_B_0D  ) , CTL( TRANSPOSE_B_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 12 ]  ) , &cb_patterns_1_0C	},
	{ COLUMN01 	, 0x11 	, CTL( PATTERNS_B_0C  )	, CTL( TRANSPOSE_C_0D  ) , CTL( PATTERNS_B_0E  ) , CTL( TRANSPOSE_B_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 13 ]  ) , &cb_patterns_1_0D	},
	{ COLUMN01 	, 0x12 	, CTL( PATTERNS_B_0D  )	, CTL( TRANSPOSE_C_0E  ) , CTL( PATTERNS_B_0F  ) , CTL( TRANSPOSE_B_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 14 ]  ) , &cb_patterns_1_0E	},
	{ COLUMN01 	, 0x13 	, CTL( PATTERNS_B_0E  )	, CTL( TRANSPOSE_C_0F  ) , CTL( SOLO_LEFT_00   ) , CTL( TRANSPOSE_B_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 1 , ORDER[ 15 ]  ) , &cb_patterns_1_0F	},
	{ COLUMN02 	, 0x04 	, CTL( SOLO_LEFT_02   ) , CTL( TRANSPOSE_D_00  ) , CTL( PATTERNS_C_01  ) , CTL( TRANSPOSE_C_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  0 ]  ) , &cb_patterns_2_00	}, 
	{ COLUMN02 	, 0x05 	, CTL( PATTERNS_C_00  )	, CTL( TRANSPOSE_D_01  ) , CTL( PATTERNS_C_02  ) , CTL( TRANSPOSE_C_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  1 ]  ) , &cb_patterns_2_01	}, 
	{ COLUMN02 	, 0x06 	, CTL( PATTERNS_C_01  )	, CTL( TRANSPOSE_D_02  ) , CTL( PATTERNS_C_03  ) , CTL( TRANSPOSE_C_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  2 ]  ) , &cb_patterns_2_02	}, 
	{ COLUMN02 	, 0x07 	, CTL( PATTERNS_C_02  )	, CTL( TRANSPOSE_D_03  ) , CTL( PATTERNS_C_04  ) , CTL( TRANSPOSE_C_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  3 ]  ) , &cb_patterns_2_03	}, 
	{ COLUMN02 	, 0x08 	, CTL( PATTERNS_C_03  )	, CTL( TRANSPOSE_D_04  ) , CTL( PATTERNS_C_05  ) , CTL( TRANSPOSE_C_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  4 ]  ) , &cb_patterns_2_04	}, 
	{ COLUMN02 	, 0x09 	, CTL( PATTERNS_C_04  )	, CTL( TRANSPOSE_D_05  ) , CTL( PATTERNS_C_06  ) , CTL( TRANSPOSE_C_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  5 ]  ) , &cb_patterns_2_05	}, 
	{ COLUMN02 	, 0x0a 	, CTL( PATTERNS_C_05  )	, CTL( TRANSPOSE_D_06  ) , CTL( PATTERNS_C_07  ) , CTL( TRANSPOSE_C_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  6 ]  ) , &cb_patterns_2_06	}, 
	{ COLUMN02 	, 0x0b 	, CTL( PATTERNS_C_06  )	, CTL( TRANSPOSE_D_07  ) , CTL( PATTERNS_C_08  ) , CTL( TRANSPOSE_C_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  7 ]  ) , &cb_patterns_2_07	}, 
	{ COLUMN02 	, 0x0c 	, CTL( PATTERNS_C_07  )	, CTL( TRANSPOSE_D_08  ) , CTL( PATTERNS_C_09  ) , CTL( TRANSPOSE_C_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  8 ]  ) , &cb_patterns_2_08	}, 
	{ COLUMN02 	, 0x0d 	, CTL( PATTERNS_C_08  )	, CTL( TRANSPOSE_D_09  ) , CTL( PATTERNS_C_0A  ) , CTL( TRANSPOSE_C_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[  9 ]  ) , &cb_patterns_2_09	}, 
	{ COLUMN02 	, 0x0e 	, CTL( PATTERNS_C_09  )	, CTL( TRANSPOSE_D_0A  ) , CTL( PATTERNS_C_0B  ) , CTL( TRANSPOSE_C_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 10 ]  ) , &cb_patterns_2_0A	}, 
	{ COLUMN02 	, 0x0f 	, CTL( PATTERNS_C_0A  )	, CTL( TRANSPOSE_D_0B  ) , CTL( PATTERNS_C_0C  ) , CTL( TRANSPOSE_C_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 11 ]  ) , &cb_patterns_2_0B	}, 
	{ COLUMN02 	, 0x10 	, CTL( PATTERNS_C_0B  )	, CTL( TRANSPOSE_D_0C  ) , CTL( PATTERNS_C_0D  ) , CTL( TRANSPOSE_C_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 12 ]  ) , &cb_patterns_2_0C	}, 
	{ COLUMN02 	, 0x11 	, CTL( PATTERNS_C_0C  )	, CTL( TRANSPOSE_D_0D  ) , CTL( PATTERNS_C_0E  ) , CTL( TRANSPOSE_C_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 13 ]  ) , &cb_patterns_2_0D	}, 
	{ COLUMN02 	, 0x12 	, CTL( PATTERNS_C_0D  )	, CTL( TRANSPOSE_D_0E  ) , CTL( PATTERNS_C_0F  ) , CTL( TRANSPOSE_C_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 14 ]  ) , &cb_patterns_2_0E	}, 
	{ COLUMN02 	, 0x13 	, CTL( PATTERNS_C_0E  )	, CTL( TRANSPOSE_D_0F  ) , CTL( SOLO_LEFT_00   ) , CTL( TRANSPOSE_C_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 2 , ORDER[ 15 ]  ) , &cb_patterns_2_0F	}, 
	{ COLUMN03 	, 0x04 	, CTL( MUTE_LEFT_02   ) , CTL( TRANSPOSE_E_00  ) , CTL( PATTERNS_D_01  ) , CTL( TRANSPOSE_D_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  0 ]  ) , &cb_patterns_3_00	},  
	{ COLUMN03 	, 0x05 	, CTL( PATTERNS_D_00  )	, CTL( TRANSPOSE_E_01  ) , CTL( PATTERNS_D_02  ) , CTL( TRANSPOSE_D_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  1 ]  ) , &cb_patterns_3_01	},  
	{ COLUMN03 	, 0x06 	, CTL( PATTERNS_D_01  )	, CTL( TRANSPOSE_E_02  ) , CTL( PATTERNS_D_03  ) , CTL( TRANSPOSE_D_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  2 ]  ) , &cb_patterns_3_02	},  
	{ COLUMN03 	, 0x07 	, CTL( PATTERNS_D_02  )	, CTL( TRANSPOSE_E_03  ) , CTL( PATTERNS_D_04  ) , CTL( TRANSPOSE_D_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  3 ]  ) , &cb_patterns_3_03	},  
	{ COLUMN03 	, 0x08 	, CTL( PATTERNS_D_03  )	, CTL( TRANSPOSE_E_04  ) , CTL( PATTERNS_D_05  ) , CTL( TRANSPOSE_D_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  4 ]  ) , &cb_patterns_3_04	},  
	{ COLUMN03 	, 0x09 	, CTL( PATTERNS_D_04  )	, CTL( TRANSPOSE_E_05  ) , CTL( PATTERNS_D_06  ) , CTL( TRANSPOSE_D_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  5 ]  ) , &cb_patterns_3_05	},  
	{ COLUMN03 	, 0x0a 	, CTL( PATTERNS_D_05  )	, CTL( TRANSPOSE_E_06  ) , CTL( PATTERNS_D_07  ) , CTL( TRANSPOSE_D_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  6 ]  ) , &cb_patterns_3_06	},  
	{ COLUMN03 	, 0x0b 	, CTL( PATTERNS_D_06  )	, CTL( TRANSPOSE_E_07  ) , CTL( PATTERNS_D_08  ) , CTL( TRANSPOSE_D_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  7 ]  ) , &cb_patterns_3_07	},  
	{ COLUMN03 	, 0x0c 	, CTL( PATTERNS_D_07  )	, CTL( TRANSPOSE_E_08  ) , CTL( PATTERNS_D_09  ) , CTL( TRANSPOSE_D_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  8 ]  ) , &cb_patterns_3_08	},  
	{ COLUMN03 	, 0x0d 	, CTL( PATTERNS_D_08  )	, CTL( TRANSPOSE_E_09  ) , CTL( PATTERNS_D_0A  ) , CTL( TRANSPOSE_D_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[  9 ]  ) , &cb_patterns_3_09	},  
	{ COLUMN03 	, 0x0e 	, CTL( PATTERNS_D_09  )	, CTL( TRANSPOSE_E_0A  ) , CTL( PATTERNS_D_0B  ) , CTL( TRANSPOSE_D_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 10 ]  ) , &cb_patterns_3_0A	},  
	{ COLUMN03 	, 0x0f 	, CTL( PATTERNS_D_0A  )	, CTL( TRANSPOSE_E_0B  ) , CTL( PATTERNS_D_0C  ) , CTL( TRANSPOSE_D_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 11 ]  ) , &cb_patterns_3_0B	},  
	{ COLUMN03 	, 0x10 	, CTL( PATTERNS_D_0B  )	, CTL( TRANSPOSE_E_0C  ) , CTL( PATTERNS_D_0D  ) , CTL( TRANSPOSE_D_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 12 ]  ) , &cb_patterns_3_0C	},  
	{ COLUMN03 	, 0x11 	, CTL( PATTERNS_D_0C  )	, CTL( TRANSPOSE_E_0D  ) , CTL( PATTERNS_D_0E  ) , CTL( TRANSPOSE_D_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 13 ]  ) , &cb_patterns_3_0D	},  
	{ COLUMN03 	, 0x12 	, CTL( PATTERNS_D_0D  )	, CTL( TRANSPOSE_E_0E  ) , CTL( PATTERNS_D_0F  ) , CTL( TRANSPOSE_D_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 14 ]  ) , &cb_patterns_3_0E	},  
	{ COLUMN03 	, 0x13 	, CTL( PATTERNS_D_0E  )	, CTL( TRANSPOSE_E_0F  ) , CTL( MUTE_LEFT_00   ) , CTL( TRANSPOSE_D_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 3 , ORDER[ 15 ]  ) , &cb_patterns_3_0F	},  
	{ COLUMN04 	, 0x04 	, CTL( MUTE_RIGHT_02  )	, CTL( TRANSPOSE_F_00  ) , CTL( PATTERNS_E_01  ) , CTL( TRANSPOSE_E_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  0 ]  ) , &cb_patterns_4_00	},  
	{ COLUMN04 	, 0x05 	, CTL( PATTERNS_E_00  )	, CTL( TRANSPOSE_F_01  ) , CTL( PATTERNS_E_02  ) , CTL( TRANSPOSE_E_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  1 ]  ) , &cb_patterns_4_01	},  
	{ COLUMN04 	, 0x06 	, CTL( PATTERNS_E_01  )	, CTL( TRANSPOSE_F_02  ) , CTL( PATTERNS_E_03  ) , CTL( TRANSPOSE_E_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  2 ]  ) , &cb_patterns_4_02	},  
	{ COLUMN04 	, 0x07 	, CTL( PATTERNS_E_02  )	, CTL( TRANSPOSE_F_03  ) , CTL( PATTERNS_E_04  ) , CTL( TRANSPOSE_E_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  3 ]  ) , &cb_patterns_4_03	},  
	{ COLUMN04 	, 0x08 	, CTL( PATTERNS_E_03  )	, CTL( TRANSPOSE_F_04  ) , CTL( PATTERNS_E_05  ) , CTL( TRANSPOSE_E_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  4 ]  ) , &cb_patterns_4_04	},  
	{ COLUMN04 	, 0x09 	, CTL( PATTERNS_E_04  )	, CTL( TRANSPOSE_F_05  ) , CTL( PATTERNS_E_06  ) , CTL( TRANSPOSE_E_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  5 ]  ) , &cb_patterns_4_05	},  
	{ COLUMN04 	, 0x0a 	, CTL( PATTERNS_E_05  )	, CTL( TRANSPOSE_F_06  ) , CTL( PATTERNS_E_07  ) , CTL( TRANSPOSE_E_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  6 ]  ) , &cb_patterns_4_06	},  
	{ COLUMN04 	, 0x0b 	, CTL( PATTERNS_E_06  )	, CTL( TRANSPOSE_F_07  ) , CTL( PATTERNS_E_08  ) , CTL( TRANSPOSE_E_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  7 ]  ) , &cb_patterns_4_07	},  
	{ COLUMN04 	, 0x0c 	, CTL( PATTERNS_E_07  )	, CTL( TRANSPOSE_F_08  ) , CTL( PATTERNS_E_09  ) , CTL( TRANSPOSE_E_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  8 ]  ) , &cb_patterns_4_08	},  
	{ COLUMN04 	, 0x0d 	, CTL( PATTERNS_E_08  )	, CTL( TRANSPOSE_F_09  ) , CTL( PATTERNS_E_0A  ) , CTL( TRANSPOSE_E_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[  9 ]  ) , &cb_patterns_4_09	},  
	{ COLUMN04 	, 0x0e 	, CTL( PATTERNS_E_09  )	, CTL( TRANSPOSE_F_0A  ) , CTL( PATTERNS_E_0B  ) , CTL( TRANSPOSE_E_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 10 ]  ) , &cb_patterns_4_0A	},  
	{ COLUMN04 	, 0x0f 	, CTL( PATTERNS_E_0A  )	, CTL( TRANSPOSE_F_0B  ) , CTL( PATTERNS_E_0C  ) , CTL( TRANSPOSE_E_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 11 ]  ) , &cb_patterns_4_0B	},  
	{ COLUMN04 	, 0x10 	, CTL( PATTERNS_E_0B  )	, CTL( TRANSPOSE_F_0C  ) , CTL( PATTERNS_E_0D  ) , CTL( TRANSPOSE_E_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 12 ]  ) , &cb_patterns_4_0C	},  
	{ COLUMN04 	, 0x11 	, CTL( PATTERNS_E_0C  )	, CTL( TRANSPOSE_F_0D  ) , CTL( PATTERNS_E_0E  ) , CTL( TRANSPOSE_E_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 13 ]  ) , &cb_patterns_4_0D	},  
	{ COLUMN04 	, 0x12 	, CTL( PATTERNS_E_0D  )	, CTL( TRANSPOSE_F_0E  ) , CTL( PATTERNS_E_0F  ) , CTL( TRANSPOSE_E_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 14 ]  ) , &cb_patterns_4_0E	},  
	{ COLUMN04 	, 0x13 	, CTL( PATTERNS_E_0E  )	, CTL( TRANSPOSE_F_0F  ) , CTL( MUTE_RIGHT_00  ) , CTL( TRANSPOSE_E_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 4 , ORDER[ 15 ]  ) , &cb_patterns_4_0F	},  
	{ COLUMN05 	, 0x04 	, CTL( SOLO_RIGHT_02  )	, CTL( TRANSPOSE_A_00  ) , CTL( PATTERNS_F_01  ) , CTL( TRANSPOSE_F_00  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  0 ]  ) , &cb_patterns_5_00	},  
	{ COLUMN05 	, 0x05 	, CTL( PATTERNS_F_00  )	, CTL( TRANSPOSE_A_01  ) , CTL( PATTERNS_F_02  ) , CTL( TRANSPOSE_F_01  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  1 ]  ) , &cb_patterns_5_01	},  
	{ COLUMN05 	, 0x06 	, CTL( PATTERNS_F_01  )	, CTL( TRANSPOSE_A_02  ) , CTL( PATTERNS_F_03  ) , CTL( TRANSPOSE_F_02  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  2 ]  ) , &cb_patterns_5_02	},  
	{ COLUMN05 	, 0x07 	, CTL( PATTERNS_F_02  )	, CTL( TRANSPOSE_A_03  ) , CTL( PATTERNS_F_04  ) , CTL( TRANSPOSE_F_03  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  3 ]  ) , &cb_patterns_5_03	},  
	{ COLUMN05 	, 0x08 	, CTL( PATTERNS_F_03  )	, CTL( TRANSPOSE_A_04  ) , CTL( PATTERNS_F_05  ) , CTL( TRANSPOSE_F_04  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  4 ]  ) , &cb_patterns_5_04	},  
	{ COLUMN05 	, 0x09 	, CTL( PATTERNS_F_04  )	, CTL( TRANSPOSE_A_05  ) , CTL( PATTERNS_F_06  ) , CTL( TRANSPOSE_F_05  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  5 ]  ) , &cb_patterns_5_05	},  
	{ COLUMN05 	, 0x0a 	, CTL( PATTERNS_F_05  )	, CTL( TRANSPOSE_A_06  ) , CTL( PATTERNS_F_07  ) , CTL( TRANSPOSE_F_06  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  6 ]  ) , &cb_patterns_5_06	},  
	{ COLUMN05 	, 0x0b 	, CTL( PATTERNS_F_06  )	, CTL( TRANSPOSE_A_07  ) , CTL( PATTERNS_F_08  ) , CTL( TRANSPOSE_F_07  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  7 ]  ) , &cb_patterns_5_07	},  
	{ COLUMN05 	, 0x0c 	, CTL( PATTERNS_F_07  )	, CTL( TRANSPOSE_A_08  ) , CTL( PATTERNS_F_09  ) , CTL( TRANSPOSE_F_08  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  8 ]  ) , &cb_patterns_5_08	},  
	{ COLUMN05 	, 0x0d 	, CTL( PATTERNS_F_08  )	, CTL( TRANSPOSE_A_09  ) , CTL( PATTERNS_F_0A  ) , CTL( TRANSPOSE_F_09  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[  9 ]  ) , &cb_patterns_5_09	},  
	{ COLUMN05 	, 0x0e 	, CTL( PATTERNS_F_09  )	, CTL( TRANSPOSE_A_0A  ) , CTL( PATTERNS_F_0B  ) , CTL( TRANSPOSE_F_0A  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 10 ]  ) , &cb_patterns_5_0A	},  
	{ COLUMN05 	, 0x0f 	, CTL( PATTERNS_F_0A  )	, CTL( TRANSPOSE_A_0B  ) , CTL( PATTERNS_F_0C  ) , CTL( TRANSPOSE_F_0B  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 11 ]  ) , &cb_patterns_5_0B	},  
	{ COLUMN05 	, 0x10 	, CTL( PATTERNS_F_0B  )	, CTL( TRANSPOSE_A_0C  ) , CTL( PATTERNS_F_0D  ) , CTL( TRANSPOSE_F_0C  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 12 ]  ) , &cb_patterns_5_0C	},  
	{ COLUMN05 	, 0x11 	, CTL( PATTERNS_F_0C  )	, CTL( TRANSPOSE_A_0D  ) , CTL( PATTERNS_F_0E  ) , CTL( TRANSPOSE_F_0D  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 13 ]  ) , &cb_patterns_5_0D	},  
	{ COLUMN05 	, 0x12 	, CTL( PATTERNS_F_0D  )	, CTL( TRANSPOSE_A_0E  ) , CTL( PATTERNS_F_0F  ) , CTL( TRANSPOSE_F_0E  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 14 ]  ) , &cb_patterns_5_0E	},  
	{ COLUMN05 	, 0x13 	, CTL( PATTERNS_F_0E  )	, CTL( TRANSPOSE_A_0F  ) , CTL( SOLO_RIGHT_00  ) , CTL( TRANSPOSE_F_0F  ) , &CACHE_HEXADECIMAL_DOUBLE_DASH, VAR( 5 , ORDER[ 15 ]  ) , &cb_patterns_5_0F	},  
	
	// TRANSPOSE CONTROLS 
	{ COLUMN00-1, 0x04 	, CTL( SOLO_LEFT_02	  ) , CTL( PATTERNS_A_00  ) , CTL( TRANSPOSE_A_01 ) , CTL( PATTERNS_F_00  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[  0 ]), &cb_transpose_0_00	},
	{ COLUMN00-1, 0x05 	, CTL( TRANSPOSE_A_00 )	, CTL( PATTERNS_A_01  ) , CTL( TRANSPOSE_A_02 ) , CTL( PATTERNS_F_01  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[  1 ]), &cb_transpose_0_01	},
	{ COLUMN00-1, 0x06 	, CTL( TRANSPOSE_A_01 )	, CTL( PATTERNS_A_02  ) , CTL( TRANSPOSE_A_03 ) , CTL( PATTERNS_F_02  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[  2 ]), &cb_transpose_0_02	},
	{ COLUMN00-1, 0x07 	, CTL( TRANSPOSE_A_02 )	, CTL( PATTERNS_A_03  ) , CTL( TRANSPOSE_A_04 ) , CTL( PATTERNS_F_03  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[  3 ]), &cb_transpose_0_03	},
	{ COLUMN00-1, 0x08 	, CTL( TRANSPOSE_A_03 )	, CTL( PATTERNS_A_04  ) , CTL( TRANSPOSE_A_05 ) , CTL( PATTERNS_F_04  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[  4 ]), &cb_transpose_0_04	},
	{ COLUMN00-1, 0x09 	, CTL( TRANSPOSE_A_04 )	, CTL( PATTERNS_A_05  ) , CTL( TRANSPOSE_A_06 ) , CTL( PATTERNS_F_05  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[  5 ]), &cb_transpose_0_05	},
	{ COLUMN00-1, 0x0a 	, CTL( TRANSPOSE_A_05 )	, CTL( PATTERNS_A_06  ) , CTL( TRANSPOSE_A_07 ) , CTL( PATTERNS_F_06  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[  6 ]), &cb_transpose_0_06	},
	{ COLUMN00-1, 0x0b 	, CTL( TRANSPOSE_A_06 )	, CTL( PATTERNS_A_07  ) , CTL( TRANSPOSE_A_08 ) , CTL( PATTERNS_F_07  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[  7 ]), &cb_transpose_0_07	},
	{ COLUMN00-1, 0x0c 	, CTL( TRANSPOSE_A_07 )	, CTL( PATTERNS_A_08  ) , CTL( TRANSPOSE_A_09 ) , CTL( PATTERNS_F_08  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[  8 ]), &cb_transpose_0_08	},
	{ COLUMN00-1, 0x0d 	, CTL( TRANSPOSE_A_08 )	, CTL( PATTERNS_A_09  ) , CTL( TRANSPOSE_A_0A ) , CTL( PATTERNS_F_09  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[  9 ]), &cb_transpose_0_09	},
	{ COLUMN00-1, 0x0e 	, CTL( TRANSPOSE_A_09 )	, CTL( PATTERNS_A_0A  ) , CTL( TRANSPOSE_A_0B ) , CTL( PATTERNS_F_0A  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[ 10 ]), &cb_transpose_0_0A	},
	{ COLUMN00-1, 0x0f 	, CTL( TRANSPOSE_A_0A )	, CTL( PATTERNS_A_0B  ) , CTL( TRANSPOSE_A_0C ) , CTL( PATTERNS_F_0B  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[ 11 ]), &cb_transpose_0_0B	},
	{ COLUMN00-1, 0x10 	, CTL( TRANSPOSE_A_0B )	, CTL( PATTERNS_A_0C  ) , CTL( TRANSPOSE_A_0D ) , CTL( PATTERNS_F_0C  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[ 12 ]), &cb_transpose_0_0C	},
	{ COLUMN00-1, 0x11 	, CTL( TRANSPOSE_A_0C )	, CTL( PATTERNS_A_0D  ) , CTL( TRANSPOSE_A_0E ) , CTL( PATTERNS_F_0D  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[ 13 ]), &cb_transpose_0_0D	},
	{ COLUMN00-1, 0x12 	, CTL( TRANSPOSE_A_0D )	, CTL( PATTERNS_A_0E  ) , CTL( TRANSPOSE_A_0F ) , CTL( PATTERNS_F_0E  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[ 14 ]), &cb_transpose_0_0E	},
	{ COLUMN00-1, 0x13 	, CTL( TRANSPOSE_A_0E )	, CTL( PATTERNS_A_0F  ) , CTL( SOLO_LEFT_00   ) , CTL( PATTERNS_F_0F  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 0 ,TRANSPOSE[ 15 ]), &cb_transpose_0_0F	},
	{ COLUMN01-1, 0x04 	, CTL( SOLO_LEFT_02   ) , CTL( PATTERNS_B_00  ) , CTL( TRANSPOSE_B_01 ) , CTL( PATTERNS_A_00  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[  0 ]), &cb_transpose_1_00	},
	{ COLUMN01-1, 0x05 	, CTL( TRANSPOSE_B_00 )	, CTL( PATTERNS_B_01  ) , CTL( TRANSPOSE_B_02 ) , CTL( PATTERNS_A_01  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[  1 ]), &cb_transpose_1_01	},
	{ COLUMN01-1, 0x06 	, CTL( TRANSPOSE_B_01 )	, CTL( PATTERNS_B_02  ) , CTL( TRANSPOSE_B_03 ) , CTL( PATTERNS_A_02  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[  2 ]), &cb_transpose_1_02	},
	{ COLUMN01-1, 0x07 	, CTL( TRANSPOSE_B_02 )	, CTL( PATTERNS_B_03  ) , CTL( TRANSPOSE_B_04 ) , CTL( PATTERNS_A_03  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[  3 ]), &cb_transpose_1_03	},
	{ COLUMN01-1, 0x08 	, CTL( TRANSPOSE_B_03 )	, CTL( PATTERNS_B_04  ) , CTL( TRANSPOSE_B_05 ) , CTL( PATTERNS_A_04  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[  4 ]), &cb_transpose_1_04	},
	{ COLUMN01-1, 0x09 	, CTL( TRANSPOSE_B_04 )	, CTL( PATTERNS_B_05  ) , CTL( TRANSPOSE_B_06 ) , CTL( PATTERNS_A_05  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[  5 ]), &cb_transpose_1_05	},
	{ COLUMN01-1, 0x0a 	, CTL( TRANSPOSE_B_05 )	, CTL( PATTERNS_B_06  ) , CTL( TRANSPOSE_B_07 ) , CTL( PATTERNS_A_06  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[  6 ]), &cb_transpose_1_06	},
	{ COLUMN01-1, 0x0b 	, CTL( TRANSPOSE_B_06 )	, CTL( PATTERNS_B_07  ) , CTL( TRANSPOSE_B_08 ) , CTL( PATTERNS_A_07  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[  7 ]), &cb_transpose_1_07	},
	{ COLUMN01-1, 0x0c 	, CTL( TRANSPOSE_B_07 )	, CTL( PATTERNS_B_08  ) , CTL( TRANSPOSE_B_09 ) , CTL( PATTERNS_A_08  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[  8 ]), &cb_transpose_1_08	},
	{ COLUMN01-1, 0x0d 	, CTL( TRANSPOSE_B_08 )	, CTL( PATTERNS_B_09  ) , CTL( TRANSPOSE_B_0A ) , CTL( PATTERNS_A_09  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[  9 ]), &cb_transpose_1_09	},
	{ COLUMN01-1, 0x0e 	, CTL( TRANSPOSE_B_09 )	, CTL( PATTERNS_B_0A  ) , CTL( TRANSPOSE_B_0B ) , CTL( PATTERNS_A_0A  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[ 10 ]), &cb_transpose_1_0A	},
	{ COLUMN01-1, 0x0f 	, CTL( TRANSPOSE_B_0A )	, CTL( PATTERNS_B_0B  ) , CTL( TRANSPOSE_B_0C ) , CTL( PATTERNS_A_0B  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[ 11 ]), &cb_transpose_1_0B	},
	{ COLUMN01-1, 0x10 	, CTL( TRANSPOSE_B_0B )	, CTL( PATTERNS_B_0C  ) , CTL( TRANSPOSE_B_0D ) , CTL( PATTERNS_A_0C  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[ 12 ]), &cb_transpose_1_0C	},
	{ COLUMN01-1, 0x11 	, CTL( TRANSPOSE_B_0C )	, CTL( PATTERNS_B_0D  ) , CTL( TRANSPOSE_B_0E ) , CTL( PATTERNS_A_0D  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[ 13 ]), &cb_transpose_1_0D	},
	{ COLUMN01-1, 0x12 	, CTL( TRANSPOSE_B_0D )	, CTL( PATTERNS_B_0E  ) , CTL( TRANSPOSE_B_0F ) , CTL( PATTERNS_A_0E  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[ 14 ]), &cb_transpose_1_0E	},
	{ COLUMN01-1, 0x13 	, CTL( TRANSPOSE_B_0E )	, CTL( PATTERNS_B_0F  ) , CTL( SOLO_LEFT_00   ) , CTL( PATTERNS_A_0F  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 1 ,TRANSPOSE[ 15 ]), &cb_transpose_1_0F	},
	{ COLUMN02-1, 0x04 	, CTL( SOLO_LEFT_02   ) , CTL( PATTERNS_C_00  ) , CTL( TRANSPOSE_C_01 ) , CTL( PATTERNS_B_00  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[  0 ]), &cb_transpose_2_00	}, 
	{ COLUMN02-1, 0x05 	, CTL( TRANSPOSE_C_00 )	, CTL( PATTERNS_C_01  ) , CTL( TRANSPOSE_C_02 ) , CTL( PATTERNS_B_01  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[  1 ]), &cb_transpose_2_01	}, 
	{ COLUMN02-1, 0x06 	, CTL( TRANSPOSE_C_01 )	, CTL( PATTERNS_C_02  ) , CTL( TRANSPOSE_C_03 ) , CTL( PATTERNS_B_02  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[  2 ]), &cb_transpose_2_02	}, 
	{ COLUMN02-1, 0x07 	, CTL( TRANSPOSE_C_02 )	, CTL( PATTERNS_C_03  ) , CTL( TRANSPOSE_C_04 ) , CTL( PATTERNS_B_03  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[  3 ]), &cb_transpose_2_03	}, 
	{ COLUMN02-1, 0x08 	, CTL( TRANSPOSE_C_03 )	, CTL( PATTERNS_C_04  ) , CTL( TRANSPOSE_C_05 ) , CTL( PATTERNS_B_04  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[  4 ]), &cb_transpose_2_04	}, 
	{ COLUMN02-1, 0x09 	, CTL( TRANSPOSE_C_04 )	, CTL( PATTERNS_C_05  ) , CTL( TRANSPOSE_C_06 ) , CTL( PATTERNS_B_05  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[  5 ]), &cb_transpose_2_05	}, 
	{ COLUMN02-1, 0x0a 	, CTL( TRANSPOSE_C_05 )	, CTL( PATTERNS_C_06  ) , CTL( TRANSPOSE_C_07 ) , CTL( PATTERNS_B_06  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[  6 ]), &cb_transpose_2_06	}, 
	{ COLUMN02-1, 0x0b 	, CTL( TRANSPOSE_C_06 )	, CTL( PATTERNS_C_07  ) , CTL( TRANSPOSE_C_08 ) , CTL( PATTERNS_B_07  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[  7 ]), &cb_transpose_2_07	}, 
	{ COLUMN02-1, 0x0c 	, CTL( TRANSPOSE_C_07 )	, CTL( PATTERNS_C_08  ) , CTL( TRANSPOSE_C_09 ) , CTL( PATTERNS_B_08  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[  8 ]), &cb_transpose_2_08	}, 
	{ COLUMN02-1, 0x0d 	, CTL( TRANSPOSE_C_08 )	, CTL( PATTERNS_C_09  ) , CTL( TRANSPOSE_C_0A ) , CTL( PATTERNS_B_09  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[  9 ]), &cb_transpose_2_09	}, 
	{ COLUMN02-1, 0x0e 	, CTL( TRANSPOSE_C_09 )	, CTL( PATTERNS_C_0A  ) , CTL( TRANSPOSE_C_0B ) , CTL( PATTERNS_B_0A  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[ 10 ]), &cb_transpose_2_0A	}, 
	{ COLUMN02-1, 0x0f 	, CTL( TRANSPOSE_C_0A )	, CTL( PATTERNS_C_0B  ) , CTL( TRANSPOSE_C_0C ) , CTL( PATTERNS_B_0B  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[ 11 ]), &cb_transpose_2_0B	}, 
	{ COLUMN02-1, 0x10 	, CTL( TRANSPOSE_C_0B )	, CTL( PATTERNS_C_0C  ) , CTL( TRANSPOSE_C_0D ) , CTL( PATTERNS_B_0C  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[ 12 ]), &cb_transpose_2_0C	}, 
	{ COLUMN02-1, 0x11 	, CTL( TRANSPOSE_C_0C )	, CTL( PATTERNS_C_0D  ) , CTL( TRANSPOSE_C_0E ) , CTL( PATTERNS_B_0D  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[ 13 ]), &cb_transpose_2_0D	}, 
	{ COLUMN02-1, 0x12 	, CTL( TRANSPOSE_C_0D )	, CTL( PATTERNS_C_0E  ) , CTL( TRANSPOSE_C_0F ) , CTL( PATTERNS_B_0E  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[ 14 ]), &cb_transpose_2_0E	}, 
	{ COLUMN02-1, 0x13 	, CTL( TRANSPOSE_C_0E )	, CTL( PATTERNS_C_0F  ) , CTL( SOLO_LEFT_00   ) , CTL( PATTERNS_B_0F  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 2 ,TRANSPOSE[ 15 ]), &cb_transpose_2_0F	}, 
	{ COLUMN03-1, 0x04 	, CTL( MUTE_LEFT_02   ) , CTL( PATTERNS_D_00  ) , CTL( TRANSPOSE_D_01 ) , CTL( PATTERNS_C_00  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[  0 ]), &cb_transpose_3_00	},  
	{ COLUMN03-1, 0x05 	, CTL( TRANSPOSE_D_00 )	, CTL( PATTERNS_D_01  ) , CTL( TRANSPOSE_D_02 ) , CTL( PATTERNS_C_01  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[  1 ]), &cb_transpose_3_01	},  
	{ COLUMN03-1, 0x06 	, CTL( TRANSPOSE_D_01 )	, CTL( PATTERNS_D_02  ) , CTL( TRANSPOSE_D_03 ) , CTL( PATTERNS_C_02  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[  2 ]), &cb_transpose_3_02	},  
	{ COLUMN03-1, 0x07 	, CTL( TRANSPOSE_D_02 )	, CTL( PATTERNS_D_03  ) , CTL( TRANSPOSE_D_04 ) , CTL( PATTERNS_C_03  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[  3 ]), &cb_transpose_3_03	},  
	{ COLUMN03-1, 0x08 	, CTL( TRANSPOSE_D_03 )	, CTL( PATTERNS_D_04  ) , CTL( TRANSPOSE_D_05 ) , CTL( PATTERNS_C_04  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[  4 ]), &cb_transpose_3_04	},  
	{ COLUMN03-1, 0x09 	, CTL( TRANSPOSE_D_04 )	, CTL( PATTERNS_D_05  ) , CTL( TRANSPOSE_D_06 ) , CTL( PATTERNS_C_05  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[  5 ]), &cb_transpose_3_05	},  
	{ COLUMN03-1, 0x0a 	, CTL( TRANSPOSE_D_05 )	, CTL( PATTERNS_D_06  ) , CTL( TRANSPOSE_D_07 ) , CTL( PATTERNS_C_06  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[  6 ]), &cb_transpose_3_06	},  
	{ COLUMN03-1, 0x0b 	, CTL( TRANSPOSE_D_06 )	, CTL( PATTERNS_D_07  ) , CTL( TRANSPOSE_D_08 ) , CTL( PATTERNS_C_07  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[  7 ]), &cb_transpose_3_07	},  
	{ COLUMN03-1, 0x0c 	, CTL( TRANSPOSE_D_07 )	, CTL( PATTERNS_D_08  ) , CTL( TRANSPOSE_D_09 ) , CTL( PATTERNS_C_08  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[  8 ]), &cb_transpose_3_08	},  
	{ COLUMN03-1, 0x0d 	, CTL( TRANSPOSE_D_08 )	, CTL( PATTERNS_D_09  ) , CTL( TRANSPOSE_D_0A ) , CTL( PATTERNS_C_09  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[  9 ]), &cb_transpose_3_09	},  
	{ COLUMN03-1, 0x0e 	, CTL( TRANSPOSE_D_09 )	, CTL( PATTERNS_D_0A  ) , CTL( TRANSPOSE_D_0B ) , CTL( PATTERNS_C_0A  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[ 10 ]), &cb_transpose_3_0A	},  
	{ COLUMN03-1, 0x0f 	, CTL( TRANSPOSE_D_0A )	, CTL( PATTERNS_D_0B  ) , CTL( TRANSPOSE_D_0C ) , CTL( PATTERNS_C_0B  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[ 11 ]), &cb_transpose_3_0B	},  
	{ COLUMN03-1, 0x10 	, CTL( TRANSPOSE_D_0B )	, CTL( PATTERNS_D_0C  ) , CTL( TRANSPOSE_D_0D ) , CTL( PATTERNS_C_0C  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[ 12 ]), &cb_transpose_3_0C	},  
	{ COLUMN03-1, 0x11 	, CTL( TRANSPOSE_D_0C )	, CTL( PATTERNS_D_0D  ) , CTL( TRANSPOSE_D_0E ) , CTL( PATTERNS_C_0D  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[ 13 ]), &cb_transpose_3_0D	},  
	{ COLUMN03-1, 0x12 	, CTL( TRANSPOSE_D_0D )	, CTL( PATTERNS_D_0E  ) , CTL( TRANSPOSE_D_0F ) , CTL( PATTERNS_C_0E  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[ 14 ]), &cb_transpose_3_0E	},  
	{ COLUMN03-1, 0x13 	, CTL( TRANSPOSE_D_0E )	, CTL( PATTERNS_D_0F  ) , CTL( MUTE_LEFT_00   ) , CTL( PATTERNS_C_0F  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 3 ,TRANSPOSE[ 15 ]), &cb_transpose_3_0F	},  
	{ COLUMN04-1, 0x04 	, CTL( MUTE_RIGHT_02  )	, CTL( PATTERNS_E_00  ) , CTL( TRANSPOSE_E_01 ) , CTL( PATTERNS_D_00  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[  0 ]), &cb_transpose_4_00	},  
	{ COLUMN04-1, 0x05 	, CTL( PATTERNS_E_00  )	, CTL( PATTERNS_E_01  ) , CTL( TRANSPOSE_E_02 ) , CTL( PATTERNS_D_01  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[  1 ]), &cb_transpose_4_01	},  
	{ COLUMN04-1, 0x06 	, CTL( TRANSPOSE_E_01 )	, CTL( PATTERNS_E_02  ) , CTL( TRANSPOSE_E_03 ) , CTL( PATTERNS_D_02  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[  2 ]), &cb_transpose_4_02	},  
	{ COLUMN04-1, 0x07 	, CTL( TRANSPOSE_E_02 )	, CTL( PATTERNS_E_03  ) , CTL( TRANSPOSE_E_04 ) , CTL( PATTERNS_D_03  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[  3 ]), &cb_transpose_4_03	},  
	{ COLUMN04-1, 0x08 	, CTL( TRANSPOSE_E_03 )	, CTL( PATTERNS_E_04  ) , CTL( TRANSPOSE_E_05 ) , CTL( PATTERNS_D_04  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[  4 ]), &cb_transpose_4_04	},  
	{ COLUMN04-1, 0x09 	, CTL( TRANSPOSE_E_04 )	, CTL( PATTERNS_E_05  ) , CTL( TRANSPOSE_E_06 ) , CTL( PATTERNS_D_05  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[  5 ]), &cb_transpose_4_05	},  
	{ COLUMN04-1, 0x0a 	, CTL( TRANSPOSE_E_05 )	, CTL( PATTERNS_E_06  ) , CTL( TRANSPOSE_E_07 ) , CTL( PATTERNS_D_06  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[  6 ]), &cb_transpose_4_06	},  
	{ COLUMN04-1, 0x0b 	, CTL( TRANSPOSE_E_06 )	, CTL( PATTERNS_E_07  ) , CTL( TRANSPOSE_E_08 ) , CTL( PATTERNS_D_07  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[  7 ]), &cb_transpose_4_07	},  
	{ COLUMN04-1, 0x0c 	, CTL( TRANSPOSE_E_07 )	, CTL( PATTERNS_E_08  ) , CTL( TRANSPOSE_E_09 ) , CTL( PATTERNS_D_08  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[  8 ]), &cb_transpose_4_08	},  
	{ COLUMN04-1, 0x0d 	, CTL( TRANSPOSE_E_08 )	, CTL( PATTERNS_E_09  ) , CTL( TRANSPOSE_E_0A ) , CTL( PATTERNS_D_09  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[  9 ]), &cb_transpose_4_09	},  
	{ COLUMN04-1, 0x0e 	, CTL( TRANSPOSE_E_09 )	, CTL( PATTERNS_E_0A  ) , CTL( TRANSPOSE_E_0B ) , CTL( PATTERNS_D_0A  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[ 10 ]), &cb_transpose_4_0A	},  
	{ COLUMN04-1, 0x0f 	, CTL( TRANSPOSE_E_0A )	, CTL( PATTERNS_E_0B  ) , CTL( TRANSPOSE_E_0C ) , CTL( PATTERNS_D_0B  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[ 11 ]), &cb_transpose_4_0B	},  
	{ COLUMN04-1, 0x10 	, CTL( TRANSPOSE_E_0B )	, CTL( PATTERNS_E_0C  ) , CTL( TRANSPOSE_E_0D ) , CTL( PATTERNS_D_0C  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[ 12 ]), &cb_transpose_4_0C	},  
	{ COLUMN04-1, 0x11 	, CTL( TRANSPOSE_E_0C )	, CTL( PATTERNS_E_0D  ) , CTL( TRANSPOSE_E_0E ) , CTL( PATTERNS_D_0D  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[ 13 ]), &cb_transpose_4_0D	},  
	{ COLUMN04-1, 0x12 	, CTL( TRANSPOSE_E_0D )	, CTL( PATTERNS_E_0E  ) , CTL( TRANSPOSE_E_0F ) , CTL( PATTERNS_D_0E  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[ 14 ]), &cb_transpose_4_0E	},  
	{ COLUMN04-1, 0x13 	, CTL( TRANSPOSE_E_0E )	, CTL( PATTERNS_E_0F  ) , CTL( MUTE_RIGHT_00  ) , CTL( PATTERNS_D_0F  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 4 ,TRANSPOSE[ 15 ]), &cb_transpose_4_0F	},  
	{ COLUMN05-1, 0x04 	, CTL( SOLO_RIGHT_02  )	, CTL( PATTERNS_F_00  ) , CTL( TRANSPOSE_F_01 ) , CTL( PATTERNS_E_00  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[  0 ]), &cb_transpose_5_00	},  
	{ COLUMN05-1, 0x05 	, CTL( TRANSPOSE_F_00 )	, CTL( PATTERNS_F_01  ) , CTL( TRANSPOSE_F_02 ) , CTL( PATTERNS_E_01  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[  1 ]), &cb_transpose_5_01	},  
	{ COLUMN05-1, 0x06 	, CTL( TRANSPOSE_F_01 )	, CTL( PATTERNS_F_02  ) , CTL( TRANSPOSE_F_03 ) , CTL( PATTERNS_E_02  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[  2 ]), &cb_transpose_5_02	},  
	{ COLUMN05-1, 0x07 	, CTL( TRANSPOSE_F_02 )	, CTL( PATTERNS_F_03  ) , CTL( TRANSPOSE_F_04 ) , CTL( PATTERNS_E_03  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[  3 ]), &cb_transpose_5_03	},  
	{ COLUMN05-1, 0x08 	, CTL( TRANSPOSE_F_03 )	, CTL( PATTERNS_F_04  ) , CTL( TRANSPOSE_F_05 ) , CTL( PATTERNS_E_04  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[  4 ]), &cb_transpose_5_04	},  
	{ COLUMN05-1, 0x09 	, CTL( TRANSPOSE_F_04 )	, CTL( PATTERNS_F_05  ) , CTL( TRANSPOSE_F_06 ) , CTL( PATTERNS_E_05  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[  5 ]), &cb_transpose_5_05	},  
	{ COLUMN05-1, 0x0a 	, CTL( TRANSPOSE_F_05 )	, CTL( PATTERNS_F_06  ) , CTL( TRANSPOSE_F_07 ) , CTL( PATTERNS_E_06  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[  6 ]), &cb_transpose_5_06	},  
	{ COLUMN05-1, 0x0b 	, CTL( TRANSPOSE_F_06 )	, CTL( PATTERNS_F_07  ) , CTL( TRANSPOSE_F_08 ) , CTL( PATTERNS_E_07  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[  7 ]), &cb_transpose_5_07	},  
	{ COLUMN05-1, 0x0c 	, CTL( TRANSPOSE_F_07 )	, CTL( PATTERNS_F_08  ) , CTL( TRANSPOSE_F_09 ) , CTL( PATTERNS_E_08  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[  8 ]), &cb_transpose_5_08	},  
	{ COLUMN05-1, 0x0d 	, CTL( TRANSPOSE_F_08 )	, CTL( PATTERNS_F_09  ) , CTL( TRANSPOSE_F_0A ) , CTL( PATTERNS_E_09  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[  9 ]), &cb_transpose_5_09	},  
	{ COLUMN05-1, 0x0e 	, CTL( TRANSPOSE_F_09 )	, CTL( PATTERNS_F_0A  ) , CTL( TRANSPOSE_F_0B ) , CTL( PATTERNS_E_0A  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[ 10 ]), &cb_transpose_5_0A	},  
	{ COLUMN05-1, 0x0f 	, CTL( TRANSPOSE_F_0A )	, CTL( PATTERNS_F_0B  ) , CTL( TRANSPOSE_F_0C ) , CTL( PATTERNS_E_0B  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[ 11 ]), &cb_transpose_5_0B	},  
	{ COLUMN05-1, 0x10 	, CTL( TRANSPOSE_F_0B )	, CTL( PATTERNS_F_0C  ) , CTL( TRANSPOSE_F_0D ) , CTL( PATTERNS_E_0C  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[ 12 ]), &cb_transpose_5_0C	},  
	{ COLUMN05-1, 0x11 	, CTL( TRANSPOSE_F_0C )	, CTL( PATTERNS_F_0D  ) , CTL( TRANSPOSE_F_0E ) , CTL( PATTERNS_E_0D  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[ 13 ]), &cb_transpose_5_0D	},  
	{ COLUMN05-1, 0x12 	, CTL( TRANSPOSE_F_0D )	, CTL( PATTERNS_F_0E  ) , CTL( TRANSPOSE_F_0F ) , CTL( PATTERNS_E_0E  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[ 14 ]), &cb_transpose_5_0E	},  
	{ COLUMN05-1, 0x13 	, CTL( TRANSPOSE_F_0E )	, CTL( PATTERNS_F_0F  ) , CTL( SOLO_RIGHT_00  ) , CTL( PATTERNS_E_0F  ) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( 5 ,TRANSPOSE[ 15 ]), &cb_transpose_5_0F	},  
	#endif
	{ 0x0c  	, 0x00 	, CTL( PATTERNS_C_0F  )	, CTL( MUTE_LEFT_00   ) , CTL( SOLO_LEFT_01   ) , CTL( SOLO_RIGHT_00  ) , &CACHE_ARROW_LEFT				, NULL					  , &cb_pat_solo_0		},
	{ 0x0c  	, 0x01 	, CTL( SOLO_LEFT_00   )	, CTL( MUTE_LEFT_01   ) , CTL( SOLO_LEFT_02   ) , CTL( SOLO_RIGHT_01  ) , &CACHE_ARROW_LEFT				, NULL					  , &cb_pat_solo_1		},
	{ 0x0c  	, 0x02 	, CTL( SOLO_LEFT_01   )	, CTL( MUTE_LEFT_02   ) , CTL( PATTERNS_C_00  ) , CTL( SOLO_RIGHT_02  ) , &CACHE_ARROW_LEFT				, NULL					  , &cb_pat_solo_2		},
	{ 0x0f  	, 0x00 	, CTL( PATTERNS_D_0F  )	, CTL( MUTE_RIGHT_00  ) , CTL( MUTE_LEFT_01   ) , CTL( SOLO_LEFT_00   ) , &CACHE_ARROW_LEFT				, NULL					  , &cb_pat_mute_0		},
	{ 0x0f  	, 0x01 	, CTL( MUTE_LEFT_00   )	, CTL( MUTE_RIGHT_01  ) , CTL( MUTE_LEFT_02   ) , CTL( SOLO_LEFT_01   ) , &CACHE_ARROW_LEFT				, NULL					  , &cb_pat_mute_1		},
	{ 0x0f  	, 0x02 	, CTL( MUTE_LEFT_01   )	, CTL( MUTE_RIGHT_02  ) , CTL( PATTERNS_D_00  ) , CTL( SOLO_LEFT_02   ) , &CACHE_ARROW_LEFT				, NULL					  , &cb_pat_mute_2		},
	{ 0x16  	, 0x00 	, CTL( PATTERNS_E_0F  )	, CTL( SOLO_RIGHT_00  ) , CTL( MUTE_RIGHT_01  ) , CTL( MUTE_LEFT_00   ) , &CACHE_ARROW_RIGHT			, NULL 					  , &cb_pat_mute_3		},
	{ 0x16  	, 0x01 	, CTL( MUTE_RIGHT_00  )	, CTL( SOLO_RIGHT_01  ) , CTL( MUTE_RIGHT_02  ) , CTL( MUTE_LEFT_01   ) , &CACHE_ARROW_RIGHT			, NULL 					  , &cb_pat_mute_4		},
	{ 0x16  	, 0x02 	, CTL( MUTE_RIGHT_01  )	, CTL( SOLO_RIGHT_02  ) , CTL( PATTERNS_E_00  ) , CTL( MUTE_LEFT_02   ) , &CACHE_ARROW_RIGHT			, NULL 		 			  , &cb_pat_mute_5		},
	{ 0x19  	, 0x00 	, CTL( PATTERNS_F_0F  )	, CTL( SOLO_LEFT_00   ) , CTL( SOLO_RIGHT_01  ) , CTL( MUTE_RIGHT_00  ) , &CACHE_ARROW_RIGHT			, NULL 					  , &cb_pat_solo_3		}, 
	{ 0x19  	, 0x01 	, CTL( SOLO_RIGHT_00  )	, CTL( SOLO_LEFT_01   ) , CTL( SOLO_RIGHT_02  ) , CTL( MUTE_RIGHT_01  ) , &CACHE_ARROW_RIGHT			, NULL 					  , &cb_pat_solo_4		},
	{ 0x19  	, 0x02 	, CTL( SOLO_RIGHT_01  )	, CTL( SOLO_LEFT_02   ) , CTL( PATTERNS_F_00  ) , CTL( MUTE_RIGHT_02  ) , &CACHE_ARROW_RIGHT			, NULL 					  , &cb_pat_solo_5		},
	CONTROL_TERMINATOR
};
#undef CTL
#undef VAR
	

void PatEdit::syncPosition( u8 position ){
	// Called each time VAR_CFG.ORDERPOSITION CHANGES
	for( int i=0, p; i < 16; i++ ){
		p = i+position;
		VAR_PATTERN[0].ORDER[i] = VAR_SONG.PATTERNS[0].ORDER[p];
		VAR_PATTERN[1].ORDER[i] = VAR_SONG.PATTERNS[1].ORDER[p];
		VAR_PATTERN[2].ORDER[i] = VAR_SONG.PATTERNS[2].ORDER[p];
		VAR_PATTERN[3].ORDER[i] = VAR_SONG.PATTERNS[3].ORDER[p];
		VAR_PATTERN[4].ORDER[i] = VAR_SONG.PATTERNS[4].ORDER[p];
		VAR_PATTERN[5].ORDER[i] = VAR_SONG.PATTERNS[5].ORDER[p];
	}
}

void PatEdit::copy( u8 channel_index ){
	// Called on value change @ PATtern editor
	VAR_CFG.CURRENTCHANNEL = channel_index;
	// *REGION_MAP_3_TRK.viewport->var = channel;	
	
	for(int i=0; i<16;i++){		
		VAR_SONG.PATTERNS[ channel_index ].ORDER[ VAR_CFG.ORDERPOSITION + i ] = VAR_PATTERN[ channel_index ].ORDER[ i ];
		#ifndef NSONGTRANSPOSE
		VAR_SONG.PATTERNS[ channel_index ].TRANSPOSE[ VAR_CFG.ORDERPOSITION + i ] = VAR_PATTERN[ channel_index ].TRANSPOSE[ i ];
		#endif
	}
	
	PatEdit::sync();
}

void patternCH0_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 0 ); }
void patternCH1_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 1 ); }
void patternCH2_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 2 ); }
void patternCH3_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 3 ); }
void patternCH4_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 4 ); }
void patternCH5_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 5 ); }

void patternCH0_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 0 ); }
void patternCH1_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 1 ); }
void patternCH2_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 2 ); }
void patternCH3_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 3 ); }
void patternCH4_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 4 ); }
void patternCH5_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); 	PatEdit::copy( 5 ); }

void patternCH0_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 0; PatEdit::bookmark_row = *(u8*)pointer;}
void patternCH1_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 1; PatEdit::bookmark_row = *(u8*)pointer;}
void patternCH2_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 2;	PatEdit::bookmark_row = *(u8*)pointer;}
void patternCH3_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 3;	PatEdit::bookmark_row = *(u8*)pointer;}
void patternCH4_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 4;	PatEdit::bookmark_row = *(u8*)pointer;}
void patternCH5_focus(Control *c, bool bigstep, bool add, u32 *pointer){	VAR_CFG.CURRENTCHANNEL = 5;	PatEdit::bookmark_row = *(u8*)pointer;}

//PATTERNS
#define CB_PATTERNS(c, a)																																				\
const Callback cb_patterns_focus_##c##_0##a = { patternCH##c##_focus	, EVENT_FOCUS 		, (u32*)&NUMBERS[0x0##a] 			, NULL							};		\
const Callback cb_patterns_paste_##c##_0##a = { patternCH##c##_paste	, EVENT_KEYDOWN_B 	, &VAR_PATTERN[0x##c].ORDER[0x##a] 	, &cb_patterns_focus_##c##_0##a };		\
const Callback cb_patterns_##c##_0##a		= { patternCH##c##_alter	, EVENT_MODIFY_B 	, &VAR_PATTERN[0x##c].ORDER[0x##a] 	, &cb_patterns_paste_##c##_0##a };  
CB_PATTERNS(0, 0);		CB_PATTERNS(1, 0);		CB_PATTERNS(2, 0);		CB_PATTERNS(3, 0);		CB_PATTERNS(4, 0);		CB_PATTERNS(5, 0);
CB_PATTERNS(0, 1);		CB_PATTERNS(1, 1);		CB_PATTERNS(2, 1);		CB_PATTERNS(3, 1);		CB_PATTERNS(4, 1);		CB_PATTERNS(5, 1);
CB_PATTERNS(0, 2);		CB_PATTERNS(1, 2);		CB_PATTERNS(2, 2);		CB_PATTERNS(3, 2);		CB_PATTERNS(4, 2);		CB_PATTERNS(5, 2);
CB_PATTERNS(0, 3);		CB_PATTERNS(1, 3);		CB_PATTERNS(2, 3);		CB_PATTERNS(3, 3);		CB_PATTERNS(4, 3);		CB_PATTERNS(5, 3);
CB_PATTERNS(0, 4);		CB_PATTERNS(1, 4);		CB_PATTERNS(2, 4);		CB_PATTERNS(3, 4);		CB_PATTERNS(4, 4);		CB_PATTERNS(5, 4);
CB_PATTERNS(0, 5);		CB_PATTERNS(1, 5);		CB_PATTERNS(2, 5);		CB_PATTERNS(3, 5);		CB_PATTERNS(4, 5);		CB_PATTERNS(5, 5);
CB_PATTERNS(0, 6);		CB_PATTERNS(1, 6);		CB_PATTERNS(2, 6);		CB_PATTERNS(3, 6);		CB_PATTERNS(4, 6);		CB_PATTERNS(5, 6);
CB_PATTERNS(0, 7);		CB_PATTERNS(1, 7);		CB_PATTERNS(2, 7);		CB_PATTERNS(3, 7);		CB_PATTERNS(4, 7);		CB_PATTERNS(5, 7);
CB_PATTERNS(0, 8);		CB_PATTERNS(1, 8);		CB_PATTERNS(2, 8);		CB_PATTERNS(3, 8);		CB_PATTERNS(4, 8);		CB_PATTERNS(5, 8);
CB_PATTERNS(0, 9);		CB_PATTERNS(1, 9);		CB_PATTERNS(2, 9);		CB_PATTERNS(3, 9);		CB_PATTERNS(4, 9);		CB_PATTERNS(5, 9);
CB_PATTERNS(0, A);		CB_PATTERNS(1, A);		CB_PATTERNS(2, A);		CB_PATTERNS(3, A);		CB_PATTERNS(4, A);		CB_PATTERNS(5, A);
CB_PATTERNS(0, B);		CB_PATTERNS(1, B);		CB_PATTERNS(2, B);		CB_PATTERNS(3, B);		CB_PATTERNS(4, B);		CB_PATTERNS(5, B);
CB_PATTERNS(0, C);		CB_PATTERNS(1, C);		CB_PATTERNS(2, C);		CB_PATTERNS(3, C);		CB_PATTERNS(4, C);		CB_PATTERNS(5, C);
CB_PATTERNS(0, D);		CB_PATTERNS(1, D);		CB_PATTERNS(2, D);		CB_PATTERNS(3, D);		CB_PATTERNS(4, D);		CB_PATTERNS(5, D);
CB_PATTERNS(0, E);		CB_PATTERNS(1, E);		CB_PATTERNS(2, E);		CB_PATTERNS(3, E);		CB_PATTERNS(4, E);		CB_PATTERNS(5, E);
CB_PATTERNS(0, F);		CB_PATTERNS(1, F);		CB_PATTERNS(2, F);		CB_PATTERNS(3, F);		CB_PATTERNS(4, F);		CB_PATTERNS(5, F);
#undef CB_PATTERNS


	
	
void PatEdit::globalUpdate(RegionHandler* rh){
	static int bookmark_timer = 0;
		
	if(KEY.press(KEY_B)) {
		bookmark_timer++;
		if(bookmark_timer > 2000) {			
			PatEdit::bookmarks[ VAR_CFG.CURRENTCHANNEL ] = VAR_CFG.ORDERPOSITION + PatEdit::bookmark_row;
			PatEdit::clean = false;
			bookmark_timer = 0;
		}
		return;
	}
	bookmark_timer = 0;
}
	
void PatEdit::update(RegionHandler* rh){
	static int last_position=0xFFFF;
	
	Clip::update( rh );
	
	PatEdit::globalUpdate( rh );
	
	// Draw position arrows
	for(int c=0, x=0, y=0; c < 6; c++) {
		if( ( !PatEdit::clean ) || ( rh->redraw ) || ( VAR_CHANNEL[ c ].POSITION != VAR_CHANNEL[ c ].LASTPOSITION ) ){
			
			if(PatEdit::bookmarks[c]>0) {
				y = PatEdit::bookmarks[c] - VAR_CFG.ORDERPOSITION;
				if((y<=15)&&(y>=0)){
					gpu.set(0, PatEdit::arrow_position[c]+1, 4+y, 0x8);
					gpu.set(0, PatEdit::arrow_position[c]+2, 4+y, 0x8);
				}
			}
			y = VAR_CHANNEL[c].POSITION - VAR_CFG.ORDERPOSITION;
			x = VAR_CHANNEL[c].LASTPOSITION - VAR_CFG.ORDERPOSITION;
			if((x<=15)&&(x>=0)) gpu.set(2, PatEdit::arrow_position[c], 4+x, 0x00FC);
			if((y<=15)&&(y>=0)) gpu.set(2, PatEdit::arrow_position[c], 4+y, 0x408D);
		}
		PatEdit::solo_clean = false;
	}

	if( !PatEdit::solo_clean ){
		/* ------------------------------------------------------------
		Redraw mute / solo icons 									 */
		for(int c=0; c < 6; c++) {
			/* ------------------------------------------------------------
			If mute is enabled, draw MUTE on High Red color 			 */
			if(VAR_CHANNEL[c].mute) { 
				gpu.set(2, PatEdit::arrow_position[c]+1, 3, 0x3057);
				gpu.set(2, PatEdit::arrow_position[c]+2, 3, 0x3058); 
				continue; 
			}
			/* ------------------------------------------------------------
			If solo is enabled, draw SOLO on High green color	   		 */
			if(VAR_CHANNEL[c].solo) { 
				gpu.set(2, PatEdit::arrow_position[c]+1, 3, 0xF055);
				gpu.set(2, PatEdit::arrow_position[c]+2, 3, 0xF056); 
				continue;
			}
			/* ------------------------------------------------------------
			Else, draw channel icon		 								 */
				gpu.set(2, PatEdit::arrow_position[c]+1, 3, PatEdit::channel_symbols[c]); 
				gpu.set(2, PatEdit::arrow_position[c]+2, 3, 0x00FC); 
			// ------------------------------------------------------------
		}
		// ------------------------------------------------------------
		PatEdit::solo_clean = true;
	}
	
	// Draw position tags (Sides)
	if( ( !PatEdit::clean ) || ( last_position != VAR_CFG.ORDERPOSITION ) || ( rh->redraw ) ){		
		for(int c=0, h=0, y=4, v=VAR_CFG.ORDERPOSITION; c < 16; c++) {
			h = (c & 1) ? 0x3 : 0x5;
			HEXADECIMAL( 1, y, h, v>>4);
			HEXADECIMAL( 2, y, h, v&0xf);
			HEXADECIMAL(27, y, h, v>>4);
			HEXADECIMAL(28, y, h, v&0xf);
			
			v++;
			y++;
		}
		last_position = VAR_CFG.ORDERPOSITION;
		PatEdit::clean = true;
	}
}
//----------------------------------------------------------------------------------------
// PAT SCREEN CALLBACKS
//----------------------------------------------------------------------------------------
void PatEdit::solo(Control *c, bool bigstep, bool add, u32 *pointer){	
	#define MATCH( match ) 	( c == &PAT_CONTROLS[ CONTROL_PAT_SOLO_##match ] ) 
	int index 	= MATCH( LEFT_00  ) ? 0x0 
				: MATCH( LEFT_01  ) ? 0x1 
				: MATCH( LEFT_02  ) ? 0x2 
				: MATCH( RIGHT_00 ) ? 0x3 
				: MATCH( RIGHT_01 ) ? 0x4 
				: MATCH( RIGHT_02 ) ? 0x5 
				: 0xFFFF;//FORBIDDEN!
	#undef MATCH
	Mixer::solo( index );
	PatEdit::solo_clean = false; 
}

void PatEdit::mute(Control *c, bool bigstep, bool add, u32 *pointer){	
	#define MATCH( match ) 	( c == &PAT_CONTROLS[ CONTROL_PAT_MUTE_##match ] ) 
	int index 	= MATCH( LEFT_00  ) ? 0x0 
				: MATCH( LEFT_01  ) ? 0x1 
				: MATCH( LEFT_02  ) ? 0x2 
				: MATCH( RIGHT_00 ) ? 0x3 
				: MATCH( RIGHT_01 ) ? 0x4 
				: MATCH( RIGHT_02 ) ? 0x5 
				: 0xFFFF;//FORBIDDEN!
	#undef MATCH
	Mixer::mute( index );
	PatEdit::solo_clean = false; 
}

// Reloads VAR_SONG.PATTERN[6][--16--] into visible VARIABLE lookup vars (VAR_PATTERN[6][16])
void PatEdit::sync( bool verbose ){
	
	const Control *ctl = &PAT_CONTROLS[ 0 ];
	for(u16 c=0; c<6;c++){
		for(u16 i=0; i<16;i++){
			/* ------------------------------------------------------------
			Copy data from VISIBLE CONTROL VARS -> Song Data Order Memory*/
			VAR_PATTERN[ c ].ORDER[ i ] 	= VAR_SONG.PATTERNS[ c ].ORDER[ i + VAR_CFG.ORDERPOSITION ];
			#ifndef NSONGTRANSPOSE
			VAR_PATTERN[ c ].TRANSPOSE[ i ] = VAR_SONG.PATTERNS[ c ].TRANSPOSE[ i + VAR_CFG.ORDERPOSITION ];
			#endif
			/* ------------------------------------------------------------
			Manually draw the controls (good to avoid message overflow)  */
			if( verbose ) regHnd.drawControl( ctl );
			/* ------------------------------------------------------------
			Ensure the pattern order tags at the sides get updated later */
			PatEdit::clean = false;
			// ------------------------------------------------------------
			ctl++;
		}
	}
}

void PatEdit::dispatchMessage(u32 msg){
	u8 x, y;
	switch(msg){
		case MESSAGE_CANCEL: // Erase
			// Clear Arrow on current channel 
			y = VAR_CHANNEL[ TRACKER_ACTIVE_CHANNEL ].POSITION - VAR_CFG.ORDERPOSITION;
			x = arrow_position[ TRACKER_ACTIVE_CHANNEL ];
			if( y <= 15 ) gpu.set( 2, x, 4 + y, 0x00FC);
			// If target cell is empty, scroll all cells one position from this position up to the end and fill last cell with 0x00
			if( VAR_PATTERN[ TRACKER_ACTIVE_CHANNEL ].ORDER[ (regHnd.control->y-4) + VAR_CFG.ORDERPOSITION] == 0){
				// Scroll from this point on up to the end
				shift( -1, y );
			} else regHnd.controlClear	( regHnd.control );
			copy( TRACKER_ACTIVE_CHANNEL ); 			
			VAR_CHANNEL[ TRACKER_ACTIVE_CHANNEL ].LASTPOSITION = VAR_CHANNEL[ TRACKER_ACTIVE_CHANNEL ].POSITION;
			sync();
			dispatchMessage		( MESSAGE_OTHER_REFRESH_DATA );
			clean = false;
			return;
		
		case MESSAGE_OTHER_REFRESH_DATA:
			return sync();
		
		/* Scroll up */
		case MESSAGE_OTHER_PREV:
			if(VAR_CFG.ORDERPOSITION>0){
				/* ------------------------------------------------------------
				Clear Arrows when pattern scrolls down 						 */
				for(int c=0, x=0; c<6;c++){
					x = VAR_CHANNEL[c].POSITION - VAR_CFG.ORDERPOSITION;
					if((x<=15)&&(x>=0))gpu.set(2, PatEdit::arrow_position[c], 4+x, 0x00FC);
				}
				// ------------------------------------------------------------
				VAR_CFG.ORDERPOSITION--;
				/* ------------------------------------------------------------
				Copy cells from VAR_DATA(Memory)->VAR_PATTERN (Visible Vars) */
				PatEdit::sync();
				/* ------------------------------------------------------------
				Ensure the pattern order tags at the sides get updated later */
				PatEdit::clean = false;				
				// ------------------------------------------------------------
			}
			return;
			
		/* Scroll down */
		case MESSAGE_OTHER_NEXT: 
			/* about the 240 below, it is 256 patterns - 16 visible entries */
			if(VAR_CFG.ORDERPOSITION < 240){ 
				/* ------------------------------------------------------------
				Clear Arrows when pattern scrolls down 						 */
				for(int c=0, x=0; c<6;c++){
					x = VAR_CHANNEL[c].POSITION - VAR_CFG.ORDERPOSITION;
					if((x<=15)&&(x>=0))gpu.set(2, PatEdit::arrow_position[c], 4+x, 0x00FC);
				}
				// ------------------------------------------------------------
				VAR_CFG.ORDERPOSITION++;
				/* ------------------------------------------------------------
				Copy cells from VAR_DATA(Memory)->VAR_PATTERN (Visible Vars) */
				PatEdit::sync();
				/* ------------------------------------------------------------
				Ensure the pattern order tags at the sides get updated later */
				PatEdit::clean = false;				
				// ------------------------------------------------------------
			}
			return;		
	}
}

void PatEdit::shift( int q, u8 starting_point ){
	bool expr = (q<0);
	Notifier::icon( 0, 0x7112, expr ? 0x408C : 0x408E);

	u16 carry = VAR_SONG.PATTERNS[ TRACKER_ACTIVE_CHANNEL ].ORDER[ expr ? starting_point : 255 ];
	
	for(int i = starting_point; i<256; i++){
		VAR_SONG.PATTERNS[ TRACKER_ACTIVE_CHANNEL ].ORDER[ expr ? i&0xFF : (0xFF-starting_point) - (i&0xFF) ] = VAR_SONG.PATTERNS[ TRACKER_ACTIVE_CHANNEL ].ORDER[ expr ? (i+1)&0xFF : (0xFF-starting_point) - ((i+1)&0xFF) ];
	} 
	VAR_SONG.PATTERNS[ TRACKER_ACTIVE_CHANNEL ].ORDER[ expr ? 0xFF : starting_point] = carry;
	
	// Save changes and Show changes in screen
	PatEdit::sync();	
}

void PatEdit::transpose( int q ){
	Notifier::icon( 0x306D, q > 0 ? 0x308C : 0x308E, 0x8A);
	#ifndef NSONGTRANSPOSE
	for(int i = 0; i<256; i++){
		// Abort if lowest note would overflow on transpose down
		if( ( VAR_SONG.PATTERNS[ TRACKER_ACTIVE_CHANNEL ].TRANSPOSE[ i ] + q ) == 0x00 ) return;
		// Abort if highest note would overflow on transpose down
		if( ( VAR_SONG.PATTERNS[ TRACKER_ACTIVE_CHANNEL ].TRANSPOSE[ i ] + q ) == 0xFF ) return;
	}
	Notifier::icon( 0x0, 0x706D, q > 0 ? 0x408C : 0x408E);
	
	for(int i = 0; i<256; i++){
		VAR_SONG.PATTERNS[ TRACKER_ACTIVE_CHANNEL ].TRANSPOSE[ i ] += q;
	}
	// Save changes and Show changes in screen
	PatEdit::sync();	
	#endif
}

void PatEdit::processInput( ){
	if		( KEY.down( KEY_LEFT  ) ) transpose( -1 );
	else if	( KEY.down( KEY_RIGHT ) ) transpose(  1 );
	if		( KEY.down( KEY_UP    ) ) shift( -1 );
	else if	( KEY.down( KEY_DOWN  ) ) shift(  1 );
}
