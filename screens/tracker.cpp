#include "tracker.hpp"
#include "config.hpp"
#include "../data/data.hpp"
#include "../kernel/gpu/gpu.hpp"
#include "../kernel/key/key.hpp"
#include "../kernel/spu/mixer.hpp"
#include "../kernel/clip/clip.hpp"
#include "../data/viewports.hpp"
#include "../data/channel.hpp"
#include "../debug.hpp"
#include "../kernel/spu/sequencer.hpp"
#include "../macros.h"

#include "tracker/callbacks.c"
#include "tracker/controls.c"
#include "tracker/displays.c"

bool 	Tracker::clean = false;

// TBC on All Channel Data Update
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

typedef struct sCellPointer {
	u8* key;
	u8* ins;
	u8* vol;
	u8* cmd;
	u8* val;
	
	void increase(){
		key++;
		ins++;
		vol++;
		cmd++;
		val++;
	}
	
	void set(PatternCell *item){
		key = item->KEY;
		ins = item->INS;
		vol = item->VOL;
		cmd = item->CMD;
		val = item->VAL;
	}
		
}CellPointer;

void Tracker::syncChannel( Channel *channel ){

	static CellPointer src;
	static CellPointer dst;
	
	// Setup pointers
	src.set( &VAR_DATA[  channel->song_patterns->ORDER[ channel->POSITION ]  ]	);
	dst.set( channel->cells	);
	
	for(int i=0; i<0x10; i++){
		*dst.key = *src.key;
		*dst.ins = *src.ins;
		*dst.vol = *src.vol;
		*dst.cmd = *src.cmd;
		*dst.val = *src.val;
		dst.increase();
		src.increase();
	}
	
	if(RegionHandler::region != &REGION_MAP_3_TRK) return;
	
	int  len = (VAR_CFG.CURRENTCHANNEL==channel->index ? (0x10 * (CHANNEL_COUNT-1) ): 0x10);
	
	channel = VAR_CHANNEL + (TRACKER_ACTIVE_CHANNEL);	// maybe c instead of TRACKER_ACTIVE_CHANNEL
	const Control *controls = channel->tracker_controls[ channel->index ];
	for(int i=0; i<len; i++){ 
		RegionHandler::drawControl( controls + i ); 
	} 
}

// TBC on channel data change
void Tracker::copyChannel( Channel *channel ){
		
	int pattern 		= channel->song_patterns->ORDER[ channel->POSITION ];
	PatternCell *data 	= VAR_DATA + pattern; 
	for(int i=0; i<0x10;i++){		
		data->KEY[ i ] = channel->cells->KEY[ i ];
		data->INS[ i ] = channel->cells->INS[ i ];
		data->VOL[ i ] = channel->cells->VOL[ i ];
		data->CMD[ i ] = channel->cells->CMD[ i ];
		data->VAL[ i ] = channel->cells->VAL[ i ];
	}
	for(int i=0; i < CHANNEL_COUNT;i++){
		if( i == channel->index )continue;
		Tracker::syncChannel( channel );
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
	Tracker::copyChannel( VAR_CHANNEL + VAR_CFG.CURRENTCHANNEL );
}

void Tracker::drawTransientInfo(){
	NOTE				 ( 16 , 0 , 7 , Transient::note			);
	HEXADECIMAL_TWOTILES ( 20 , 0 , 7 , Transient::instrument	);
	HEXADECIMAL			 ( 23 , 0 , 7 , Transient::volume		);
	COMMAND				 ( 27 , 0 , 7 , Transient::command		);
	HEXADECIMAL_TWOTILES ( 28 , 0 , 7 , Transient::value		);
	Transient::changed = false;
}

void Tracker::drawPosition( Channel *channel ){
	bool hl = channel->index == VAR_CFG.CURRENTCHANNEL;
		
	Gpu::set( 2 , 			channel->tracker_position.x		, channel->tracker_position.y , channel->symbols[ channel->index ] );
	HEXADECIMAL( 			channel->tracker_position.x + 1	, channel->tracker_position.y , 0x1 + hl 		, channel->POSITION >>  4 );
	HEXADECIMAL( 			channel->tracker_position.x + 2	, channel->tracker_position.y , 0x1 + hl 		, channel->POSITION & 0xf );
	HEXADECIMAL_TWOTILES( 	channel->tracker_position.x + 3 , channel->tracker_position.y , hl ? 0x6 : 0xD 	, channel->song_patterns->ORDER[ channel->POSITION ] );

	channel->LASTPOSITION = channel->POSITION;
}

void Tracker::drawLine( Channel *channel ){
	#ifdef BGCURSORS
	int y = 4 + channel->STEP;
	int x = Tracker::columns[ VAR_CFG.CURRENTCHANNEL ][ channel->index ];
	for(int i=0, li=( channel->index == VAR_CFG.CURRENTCHANNEL ? 9 : 3 ); i<li; i++){
		Gpu::set(0, x+i, 4 + channel->LASTSTEP , 0x0);
		Gpu::set(0, x+i, y									, 0x15);
	}	
	#endif 
	channel->LASTSTEP = channel->STEP;	
}

void Tracker::nextPattern( ){
	Channel *channel = &VAR_CHANNEL[ VAR_CFG.CURRENTCHANNEL ];
	channel->POSITION++;
	Tracker::drawPosition( channel );
	Tracker::clean = false;
	// must redraw controls
	syncChannel( channel );
}

void Tracker::prevPattern( ){
	Channel *channel = &VAR_CHANNEL[ VAR_CFG.CURRENTCHANNEL ];
	channel->POSITION--;
	Tracker::drawPosition( channel );
}

void Tracker::nextChannel( ){
	Channel *before = VAR_CHANNEL + VAR_CFG.CURRENTCHANNEL;
	VAR_CFG.CURRENTCHANNEL = VAR_CFG.CURRENTCHANNEL<5?VAR_CFG.CURRENTCHANNEL+1:0;
	Tracker::drawPosition( before								);
	Tracker::drawPosition( VAR_CHANNEL + VAR_CFG.CURRENTCHANNEL	);
	RegionHandler::updateViewport(&VIEWPORT_TRK, RegionHandler::region->xadd, RegionHandler::region->yadd);
}

void Tracker::prevChannel( ){
	Channel *before = VAR_CHANNEL + VAR_CFG.CURRENTCHANNEL;
	VAR_CFG.CURRENTCHANNEL = VAR_CFG.CURRENTCHANNEL>0?VAR_CFG.CURRENTCHANNEL-1:5;
	Tracker::drawPosition( before 								);
	Tracker::drawPosition( VAR_CHANNEL + VAR_CFG.CURRENTCHANNEL	);
	RegionHandler::updateViewport(&VIEWPORT_TRK, RegionHandler::region->xadd,RegionHandler::region->yadd);
}

void Tracker::dispatchMessage(u32 msg, u32 pointer){
	
	switch(msg){
		case MESSAGE_NAVIGATE_LEFT	: dispatchMessage(MESSAGE_EXIT); RegionHandler::load( ((Region*)pointer)->left ); break;
		case MESSAGE_NAVIGATE_RIGHT	: dispatchMessage(MESSAGE_EXIT); RegionHandler::load( ((Region*)pointer)->right); break;
		case MESSAGE_NAVIGATE_DOWN	: dispatchMessage(MESSAGE_EXIT); RegionHandler::load( ((Region*)pointer)->down ); break;
		case MESSAGE_NAVIGATE_UP	: dispatchMessage(MESSAGE_EXIT); RegionHandler::load( ((Region*)pointer)->up   ); break;
		case MESSAGE_ENTER			: break;
		case MESSAGE_EXIT			: break;
		
		case MESSAGE_CANCEL:
			RegionHandler::controlClear	( RegionHandler::control );
			Tracker::copyChannel( VAR_CHANNEL + VAR_CFG.CURRENTCHANNEL );	//dispatchMessage		( MESSAGE_OTHER_REFRESH_DATA );
			return;
		
		case MESSAGE_OTHER_REFRESH_DATA:
			Tracker::copyChannel( VAR_CHANNEL + VAR_CFG.CURRENTCHANNEL );	
			return;
			
		case MESSAGE_OTHER_PREV:
			// Jump to previous channel
			if( Clip::visible ) return;
			if( KEYPRESS_SELECT) prevPattern();
			else prevChannel();
			return;
		
		case MESSAGE_OTHER_NEXT:
			// Jump to next channel
			if( Clip::visible ) return;
			if( KEYPRESS_SELECT) nextPattern();
			else nextChannel();
			return;
	}
}

void Tracker::globalUpdate(  ){
	//Tracker::clean = false;

	Clip::update( );
	
	// Draw Metronome and Play / Stop icon
	static bool transient_playing;
	static u8 	transient_currentBeats;
	if( (transient_playing != Sequencer::playing) || RegionHandler::redraw || (transient_currentBeats != Sequencer::currentBeats) ){
		Gpu::set(2,0,1, Sequencer::playing ? 0xF08D  : 0x31FE );
		Gpu::set(1,0,0, Sequencer::playing ? ((Sequencer::currentBeats) == 0?0x32 : ((Sequencer::currentBeats&3) == 0?0x34 : 0x33)):0x33);
		transient_currentBeats = Sequencer::currentBeats;
		transient_playing = Sequencer::playing;
	}
	
	if( Transient::changed || RegionHandler::redraw ) Tracker::drawTransientInfo();

	Channel *channel = VAR_CHANNEL;
	for( int i=0; i < CHANNEL_COUNT ;i++, channel++ ){
		if( RegionHandler::redraw ) 
			Tracker::drawPosition( channel );

		if( !Sequencer::playing) continue;
		
		// Update vumeters
		u8 last_peak = channel->lastpeak;
		u8 peak 	 = channel->peak;
		if( last_peak != peak ) VUMETER_V3(8+i, 0, 0x0, peak>12?12:peak);			
		
		// Update next step (cell) reactive elements
		if(channel->LASTSTEP != channel->STEP) Tracker::drawLine( channel );
		
		// Update next pattern reactive elements
		if((channel->LASTPOSITION != channel->POSITION) || RegionHandler::redraw) {
			Tracker::drawPosition( channel );
			Tracker::syncChannel( channel );
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
	
	Channel *channel = &VAR_CHANNEL[ TRACKER_ACTIVE_CHANNEL ];
	
	PatternCell copy = *channel->cells;
	
	for(int i = 0; i<0x10; i++){
		channel->cells->KEY[ expr ? i&0xF : 0xF - (i&0xF) ] = channel->cells->KEY[ expr ? (i+1)&0xF : 0xF - ((i+1)&0xF) ];
		channel->cells->INS[ expr ? i&0xF : 0xF - (i&0xF) ] = channel->cells->INS[ expr ? (i+1)&0xF : 0xF - ((i+1)&0xF) ];
		channel->cells->VOL[ expr ? i&0xF : 0xF - (i&0xF) ] = channel->cells->VOL[ expr ? (i+1)&0xF : 0xF - ((i+1)&0xF) ];
		channel->cells->CMD[ expr ? i&0xF : 0xF - (i&0xF) ] = channel->cells->CMD[ expr ? (i+1)&0xF : 0xF - ((i+1)&0xF) ];
		channel->cells->VAL[ expr ? i&0xF : 0xF - (i&0xF) ] = channel->cells->VAL[ expr ? (i+1)&0xF : 0xF - ((i+1)&0xF) ];
	} 
	channel->cells->KEY[ expr ? 0xF : 0x0 ] = copy.KEY[ expr ? 0x0 : 0xF ];
	channel->cells->INS[ expr ? 0xF : 0x0 ] = copy.INS[ expr ? 0x0 : 0xF ];
	channel->cells->VOL[ expr ? 0xF : 0x0 ] = copy.VOL[ expr ? 0x0 : 0xF ];
	channel->cells->CMD[ expr ? 0xF : 0x0 ] = copy.CMD[ expr ? 0x0 : 0xF ];
	channel->cells->VAL[ expr ? 0xF : 0x0 ] = copy.VAL[ expr ? 0x0 : 0xF ];
	
	// Copy temporary data to pattern data
	Tracker::copyChannel( channel );
	// Reload data from pattern data to temporary AND REDRAW CURRENT CHANNEL (not needed if playing since it will be updated automatically and quickly)
	if(!Sequencer::playing) Tracker::syncChannel( channel );
}

void Tracker::alterColumn( u8 index, u8 min_value, u8 max_value, int q ){
	u16 iconlist[5]    	= { 0xFC6D, 0x4A4B, 0xFC4C, 0xFC0C, 0xFC1F };
	u8 	arrows[2] 		= { 0x008E, 0x008C }; 
	
	// Select icon from list upon requested column and unpack icon(s) 
	u8  icons[2]		= { (iconlist[ index ]>>8), iconlist[ index ] & 0xFF };
	u8  arrow 			= ( q > 0 ) ? arrows[ 1 ] : arrows[ 0 ];
	
	Channel *channel = &VAR_CHANNEL[ TRACKER_ACTIVE_CHANNEL ];
	// Select which array will be modified
	PatternCell *cell 	= channel->cells;
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
	Tracker::copyChannel( channel );
	
	// Reload data from pattern data to temporary AND REDRAW CURRENT CHANNEL (not needed if playing since it will be updated automatically and quickly)
	if(!Sequencer::playing) Tracker::syncChannel( channel );
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
	
	Channel *channel = &VAR_CHANNEL[TRACKER_ACTIVE_CHANNEL];
	
	Notifier::icon( 0x306D, q > 0 ? 0x308C : 0x308E, 0x8A);
	// Check first we dont destroy data 
	for(int i = 0; i<16; i++){
		// Ignore silenced cells 
		if( channel->cells->KEY[ i ] == 0) continue;
		// Abort operation if transformation would move the lowest note into a silence
		if(( channel->cells->KEY[ i ] + q) ==   0) return;
		// Abort operation if transformation would move the highest note out of range
		if(( channel->cells->KEY[ i ] + q) >= 160) return;
	}
	Notifier::icon( 0, 0x706D, q > 0 ? 0x408C : 0x408E);
	// Transpose notes
	for(int i = 0; i<16; i++){
		// Ignore silenced cells 
		if( channel->cells->KEY[ i ] == 0) continue; 
		// Transpose note
		channel->cells->KEY[ i ] = channel->cells->KEY[ i ] + q;
	}

	// Copy temporary data to pattern data
	Tracker::copyChannel( channel );
	// Reload data from pattern data to temporary AND REDRAW CURRENT CHANNEL (not needed if playing since it will be updated automatically and quickly)
	if(!Sequencer::playing) Tracker::syncChannel( channel );
}

void Tracker::processInput( ){
		 if ( KEYDOWN_LEFT 	) transpose	( -1 );
	else if ( KEYDOWN_RIGHT ) transpose	(  1 );
	else if ( KEYDOWN_UP    ) shift		( -1 );
	else if ( KEYDOWN_DOWN  ) shift		(  1 );
}
