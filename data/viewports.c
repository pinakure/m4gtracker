#include "viewports.hpp"
#include "regions.hpp"
#include "data.hpp"

const Region *const VIEWPORT_TRK_REGIONS[6] = {
	&REGION_MAP_3_CHANNEL0,
	&REGION_MAP_3_CHANNEL1,
	&REGION_MAP_3_CHANNEL2,
	&REGION_MAP_3_CHANNEL3,
	&REGION_MAP_3_CHANNEL4,
	&REGION_MAP_3_CHANNEL5
};

const Region *const VIEWPORT_INS_REGIONS[4] = {
	&REGION_MAP_2_INS_PWM,
	&REGION_MAP_2_INS_WAV,
	&REGION_MAP_2_INS_FMW,
	&REGION_MAP_2_INS_SMP	
};

const Region *const VIEWPORT_CFG_REGIONS[5] = {
	&REGION_MAP_4_LOOKNFEEL,
	&REGION_MAP_4_LINKMODE,
	&REGION_MAP_4_BEHAVIOR,
	&REGION_MAP_4_TRACKER,
	&REGION_MAP_4_MEMORY	
};

const Viewport VIEWPORT_INS = { 0x00, 0x00, 0x0E, 0x14, &(VAR_INSTRUMENT.TYPE),		VIEWPORT_INS_REGIONS, 		4};
const Viewport VIEWPORT_CFG = { 0x04, 0x04, 0x19, 0x0A, &(VAR_CFG.MENUSLOT), 		VIEWPORT_CFG_REGIONS,		5};
const Viewport VIEWPORT_TRK = { 0x01, 0x03, 0x1D, 0x11, &(VAR_CFG.CURRENTCHANNEL),	VIEWPORT_TRK_REGIONS,		6};

/*const Viewport VIEWPORT_TABLEVIS = (Viewport) {
.x=0x0E, .y=0x15, .width=0x10, .height=0x12, .var=CFG.INSTRUMENTVISTYPE, .regions=NULL, .count=0, .region=NULL
const Viewport VIEWPORT_HEADER = (Viewport) {
.x=0x00, .y=0x00, .width=0x1E, .height=0x03, .var=NULL, .regions=NULL, .count=0, .region=NULL
const Viewport VIEWPORT_SUBMENU2 = (Viewport) {
.x=0x0B, .y=0x0F, .width=0x12, .height=0x04, .var=CFG.MENUSLOT, .regions=NULL, .count=0, .region=NULL
*/
