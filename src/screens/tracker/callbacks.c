const Callback cb_cfg_finetune		= { modify4BIT					, EVENT_MODIFY_B	, &VAR_CFG.TRACKER.FINETUNE		, NULL };
const Callback cb_cfg_prelisten		= { modify1BIT					, EVENT_KEYDOWN_B	, &VAR_CFG.TRACKER.PRELISTEN	, NULL };
const Callback cb_cfg_transpose		= { modify8BIT					, EVENT_MODIFY_B	, &VAR_CFG.TRACKER.TRANSPOSE	, NULL };
const Callback cb_cfg_inputmode		= { modify1BIT					, EVENT_MODIFY_B	, &VAR_CFG.TRACKER.INPUTMODE	, NULL };
const Callback cb_cfg_soundbias		= { modify8BIT					, EVENT_MODIFY_B	, &VAR_CFG.TRACKER.SOUNDBIAS	, NULL };
const Callback cb_cfg_mixer			= { Mixer::show					, EVENT_KEYUP_B		, NULL 							, NULL };

#define CB_CHAN_KEY(c,a)	const Callback cb_ch##c##_keypaste_0##a = { pasteNote	    , EVENT_KEYDOWN_B 		, &VAR_CELLS[ 0x##c ].KEY[ 0x##a ], NULL						};	\
							const Callback cb_ch##c##_key_0##a 		= { modifyNote		, EVENT_MODIFY_B 		, &VAR_CELLS[ 0x##c ].KEY[ 0x##a ], &cb_ch##c##_keypaste_0##a 	};
#define CB_CHAN_INS(c,a)	const Callback cb_ch##c##_inspaste_0##a = { pasteInst		, EVENT_KEYDOWN_B 		, &VAR_CELLS[ 0x##c ].INS[ 0x##a ], NULL						};	\
							const Callback cb_ch##c##_ins_0##a 		= { modifyInst		, EVENT_MODIFY_B 		, &VAR_CELLS[ 0x##c ].INS[ 0x##a ], &cb_ch##c##_inspaste_0##a 	};
#define CB_CHAN_VOL(c,a)	const Callback cb_ch##c##_volpaste_0##a = { pasteVolume		, EVENT_KEYDOWN_B 		, &VAR_CELLS[ 0x##c ].VOL[ 0x##a ], NULL						};	\
							const Callback cb_ch##c##_vol_0##a 		= { modifyVolume	, EVENT_MODIFY_B 		, &VAR_CELLS[ 0x##c ].VOL[ 0x##a ], &cb_ch##c##_volpaste_0##a	}; 
#define CB_CHAN_CMD(c,a)	const Callback cb_ch##c##_cmdpaste_0##a = { pasteCommand	, EVENT_KEYDOWN_B 		, &VAR_CELLS[ 0x##c ].CMD[ 0x##a ], NULL						};	\
							const Callback cb_ch##c##_cmd_0##a 		= { modifyCommand	, EVENT_MODIFY_B 		, &VAR_CELLS[ 0x##c ].CMD[ 0x##a ], &cb_ch##c##_cmdpaste_0##a 	}; 
#define CB_CHAN_VAL(c,a)	const Callback cb_ch##c##_valpaste_0##a = { pasteValue		, EVENT_KEYDOWN_B 		, &VAR_CELLS[ 0x##c ].VAL[ 0x##a ], NULL						};	\
							const Callback cb_ch##c##_val_0##a 		= { modifyValue		, EVENT_MODIFY_B 		, &VAR_CELLS[ 0x##c ].VAL[ 0x##a ], &cb_ch##c##_valpaste_0##a	}; 

#include "callbacks.inc"

#undef CB_CHAN_VAL
#undef CB_CHAN_CMD
#undef CB_CHAN_VOL
#undef CB_CHAN_INS
#undef CB_CHAN_KEY
