#ifndef INS_HPP
#define INS_HPP

#include "../agb.h"
#include "../data/control.hpp"
#include "../data/settings.hpp"
#include "../data/instrument.hpp"
#include "../data/controls.hpp"

class RegionHandler;

class InstEdit {
	public:
	
		static void dispatchMessage(u32 msg);
	
		// Specific type modifiers
		static void updateWav( RegionHandler* rh );
		static void updateFmw( RegionHandler* rh );
		static void updateSmp( RegionHandler* rh );
		static void updatePwm( RegionHandler* rh );
		
		// Visible instrument modifiers
		static SETTINGS_PWM unpackPwm	( Instrument* i );
		static SETTINGS_WAV unpackWav	( Instrument* i );
		static SETTINGS_FMW unpackFmw	( Instrument* i );
		static SETTINGS_SMP unpackSmp	( Instrument* i );
		static void 		unpack		( Instrument* i );
		static void 		pack		( Instrument* i);
		static void 		repack		( );
		static void 		synchronize	( );
		static void 		load 		( );
		static void 		copy		( Instrument* s, Instrument* d );
		
		// Control Callbacks
		static void index  		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void wavPreset 	( Control *c, bool bigstep, bool add, u32 *pointer );
		static void smpPreset 	( Control *c, bool bigstep, bool add, u32 *pointer );
		static void type 		( Control *c, bool bigstep, bool add, u32 *pointer );
		
		// Value modifier wrappers
		static void modBit		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mod2b		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mod3b		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modNibble	( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mod5b		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mod6b		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mod7b		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modByte		( Control *c, bool bigstep, bool add, u32 *pointer );

		static void modSmpKit	( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modAlgorithm( Control *c, bool bigstep, bool add, u32 *pointer );
		
		static void modVal3		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modVal5		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modVal6		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modVal27	( Control *c, bool bigstep, bool add, u32 *pointer );

		static void modAdsrWav	( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modAdsrFmw  ( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modAdsrSmp	( Control *c, bool bigstep, bool add, u32 *pointer );

		static void viewADSR 	( u8 adsr_table    [0x40] , u8 adsr_position );
		static void viewQuadADSR( u8 adsr_tables[4][0x40] , u8 adsr_position );

};



#endif
