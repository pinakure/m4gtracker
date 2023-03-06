
#define DEVICE_EEPROM		0x0000
#define DEVICE_SST			0xD4BF
#define DEVICE_MACRONIX		0x1CC2
#define DEVICE_PANASONIC	0x1B32
#define DEVICE_ATMEL		0x3D1F
#define DEVICE_SANYO_128	0x1362
#define DEVICE_MACRONIX_128	0x09C2

class Sram {
	private:
		u8 *sram;
		
	public:
		int position;
		u16 waitstateBackup;
	
		Sram(){};
		void Init();
		void erase();
		void seek(int p);
		void forward(int p);
		u8   read();
		u16  read16();
		u32  read32();
		void write(u8);
		void write16(u16);
		void write32(u32);
		
		void songLoad		( bool verbose );
		void songSave		( bool verbose );
		void songDefaults	( bool verbose );
		void sharedDataLoad	( bool verbose );
		void sharedDataSave	( bool verbose );
		void dataRevert		( bool verbose );
		void dataBackup		( bool verbose );
		
		void drawPosition(u8 x, u8 y,u8 color);
};

extern Sram SRAM;
