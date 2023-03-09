#ifndef DEBUG_HPP
#define DEBUG_HPP

//#define NDEBUG

#include "../agb.h"

#include "../modules/regionhandler/regionhandler.hpp"

class Debug {
	public:
		static int  		counter;
		static void* 		watch_var;
		static const char* 	watch_name;
		static u8		 	watch_size;
		static u32 			last_var;
	
		static bool 		step_by_step;
	
		static void init			( );
	
		static void benchmark		( RegionHandler &regHnd );
		static void halt			( const char *filename, int line );
		static void panic			( const char *filename, u32 *pointer=NULL);
		static void error			( int error_code , bool recoverable=false);
		
		static void watch			( const char *varname	, u8 var 	 );
		static void watch			( const char *varname	, size_t var );
		static void watch			( const char *varname	, u32 var 	 );
		static void watch			( const char *varname	, u16 var 	 );
		static void watch			( const char *varname	, u32 var 	 , u8 size);
		
		static void string			( u8 x, u8 y, const char *filename	, u8 color=6 );
		static void ascii			( u8 x, u8 y, const char *filename	, u8 color=6 );
		static void number			( u8 x, u8 y, u32 number 			, u8 color=6 );
		static void hexnum			( u8 x, u8 y, u32 number 			, u8 color=6 );
		static void bigString		( u8 x, u8 y, const char *filename	, u8 color=6 );
		static void runTests		( );
		static void clear			( u8 color = 0x00 );
		static void updateMemory	( RegionHandler* rh );
		static void updateWatch		( );
		static void updateMemTest   ( RegionHandler* rh );
		static void memoryTest		( Control* c, bool bigstep, bool add, u32* pointer );
};

#ifndef NDEBUG
	#define HALT 					Debug::halt( __FILE__, __LINE__ );
	#define BREAK 					Debug::error(0x70CEBAD0, true	);
	#define WATCH(a) 				Debug::watch_var = &a; Debug::watch( #a, a);
	#define DEBUG_UPDATE()			Debug::updateWatch()
	#define DEBUG_INIT()			Debug::init()
#else
	#define HALT
	#define BREAK
	#define WATCH(a)
	#define DEBUG_UPDATE()
	#define DEBUG_INIT()
#endif
	
#endif
