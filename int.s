	.file	"int.cpp"
@ GNU C++ version 3.3.2 (arm-thumb-elf)
@	compiled by GNU C version 3.3.1 (cygming special).
@ GGC heuristics: --param ggc-min-expand=47 --param ggc-min-heapsize=32702
@ options passed:  -fpreprocessed -mthumb-interwork -mlong-calls
@ -auxbase-strip -O2 -Wall -fverbose-asm
@ options enabled:  -fdefer-pop -fomit-frame-pointer
@ -foptimize-sibling-calls -fcse-follow-jumps -fcse-skip-blocks
@ -fexpensive-optimizations -fthread-jumps -fstrength-reduce -fpeephole
@ -fforce-mem -ffunction-cse -fkeep-static-consts -fcaller-saves
@ -freg-struct-return -fgcse -fgcse-lm -fgcse-sm -floop-optimize
@ -fcrossjumping -fif-conversion -fif-conversion2 -frerun-cse-after-loop
@ -frerun-loop-opt -fdelete-null-pointer-checks -fschedule-insns
@ -fschedule-insns2 -fsched-interblock -fsched-spec -fbranch-count-reg
@ -fexceptions -freorder-blocks -freorder-functions -fcprop-registers
@ -fcommon -fverbose-asm -fgnu-linker -fregmove -foptimize-register-move
@ -fargument-alias -fstrict-aliasing -fmerge-constants
@ -fzero-initialized-in-bss -fident -fpeephole2 -fguess-branch-probability
@ -fmath-errno -ftrapping-math -mapcs -mapcs-frame -mapcs-32 -msoft-float
@ -mthumb-interwork -mlong-calls

	.global	INT
	.bss
	.global	INT
	.align	2
	.type	INT, %object
	.size	INT, 4
INT:
	.space	4
	.global	Toggle
	.global	Toggle
	.align	2
	.type	Toggle, %object
	.size	Toggle, 4
Toggle:
	.space	4
	.global	IntrTableBuf
	.global	IntrTableBuf
	.align	2
	.type	IntrTableBuf, %object
	.size	IntrTableBuf, 56
IntrTableBuf:
	.space	56
	.global	IntrMainBuf
	.global	IntrMainBuf
	.align	2
	.type	IntrMainBuf, %object
	.size	IntrMainBuf, 16384
IntrMainBuf:
	.space	16384
	.text
	.align	2
	.global	IntHandler
	.type	IntHandler, %function
IntHandler:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	mov	r2, #512
	add	r2, r2, #67108866
	ldrh	r3, [r2, #0]
	tst	r3, #1
	sub	fp, ip, #4
	bne	.L6
	tst	r3, #2
	bne	.L7
	tst	r3, #8
	bne	.L8
	tst	r3, #8192
	bne	.L9
.L1:
	ldmea	fp, {fp, sp, lr}
	bx	lr
.L9:
	mov	r3, #8192	@ movhi
	strh	r3, [r2, #0]	@ movhi 
	bl	_Z10INT_AGBPakv
	b	.L1
.L8:
	mov	r3, #8	@ movhi
	strh	r3, [r2, #0]	@ movhi 
	bl	_Z10INT_Timer0v
	b	.L1
.L7:
	mov	r3, #2	@ movhi
	strh	r3, [r2, #0]	@ movhi 
	bl	_Z10INT_HBlankv
	b	.L1
.L6:
	mov	r3, #1	@ movhi
	strh	r3, [r2, #0]	@ movhi 
	bl	_Z10INT_VBlankv
	b	.L1
	.size	IntHandler, .-IntHandler
	.align	2
	.type	_Z9INT_Dummyv, %function
_Z9INT_Dummyv:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	@ lr needed for prologue
	bx	lr
	.size	_Z9INT_Dummyv, .-_Z9INT_Dummyv
	.align	2
	.type	_Z10INT_VBlankv, %function
_Z10INT_VBlankv:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, lr}
	mov	lr, #67108864
	mov	r4, #0
	add	lr, lr, #520
	strh	r4, [lr, #0]	@ movhi 
	ldr	r0, .L13
	ldr	r3, [r0, #0]	@  SYS_TIMER
	add	r3, r3, #1
	str	r3, [r0, #0]	@  SYS_TIMER
	ldr	r3, [r0, #0]	@  SYS_TIMER
	ldr	r6, .L13+4
	cmp	r3, #60
	ldreq	r2, [r6, #0]	@  SYS_FRAMES
	ldreq	r3, .L13+8
	streq	r2, [r3, #0]	@  SYS_FPS
	ldr	r3, .L13+12
	mov	r1, #1
	streq	r4, [r6, #0]	@  SYS_FRAMES
	mov	ip, #50331648
	streq	r4, [r0, #0]	@  SYS_TIMER
	strb	r1, [r3, #0]	@  SYS_SOUNDTIME
	ldr	r3, .L13+16
	add	ip, ip, #32512
	mov	r5, r1	@ movhi
	strh	r1, [ip, #248]	@ movhi 
	strb	r5, [r3, #0]	@  SYS_QUERYKEY
	strh	r1, [lr, #0]	@ movhi 
	ldmfd	sp!, {r4, r5, r6, lr}
	bx	lr
.L14:
	.align	2
.L13:
	.word	SYS_TIMER
	.word	SYS_FRAMES
	.word	SYS_FPS
	.word	SYS_SOUNDTIME
	.word	SYS_QUERYKEY
	.size	_Z10INT_VBlankv, .-_Z10INT_VBlankv
	.align	2
	.type	_Z10INT_HBlankv, %function
_Z10INT_HBlankv:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #67108864
	add	r2, r2, #520
	mov	r1, #0	@ movhi
	mov	r3, #50331648
	strh	r1, [r2, #0]	@ movhi 
	add	r3, r3, #32512
	mov	r1, #2	@ movhi
	strh	r1, [r3, #248]	@ movhi 
	mov	r3, #1	@ movhi
	@ lr needed for prologue
	strh	r3, [r2, #0]	@ movhi 
	bx	lr
	.size	_Z10INT_HBlankv, .-_Z10INT_HBlankv
	.align	2
	.type	_Z10INT_VCountv, %function
_Z10INT_VCountv:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #67108864
	add	r2, r2, #520
	mov	r1, #0	@ movhi
	mov	r3, #50331648
	strh	r1, [r2, #0]	@ movhi 
	add	r3, r3, #32512
	mov	r1, #4	@ movhi
	strh	r1, [r3, #248]	@ movhi 
	mov	r3, #1	@ movhi
	@ lr needed for prologue
	strh	r3, [r2, #0]	@ movhi 
	bx	lr
	.size	_Z10INT_VCountv, .-_Z10INT_VCountv
	.align	2
	.type	_Z10INT_Timer0v, %function
_Z10INT_Timer0v:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r0, #67108864
	add	r0, r0, #520
	mov	ip, #0
	ldr	r3, .L19
	strh	ip, [r0, #0]	@ movhi 
	ldr	r2, [r3, #0]	@  SYS_PROFILETIMER
	add	r2, r2, #1
	str	r2, [r3, #0]	@  SYS_PROFILETIMER
	ldr	r1, [r3, #0]	@  SYS_PROFILETIMER
	cmp	r1, #9
	ldrls	r3, .L19+4
	mov	r2, #50331648
	add	r2, r2, #32512
	strls	ip, [r3, #0]	@  SYS_PROFILEDTIME
	mov	r3, #8	@ movhi
	strh	r3, [r2, #248]	@ movhi 
	mov	r3, #1	@ movhi
	@ lr needed for prologue
	strh	r3, [r0, #0]	@ movhi 
	bx	lr
.L20:
	.align	2
.L19:
	.word	SYS_PROFILETIMER
	.word	SYS_PROFILEDTIME
	.size	_Z10INT_Timer0v, .-_Z10INT_Timer0v
	.align	2
	.type	_Z10INT_Timer1v, %function
_Z10INT_Timer1v:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #67108864
	add	r2, r2, #520
	mov	r1, #0	@ movhi
	mov	r3, #50331648
	strh	r1, [r2, #0]	@ movhi 
	add	r3, r3, #32512
	mov	r1, #16	@ movhi
	strh	r1, [r3, #248]	@ movhi 
	mov	r3, #1	@ movhi
	@ lr needed for prologue
	strh	r3, [r2, #0]	@ movhi 
	bx	lr
	.size	_Z10INT_Timer1v, .-_Z10INT_Timer1v
	.align	2
	.type	_Z10INT_Timer2v, %function
_Z10INT_Timer2v:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #67108864
	add	r2, r2, #520
	mov	r1, #0	@ movhi
	mov	r3, #50331648
	strh	r1, [r2, #0]	@ movhi 
	add	r3, r3, #32512
	mov	r1, #32	@ movhi
	strh	r1, [r3, #248]	@ movhi 
	mov	r3, #1	@ movhi
	@ lr needed for prologue
	strh	r3, [r2, #0]	@ movhi 
	bx	lr
	.size	_Z10INT_Timer2v, .-_Z10INT_Timer2v
	.align	2
	.type	_Z10INT_Timer3v, %function
_Z10INT_Timer3v:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #67108864
	add	r2, r2, #520
	mov	r1, #0	@ movhi
	mov	r3, #50331648
	strh	r1, [r2, #0]	@ movhi 
	add	r3, r3, #32512
	mov	r1, #64	@ movhi
	strh	r1, [r3, #248]	@ movhi 
	mov	r3, #1	@ movhi
	@ lr needed for prologue
	strh	r3, [r2, #0]	@ movhi 
	bx	lr
	.size	_Z10INT_Timer3v, .-_Z10INT_Timer3v
	.align	2
	.type	_Z10INT_KeyPadv, %function
_Z10INT_KeyPadv:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #67108864
	add	r2, r2, #520
	mov	r1, #0	@ movhi
	mov	r3, #50331648
	strh	r1, [r2, #0]	@ movhi 
	add	r3, r3, #32512
	mov	r1, #4096	@ movhi
	strh	r1, [r3, #248]	@ movhi 
	mov	r3, #1	@ movhi
	@ lr needed for prologue
	strh	r3, [r2, #0]	@ movhi 
	bx	lr
	.size	_Z10INT_KeyPadv, .-_Z10INT_KeyPadv
	.align	2
	.type	_Z10INT_AGBPakv, %function
_Z10INT_AGBPakv:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #67108864
	add	r2, r2, #520
	mov	r1, #0	@ movhi
	mov	r3, #50331648
	strh	r1, [r2, #0]	@ movhi 
	add	r3, r3, #32512
	mov	r1, #8192	@ movhi
	strh	r1, [r3, #248]	@ movhi 
	mov	r3, #1	@ movhi
	@ lr needed for prologue
	strh	r3, [r2, #0]	@ movhi 
	bx	lr
	.size	_Z10INT_AGBPakv, .-_Z10INT_AGBPakv
	.align	2
	.global	_ZN4cINT4InitEv
	.type	_ZN4cINT4InitEv, %function
_ZN4cINT4InitEv:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, lr}
	sub	sp, sp, #4
	add	r2, sp, #4
	mov	r4, #0
	str	r4, [r2, #-4]!	@  Tmp
	mov	lr, #67108864
	mov	ip, #-2063597568
	add	r1, lr, #212	@  DmaCntp
	add	r0, ip, #24576
	mov	r3, #100663296
	str	r2, [lr, #212]
	str	r3, [r1, #4]
	str	r0, [r1, #8]
	mov	r3, #160
	ldr	r0, [r1, #8]
	add	ip, ip, #256
	str	r3, [sp, #0]	@  Tmp
	mov	r3, #117440512
	str	r2, [lr, #212]
	str	r3, [r1, #4]
	str	ip, [r1, #8]
	ldr	r3, [r1, #8]
	mov	r3, #83886080
	str	r4, [sp, #0]	@  Tmp
	str	r2, [lr, #212]
	str	r3, [r1, #4]
	str	ip, [r1, #8]
	mov	r0, #-2080374784
	ldr	r2, [r1, #8]
	ldr	r3, .L27
	ldr	r2, .L27+4
	add	ip, r0, #14
	str	r3, [lr, #212]
	str	r2, [r1, #4]
	ldr	r3, .L27+8
	str	ip, [r1, #8]
	ldr	r2, .L27+12
	add	r0, r0, #4096
	ldr	ip, [r1, #8]
	str	r3, [lr, #212]
	str	r2, [r1, #4]
	str	r0, [r1, #8]
	mov	r3, #50331648
	ldr	r0, [r1, #8]
	add	r3, r3, #32512
	str	r2, [r3, #252]
	add	sp, sp, #4
	ldmfd	sp!, {r4, lr}
	bx	lr
.L28:
	.align	2
.L27:
	.word	IntrTable
	.word	IntrTableBuf
	.word	int_main
	.word	IntrMainBuf
	.size	_ZN4cINT4InitEv, .-_ZN4cINT4InitEv
	.align	2
	.global	_ZN4cINT7DisableEh
	.type	_ZN4cINT7DisableEh, %function
_ZN4cINT7DisableEh:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, lr}
	mov	ip, #67108864
	add	r4, ip, #520
	mov	r3, #0	@ movhi
	strh	r3, [r4, #0]	@ movhi 
	and	r1, r1, #255	@  value
	ldr	r3, .L30
	mov	r0, r1, asl #1	@  value
	add	ip, ip, #512
	ldrh	r2, [ip, #0]
	ldrh	lr, [r0, r3]	@  IntrFlags
	ldr	r3, .L30+4
	mov	r1, r1, asl #2	@  value
	ldrh	r0, [r1, r3]	@  IntrSender
	bic	r2, r2, lr
	strh	r2, [ip, #0]	@ movhi 
	add	r0, r0, #67108864	@  addr
	add	r1, r1, r3
	ldrsh	r2, [r1, #2]	@  flag,  IntrSender
	ldrh	r3, [r0, #0]	@ * addr
	bic	r3, r3, r2	@  flag
	strh	r3, [r0, #0]	@ movhi 	@ * addr
	mov	r3, #1	@ movhi
	strh	r3, [r4, #0]	@ movhi 
	ldmfd	sp!, {r4, lr}
	bx	lr
.L31:
	.align	2
.L30:
	.word	IntrFlags
	.word	IntrSender
	.size	_ZN4cINT7DisableEh, .-_ZN4cINT7DisableEh
	.align	2
	.global	_ZN4cINT6EnableEh
	.type	_ZN4cINT6EnableEh, %function
_ZN4cINT6EnableEh:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, lr}
	mov	ip, #67108864
	add	r4, ip, #520
	mov	r3, #0	@ movhi
	strh	r3, [r4, #0]	@ movhi 
	and	r1, r1, #255	@  value
	ldr	r3, .L33
	mov	r0, r1, asl #1	@  value
	add	ip, ip, #512
	ldrh	r2, [ip, #0]
	ldrh	lr, [r0, r3]	@  IntrFlags
	ldr	r3, .L33+4
	mov	r1, r1, asl #2	@  value
	ldrh	r0, [r1, r3]	@  IntrSender
	orr	r2, r2, lr
	strh	r2, [ip, #0]	@ movhi 
	add	r0, r0, #67108864	@  addr
	add	r1, r1, r3
	ldrsh	r2, [r1, #2]	@  flag,  IntrSender
	ldrh	r3, [r0, #0]	@ * addr
	orr	r3, r3, r2	@  flag
	strh	r3, [r0, #0]	@ movhi 	@ * addr
	mov	r3, #1	@ movhi
	strh	r3, [r4, #0]	@ movhi 
	ldmfd	sp!, {r4, lr}
	bx	lr
.L34:
	.align	2
.L33:
	.word	IntrFlags
	.word	IntrSender
	.size	_ZN4cINT6EnableEh, .-_ZN4cINT6EnableEh
	.align	2
	.type	_Z41__static_initialization_and_destruction_0ii, %function
_Z41__static_initialization_and_destruction_0ii:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	@ lr needed for prologue
	bx	lr
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.section	.rodata
	.align	1
	.type	IntrSender, %object
	.size	IntrSender, 56
IntrSender:
	.short	4
	.short	8
	.short	4
	.short	8
	.short	4
	.short	32
	.short	258
	.short	64
	.short	262
	.short	64
	.short	266
	.short	64
	.short	270
	.short	64
	.short	296
	.short	16384
	.short	186
	.short	16384
	.short	198
	.short	16384
	.short	210
	.short	16384
	.short	222
	.short	16384
	.short	306
	.short	16384
	.short	0
	.short	0
	.align	1
	.type	IntrFlags, %object
	.size	IntrFlags, 28
IntrFlags:
	.short	1
	.short	2
	.short	4
	.short	8
	.short	16
	.short	32
	.short	64
	.short	128
	.short	256
	.short	512
	.short	1024
	.short	2048
	.short	4096
	.short	8192
	.align	2
	.type	IntrTable, %object
	.size	IntrTable, 56
IntrTable:
	.word	_Z10INT_VBlankv
	.word	_Z10INT_HBlankv
	.word	_Z10INT_VCountv
	.word	_Z10INT_Timer0v
	.word	_Z10INT_Timer1v
	.word	_Z10INT_Timer2v
	.word	_Z10INT_Timer3v
	.word	_Z9INT_Dummyv
	.word	_Z9INT_Dummyv
	.word	_Z9INT_Dummyv
	.word	_Z9INT_Dummyv
	.word	_Z9INT_Dummyv
	.word	_Z9INT_Dummyv
	.word	_Z10INT_AGBPakv
	.text
	.align	2
	.type	_GLOBAL__I_INT, %function
_GLOBAL__I_INT:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r1, #65280
	add	r1, r1, #255
	mov	r0, #1
	@ lr needed for prologue
	b	_Z41__static_initialization_and_destruction_0ii
	.size	_GLOBAL__I_INT, .-_GLOBAL__I_INT
	.section	.ctors,"aw",%progbits
	.align	2
	.word	_GLOBAL__I_INT
	.text
	.align	2
	.type	_GLOBAL__D_INT, %function
_GLOBAL__D_INT:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r1, #65280
	add	r1, r1, #255
	mov	r0, #0
	@ lr needed for prologue
	b	_Z41__static_initialization_and_destruction_0ii
	.size	_GLOBAL__D_INT, .-_GLOBAL__D_INT
	.section	.dtors,"aw",%progbits
	.align	2
	.word	_GLOBAL__D_INT
	.ident	"GCC: (GNU) 3.3.2"
