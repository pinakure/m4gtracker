/* Palette: 5 bit per color, 2 byte per pixel (value 31 max), byte order: 0xBBGR : 0bbbBBGGgggrRRRR
           There are 16 subpalettes for the text being drawn in one of the 16 color without losing transparency*/
const unsigned short Palette[256] = {
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x7c30,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x7c30,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x4601,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7c30,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x7fc4,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x7c30,0x057f,0x042d,0x7ffe,0x06ff,0x0438,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x7c30,0x042d,0x7ffe,0x06ff,0x057f,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x7c30,0x7ffe,0x06ff,0x042d,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7c30,0x06ff,0x7ffe,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x7c30,0x06ff,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x7c30,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x28e2,0x7c30,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x04b0,0x28e2,0x7c30,0x0570,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x0570,0x28e2,0x04b0,0x7c30,0x0601,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x0601,0x28e2,0x04b0,0x0570,0x7c30,0x4230,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x4230,0x28e2,0x04b0,0x0570,0x0601,0x7c30,0x7c00,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x7c00,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c30,0x03e0,
	0x0401,0x4601,0x7fc4,0x0438,0x057f,0x042d,0x7ffe,0x06ff,0x03e0,0x28e2,0x04b0,0x0570,0x0601,0x4230,0x7c00,0x7c30 };
