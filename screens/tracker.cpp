#include "tracker.hpp"
#include "config.hpp"
#include "../data/data.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"
#include "../modules/spu/mixer.hpp"
#include "../modules/clip/clip.hpp"
#include "../data/viewports.hpp"
#include "../data/channel.hpp"
#include "../debug.hpp"
#include "../modules/spu/sequencer.hpp"
#include "../macros.h"

#include "tracker/callbacks.c"
#include "tracker/controls.c"
#include "tracker/displays.c"

//move to Channel if more apropiate
const u8 Tracker::columns[ CHANNEL_COUNT ][ CHANNEL_COUNT ]	= {
	{ 1, 11, 15, 19, 23, 27 },
	{ 1,  5, 15, 19, 23, 27 },
	{ 1,  5,  9, 19, 23, 27 },
	{ 1,  5,  9, 13, 23, 27 },
	{ 1,  5,  9, 13, 17, 27 },
	{ 1,  5,  9, 13, 17, 21 }
};
//move to Channel if more apropiate
const u8 	Tracker::positions_x[ CHANNEL_COUNT ] = { 15, 20, 25, 15, 20, 25 };    
//move to Channel if more apropiate
const u8 	Tracker::positions_y[ CHANNEL_COUNT ] = {  1,  1,  1,  2,  2,  2 };    
//move to Channel if more apropiate
const u16 	Tracker::channel_symbols[ CHANNEL_COUNT ][ CHANNEL_COUNT ] = {
	{ 0x7035, 0xB035, 0xB036, 0xB037, 0xB038, 0xB039 },
	{ 0xB035, 0x7035, 0xB036, 0xB037, 0xB038, 0xB039 },
	{ 0xB035, 0xB035, 0x7036, 0xB037, 0xB038, 0xB039 },
	{ 0xB035, 0xB035, 0xB036, 0x7037, 0xB038, 0xB039 },
	{ 0xB035, 0xB035, 0xB036, 0xB037, 0x7038, 0xB039 },
	{ 0xB035, 0xB035, 0xB036, 0xB037, 0xB038, 0x7039 }	
};

bool 	Tracker::clean = false;

// TBC on All Channel Data Update
// move to channel if more apropiate
void Tracker::syncPattern(){
	for(int c=0; c < CHANNEL_COUNT; c++){
		for(int i=0, pattern; i<0x10; i++){
			pattern = VAR_SONG.PATTERNS[ c ].ORDER[ VAR_CHANNEL[ c ].POSITION ];
			VAR_CELLS[ c ].KEY[ i ] = VAR_DATA[ pattern ].KEY[ i ];
			VAR_CELLS[ c ].INS[ i ] = VAR_DATA[ pattern ].INS[ i ];
			VAR_CELLS[ c ].VOL[ i ] = VAR_DATA[ pattern ].VOL[ i ];
			VAR_CELLS[ c ].CMD[ i ] = VAR_DATA[ pattern ].CMD[ i ];
			VAR_CELLS[ c ].VAL[ i ] = VAR_DATA[ pattern ].VAL[ i ];
		}
	}
}

void Tracker::syncChannel(u8 c){
	for(int i=0, pattern; i<0x10; i++){
		pattern = VAR_SONG.PATTERNS[c].ORDER[VAR_CHANNEL[c].POSITION];
		VAR_CELLS[ c ].KEY[ i ] = VAR_DATA[ pattern ].KEY[ i ];
		VAR_CELLS[ c ].INS[ i ] = VAR_DATA[ pattern ].INS[ i ];
		VAR_CELLS[ c ].VOL[ i ] = VAR_DATA[ pattern ].VOL[ i ];
		VAR_CELLS[ c ].CMD[ i ] = VAR_DATA[ pattern ].CMD[ i ];
		VAR_CELLS[ c ].VAL[ i ] = VAR_DATA[ pattern ].VAL[ i ];
	}
	
	if(RegionHandler::region != &REGION_MAP_3_TRK) return;
	
	int  len = (VAR_CFG.CURRENTCHANNEL==c?(0x10 * (CHANNEL_COUNT-1) ): 0x10);
	
	switch(VAR_CFG.CURRENTCHANNEL){
		case 0:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN0_CONTROLS[i	   	  ]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN0_CONTROLS[i+(0x10*5)]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN0_CONTROLS[i+(0x10*6)]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN0_CONTROLS[i+(0x10*7)]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN0_CONTROLS[i+(0x10*8)]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN0_CONTROLS[i+(0x10*9)]); } return;
			}				
		case 1:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN1_CONTROLS[i+(0x10*5)]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN1_CONTROLS[i		  ]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN1_CONTROLS[i+(0x10*6)]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN1_CONTROLS[i+(0x10*7)]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN1_CONTROLS[i+(0x10*8)]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN1_CONTROLS[i+(0x10*9)]); } return;
			}
		case 2:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN2_CONTROLS[i+(0x10*5)]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN2_CONTROLS[i+(0x10*6)]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN2_CONTROLS[i		  ]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN2_CONTROLS[i+(0x10*7)]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN2_CONTROLS[i+(0x10*8)]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN2_CONTROLS[i+(0x10*9)]); } return;
			}
		case 3:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN3_CONTROLS[i+(0x10*5)]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN3_CONTROLS[i+(0x10*6)]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN3_CONTROLS[i+(0x10*7)]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN3_CONTROLS[i		  ]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN3_CONTROLS[i+(0x10*8)]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN3_CONTROLS[i+(0x10*9)]); } return;
			}
		case 4:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN4_CONTROLS[i+(0x10*5)]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN4_CONTROLS[i+(0x10*6)]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN4_CONTROLS[i+(0x10*7)]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN4_CONTROLS[i+(0x10*8)]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN4_CONTROLS[i		  ]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN4_CONTROLS[i+(0x10*9)]); } return;
			}
		case 5:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN5_CONTROLS[i+(0x10*5)]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN5_CONTROLS[i+(0x10*6)]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN5_CONTROLS[i+(0x10*7)]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN5_CONTROLS[i+(0x10*8)]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN5_CONTROLS[i+(0x10*9)]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ RegionHandler::drawControl(&CHAN5_CONTROLS[i		  ]); } return;
			}
	}
}

// TBC on channel data change
void Tracker::copyChannel(u8 channel){
		
	for(int i=0, pattern; i<0x10;i++){		
		
		pattern = VAR_SONG.PATTERNS[channel].ORDER[VAR_CHANNEL[channel].POSITION];
		
		VAR_DATA[ pattern ].KEY[ i ] = VAR_CELLS[ channel ].KEY[ i ];
		VAR_DATA[ pattern ].INS[ i ] = VAR_CELLS[ channel ].INS[ i ];
		VAR_DATA[ pattern ].VOL[ i ] = VAR_CELLS[ channel ].VOL[ i ];
		VAR_DATA[ pattern ].CMD[ i ] = VAR_CELLS[ channel ].CMD[ i ];
		VAR_DATA[ pattern ].VAL[ i ] = VAR_CELLS[ channel ].VAL[ i ];
	}
	for(int i=0; i < CHANNEL_COUNT;i++){
		if( i == channel )continue;
		Tracker::syncChannel( i );
	}
}

void modifyNote(Control *c, bool bigstep, bool add, u32 *pointer){
	if( CURRENT_PATTERN == 0x00 ) return;
	
	U8P( c->var ) += (bigstep?0xC:0x1) * (add?1:-1);
	
	if( U8P( c->var ) > 159) U8P( c->var ) =   0; else 
	if( U8P( c->var ) > 119) U8P( c->var ) = 119;
	
	if( U8P( c->var ) == 0 ) U8P( c->var ) = 1;
	
	Transient::note 	= U8P( c->var );
	Transient::changed 	= true;
	Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL );
}

void Tracker::drawTransientInfo(){
	NOTE				 ( 16 , 0 , 7 , Transient::note			);
	HEXADECIMAL_TWOTILES ( 20 , 0 , 7 , Transient::instrument	);
	HEXADECIMAL			 ( 23 , 0 , 7 , Transient::volume		);
	COMMAND				 ( 27 , 0 , 7 , Transient::command		);
	HEXADECIMAL_TWOTILES ( 28 , 0 , 7 , Transient::value		);
	Transient::changed = false;
}

void Tracker::drawPosition( int i ){
	bool hl = i==VAR_CFG.CURRENTCHANNEL;
	Gpu::set( 2 , 			Tracker::positions_x[ i ]		, Tracker::positions_y[ i ] , Tracker::channel_symbols[ VAR_CFG.CURRENTCHANNEL ][ i ] );
	HEXADECIMAL( 			Tracker::positions_x[ i ] + 1	, Tracker::positions_y[ i ] , 0x1 + hl 		, VAR_CHANNEL[ i ].POSITION >>  4 );
	HEXADECIMAL( 			Tracker::positions_x[ i ] + 2	, Tracker::positions_y[ i ] , 0x1 + hl 		, VAR_CHANNEL[ i ].POSITION & 0xf );
	HEXADECIMAL_TWOTILES( 	Tracker::positions_x[ i ] + 3 	, Tracker::positions_y[ i ] , hl ? 0x6 : 0xD 	, VAR_SONG.PATTERNS[ i ].ORDER[ VAR_CHANNEL[ i ].POSITION ] );
	VAR_CHANNEL[ i ].LASTPOSITION = VAR_CHANNEL[ i ].POSITION;
}

void Tracker::drawLine( int channel ){
	int y = 4 + VAR_CHANNEL[ channel ].STEP;
	int x = Tracker::columns[ VAR_CFG.CURRENTCHANNEL ][ channel ];
	for(int i=0, li=( channel==VAR_CFG.CURRENTCHANNEL ? 9 : 3 ); i<li; i++){
		Gpu::set(0, x+i, 4 + VAR_CHANNEL[ channel ].LASTSTEP , 0x0);
		Gpu::set(0, x+i, y									, 0x15);
	}	
	VAR_CHANNEL[ channel ].LASTSTEP = VAR_CHANNEL[ channel ].STEP;	
}

void Tracker::dispatchMessage(u32 msg){
	switch(msg){
		
		case MESSAGE_CANCEL:
			RegionHandler::controlClear	( RegionHandler::control );
			Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL );	//dispatchMessage		( MESSAGE_OTHER_REFRESH_DATA );
			return;
		
		case MESSAGE_OTHER_REFRESH_DATA:
			Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL );	
			return;
			
		case MESSAGE_OTHER_PREV:
			// Jump to previous channel
			if( Clip::visible ) return;
			VAR_CFG.CURRENTCHANNEL = VAR_CFG.CURRENTCHANNEL>0?VAR_CFG.CURRENTCHANNEL-1:5;
			for(int i=0; i < CHANNEL_COUNT ;i++){
				Tracker::drawPosition( i );
			}	
			RegionHandler::updateViewport(&VIEWPORT_TRK, RegionHandler::region->xadd,RegionHandler::region->yadd);
			return;
		
		case MESSAGE_OTHER_NEXT:
			// Jump to next channel
			if( Clip::visible ) return;
			VAR_CFG.CURRENTCHANNEL = VAR_CFG.CURRENTCHANNEL<5?VAR_CFG.CURRENTCHANNEL+1:0;
			for(int i=0; i < CHANNEL_COUNT ;i++){
				Tracker::drawPosition( i );
			}	
			RegionHandler::updateViewport(&VIEWPORT_TRK, RegionHandler::region->xadd, RegionHandler::region->yadd);
			return;
	}
}

void Tracker::globalUpdate(  ){
	//Tracker::clean = false;

	Clip::update( );
	
	Gpu::set(2,0,1, Sequencer::playing ? 0xF08D  : 0x31FE );
	Gpu::set(1,0,0, Sequencer::playing ? ((Sequencer::currentBeats) == 0?0x32 : ((Sequencer::currentBeats&3) == 0?0x34 : 0x33)):0x33);
		
	if( Transient::changed || RegionHandler::redraw) Tracker::drawTransientInfo();

	for(int i=0; i < CHANNEL_COUNT ;i++){
		if( RegionHandler::redraw ) 
			Tracker::drawPosition( i );
		
		if( !Sequencer::playing) continue;
		
		Channel *channel = &VAR_CHANNEL[i];
		// Update vumeters
		u8 last_peak = channel->lastpeak;
		u8 peak = channel->peak;
		if( last_peak != peak ) VUMETER_V3(8+i, 0, 0x0, peak>12?12:peak);			
		
		// Update next step (cell) reactive elements
		if(channel->LASTSTEP != channel->STEP) Tracker::drawLine(i);
		
		// Update next pattern reactive elements
		if((channel->LASTPOSITION != channel->POSITION) || RegionHandler::redraw) {
			Tracker::drawPosition( i );
			Tracker::syncChannel( i );
			channel->LASTPOSITION =  channel->POSITION;
		}
	}
	
	if( !Sequencer::playing){
		Tracker::clean = false;
		return;
	}
	
	// Tracker screen is dirty; Clean and redraw active control
	if( !Tracker::clean ) {
		for(int y=4; y<20;y++){
			for(int i=1; i<30; i++){
				Gpu::set(0, i, y, 0x0);
			}
		}
		RegionHandler::sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)(RegionHandler::control)&0x0fffffff);
		Tracker::clean = true;
	}
}

void Tracker::updateVis(  ){

}

void Tracker::updateHeader1(  ){

}

void Tracker::updateHeader2(  ){

}

void Tracker::updateTable(  ){

}

void Tracker::update(  ){
	const Region *c = &REGION_MAP_4_CHANNELMIXER;	
	Gpu::blitAlt(MAPDATA + ((MAP_CFG * 3) << 12), c->x, c->y, 0xb, 0xf, c->width, c->height);
}

void Tracker::shift( int q ){
	bool expr = (q<0);
	Notifier::icon( 0, 0x7112, expr ? 0x408C : 0x408E);
	PatternCell copy = VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ];
	
	for(int i = 0; i<0x10; i++){
		VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].KEY[ expr ? i&0xF : 0xF - (i&0xF) ] = VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].KEY[ expr ? (i+1)&0xF : 0xF - ((i+1)&0xF) ];
		VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].INS[ expr ? i&0xF : 0xF - (i&0xF) ] = VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].INS[ expr ? (i+1)&0xF : 0xF - ((i+1)&0xF) ];
		VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].VOL[ expr ? i&0xF : 0xF - (i&0xF) ] = VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].VOL[ expr ? (i+1)&0xF : 0xF - ((i+1)&0xF) ];
		VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].CMD[ expr ? i&0xF : 0xF - (i&0xF) ] = VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].CMD[ expr ? (i+1)&0xF : 0xF - ((i+1)&0xF) ];
		VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].VAL[ expr ? i&0xF : 0xF - (i&0xF) ] = VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].VAL[ expr ? (i+1)&0xF : 0xF - ((i+1)&0xF) ];
	} 
	VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].KEY[ expr ? 0xF : 0x0 ] = copy.KEY[ expr ? 0x0 : 0xF ];
	VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].INS[ expr ? 0xF : 0x0 ] = copy.INS[ expr ? 0x0 : 0xF ];
	VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].VOL[ expr ? 0xF : 0x0 ] = copy.VOL[ expr ? 0x0 : 0xF ];
	VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].CMD[ expr ? 0xF : 0x0 ] = copy.CMD[ expr ? 0x0 : 0xF ];
	VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].VAL[ expr ? 0xF : 0x0 ] = copy.VAL[ expr ? 0x0 : 0xF ];
	
	// Copy temporary data to pattern data
	Tracker::copyChannel( TRACKER_ACTIVE_CHANNEL );
	// Reload data from pattern data to temporary AND REDRAW CURRENT CHANNEL (not needed if playing since it will be updated automatically and quickly)
	if(!Sequencer::playing) Tracker::syncChannel( TRACKER_ACTIVE_CHANNEL );
}

void Tracker::alterColumn( u8 index, u8 min_value, u8 max_value, int q ){
	u16 iconlist[5]    	= { 0xFC6D, 0x4A4B, 0xFC4C, 0xFC0C, 0xFC1F };
	u8 	arrows[2] 		= { 0x008E, 0x008C }; 
	
	// Select icon from list upon requested column and unpack icon(s) 
	u8  icons[2]		= { (iconlist[ index ]>>8), iconlist[ index ] & 0xFF };
	u8  arrow 			= ( q > 0 ) ? arrows[ 1 ] : arrows[ 0 ];
	
	// Select which array will be modified
	PatternCell *cell 	= &VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ];
	u8 *array			= NULL;
	switch( index ){
		case 0: HALT;//array = &cell.KEY; break; // WHY THE HELL KEY has 2 BYTES WIDTH ? max value is 160!!!
		case 1: array = &cell->INS[0]; break;
		case 2: array = &cell->VOL[0]; break;
		case 3: array = &cell->CMD[0]; break;
		case 4: array = &cell->VAL[0]; break;
	}
	
	// Pre-show failing status prior to check ( Assume fail then check if success )
	Notifier::icon( 0x3000 | icons[0], 0x3000 | icons[1], 0x3000 | arrow );
	
	// Integrity checks
	for(int i = 0; i<0x10; i++){
		// Ignore cells which are on 'illegal' values
		if(( min_value > 0)&&( array[i] < min_value )) continue;
		// Abort operation if transformation would move the lowest note into a 'illegal' minimal value
		if( ((s16)( array[i])) + ((s16)q) < ((s16)min_value) ) return;
		// Abort operation if transformation would move the highest note into a 'illegal' maximal value
		if( ((s16)( array[i])) + ((s16)q) > ((s16)max_value) ) return;
	}
	
	// If we reach this point, it is safe to show a sucessfull icon
	Notifier::icon( 0x7000 | icons[0], 0x7000 | icons[1], 0x4000  | ( ( q > 0 ) ? arrows[ 1 ] : arrows[ 0 ] ) );
	
	// Alter column values
	for(int i = 0; i<0x10; i++){
		// Don't transform cells which are on 'illegal' values
		if( ( min_value > 0 ) && ( array[ i ] == 0 ) ) continue; 
		// Alter value
		array[ i ] += q;
	}
	
	// Copy temporary data to pattern data
	Tracker::copyChannel( TRACKER_ACTIVE_CHANNEL );
	
	// Reload data from pattern data to temporary AND REDRAW CURRENT CHANNEL (not needed if playing since it will be updated automatically and quickly)
	if(!Sequencer::playing) Tracker::syncChannel( TRACKER_ACTIVE_CHANNEL );
}

void Tracker::transpose( int q ){
	
	switch( TRACKER_ACTIVE_COLUMN ){
		case 1: return Tracker::alterColumn( 1, 0x1	, 0x3F, q );
		case 2: return Tracker::alterColumn( 2, 0x0	, 0x0F, q );
		case 3: return Tracker::alterColumn( 3, 0x0	,   26, q );
		case 4: return Tracker::alterColumn( 4, 0x0	, 0xFF, q );
		// case 0: return Tracker::alterColumn( 0, 0x1	,  160, q );
		default: break;
	}
	
	Notifier::icon( 0x306D, q > 0 ? 0x308C : 0x308E, 0x8A);
	// Check first we dont destroy data 
	for(int i = 0; i<16; i++){
		// Ignore silenced cells 
		if(VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].KEY[ i ] == 0) continue;
		// Abort operation if transformation would move the lowest note into a silence
		if(( VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].KEY[ i ] + q) ==   0) return;
		// Abort operation if transformation would move the highest note out of range
		if(( VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].KEY[ i ] + q) >= 160) return;
	}
	Notifier::icon( 0, 0x706D, q > 0 ? 0x408C : 0x408E);
	// Transpose notes
	for(int i = 0; i<16; i++){
		// Ignore silenced cells 
		if(VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].KEY[ i ] == 0) continue; 
		// Transpose note
		VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].KEY[ i ] = VAR_CELLS[ TRACKER_ACTIVE_CHANNEL ].KEY[ i ] + q;
	}

	// Copy temporary data to pattern data
	Tracker::copyChannel( TRACKER_ACTIVE_CHANNEL );
	// Reload data from pattern data to temporary AND REDRAW CURRENT CHANNEL (not needed if playing since it will be updated automatically and quickly)
	if(!Sequencer::playing) Tracker::syncChannel( TRACKER_ACTIVE_CHANNEL );
}

void Tracker::processInput( ){
		 if ( KEYDOWN_LEFT 	) transpose	( -1 );
	else if ( KEYDOWN_RIGHT ) transpose	(  1 );
	else if ( KEYDOWN_UP    ) shift		( -1 );
	else if ( KEYDOWN_DOWN  ) shift		(  1 );
}
