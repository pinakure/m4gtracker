#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "agb.h"

//#define NDEBUG

#include "kernel/regionhandler/regionhandler.hpp"


#ifndef NDEBUG
	
#include "screens/console.hpp"

typedef struct sVariableWatch {
	u8 				size;
	void*			variable;
	const char* 	name;
	u16 			timer;
	u32 			last_value;
	bool 			redraw;
} VariableWatch;

typedef struct sArrayWatch {
	u8 				size;
	void**			array;
	u32 			last_value[16];
	const char* 	name;
	u16 			timer;
	bool 			redraw;
} ArrayWatch;

#endif


class Debug {
	public:
		#ifndef NDEBUG
		static int  				counter;
//		static bool 				redraw;
		
		static bool 				step_by_step;
		static bool 				watch_hex;
		
		static VariableWatch		variables[4];
		static ArrayWatch			arrays[4];
		static u8 					array_count;
		static u8 					variable_count;
		
		#endif
	
		static void init			( );
	
		static void benchmark		( );
		static void halt			( const char *filename, int line );
		static void bsod			( const char *title		, const char *message1, const char *message2, const char *message3, u32 *pointer=NULL);
		static void panic			( const char *message	, u32 *pointer=NULL);
		static void error			( int error_code , bool recoverable=false);
		
		static void watchUpdate		( u8 index );
		static void watchUpdateArray( u8 index );
		
		static void watchArray		( const char *varname , u8**	 var );
		static void watchArray		( const char *varname , u16**	 var );
		static void watchArray		( const char *varname , u32**	 var );
		static void watchArray		( const char *varname , size_t** var );
		static void watchArray		( const char *varname , void**	 var , u8 size );

		static void watch			( const char *varname , u8*		 var );
		static void watch			( const char *varname , u16*	 var );
		static void watch			( const char *varname , u32*	 var );
		static void watch			( const char *varname , size_t*	 var );
		static void watch			( const char *varname , void*	 var , u8 size);
		
		static void runTests		( );
		static void updateWatch		( );
		static void updateMemTest   (  );
		static void memoryTest		( Control* c, bool bigstep, bool add, u32* pointer );
		
		static void drawFrame		( u8 pos_x, u8 pos_y, u8 width, u8 height, const char *title, u8 size, bool alt_color );

};

#ifndef NDEBUG
	#define STRINGIZE(x)				STRINGIFY(x)
	#define STRINGIFY(x)				#x
	
	#define ASSERT( expr ) 			if( ! expr ) Debug::bsod("Assertion "#expr" failed", " ", "FILE: "__FILE__ , "LINE: "STRINGIZE(__LINE__), 0 );
	#define BREAK 						Debug::error(0x70CEBAD0, true	);
	#define DEBUG_UPDATE()			Debug::updateWatch()
	#define HALT 						Debug::bsod("HALT", "Program requested execution stop. ", "FILE: "__FILE__ , "LINE: "STRINGIZE(__LINE__), 0 );
	#define WATCH(a) 					Debug::watch( #a, &a);
	#define WATCH_U8_ARRAY(a) 		Debug::watchArray( #a, ( u8**)&a[0]);
	#define WATCH_U16_ARRAY(a) 		Debug::watchArray( #a, (u16**)&a[0]);
	#define WATCH_U32_ARRAY(a) 		Debug::watchArray( #a, (u32**)&a[0]);
#else
	#define ASSERT(a)
	#define BREAK
	#define DEBUG_UPDATE()
	#define HALT
	#define WATCH(a)
	#define WATCH_U8_ARRAY(a)
	#define WATCH_U16_ARRAY(a)
	#define WATCH_U32_ARRAY(a)
#endif
	
#endif
