#ifndef BEHAVIOR_HPP
#define BEHAVIOR_HPP

#include "../../agb.h"

#include "../../modules/regionhandler/regionhandler.hpp"

enum E_BEHAVIOR_CONTROLS { 
	CONTROL_BEHAVIOR_MENU3,			CONTROL_BEHAVIOR_AUTOLOAD,		CONTROL_BEHAVIOR_AUTOSAVE,		CONTROL_BEHAVIOR_KEYRATE,
	CONTROL_BEHAVIOR_SAVE,			CONTROL_BEHAVIOR_LOAD,			CONTROL_BEHAVIOR_DEFAULTS,		CONTROL_BEHAVIOR_COUNT,
	CONTROL_BEHAVIOR_MAX
};

class Behavior {
	public:
		static void update(  );
};

extern const Callback cb_cfg_autoload;
extern const Callback cb_cfg_autosave;
extern const Callback cb_cfg_keyrate;
extern const Callback cb_cfg_saveconfig;
extern const Callback cb_cfg_loadconfig;
extern const Callback cb_cfg_initconfig;

extern const Control BEHAVIOR_CONTROLS[CONTROL_BEHAVIOR_MAX];

#endif
