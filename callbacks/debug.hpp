#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "../agb.h"

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
};

#define HALT 	Debug::halt( __FILE__, __LINE__ );


#endif
