#include "net.hpp"
#include "../../data/data.hpp"

/*

In this mode, the SIO is 'misused' as a 4bit bi-directional parallel port, each
of the SI,SO,SC,SD pins may be directly controlled, each can be separately
declared as input (with internal pull-up) or as output signal.

4000134h - RCNT (R) - SIO Mode, usage in GENERAL-PURPOSE Mode (R/W)
Interrupts can be requested when SI changes from HIGH to LOW, as General
Purpose mode does not require a serial shift clock, this interrupt may be
produced even when the GBA is in Stop (low power standby) state.
  Bit   Expl.
  0     SC Data Bit         (0=Low, 1=High)
  1     SD Data Bit         (0=Low, 1=High)
  2     SI Data Bit         (0=Low, 1=High)
  3     SO Data Bit         (0=Low, 1=High)
  4     SC Direction        (0=Input, 1=Output)
  5     SD Direction        (0=Input, 1=Output)
  6     SI Direction        (0=Input, 1=Output, but see below)
  7     SO Direction        (0=Input, 1=Output)
  8     SI Interrupt Enable (0=Disable, 1=Enable)
  9-13  Not used
  14    Must be "0" for General-Purpose Mode
  15    Must be "1" for General-Purpose or JOYBUS Mode
SI should be always used as Input to avoid problems with other hardware which
does not expect data to be output there.

*/
#define RCNT_SIO	*(u16*)0x4000134
#define RCNT_SC		0x0001		
#define RCNT_SD		0x0002		
#define RCNT_SI		0x0004		
#define RCNT_SO		0x0008		
#define RCNT_SC_OUT 0x0010		
#define RCNT_SD_OUT 0x0020		
#define RCNT_SI_OUT 0x0040		
#define RCNT_SO_OUT 0x0080		

NetMode Net::netmode = NET_GBA;

void Net::init(){
	setMode( (NetMode) VAR_CFG.LINKMODE.LINKMODE );
}

void Net::setMode(NetMode mode){
	netmode = mode;
	switch(netmode){
		case NET_GBA: 			RCNT_SIO = 0x8000; return;
		case NET_IBM: 			RCNT_SIO = 0x8000; return;
		case NET_SYNC_MASTER: 	RCNT_SIO = 0x8100 | RCNT_SC_OUT | RCNT_SD_OUT | RCNT_SO_OUT ; return;
		case NET_SYNC_SLAVE: 	RCNT_SIO = 0x8100; return;
	}
}

void Net::clear(){
	setMode( netmode );
}


void Net::update(){
	switch(netmode){
		case NET_GBA:			return Net::gbaSync();
		case NET_IBM:			return Net::ibmSync();
		case NET_SYNC_SLAVE:	return Net::recvSync();	
		case NET_SYNC_MASTER:	return Net::sendSync();
	}
}

void Net::gbaSync(){
}

void Net::ibmSync(){
}

void Net::recvSync(){
	while(!(RCNT_SIO & RCNT_SI)){
	}
	RCNT_SIO = 0x8100;			
}

void Net::sendSync(){
	// Working! Set up your synth to sync @ 48 PPQ, then preferably work using triplets
	RCNT_SIO = 0x8100 | RCNT_SO | RCNT_SO_OUT;	
}

void Net::alterMode(Control *c, bool bigstep, bool add, u32 *pointer){	
	VAR_CFG.LINKMODE.LINKMODE 	= ( bigstep ? (add?0x3:0) : (VAR_CFG.LINKMODE.LINKMODE + (add?1:-1)) ) & 0x3; 	
	Transient::bit2 	 		= VAR_CFG.LINKMODE.LINKMODE ; 
	Transient::changed 			= true;
	setMode( (eNetMode) VAR_CFG.LINKMODE.LINKMODE );
}
