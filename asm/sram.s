	.text
	.align 2
	.code 32
	.arm
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align 8
	.asciz "SRAM_V130"
	.word 0
	.align 8	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@ Write Single Data Byte (all device types, except Atmel)
@@ The target memory location must have been previously erased.
@@ parameter r0 = position (16 bit)
@@ parameter r1 = byte value
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.globl SRAM_WriteByte
SRAM_WriteByte:
	@@ 'Write byte to address xxxx'	
	ldr r3, =0x0E000000					@ define base address in r3
	strb r1, [r3, +r0]					@ write r1(value) to r3(base) + position (r0)
	bx lr								@ return

	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@ parameter r0 = offset (16 bit)
@@ returns byte read
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.globl SRAM_ReadByte
SRAM_ReadByte:
	ldr r3, =0x0E000000					@ define base address in r3
	ldrb r0, [r3, +r0]					@ load to r0 value stored at r3(base) + offset (r0)
	bx lr								@ return

.end
