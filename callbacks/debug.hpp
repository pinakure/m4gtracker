#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "../agb.h"

#include "../modules/regionhandler/regionhandler.hpp"

class Debug {
	public:
		static int  counter;
		static void benchmark		( RegionHandler &regHnd );
		static void halt			( const char *filename, int line );
		static void error			( int error_code , bool recoverable=false);
		static void string			( u8 x, u8 y, const char *filename	, u8 color=6 );
		static void number			( u8 x, u8 y, int number 			, u8 color=6 );
		static void hexnum			( u8 x, u8 y, int number 			, u8 color=6 );
		static void bigString		( u8 x, u8 y, const char *filename	, u8 color=6 );
		static void runTests		( );
		static void clear			( );
		static void updateMemory	( RegionHandler* rh );
		static void updateMemTest   ( RegionHandler* rh );
		static void memoryTest		( Control* c, bool bigstep, bool add, u32* pointer );
};

#define HALT 	Debug::halt( __FILE__, __LINE__ );
#define BREAK 	Debug::error(0x70CEBAD0, true	);
	
#endif
