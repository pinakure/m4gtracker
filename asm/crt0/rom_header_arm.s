;********************************************************************
;*                      Data Registered in ROM (ARMASM)             *
;********************************************************************
        ALIGN
        CODE32
        ; Start vector
rom_header      b       rom_header_end

        ; Nintendo logo character data (8000004h)
                DCB     0x24,0xff,0xae,0x51,0x69,0x9a,0xa2,0x21
                DCB     0x3d,0x84,0x82,0x0a,0x84,0xe4,0x09,0xad
                DCB     0x11,0x24,0x8b,0x98,0xc0,0x81,0x7f,0x21
                DCB     0xa3,0x52,0xbe,0x19,0x93,0x09,0xce,0x20
                DCB     0x10,0x46,0x4a,0x4a,0xf8,0x27,0x31,0xec
                DCB     0x58,0xc7,0xe8,0x33,0x82,0xe3,0xce,0xbf
                DCB     0x85,0xf4,0xdf,0x94,0xce,0x4b,0x09,0xc1
                DCB     0x94,0x56,0x8a,0xc0,0x13,0x72,0xa7,0xfc
                DCB     0x9f,0x84,0x4d,0x73,0xa3,0xca,0x9a,0x61
                DCB     0x58,0x97,0xa3,0x27,0xfc,0x03,0x98,0x76
                DCB     0x23,0x1d,0xc7,0x61,0x03,0x04,0xae,0x56
                DCB     0xbf,0x38,0x84,0x00,0x40,0xa7,0x0e,0xfd
                DCB     0xff,0x52,0xfe,0x03,0x6f,0x95,0x30,0xf1
                DCB     0x97,0xfb,0xc0,0x85,0x60,0xd6,0x80,0x25
                DCB     0xa9,0x63,0xbe,0x03,0x01,0x4e,0x38,0xe2
                DCB     0xf9,0xa2,0x34,0xff,0xbb,0x3e,0x03,0x44
                DCB     0x78,0x00,0x90,0xcb,0x88,0x11,0x3a,0x94
                DCB     0x65,0xc0,0x7c,0x63,0x87,0xf0,0x3c,0xaf
                DCB     0xd6,0x25,0xe4,0x8b,0x38,0x0a,0xac,0x72
                DCB     0x21,0xd4,0xf8,0x07

        ; Game pak titles (80000A0h)
                DCB     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
                DCB     0x00,0x00,0x00,0x00

        ; Initial code (80000ACh)
                DCB     0x00,0x00,0x00,0x00

        ; Maker code (80000B0h)
                DCB     0x30,0x31

        ; Fixed value (80000B2h)
                DCB     0x96

        ; Main unit code (80000B3h)
                DCB     0x00

        ; Device type (80000B4h)
                DCB     0x00

       ; Reserved Area (Fixed 0 | 7Byte) (80000B5h)
                DCB     0x00,0x00,0x00,0x00,0x00,0x00,0x00

        ; Game pak version No. (80000BCh)
                DCB     0x00

        ; Complement check (80000BDh)
                DCB     0xf0

       ; Reserved Area (Fixed 0 | 2Byte) (80000BEh)
                DCB     0x00,0x00

rom_header_end


	END

