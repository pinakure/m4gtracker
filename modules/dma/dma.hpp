#ifndef _MODULE_DMA
#define _MODULE_DMA

/* These pieces of code were directly taken from Nintendo AGB libary */
/* if you don't own a licensed copy of the SDK you are owed to erase this file */

#define REG_DMA0        (REG_BASE + 0xb0)   // DMA 0
#define REG_DMA0SAD     (REG_BASE + 0xb0)   // (Internal Memory -> 
                                            // Internal Memory)
#define REG_DMA0SAD_L   (REG_BASE + 0xb0)
#define REG_DMA0SAD_H   (REG_BASE + 0xb2)
#define REG_DMA0DAD     (REG_BASE + 0xb4)
#define REG_DMA0DAD_L   (REG_BASE + 0xb4)
#define REG_DMA0DAD_H   (REG_BASE + 0xb6)
#define REG_DMA0CNT     (REG_BASE + 0xb8)
#define REG_DMA0CNT_L   (REG_BASE + 0xb8)
#define REG_DMA0CNT_H   (REG_BASE + 0xba)
#define REG_DMA1        (REG_BASE + 0xbc)   // DMA 1
#define REG_DMA1SAD     (REG_BASE + 0xbc)   // (Internal Memory -> Sound 
                                            // FIFO/Internal Memory)
#define REG_DMA1SAD_L   (REG_BASE + 0xbc)
#define REG_DMA1SAD_H   (REG_BASE + 0xbe)
#define REG_DMA1DAD     (REG_BASE + 0xc0)
#define REG_DMA1DAD_L   (REG_BASE + 0xc0)
#define REG_DMA1DAD_H   (REG_BASE + 0xc2)
#define REG_DMA1CNT     (REG_BASE + 0xc4)
#define REG_DMA1CNT_L   (REG_BASE + 0xc4)
#define REG_DMA1CNT_H   (REG_BASE + 0xc6)
#define REG_DMA2        (REG_BASE + 0xc8)   // DMA 2
#define REG_DMA2SAD     (REG_BASE + 0xc8)   // (Internal Memory -> Sound 
                                            // FIFO/Internal Memory)
#define REG_DMA2SAD_L   (REG_BASE + 0xc8)
#define REG_DMA2SAD_H   (REG_BASE + 0xca)
#define REG_DMA2DAD     (REG_BASE + 0xcc)
#define REG_DMA2DAD_L   (REG_BASE + 0xcc)
#define REG_DMA2DAD_H   (REG_BASE + 0xce)
#define REG_DMA2CNT     (REG_BASE + 0xd0)
#define REG_DMA2CNT_L   (REG_BASE + 0xd0)
#define REG_DMA2CNT_H   (REG_BASE + 0xd2)
#define REG_DMA3        (REG_BASE + 0xd4)   // DMA 3
#define REG_DMA3SAD     (REG_BASE + 0xd4)   // (Game Pak/Internal Memory <--> 
                                            // Internal Memory)
#define REG_DMA3SAD_L   (REG_BASE + 0xd4)
#define REG_DMA3SAD_H   (REG_BASE + 0xd6)
#define REG_DMA3DAD     (REG_BASE + 0xd8)
#define REG_DMA3DAD_L   (REG_BASE + 0xd8)
#define REG_DMA3DAD_H   (REG_BASE + 0xda)
#define REG_DMA3CNT     (REG_BASE + 0xdc)
#define REG_DMA3CNT_L   (REG_BASE + 0xdc)
#define REG_DMA3CNT_H   (REG_BASE + 0xde)

/*------------------------------------------------------------------*/
/*                      DMA Control                                 */
/*------------------------------------------------------------------*/

#define DMA_TIMMING_MASK        0x30000000  // Start Timing
#define DMA_COUNT_MASK          0x0000ffff  // Transmission Count

#define DMA_TIMMING_SHIFT       30
#define DMA_COUNT_SHIFT         0

#define DMA_ENABLE              0x80000000  // DMA Enable
#define DMA_IF_ENABLE           0x40000000  // Interrupt Request Enable
#define DMA_TIMMING_IMM         0x00000000  // Run Immediately
#define DMA_TIMMING_V_BLANK     0x10000000  // Run V Blank
#define DMA_TIMMING_H_BLANK     0x20000000  // Run H Blank
#define DMA_TIMMING_DISP        0x30000000  // Run Display
#define DMA_TIMMING_SOUND       0x30000000  // Run Sound FIFO Request
#define DMA_DREQ_ON             0x08000000  // Data Request Synchronize Mode ON
#define DMA_16BIT_BUS           0x00000000  // Select Bus Size 16Bit
#define DMA_32BIT_BUS           0x04000000  // Select Bus Size 32Bit
#define DMA_CONTINUOUS_ON       0x02000000  // Continuous Mode ON
#define DMA_SRC_INC             0x00000000  // Select Source Increment
#define DMA_SRC_DEC             0x00800000  // Select Source Decrement
#define DMA_SRC_FIX             0x01000000  // Select Source Fixed
#define DMA_DEST_INC            0x00000000  // Select Destination Increment
#define DMA_DEST_DEC            0x00200000  // Select Destination Decrement
#define DMA_DEST_FIX            0x00400000  // Select Destination Fixed
#define DMA_DEST_RELOAD         0x00600000  // Select Destination 
                                            // Increment/Reload
#define DmaSet(DmaNo, Srcp, Destp, DmaCntData)          \
{                                                       \
	vu32 *(DmaCntp) = (vu32 *)REG_DMA##DmaNo;           \
	DmaCntp[0] = (vu32 )(Srcp);                         \
	DmaCntp[1] = (vu32 )(Destp);                        \
	DmaCntp[2] = (vu32 )(DmaCntData);                   \
	DmaCntp[2];                                         \
}

#define DmaClear(DmaNo, Data, Destp, Size, Bit)			\
{                                                       \
	vu##Bit Tmp = (vu##Bit )(Data);                     \
	DmaSet(DmaNo, &Tmp, Destp, (                        \
		DMA_ENABLE         | DMA_TIMMING_IMM |          \
		DMA_SRC_FIX        | DMA_DEST_INC    |          \
		DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))));        \
}

#define DmaClearIf(DmaNo, Data, Destp, Size, Bit)       \
{                                                       \
	vu##Bit Tmp = (vu##Bit )(Data);                     \
	DmaSet(DmaNo, &Tmp, Destp, (                        \
		DMA_ENABLE         | DMA_TIMMING_IMM |          \
		DMA_IF_ENABLE      |                            \
		DMA_SRC_FIX        | DMA_DEST_INC    |          \
		DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))));        \
}

#define DmaArrayClear(  DmaNo, Data, Destp, Bit)        \
		DmaClear(       DmaNo, Data, Destp, sizeof(Destp), Bit)

#define DmaArrayClearIf(DmaNo, Data, Destp, Bit)        \
		DmaClearIf(     DmaNo, Data, Destp, sizeof(Destp), Bit)

#define DmaCopy(DmaNo, Srcp, Destp, Size, Bit)          \
                                                        \
DmaSet(DmaNo, Srcp, Destp,  (                           \
    DMA_ENABLE         | DMA_TIMMING_IMM |              \
    DMA_SRC_INC        | DMA_DEST_INC    |              \
    DMA_##Bit##BIT_BUS | ((Size)/((Bit)/8))))

#define DmaCopyIf(DmaNo, Srcp, Destp, Size, Bit)        \
                                                        \
DmaSet(DmaNo, Srcp, Destp,  (                           \
    DMA_ENABLE         | DMA_TIMMING_IMM |              \
    DMA_IF_ENABLE      |                                \
    DMA_SRC_INC        | DMA_DEST_INC    |              \
    DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))))

#define DmaArrayCopy(  DmaNo, Srcp, Destp, Bit)         \
    DmaCopy(       DmaNo, Srcp, Destp, sizeof(Srcp), Bit)

#define DmaArrayCopyIf(DmaNo, Srcp, Destp, Bit)         \
    DmaCopyIf(     DmaNo, Srcp, Destp, sizeof(Srcp), Bit)

#endif 
