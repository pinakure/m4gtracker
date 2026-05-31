#ifndef CONSOLE_HPP
#define CONSOLE_HPP

class Console {
	private:
		void setTitle		( const char *new_title, u16 color = COLOR_CYAN );
		static u16 			cursor_color;
		static u16 	 		title_color;
		static u16 	 		background_color;
		static u8 	 		cursor_x;
		static u8 	 		cursor_y;
		static const char* 	title;
		
	public:
		
		Console				( const char *new_title=NULL, u16 color = COLOR_BLUE );
		void percent		( float q );
		void print			( const char *text, u16 color = COLOR_CYAN );
		void setCursor		( u8 x, u8 y);
		void render			( );
		void update			( );
		void wait			( u8 time );
};

class ProgressBar {
	public:
		ProgressBar			( );
		void redraw			( );
		void render			(float q, bool highlight );
};

#endif 
