#ifndef SNG_HPP
#define SNG_HPP

#include "../agb.h"

#include "../modules/regionhandler/regionhandler.hpp"
#include "../data/callback.hpp"
#include "../data/enum.h"

#define SONG_SLOT_COUNT 	6


class SongEdit {
	public:
		static MEM_IN_EWRAM bool has_data[ SONG_SLOT_COUNT ];
		static void update			( RegionHandler* rh );
		static void runGroove		(  );
		static void load			( Control *c=NULL	, bool bigstep=false	, bool add=false	, u32 *pointer=NULL );
		static void save 			( Control *c=NULL	, bool bigstep=false	, bool add=false	, u32 *pointer=NULL );
		static void purge			( Control *c=NULL	, bool bigstep=false	, bool add=false	, u32 *pointer=NULL );
		static void erase			( Control *c=NULL	, bool bigstep=false	, bool add=false	, u32 *pointer=NULL );
		static void tapTempo		( Control *c=NULL	, bool bigstep=false	, bool add=false	, u32 *pointer=NULL );
		static void setTempo		( Control *c	 		, bool bigstep		 	, bool add		 	, u32 *pointer		 );
		static void select			( Control *c	 		, bool bigstep		 	, bool add		 	, u32 *pointer		 );
		static void toggleGroove	( Control *c	 		, bool bigstep		 	, bool add		 	, u32 *pointer		 );
		static void redrawCtl		(eSngControl control);
		static void focusCtl		(eSngControl control);
};



extern const Callback cb_sng_load;
extern const Callback cb_sng_save;
extern const Callback cb_sng_purge;
extern const Callback cb_sng_erase;
extern const Callback cb_sng_slot;
extern const Callback cb_sng_artist;
extern const Callback cb_sng_title;
extern const Callback cb_sng_tempotap;
extern const Callback cb_sng_patlength;
extern const Callback cb_sng_transpose;
extern const Callback cb_sng_bpm;
extern const Callback cb_sng_groove;

extern const Callback cb_sng_groove_00;
extern const Callback cb_sng_groove_01;
extern const Callback cb_sng_groove_02;
extern const Callback cb_sng_groove_03;
extern const Callback cb_sng_groove_04;
extern const Callback cb_sng_groove_05;
extern const Callback cb_sng_groove_06;
extern const Callback cb_sng_groove_07;
extern const Callback cb_sng_groove_08;
extern const Callback cb_sng_groove_09;
extern const Callback cb_sng_groove_0A;
extern const Callback cb_sng_groove_0B;
extern const Callback cb_sng_groove_0C;
extern const Callback cb_sng_groove_0D;
extern const Callback cb_sng_groove_0E;
extern const Callback cb_sng_groove_0F;

extern const Control SNG_CONTROLS[CONTROL_SNG_MAX];

#endif
