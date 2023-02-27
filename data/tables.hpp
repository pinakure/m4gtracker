#ifndef _DATA_TABLES
#define _DATA_TABLES

class Tables {
	public:
		static const unsigned short DECIMAL_TWOTILES[100][2];
		static const unsigned short HEXADECIMAL_TWOTILES[256][2];
		static const unsigned short HEXADECIMAL_DOUBLE[256][1];
		static const unsigned short HEXADECIMAL[16][1];
		static const unsigned short DECIMAL[10][1];
		static const unsigned short DECIMAL_DOUBLE_TWOTILES[320][2];
		static const unsigned short TEXT[54][1];
		static const unsigned short DECIMAL_DOUBLE[100][1];
		static const unsigned short NIBBLE[16][2];
		static const unsigned short PROGRESS_LINE[36][9];
		static const unsigned short WAVE_DOUBLE[64][2];
		static const unsigned short WAVE_SINGLE[49][1];
		static const unsigned short BIGFONT_HEX[16][2];
		static const unsigned short BIGFONT[40][2];
		static const unsigned short VU3_V[13][3];
		static const unsigned short VU3_H[13][3];
		static const unsigned short VU1_V[8][1]; 
		static const unsigned short VU1_H[8][1];
		static const unsigned short COMMANDS[27][1];
		static const unsigned short TABLEPOSITION[16][16];
		static const unsigned short VISPOSITION1[16][16];
		static const unsigned short VISPOSITION2[16][16];
		static const unsigned short NOTES[120][3];
		static const unsigned short STATUS_STRINGS[21][18];
		static const unsigned short PIXEL[16][4];
};

#endif 
