@********************************************************************
@*          crt0.s                                                  *
@*            Startup Routine (GAS)                                 *
@*                                                                  *
@*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               *
@********************************************************************
    .INCLUDE    "asm/crt0/AgbDefine.s"
    .INCLUDE    "asm/crt0/AgbMemoryMap.s"
    .INCLUDE    "asm/crt0/AgbSyscallDefine.s"
    .INCLUDE    "asm/crt0/AgbMacro.s"
    .TEXT

    .INCLUDE    "asm/crt0/MultiSioDefine.s"

    .GLOBAL     _start
_start:
    .INCLUDE    "asm/crt0/rom_header.s"

@--------------------------------------------------------------------
@-                          Reset                                -
@--------------------------------------------------------------------
    .EXTERN     main
    .GLOBAL     start_vector
    .CODE 32
start_vector:
        mov     r0, #PSR_IRQ_MODE       @ Swithces to IRQ mode
        msr     cpsr, r0
        ldr     sp, sp_irq              @ Sets SP_irq
        mov     r0, #PSR_SYS_MODE       @ Switches to system mode
        msr     cpsr, r0
        ldr     sp, sp_usr              @ Sets SP_usr
        ldr     r1, =INTR_VECTOR_BUF    @ Sets interrupt address
        adr     r0, intr_main
        str     r0, [r1]
        ldr     r1, =main            @ Starts and switches to 16-bit code
        mov     lr, pc
        bx      r1
        b       start_vector            @ Resets

    .ALIGN
sp_usr: .word   WRAM_END - 0x100
sp_irq: .word   WRAM_END - 0x60


@--------------------------------------------------------------------
@-          Interrupt Branch Process (Jump Table) 32 bits    27-62c -
@--------------------------------------------------------------------
	.EXTERN     IntHandler
    .EXTERN     IntrTableBuf
    .GLOBAL     int_main
    .ALIGN
    .CODE 32
int_main:
		ldr     r0, =IntHandler
		bx r0

		
		


@   .ORG    0x200


    .END

