#ifndef _DATA_HELP
#define _DATA_HELP

#include "../agb.h"

//Help structs declaration
typedef struct s_Help_Topic helptopic;
typedef struct s_Help_Button {
	int					destination_index;
	const char			text[32];
	u16					x;
	u16					y;
} helpbutton;

typedef struct s_Help_Topic {
	const u32			button_count;
	const u16			*map_data;
	const int 		 	*button_list;
} helptopic;

enum E_HELP_TOPICS {
	HELP_TOPICS	,
	HELP_INTRO	,
	HELP_USAGE	,
	HELP_CONTROLS	,
	HELP_INSTR	,
	HELP_INSTRPWM	,
	HELP_INSTRWAV	,
	HELP_INSTRFMW	,
	HELP_INSTRSMP	,
	HELP_INSTRPWMTBL	,
	HELP_PATTERNS	,
	HELP_COMMANDS	,
	HELP_EFFECTMOD	,
	HELP_LIVE	,
	HELP_LINK	,
	HELP_CREDITS	,
	HELP_MAX
};
extern const helptopic help_topics[HELP_MAX];

enum E_HELP_BUTTONS {
	HELP_BUTTON_TOPICS_INTRO	,
	HELP_BUTTON_TOPICS_USAGE	,
	HELP_BUTTON_TOPICS_CONTROLS	,
	HELP_BUTTON_TOPICS_INSTR	,
	HELP_BUTTON_TOPICS_PATTERNS	,
	HELP_BUTTON_TOPICS_COMMANDS	,
	HELP_BUTTON_TOPICS_LIVE	,
	HELP_BUTTON_TOPICS_LINK	,
	HELP_BUTTON_TOPICS_CREDITS	,
	HELP_BUTTON_INTRO_TOPICS	,
	HELP_BUTTON_USAGE_TOPICS	,
	HELP_BUTTON_CONTROLS_TOPICS	,
	HELP_BUTTON_INSTR_INSTRPWM	,
	HELP_BUTTON_INSTR_INSTRWAV	,
	HELP_BUTTON_INSTR_INSTRFMW	,
	HELP_BUTTON_INSTR_INSTRSMP	,
	HELP_BUTTON_INSTR_TOPICS	,
	HELP_BUTTON_INSTRPWM_INSTR	,
	HELP_BUTTON_INSTRPWM_INSTRPWMTBL	,
	HELP_BUTTON_INSTRPWM_TOPICS	,
	HELP_BUTTON_INSTRWAV_INSTR	,
	HELP_BUTTON_INSTRWAV_INSTRPWMTBL	,
	HELP_BUTTON_INSTRWAV_TOPICS	,
	HELP_BUTTON_INSTRFMW_INSTR	,
	HELP_BUTTON_INSTRFMW_INSTRPWMTBL	,
	HELP_BUTTON_INSTRFMW_TOPICS	,
	HELP_BUTTON_INSTRSMP_INSTR	,
	HELP_BUTTON_INSTRSMP_INSTRPWMTBL	,
	HELP_BUTTON_INSTRSMP_TOPICS	,
	HELP_BUTTON_INSTRPWMTBL_INSTRPWM	,
	HELP_BUTTON_INSTRPWMTBL_TOPICS	,
	HELP_BUTTON_PATTERNS_TOPICS	,
	HELP_BUTTON_COMMANDS_EFFECTMOD	,
	HELP_BUTTON_COMMANDS_TOPICS	,
	HELP_BUTTON_EFFECTMOD_TOPICS	,
	HELP_BUTTON_LIVE_TOPICS	,
	HELP_BUTTON_LINK_TOPICS	,
	HELP_BUTTON_CREDITS_TOPICS	,
	HELP_BUTTON_MAX
};
extern const helptopic help_topics[HELP_MAX];
extern const helpbutton help_buttons[HELP_BUTTON_MAX];
extern const int help_buttons_topics[16];
extern const int help_buttons_intro[16];
extern const int help_buttons_usage[16];
extern const int help_buttons_controls[16];
extern const int help_buttons_instr[16];
extern const int help_buttons_instrpwm[16];
extern const int help_buttons_instrwav[16];
extern const int help_buttons_instrfmw[16];
extern const int help_buttons_instrsmp[16];
extern const int help_buttons_instrpwmtbl[16];
extern const int help_buttons_patterns[16];
extern const int help_buttons_commands[16];
extern const int help_buttons_effectmod[16];
extern const int help_buttons_live[16];
extern const int help_buttons_link[16];
extern const int help_buttons_credits[16];


extern const u16 help_data_topics[ 1664 ];
extern const u16 help_data_intro[ 1664 ];
extern const u16 help_data_usage[ 1664 ];
extern const u16 help_data_controls[ 1664 ];
extern const u16 help_data_instr[ 1664 ];
extern const u16 help_data_instrpwm[ 1664 ];
extern const u16 help_data_instrwav[ 1664 ];
extern const u16 help_data_instrfmw[ 1664 ];
extern const u16 help_data_instrsmp[ 1664 ];
extern const u16 help_data_instrpwmtbl[ 1664 ];
extern const u16 help_data_patterns[ 1664 ];
extern const u16 help_data_commands[ 1664 ];
extern const u16 help_data_effectmod[ 1664 ];
extern const u16 help_data_live[ 1664 ];
extern const u16 help_data_link[ 1664 ];
extern const u16 help_data_credits[ 1664 ];



#endif
