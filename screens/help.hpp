#ifndef HELP_HPP
#define HELP_HPP

#include "../agb.h"

#include "../kernel/regionhandler/regionhandler.hpp"

//Help structs declaration
typedef struct sHelpTopic helptopic;
typedef struct sHelpButton {
	int					destination_index;
	const char			text[32];
	u16					x;
	u16					y;
} HelpButton;

typedef struct sHelpTopic {
	const u32			button_count;
	const u16			*map_data;
	const int 		 	*button_list;
} HelpTopic;

typedef enum eHelpTopics {
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
} HelpTopics;

typedef enum eHelpButtons {
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
} HelpButtons;

class Help {
	public:			
		static const HelpButton buttons				[ HELP_BUTTON_MAX ];
		static const HelpTopic  topics				[ HELP_MAX ];

		static const int 		buttons_topics		[16];
		static const int 		buttons_intro		[16];
		static const int 		buttons_usage		[16];
		static const int 		buttons_controls	[16];
		static const int 		buttons_instr		[16];
		static const int 		buttons_instrpwm	[16];
		static const int 		buttons_instrwav	[16];
		static const int 		buttons_instrfmw	[16];
		static const int 		buttons_instrsmp	[16];
		static const int 		buttons_instrpwmtbl	[16];
		static const int 		buttons_patterns	[16];
		static const int 		buttons_commands	[16];
		static const int 		buttons_effectmod	[16];
		static const int 		buttons_live		[16];
		static const int 		buttons_link		[16];
		static const int 		buttons_credits		[16];

		static const u16 		data_topics			[ 1664 ];
		static const u16 		data_credits		[ 1664 ];
		static const u16 		data_link			[ 1664 ];
		static const u16 		data_live			[ 1664 ];
		static const u16 		data_effectmod		[ 1664 ];
		static const u16 		data_commands		[ 1664 ];
		static const u16 		data_patterns		[ 1664 ];
		static const u16 		data_intro			[ 1664 ];
		static const u16 		data_usage			[ 1664 ];
		static const u16 		data_controls		[ 1664 ];
		static const u16 		data_instr			[ 1664 ];
		static const u16 		data_instrpwm		[ 1664 ];
		static const u16 		data_instrwav		[ 1664 ];
		static const u16 		data_instrfmw		[ 1664 ];
		static const u16 		data_instrsmp		[ 1664 ];
		static const u16 		data_instrpwmtbl	[ 1664 ];

		static u16 	index_button;
		static u16 	index_topic;
		static u16 	index_last_topic;
		static bool button_redraw;
		static bool data_redraw;
		static u16 	old_offset;
		static u16 	offset;
		static int button_last_x;
		static int button_last_y;
		static int button_last_index;
		static int last_offset;
		static int button_moved;

		static void blit( const u16 *help_data, int startx, int starty, int x, int y, int width, int height );
		static void drawButton(int x, int y, const char *t, bool active);
		static void eraseButton();
		static void scrollDown();
		static void scrollUp();
		static void prevButton();
		static void nextButton();
		static void activateBack();
		static void activateButton();
		static void updateButtons();
		static void update(  );
		static void dispatchMessage( u32 msg, u32 pointer=0 );
		
};

#endif
