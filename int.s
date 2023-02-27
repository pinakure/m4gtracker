	.file	"int.cpp"
@ GNU C++ version 3.3.2 (arm-thumb-elf)
@	compiled by GNU C version 3.3.1 (cygming special).
@ GGC heuristics: --param ggc-min-expand=64 --param ggc-min-heapsize=65462
@ options passed:  -fpreprocessed -mthumb-interwork -mlong-calls -mthumb
@ -auxbase-strip -g -O2 -Wall -fverbose-asm
@ options enabled:  -fdefer-pop -fomit-frame-pointer
@ -foptimize-sibling-calls -fcse-follow-jumps -fcse-skip-blocks
@ -fexpensive-optimizations -fthread-jumps -fstrength-reduce -fpeephole
@ -fforce-mem -ffunction-cse -fkeep-static-consts -fcaller-saves
@ -freg-struct-return -fgcse -fgcse-lm -fgcse-sm -floop-optimize
@ -fcrossjumping -fif-conversion -fif-conversion2 -frerun-cse-after-loop
@ -frerun-loop-opt -fdelete-null-pointer-checks -fschedule-insns2
@ -fsched-interblock -fsched-spec -fbranch-count-reg -fexceptions
@ -freorder-blocks -freorder-functions -fcprop-registers -fcommon
@ -fverbose-asm -fgnu-linker -fregmove -foptimize-register-move
@ -fargument-alias -fstrict-aliasing -fmerge-constants
@ -fzero-initialized-in-bss -fident -fpeephole2 -fguess-branch-probability
@ -fmath-errno -ftrapping-math -mapcs-32 -msoft-float -mthumb-interwork
@ -mlong-calls -mthumb

	.file 1 "modules/int/int.cpp"
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.text
.Ltext0:
	.file 2 "C:/HAM/gcc-arm/lib/gcc-lib/arm-thumb-elf/3.3.2/include/stdarg.h"
	.file 3 "C:/HAM/gcc-arm/lib/gcc-lib/arm-thumb-elf/3.3.2/include/stddef.h"
	.file 4 "C:/HAM/gcc-arm/arm-thumb-elf/include/sys/_types.h"
	.file 5 "C:/HAM/gcc-arm/arm-thumb-elf/include/sys/reent.h"
	.file 6 "C:/HAM/gcc-arm/arm-thumb-elf/include/stdlib.h"
	.file 7 "agb.h"
	.file 8 "modules/int/int.hpp"
	.global	SYS_TIMER
	.bss
	.global	SYS_TIMER
	.align	2
	.type	SYS_TIMER, %object
	.size	SYS_TIMER, 4
SYS_TIMER:
	.space	4
	.global	SYS_PROFILEDTIME
	.global	SYS_PROFILEDTIME
	.align	2
	.type	SYS_PROFILEDTIME, %object
	.size	SYS_PROFILEDTIME, 4
SYS_PROFILEDTIME:
	.space	4
	.global	SYS_PROFILETIMER
	.global	SYS_PROFILETIMER
	.align	2
	.type	SYS_PROFILETIMER, %object
	.size	SYS_PROFILETIMER, 4
SYS_PROFILETIMER:
	.space	4
	.global	SYS_FPS
	.global	SYS_FPS
	.align	2
	.type	SYS_FPS, %object
	.size	SYS_FPS, 4
SYS_FPS:
	.space	4
	.global	SYS_FRAMES
	.global	SYS_FRAMES
	.align	2
	.type	SYS_FRAMES, %object
	.size	SYS_FRAMES, 4
SYS_FRAMES:
	.space	4
	.global	SYS_SOUNDTIME
	.global	SYS_SOUNDTIME
	.type	SYS_SOUNDTIME, %object
	.size	SYS_SOUNDTIME, 1
SYS_SOUNDTIME:
	.space	1
	.global	SYS_QUERYKEY
	.global	SYS_QUERYKEY
	.type	SYS_QUERYKEY, %object
	.size	SYS_QUERYKEY, 1
SYS_QUERYKEY:
	.space	1
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
	.thumb_func
	.type	IntHandler, %function
IntHandler:
.LFB3:
	.loc 1 117 0
	push	{lr}
	.loc 1 118 0
.LBB2:
	ldr	r0, .L12
	ldrh	r3, [r0]
	.loc 1 122 0
	lsl	r3, r3, #16
	lsr	r2, r3, #16
	mov	r1, #1
	mov	r3, r2
	and	r3, r3, r1
	cmp	r3, #0
	bne	.L8
	.loc 1 123 0
	mov	r1, #2
	mov	r3, r2
	and	r3, r3, r1
	cmp	r3, #0
	bne	.L9
	.loc 1 125 0
	mov	r1, #8
	mov	r3, r2
	and	r3, r3, r1
	cmp	r3, #0
	bne	.L10
	.loc 1 130 0
	mov	r1, #128
	lsl	r1, r1, #6
	mov	r3, r2
	and	r3, r3, r1
	cmp	r3, #0
	bne	.L11
	.loc 1 132 0
.L1:
	@ sp needed for prologue
	pop	{r0}
	bx	r0
.L11:
	strh	r1, [r0]
	ldr	r3, .L12+4
.L6:
	bl	_call_via_r3
	b	.L1
.L10:
	ldr	r3, .L12+8
.L7:
	strh	r1, [r0]
	b	.L6
.L9:
	ldr	r3, .L12+12
	b	.L7
.L8:
	ldr	r3, .L12+16
	b	.L7
.L13:
	.align	2
.L12:
	.word	67109378
	.word	_ZN3INT9sigAgbPakEv
	.word	_ZN3INT8sigDummyEv
	.word	_ZN3INT9sigHBlankEv
	.word	_ZN3INT9sigVBlankEv
.LBE2:
.LFE3:
	.size	IntHandler, .-IntHandler
	.align	2
	.global	_ZN3INT8sigDummyEv
	.thumb_func
	.type	_ZN3INT8sigDummyEv, %function
_ZN3INT8sigDummyEv:
.LFB5:
	.loc 1 138 0
	.loc 1 139 0
	@ sp needed for prologue
	bx	lr
.LFE5:
	.size	_ZN3INT8sigDummyEv, .-_ZN3INT8sigDummyEv
	.align	2
	.global	_ZN3INT9sigVBlankEv
	.thumb_func
	.type	_ZN3INT9sigVBlankEv, %function
_ZN3INT9sigVBlankEv:
.LFB7:
	.loc 1 141 0
	push	{r4, lr}
	.loc 1 147 0
.LBB3:
	ldrh	r3, .L18
	ldr	r4, .L18+8
	.loc 1 148 0
	ldr	r0, .L18+12
	.loc 1 147 0
	strh	r3, [r4]
	.loc 1 148 0
	ldr	r3, [r0]	@  SYS_TIMER
	add	r3, r3, #1
	str	r3, [r0]	@  SYS_TIMER
	.loc 1 149 0
	ldr	r3, [r0]	@  SYS_TIMER
	cmp	r3, #60
	beq	.L17
.L16:
	.loc 1 155 0
	ldr	r3, .L18+16
	mov	r2, #1
	strb	r2, [r3]	@  SYS_SOUNDTIME
	.loc 1 156 0
	ldrh	r2, .L18+4
	ldr	r3, .L18+20
	strh	r2, [r3]
	.loc 1 158 0
	ldr	r3, .L18+24
	.loc 1 161 0
	@ sp needed for prologue
	.loc 1 158 0
	strb	r2, [r3]	@  SYS_QUERYKEY
	.loc 1 160 0
	strh	r2, [r4]
	b	.L19
.L20:
	.align	2
.L18:
	.word	0
	.word	1
	.word	67109384
	.word	SYS_TIMER
	.word	SYS_SOUNDTIME
	.word	50364408
	.word	SYS_QUERYKEY
.L19:
	.loc 1 161 0
	pop	{r4}
	pop	{r0}
	bx	r0
	.loc 1 151 0
.L17:
	ldr	r1, .L21
	ldr	r3, .L21+4
	ldr	r2, [r1]	@  SYS_FRAMES
	str	r2, [r3]	@  SYS_FPS
	.loc 1 152 0
	mov	r3, #0
	str	r3, [r1]	@  SYS_FRAMES
	.loc 1 153 0
	str	r3, [r0]	@  SYS_TIMER
	b	.L16
.L22:
	.align	2
.L21:
	.word	SYS_FRAMES
	.word	SYS_FPS
.LBE3:
.LFE7:
	.size	_ZN3INT9sigVBlankEv, .-_ZN3INT9sigVBlankEv
	.align	2
	.global	_ZN3INT9sigHBlankEv
	.thumb_func
	.type	_ZN3INT9sigHBlankEv, %function
_ZN3INT9sigHBlankEv:
.LFB9:
	.loc 1 163 0
	.loc 1 164 0
.LBB4:
	ldr	r1, .L24+12
	ldrh	r3, .L24
	.loc 1 165 0
	ldrh	r2, .L24+4
	.loc 1 164 0
	strh	r3, [r1]
	.loc 1 165 0
	ldr	r3, .L24+16
	strh	r2, [r3]
	.loc 1 166 0
	ldrh	r3, .L24+8
	.loc 1 167 0
	@ sp needed for prologue
	.loc 1 166 0
	strh	r3, [r1]
	b	.L25
.L26:
	.align	2
.L24:
	.word	0
	.word	2
	.word	1
	.word	67109384
	.word	50364408
.L25:
	.loc 1 167 0
	bx	lr
.LBE4:
.LFE9:
	.size	_ZN3INT9sigHBlankEv, .-_ZN3INT9sigHBlankEv
	.align	2
	.global	_ZN3INT9sigVCountEv
	.thumb_func
	.type	_ZN3INT9sigVCountEv, %function
_ZN3INT9sigVCountEv:
.LFB11:
	.loc 1 169 0
	.loc 1 170 0
.LBB5:
	ldr	r1, .L28+12
	ldrh	r3, .L28
	.loc 1 171 0
	ldrh	r2, .L28+4
	.loc 1 170 0
	strh	r3, [r1]
	.loc 1 171 0
	ldr	r3, .L28+16
	strh	r2, [r3]
	.loc 1 172 0
	ldrh	r3, .L28+8
	.loc 1 173 0
	@ sp needed for prologue
	.loc 1 172 0
	strh	r3, [r1]
	b	.L29
.L30:
	.align	2
.L28:
	.word	0
	.word	4
	.word	1
	.word	67109384
	.word	50364408
.L29:
	.loc 1 173 0
	bx	lr
.LBE5:
.LFE11:
	.size	_ZN3INT9sigVCountEv, .-_ZN3INT9sigVCountEv
	.align	2
	.global	_ZN3INT9sigTimer0Ev
	.thumb_func
	.type	_ZN3INT9sigTimer0Ev, %function
_ZN3INT9sigTimer0Ev:
.LFB13:
	.loc 1 175 0
	push	{lr}
	.loc 1 176 0
.LBB6:
	ldrh	r3, .L33
	ldr	r1, .L33+12
	.loc 1 177 0
	ldr	r2, .L33+16
	.loc 1 176 0
	strh	r3, [r1]
	.loc 1 177 0
	ldr	r3, [r2]	@  SYS_PROFILETIMER
	add	r3, r3, #1
	str	r3, [r2]	@  SYS_PROFILETIMER
	.loc 1 178 0
	ldr	r3, [r2]	@  SYS_PROFILETIMER
	cmp	r3, #9
	bhi	.L32
	ldr	r3, .L33+20
	mov	r2, #0
	str	r2, [r3]	@  SYS_PROFILEDTIME
.L32:
	.loc 1 179 0
	ldrh	r3, .L33+4
	ldr	r2, .L33+24
	strh	r3, [r2]
	.loc 1 180 0
	ldrh	r3, .L33+8
	.loc 1 181 0
	@ sp needed for prologue
	.loc 1 180 0
	strh	r3, [r1]
	b	.L34
.L35:
	.align	2
.L33:
	.word	0
	.word	8
	.word	1
	.word	67109384
	.word	SYS_PROFILETIMER
	.word	SYS_PROFILEDTIME
	.word	50364408
.L34:
	.loc 1 181 0
	pop	{r0}
	bx	r0
.LBE6:
.LFE13:
	.size	_ZN3INT9sigTimer0Ev, .-_ZN3INT9sigTimer0Ev
	.align	2
	.global	_ZN3INT9sigTimer1Ev
	.thumb_func
	.type	_ZN3INT9sigTimer1Ev, %function
_ZN3INT9sigTimer1Ev:
.LFB15:
	.loc 1 183 0
	.loc 1 184 0
.LBB7:
	ldr	r1, .L37+12
	ldrh	r3, .L37
	.loc 1 185 0
	ldrh	r2, .L37+4
	.loc 1 184 0
	strh	r3, [r1]
	.loc 1 185 0
	ldr	r3, .L37+16
	strh	r2, [r3]
	.loc 1 186 0
	ldrh	r3, .L37+8
	.loc 1 187 0
	@ sp needed for prologue
	.loc 1 186 0
	strh	r3, [r1]
	b	.L38
.L39:
	.align	2
.L37:
	.word	0
	.word	16
	.word	1
	.word	67109384
	.word	50364408
.L38:
	.loc 1 187 0
	bx	lr
.LBE7:
.LFE15:
	.size	_ZN3INT9sigTimer1Ev, .-_ZN3INT9sigTimer1Ev
	.align	2
	.global	_ZN3INT9sigTimer2Ev
	.thumb_func
	.type	_ZN3INT9sigTimer2Ev, %function
_ZN3INT9sigTimer2Ev:
.LFB17:
	.loc 1 189 0
	.loc 1 190 0
.LBB8:
	ldr	r1, .L41+12
	ldrh	r3, .L41
	.loc 1 191 0
	ldrh	r2, .L41+4
	.loc 1 190 0
	strh	r3, [r1]
	.loc 1 191 0
	ldr	r3, .L41+16
	strh	r2, [r3]
	.loc 1 192 0
	ldrh	r3, .L41+8
	.loc 1 193 0
	@ sp needed for prologue
	.loc 1 192 0
	strh	r3, [r1]
	b	.L42
.L43:
	.align	2
.L41:
	.word	0
	.word	32
	.word	1
	.word	67109384
	.word	50364408
.L42:
	.loc 1 193 0
	bx	lr
.LBE8:
.LFE17:
	.size	_ZN3INT9sigTimer2Ev, .-_ZN3INT9sigTimer2Ev
	.align	2
	.global	_ZN3INT9sigTimer3Ev
	.thumb_func
	.type	_ZN3INT9sigTimer3Ev, %function
_ZN3INT9sigTimer3Ev:
.LFB19:
	.loc 1 195 0
	.loc 1 196 0
.LBB9:
	ldr	r1, .L45+12
	ldrh	r3, .L45
	.loc 1 197 0
	ldrh	r2, .L45+4
	.loc 1 196 0
	strh	r3, [r1]
	.loc 1 197 0
	ldr	r3, .L45+16
	strh	r2, [r3]
	.loc 1 198 0
	ldrh	r3, .L45+8
	.loc 1 199 0
	@ sp needed for prologue
	.loc 1 198 0
	strh	r3, [r1]
	b	.L46
.L47:
	.align	2
.L45:
	.word	0
	.word	64
	.word	1
	.word	67109384
	.word	50364408
.L46:
	.loc 1 199 0
	bx	lr
.LBE9:
.LFE19:
	.size	_ZN3INT9sigTimer3Ev, .-_ZN3INT9sigTimer3Ev
	.align	2
	.global	_ZN3INT9sigKeyPadEv
	.thumb_func
	.type	_ZN3INT9sigKeyPadEv, %function
_ZN3INT9sigKeyPadEv:
.LFB21:
	.loc 1 201 0
	.loc 1 202 0
.LBB10:
	ldr	r1, .L49+12
	ldrh	r3, .L49
	.loc 1 205 0
	ldrh	r2, .L49+4
	.loc 1 202 0
	strh	r3, [r1]
	.loc 1 205 0
	ldr	r3, .L49+16
	strh	r2, [r3]
	.loc 1 206 0
	ldrh	r3, .L49+8
	.loc 1 207 0
	@ sp needed for prologue
	.loc 1 206 0
	strh	r3, [r1]
	b	.L50
.L51:
	.align	2
.L49:
	.word	0
	.word	4096
	.word	1
	.word	67109384
	.word	50364408
.L50:
	.loc 1 207 0
	bx	lr
.LBE10:
.LFE21:
	.size	_ZN3INT9sigKeyPadEv, .-_ZN3INT9sigKeyPadEv
	.align	2
	.global	_ZN3INT9sigAgbPakEv
	.thumb_func
	.type	_ZN3INT9sigAgbPakEv, %function
_ZN3INT9sigAgbPakEv:
.LFB23:
	.loc 1 209 0
	.loc 1 210 0
.LBB11:
	ldr	r1, .L53+12
	ldrh	r3, .L53
	.loc 1 212 0
	ldrh	r2, .L53+4
	.loc 1 210 0
	strh	r3, [r1]
	.loc 1 212 0
	ldr	r3, .L53+16
	strh	r2, [r3]
	.loc 1 213 0
	ldrh	r3, .L53+8
	.loc 1 214 0
	@ sp needed for prologue
	.loc 1 213 0
	strh	r3, [r1]
	b	.L54
.L55:
	.align	2
.L53:
	.word	0
	.word	8192
	.word	1
	.word	67109384
	.word	50364408
.L54:
	.loc 1 214 0
	bx	lr
.LBE11:
.LFE23:
	.size	_ZN3INT9sigAgbPakEv, .-_ZN3INT9sigAgbPakEv
	.align	2
	.global	_ZN3INT4initEv
	.thumb_func
	.type	_ZN3INT4initEv, %function
_ZN3INT4initEv:
.LFB25:
	.loc 1 219 0
	push	{r4, lr}
	.loc 1 222 0
.LBB12:
.LBB13:
.LBB14:
	ldr	r2, .L57	@  DmaCntp
	.loc 1 219 0
	sub	sp, sp, #4
	.loc 1 222 0
	mov	r3, #192
.LBE14:
	mov	r1, sp
	mov	r4, #0
.LBB15:
	lsl	r3, r3, #19
.LBE15:
	str	r4, [sp]	@  Tmp
.LBB16:
	str	r1, [r2]	@ * DmaCntp
	str	r3, [r2, #4]
	ldr	r3, .L57+4
	str	r3, [r2, #8]
	ldr	r3, [r2, #8]
	.loc 1 223 0
.LBE16:
.LBE13:
.LBB17:
	mov	r3, #160
	str	r3, [sp]	@  Tmp
.LBB18:
	ldr	r0, .L57+8
	mov	r3, #224
	lsl	r3, r3, #19
	str	r1, [r2]	@ * DmaCntp
	str	r3, [r2, #4]
	str	r0, [r2, #8]
	ldr	r3, [r2, #8]
	.loc 1 224 0
.LBE18:
.LBE17:
.LBB19:
.LBB20:
	mov	r3, #160
	lsl	r3, r3, #19
.LBE20:
	str	r4, [sp]	@  Tmp
.LBB21:
	str	r1, [r2]	@ * DmaCntp
	str	r3, [r2, #4]
	str	r0, [r2, #8]
	ldr	r3, [r2, #8]
	.loc 1 225 0
.LBE21:
.LBE19:
.LBB22:
	ldr	r3, .L57+12
	str	r3, [r2]	@ * DmaCntp
	ldr	r3, .L57+16
	str	r3, [r2, #4]
	ldr	r3, .L57+20
	str	r3, [r2, #8]
	ldr	r3, [r2, #8]
	.loc 1 226 0
.LBE22:
.LBB23:
	ldr	r3, .L57+24
	ldr	r1, .L57+28
	str	r3, [r2]	@ * DmaCntp
	ldr	r3, .L57+32
	str	r1, [r2, #4]
	str	r3, [r2, #8]
	ldr	r3, [r2, #8]
	.loc 1 227 0
.LBE23:
	ldr	r3, .L57+36
	.loc 1 228 0
	add	sp, sp, #4
	.loc 1 227 0
	str	r1, [r3]
	.loc 1 228 0
	@ sp needed for prologue
	pop	{r4}
	pop	{r0}
	bx	r0
.L58:
	.align	2
.L57:
	.word	67109076
	.word	-2063572992
	.word	-2063597312
	.word	IntrTable
	.word	IntrTableBuf
	.word	-2080374770
	.word	int_main
	.word	IntrMainBuf
	.word	-2080370688
	.word	50364412
.LBE12:
.LFE25:
	.size	_ZN3INT4initEv, .-_ZN3INT4initEv
	.align	2
	.global	_ZN3INT7disableEh
	.thumb_func
	.type	_ZN3INT7disableEh, %function
_ZN3INT7disableEh:
.LFB27:
	.loc 1 230 0
	push	{r4, lr}
	.loc 1 232 0
.LBB24:
	ldr	r4, .L60+8
	ldrh	r3, .L60
	.loc 1 230 0
	lsl	r0, r0, #24
	.loc 1 232 0
	strh	r3, [r4]
	.loc 1 233 0
	ldr	r1, .L60+12
	.loc 1 230 0
	lsr	r0, r0, #24	@  value
	.loc 1 233 0
	ldr	r3, .L60+16
	lsl	r2, r0, #1	@  value
	ldrh	r2, [r2, r3]
	ldrh	r3, [r1]
	bic	r3, r3, r2
	strh	r3, [r1]
	.loc 1 235 0
	ldr	r3, .L60+20
	lsl	r0, r0, #2	@  value
	add	r0, r0, r3
	mov	r3, #2
	ldrsh	r1, [r0, r3]	@  flag
	.loc 1 236 0
	ldrh	r2, [r0]
	mov	r3, #128
	lsl	r3, r3, #19
	add	r2, r2, r3	@  addr
	.loc 1 238 0
	ldrh	r3, [r2]	@ * addr
	bic	r3, r3, r1	@  flag
	strh	r3, [r2]	@ * addr
	.loc 1 239 0
	ldrh	r3, .L60+4
	b	.L61
.L62:
	.align	2
.L60:
	.word	0
	.word	1
	.word	67109384
	.word	67109376
	.word	IntrFlags
	.word	IntrSender
.L61:
	.loc 1 240 0
	@ sp needed for prologue
	.loc 1 239 0
	strh	r3, [r4]
	.loc 1 240 0
	pop	{r4}
	pop	{r0}
	bx	r0
.LBE24:
.LFE27:
	.size	_ZN3INT7disableEh, .-_ZN3INT7disableEh
	.align	2
	.global	_ZN3INT6enableEh
	.thumb_func
	.type	_ZN3INT6enableEh, %function
_ZN3INT6enableEh:
.LFB29:
	.loc 1 242 0
	push	{r4, r5, lr}
	.loc 1 244 0
.LBB25:
	ldr	r5, .L64+8
	ldrh	r3, .L64
	.loc 1 242 0
	lsl	r0, r0, #24
	.loc 1 244 0
	strh	r3, [r5]
	.loc 1 242 0
	lsr	r0, r0, #24	@  value
	.loc 1 245 0
	ldr	r4, .L64+12
	ldr	r2, .L64+16
	lsl	r1, r0, #1	@  value
	ldrh	r3, [r4]
	ldrh	r2, [r1, r2]
	orr	r3, r3, r2
	strh	r3, [r4]
	.loc 1 247 0
	ldr	r3, .L64+20
	lsl	r0, r0, #2	@  value
	add	r0, r0, r3
	mov	r2, #2
	ldrsh	r1, [r0, r2]	@  flag
	.loc 1 248 0
	ldrh	r3, [r0]
	mov	r2, #128
	lsl	r2, r2, #19
	add	r3, r3, r2	@  addr
	.loc 1 250 0
	ldrh	r2, [r3]	@ * addr
	orr	r1, r1, r2	@  flag
	strh	r1, [r3]	@  flag, * addr
	.loc 1 251 0
	ldrh	r3, .L64+4
	b	.L65
.L66:
	.align	2
.L64:
	.word	0
	.word	1
	.word	67109384
	.word	67109376
	.word	IntrFlags
	.word	IntrSender
.L65:
	.loc 1 252 0
	@ sp needed for prologue
	.loc 1 251 0
	strh	r3, [r5]
	.loc 1 252 0
	pop	{r4, r5}
	pop	{r0}
	bx	r0
.LBE25:
.LFE29:
	.size	_ZN3INT6enableEh, .-_ZN3INT6enableEh
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
	.word	_ZN3INT9sigVBlankEv
	.word	_ZN3INT9sigHBlankEv
	.word	_ZN3INT9sigVCountEv
	.word	_ZN3INT8sigDummyEv
	.word	_ZN3INT9sigTimer1Ev
	.word	_ZN3INT9sigTimer2Ev
	.word	_ZN3INT9sigTimer3Ev
	.word	_ZN3INT8sigDummyEv
	.word	_ZN3INT8sigDummyEv
	.word	_ZN3INT8sigDummyEv
	.word	_ZN3INT8sigDummyEv
	.word	_ZN3INT8sigDummyEv
	.word	_ZN3INT8sigDummyEv
	.word	_ZN3INT9sigAgbPakEv
	.section	.debug_frame,"",%progbits
.Lframe0:
	.4byte	.LECIE0-.LSCIE0
.LSCIE0:
	.4byte	0xffffffff
	.byte	0x1
	.ascii	"\000"
	.uleb128 0x1
	.sleb128 -4
	.byte	0xe
	.byte	0xc
	.uleb128 0xd
	.uleb128 0x0
	.align	2
.LECIE0:
.LSFDE0:
	.4byte	.LEFDE0-.LASFDE0
.LASFDE0:
	.4byte	.Lframe0
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.align	2
.LEFDE0:
.LSFDE2:
	.4byte	.LEFDE2-.LASFDE2
.LASFDE2:
	.4byte	.Lframe0
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.align	2
.LEFDE2:
.LSFDE4:
	.4byte	.LEFDE4-.LASFDE4
.LASFDE4:
	.4byte	.Lframe0
	.4byte	.LFB7
	.4byte	.LFE7-.LFB7
	.align	2
.LEFDE4:
.LSFDE6:
	.4byte	.LEFDE6-.LASFDE6
.LASFDE6:
	.4byte	.Lframe0
	.4byte	.LFB9
	.4byte	.LFE9-.LFB9
	.align	2
.LEFDE6:
.LSFDE8:
	.4byte	.LEFDE8-.LASFDE8
.LASFDE8:
	.4byte	.Lframe0
	.4byte	.LFB11
	.4byte	.LFE11-.LFB11
	.align	2
.LEFDE8:
.LSFDE10:
	.4byte	.LEFDE10-.LASFDE10
.LASFDE10:
	.4byte	.Lframe0
	.4byte	.LFB13
	.4byte	.LFE13-.LFB13
	.align	2
.LEFDE10:
.LSFDE12:
	.4byte	.LEFDE12-.LASFDE12
.LASFDE12:
	.4byte	.Lframe0
	.4byte	.LFB15
	.4byte	.LFE15-.LFB15
	.align	2
.LEFDE12:
.LSFDE14:
	.4byte	.LEFDE14-.LASFDE14
.LASFDE14:
	.4byte	.Lframe0
	.4byte	.LFB17
	.4byte	.LFE17-.LFB17
	.align	2
.LEFDE14:
.LSFDE16:
	.4byte	.LEFDE16-.LASFDE16
.LASFDE16:
	.4byte	.Lframe0
	.4byte	.LFB19
	.4byte	.LFE19-.LFB19
	.align	2
.LEFDE16:
.LSFDE18:
	.4byte	.LEFDE18-.LASFDE18
.LASFDE18:
	.4byte	.Lframe0
	.4byte	.LFB21
	.4byte	.LFE21-.LFB21
	.align	2
.LEFDE18:
.LSFDE20:
	.4byte	.LEFDE20-.LASFDE20
.LASFDE20:
	.4byte	.Lframe0
	.4byte	.LFB23
	.4byte	.LFE23-.LFB23
	.align	2
.LEFDE20:
.LSFDE22:
	.4byte	.LEFDE22-.LASFDE22
.LASFDE22:
	.4byte	.Lframe0
	.4byte	.LFB25
	.4byte	.LFE25-.LFB25
	.align	2
.LEFDE22:
.LSFDE24:
	.4byte	.LEFDE24-.LASFDE24
.LASFDE24:
	.4byte	.Lframe0
	.4byte	.LFB27
	.4byte	.LFE27-.LFB27
	.align	2
.LEFDE24:
.LSFDE26:
	.4byte	.LEFDE26-.LASFDE26
.LASFDE26:
	.4byte	.Lframe0
	.4byte	.LFB29
	.4byte	.LFE29-.LFB29
	.align	2
.LEFDE26:
	.text
.Letext0:
	.section	.debug_info
	.4byte	0x1465
	.2byte	0x2
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.Ldebug_line0
	.4byte	.Letext0
	.4byte	.Ltext0
	.4byte	.LC204
	.4byte	.LC205
	.4byte	.LC206
	.byte	0x4
	.uleb128 0x2
	.4byte	.LC19
	.byte	0x2
	.byte	0x2b
	.4byte	0x30
	.uleb128 0x3
	.byte	0x4
	.uleb128 0x2
	.4byte	.LC20
	.byte	0x2
	.byte	0x69
	.4byte	0x25
	.uleb128 0x2
	.4byte	.LC21
	.byte	0x3
	.byte	0xd5
	.4byte	0x48
	.uleb128 0x4
	.4byte	.LC23
	.byte	0x4
	.byte	0x7
	.uleb128 0x2
	.4byte	.LC22
	.byte	0x4
	.byte	0xc
	.4byte	0x5a
	.uleb128 0x4
	.4byte	.LC24
	.byte	0x4
	.byte	0x5
	.uleb128 0x2
	.4byte	.LC25
	.byte	0x4
	.byte	0xd
	.4byte	0x6c
	.uleb128 0x4
	.4byte	.LC26
	.byte	0x8
	.byte	0x5
	.uleb128 0x2
	.4byte	.LC27
	.byte	0x4
	.byte	0x10
	.4byte	0x7e
	.uleb128 0x5
	.ascii	"int\000"
	.byte	0x4
	.byte	0x5
	.uleb128 0x6
	.4byte	.LC28
	.byte	0x3
	.2byte	0x162
	.4byte	0x91
	.uleb128 0x4
	.4byte	.LC29
	.byte	0x4
	.byte	0x7
	.uleb128 0x2
	.4byte	.LC30
	.byte	0x4
	.byte	0x21
	.4byte	0xa3
	.uleb128 0x7
	.4byte	0x174
	.4byte	.LC30
	.byte	0x8
	.byte	0x4
	.byte	0x1a
	.uleb128 0x8
	.4byte	.LC31
	.byte	0x4
	.byte	0x1b
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x9
	.4byte	0x11c
	.byte	0x4
	.byte	0x4
	.byte	0x1d
	.uleb128 0xa
	.4byte	.LC32
	.byte	0x4
	.byte	0x1e
	.4byte	0x85
	.uleb128 0xa
	.4byte	.LC33
	.byte	0x4
	.byte	0x1f
	.4byte	0x174
	.uleb128 0xb
	.4byte	0xf6
	.4byte	.LC34
	.4byte	0x192
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x198
	.byte	0x1
	.uleb128 0xd
	.4byte	0x19e
	.byte	0x0
	.uleb128 0xe
	.4byte	0x10d
	.ascii	"._1\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x198
	.byte	0x1
	.uleb128 0xd
	.4byte	0x19e
	.byte	0x0
	.uleb128 0xf
	.ascii	"._1\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x198
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.4byte	.LC35
	.byte	0x4
	.byte	0x20
	.4byte	0xbd
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x10
	.ascii	"._0\000"
	.4byte	0xa3
	.uleb128 0xb
	.4byte	0x14e
	.4byte	.LC34
	.4byte	0x1a9
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x1af
	.byte	0x1
	.uleb128 0xd
	.4byte	0x1b5
	.byte	0x0
	.uleb128 0xe
	.4byte	0x165
	.ascii	"._0\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x1af
	.byte	0x1
	.uleb128 0xd
	.4byte	0x1b5
	.byte	0x0
	.uleb128 0xf
	.ascii	"._0\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x1af
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.4byte	0x184
	.4byte	0x18b
	.uleb128 0x12
	.4byte	0x184
	.byte	0x3
	.byte	0x0
	.uleb128 0x4
	.4byte	.LC23
	.byte	0x4
	.byte	0x7
	.uleb128 0x4
	.4byte	.LC36
	.byte	0x1
	.byte	0x8
	.uleb128 0x13
	.byte	0x4
	.4byte	0xbd
	.uleb128 0x14
	.byte	0x4
	.4byte	0xbd
	.uleb128 0x13
	.byte	0x4
	.4byte	0x1a4
	.uleb128 0x15
	.4byte	0xbd
	.uleb128 0x13
	.byte	0x4
	.4byte	0xa3
	.uleb128 0x14
	.byte	0x4
	.4byte	0xa3
	.uleb128 0x13
	.byte	0x4
	.4byte	0x1bb
	.uleb128 0x15
	.4byte	0xa3
	.uleb128 0x2
	.4byte	.LC37
	.byte	0x4
	.byte	0x23
	.4byte	0x7e
	.uleb128 0x2
	.4byte	.LC38
	.byte	0x5
	.byte	0x13
	.4byte	0x48
	.uleb128 0x7
	.4byte	0x27c
	.4byte	.LC39
	.byte	0x18
	.byte	0x5
	.byte	0x29
	.uleb128 0x8
	.4byte	.LC40
	.byte	0x5
	.byte	0x2a
	.4byte	0x27c
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x16
	.ascii	"_k\000"
	.byte	0x5
	.byte	0x2b
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x8
	.4byte	.LC41
	.byte	0x5
	.byte	0x2b
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x8
	.4byte	.LC42
	.byte	0x5
	.byte	0x2b
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.uleb128 0x8
	.4byte	.LC43
	.byte	0x5
	.byte	0x2b
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.uleb128 0x16
	.ascii	"_x\000"
	.byte	0x5
	.byte	0x2c
	.4byte	0x282
	.byte	0x2
	.byte	0x23
	.uleb128 0x14
	.uleb128 0x17
	.4byte	0x254
	.byte	0x1
	.4byte	.LC34
	.4byte	.LC54
	.4byte	0x292
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x27c
	.byte	0x1
	.uleb128 0xd
	.4byte	0x298
	.byte	0x0
	.uleb128 0x18
	.4byte	0x26c
	.byte	0x1
	.4byte	.LC39
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x27c
	.byte	0x1
	.uleb128 0xd
	.4byte	0x298
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.4byte	.LC39
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x27c
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0x1d6
	.uleb128 0x11
	.4byte	0x292
	.4byte	0x1cb
	.uleb128 0x12
	.4byte	0x184
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.byte	0x4
	.4byte	0x1d6
	.uleb128 0x13
	.byte	0x4
	.4byte	0x29e
	.uleb128 0x15
	.4byte	0x1d6
	.uleb128 0x7
	.4byte	0x375
	.4byte	.LC44
	.byte	0x24
	.byte	0x5
	.byte	0x31
	.uleb128 0x8
	.4byte	.LC45
	.byte	0x5
	.byte	0x32
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x8
	.4byte	.LC46
	.byte	0x5
	.byte	0x33
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x8
	.4byte	.LC47
	.byte	0x5
	.byte	0x34
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x8
	.4byte	.LC48
	.byte	0x5
	.byte	0x35
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.uleb128 0x8
	.4byte	.LC49
	.byte	0x5
	.byte	0x36
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.uleb128 0x8
	.4byte	.LC50
	.byte	0x5
	.byte	0x37
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x14
	.uleb128 0x8
	.4byte	.LC51
	.byte	0x5
	.byte	0x38
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x18
	.uleb128 0x8
	.4byte	.LC52
	.byte	0x5
	.byte	0x39
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x1c
	.uleb128 0x8
	.4byte	.LC53
	.byte	0x5
	.byte	0x3a
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x20
	.uleb128 0x17
	.4byte	0x34d
	.byte	0x1
	.4byte	.LC34
	.4byte	.LC55
	.4byte	0x375
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x37b
	.byte	0x1
	.uleb128 0xd
	.4byte	0x381
	.byte	0x0
	.uleb128 0x18
	.4byte	0x365
	.byte	0x1
	.4byte	.LC44
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x37b
	.byte	0x1
	.uleb128 0xd
	.4byte	0x381
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.4byte	.LC44
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x37b
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.byte	0x4
	.4byte	0x2a3
	.uleb128 0x14
	.byte	0x4
	.4byte	0x2a3
	.uleb128 0x13
	.byte	0x4
	.4byte	0x387
	.uleb128 0x15
	.4byte	0x2a3
	.uleb128 0x1a
	.4byte	0x429
	.4byte	.LC56
	.2byte	0x10c
	.byte	0x5
	.byte	0x44
	.uleb128 0x8
	.4byte	.LC40
	.byte	0x5
	.byte	0x45
	.4byte	0x429
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x8
	.4byte	.LC57
	.byte	0x5
	.byte	0x46
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x8
	.4byte	.LC58
	.byte	0x5
	.byte	0x47
	.4byte	0x42f
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x8
	.4byte	.LC59
	.byte	0x5
	.byte	0x48
	.4byte	0x446
	.byte	0x3
	.byte	0x23
	.uleb128 0x88
	.uleb128 0x8
	.4byte	.LC60
	.byte	0x5
	.byte	0x49
	.4byte	0x1cb
	.byte	0x3
	.byte	0x23
	.uleb128 0x108
	.uleb128 0x17
	.4byte	0x401
	.byte	0x1
	.4byte	.LC34
	.4byte	.LC61
	.4byte	0x458
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x429
	.byte	0x1
	.uleb128 0xd
	.4byte	0x45e
	.byte	0x0
	.uleb128 0x18
	.4byte	0x419
	.byte	0x1
	.4byte	.LC56
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x429
	.byte	0x1
	.uleb128 0xd
	.4byte	0x45e
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.4byte	.LC56
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x429
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0x38c
	.uleb128 0x11
	.4byte	0x43f
	.4byte	0x440
	.uleb128 0x12
	.4byte	0x184
	.byte	0x1f
	.byte	0x0
	.uleb128 0x1b
	.uleb128 0x14
	.byte	0x4
	.4byte	0x43f
	.uleb128 0x11
	.4byte	0x456
	.4byte	0x456
	.uleb128 0x12
	.4byte	0x184
	.byte	0x1f
	.byte	0x0
	.uleb128 0x3
	.byte	0x4
	.uleb128 0x13
	.byte	0x4
	.4byte	0x38c
	.uleb128 0x13
	.byte	0x4
	.4byte	0x464
	.uleb128 0x15
	.4byte	0x38c
	.uleb128 0x7
	.4byte	0x4d9
	.4byte	.LC62
	.byte	0x8
	.byte	0x5
	.byte	0x5b
	.uleb128 0x8
	.4byte	.LC63
	.byte	0x5
	.byte	0x5c
	.4byte	0x4d9
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x8
	.4byte	.LC64
	.byte	0x5
	.byte	0x5d
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x17
	.4byte	0x4b1
	.byte	0x1
	.4byte	.LC34
	.4byte	.LC65
	.4byte	0x4df
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x4e5
	.byte	0x1
	.uleb128 0xd
	.4byte	0x4eb
	.byte	0x0
	.uleb128 0x18
	.4byte	0x4c9
	.byte	0x1
	.4byte	.LC62
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x4e5
	.byte	0x1
	.uleb128 0xd
	.4byte	0x4eb
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.4byte	.LC62
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x4e5
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0x18b
	.uleb128 0x13
	.byte	0x4
	.4byte	0x469
	.uleb128 0x14
	.byte	0x4
	.4byte	0x469
	.uleb128 0x13
	.byte	0x4
	.4byte	0x4f1
	.uleb128 0x15
	.4byte	0x469
	.uleb128 0x2
	.4byte	.LC66
	.byte	0x5
	.byte	0x65
	.4byte	0x5a
	.uleb128 0x7
	.4byte	0x686
	.4byte	.LC67
	.byte	0x5c
	.byte	0x5
	.byte	0x9c
	.uleb128 0x16
	.ascii	"_p\000"
	.byte	0x5
	.byte	0x9d
	.4byte	0x4d9
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x16
	.ascii	"_r\000"
	.byte	0x5
	.byte	0x9e
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x16
	.ascii	"_w\000"
	.byte	0x5
	.byte	0x9f
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x8
	.4byte	.LC68
	.byte	0x5
	.byte	0xa0
	.4byte	0x686
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.uleb128 0x8
	.4byte	.LC69
	.byte	0x5
	.byte	0xa1
	.4byte	0x686
	.byte	0x2
	.byte	0x23
	.uleb128 0xe
	.uleb128 0x16
	.ascii	"_bf\000"
	.byte	0x5
	.byte	0xa2
	.4byte	0x469
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.uleb128 0x8
	.4byte	.LC70
	.byte	0x5
	.byte	0xa3
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x18
	.uleb128 0x8
	.4byte	.LC71
	.byte	0x5
	.byte	0xaa
	.4byte	0x456
	.byte	0x2
	.byte	0x23
	.uleb128 0x1c
	.uleb128 0x8
	.4byte	.LC72
	.byte	0x5
	.byte	0xac
	.4byte	0x6b3
	.byte	0x2
	.byte	0x23
	.uleb128 0x20
	.uleb128 0x8
	.4byte	.LC73
	.byte	0x5
	.byte	0xad
	.4byte	0x6dd
	.byte	0x2
	.byte	0x23
	.uleb128 0x24
	.uleb128 0x8
	.4byte	.LC74
	.byte	0x5
	.byte	0xaf
	.4byte	0x6fc
	.byte	0x2
	.byte	0x23
	.uleb128 0x28
	.uleb128 0x8
	.4byte	.LC75
	.byte	0x5
	.byte	0xb0
	.4byte	0x711
	.byte	0x2
	.byte	0x23
	.uleb128 0x2c
	.uleb128 0x16
	.ascii	"_ub\000"
	.byte	0x5
	.byte	0xb3
	.4byte	0x469
	.byte	0x2
	.byte	0x23
	.uleb128 0x30
	.uleb128 0x16
	.ascii	"_up\000"
	.byte	0x5
	.byte	0xb4
	.4byte	0x4d9
	.byte	0x2
	.byte	0x23
	.uleb128 0x38
	.uleb128 0x16
	.ascii	"_ur\000"
	.byte	0x5
	.byte	0xb5
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x3c
	.uleb128 0x8
	.4byte	.LC76
	.byte	0x5
	.byte	0xb8
	.4byte	0x717
	.byte	0x2
	.byte	0x23
	.uleb128 0x40
	.uleb128 0x8
	.4byte	.LC77
	.byte	0x5
	.byte	0xb9
	.4byte	0x727
	.byte	0x2
	.byte	0x23
	.uleb128 0x43
	.uleb128 0x16
	.ascii	"_lb\000"
	.byte	0x5
	.byte	0xbc
	.4byte	0x469
	.byte	0x2
	.byte	0x23
	.uleb128 0x44
	.uleb128 0x8
	.4byte	.LC78
	.byte	0x5
	.byte	0xbf
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x4c
	.uleb128 0x8
	.4byte	.LC79
	.byte	0x5
	.byte	0xc0
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x50
	.uleb128 0x8
	.4byte	.LC80
	.byte	0x5
	.byte	0xc3
	.4byte	0xb05
	.byte	0x2
	.byte	0x23
	.uleb128 0x54
	.uleb128 0x8
	.4byte	.LC81
	.byte	0x5
	.byte	0xc7
	.4byte	0x1c0
	.byte	0x2
	.byte	0x23
	.uleb128 0x58
	.uleb128 0x17
	.4byte	0x65e
	.byte	0x1
	.4byte	.LC34
	.4byte	.LC82
	.4byte	0xb0b
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xb11
	.byte	0x1
	.uleb128 0xd
	.4byte	0xb17
	.byte	0x0
	.uleb128 0x18
	.4byte	0x676
	.byte	0x1
	.4byte	.LC67
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xb11
	.byte	0x1
	.uleb128 0xd
	.4byte	0xb17
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.4byte	.LC67
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xb11
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.4byte	.LC83
	.byte	0x2
	.byte	0x5
	.uleb128 0x1c
	.4byte	0x6a6
	.4byte	0x7e
	.uleb128 0xd
	.4byte	0x456
	.uleb128 0xd
	.4byte	0x6a6
	.uleb128 0xd
	.4byte	0x7e
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0x6ac
	.uleb128 0x4
	.4byte	.LC84
	.byte	0x1
	.byte	0x8
	.uleb128 0x14
	.byte	0x4
	.4byte	0x68d
	.uleb128 0x1c
	.4byte	0x6d2
	.4byte	0x7e
	.uleb128 0xd
	.4byte	0x456
	.uleb128 0xd
	.4byte	0x6d2
	.uleb128 0xd
	.4byte	0x7e
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0x6d8
	.uleb128 0x15
	.4byte	0x6ac
	.uleb128 0x14
	.byte	0x4
	.4byte	0x6b9
	.uleb128 0x1c
	.4byte	0x6fc
	.4byte	0x4f6
	.uleb128 0xd
	.4byte	0x456
	.uleb128 0xd
	.4byte	0x5a
	.uleb128 0xd
	.4byte	0x7e
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0x6e3
	.uleb128 0x1c
	.4byte	0x711
	.4byte	0x7e
	.uleb128 0xd
	.4byte	0x456
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0x702
	.uleb128 0x11
	.4byte	0x727
	.4byte	0x18b
	.uleb128 0x12
	.4byte	0x184
	.byte	0x2
	.byte	0x0
	.uleb128 0x11
	.4byte	0x737
	.4byte	0x18b
	.uleb128 0x12
	.4byte	0x184
	.byte	0x0
	.byte	0x0
	.uleb128 0x1d
	.4byte	0xb05
	.4byte	.LC85
	.2byte	0x37c
	.byte	0x5
	.2byte	0x215
	.uleb128 0x1e
	.4byte	.LC86
	.byte	0x5
	.2byte	0x216
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x1e
	.4byte	.LC87
	.byte	0x5
	.2byte	0x21b
	.4byte	0xbb2
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x1e
	.4byte	.LC88
	.byte	0x5
	.2byte	0x21b
	.4byte	0xbb2
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x1e
	.4byte	.LC89
	.byte	0x5
	.2byte	0x21b
	.4byte	0xbb2
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.uleb128 0x1e
	.4byte	.LC90
	.byte	0x5
	.2byte	0x21d
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.uleb128 0x1e
	.4byte	.LC91
	.byte	0x5
	.2byte	0x21e
	.4byte	0xc79
	.byte	0x2
	.byte	0x23
	.uleb128 0x14
	.uleb128 0x1e
	.4byte	.LC92
	.byte	0x5
	.2byte	0x220
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x30
	.uleb128 0x1e
	.4byte	.LC93
	.byte	0x5
	.2byte	0x221
	.4byte	0x6d2
	.byte	0x2
	.byte	0x23
	.uleb128 0x34
	.uleb128 0x1e
	.4byte	.LC94
	.byte	0x5
	.2byte	0x223
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x38
	.uleb128 0x1e
	.4byte	.LC95
	.byte	0x5
	.2byte	0x225
	.4byte	0xc94
	.byte	0x2
	.byte	0x23
	.uleb128 0x3c
	.uleb128 0x1e
	.4byte	.LC96
	.byte	0x5
	.2byte	0x228
	.4byte	0x27c
	.byte	0x2
	.byte	0x23
	.uleb128 0x40
	.uleb128 0x1e
	.4byte	.LC97
	.byte	0x5
	.2byte	0x229
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x44
	.uleb128 0x1e
	.4byte	.LC98
	.byte	0x5
	.2byte	0x22a
	.4byte	0x27c
	.byte	0x2
	.byte	0x23
	.uleb128 0x48
	.uleb128 0x1e
	.4byte	.LC99
	.byte	0x5
	.2byte	0x22b
	.4byte	0xc9a
	.byte	0x2
	.byte	0x23
	.uleb128 0x4c
	.uleb128 0x1e
	.4byte	.LC100
	.byte	0x5
	.2byte	0x22e
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x50
	.uleb128 0x1e
	.4byte	.LC101
	.byte	0x5
	.2byte	0x22f
	.4byte	0x6a6
	.byte	0x2
	.byte	0x23
	.uleb128 0x54
	.uleb128 0x1f
	.4byte	0xa5e
	.byte	0xf0
	.byte	0x5
	.2byte	0x232
	.uleb128 0x20
	.4byte	0x99d
	.byte	0xc8
	.byte	0x5
	.2byte	0x234
	.uleb128 0x1e
	.4byte	.LC102
	.byte	0x5
	.2byte	0x235
	.4byte	0x91
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x1e
	.4byte	.LC103
	.byte	0x5
	.2byte	0x236
	.4byte	0x6a6
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x1e
	.4byte	.LC104
	.byte	0x5
	.2byte	0x237
	.4byte	0xca0
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x1e
	.4byte	.LC105
	.byte	0x5
	.2byte	0x238
	.4byte	0x2a3
	.byte	0x2
	.byte	0x23
	.uleb128 0x24
	.uleb128 0x1e
	.4byte	.LC106
	.byte	0x5
	.2byte	0x239
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x48
	.uleb128 0x1e
	.4byte	.LC107
	.byte	0x5
	.2byte	0x23a
	.4byte	0xcb0
	.byte	0x2
	.byte	0x23
	.uleb128 0x4c
	.uleb128 0x1e
	.4byte	.LC108
	.byte	0x5
	.2byte	0x23b
	.4byte	0xbc9
	.byte	0x2
	.byte	0x23
	.uleb128 0x54
	.uleb128 0x1e
	.4byte	.LC109
	.byte	0x5
	.2byte	0x23c
	.4byte	0xa3
	.byte	0x2
	.byte	0x23
	.uleb128 0x64
	.uleb128 0x1e
	.4byte	.LC110
	.byte	0x5
	.2byte	0x23d
	.4byte	0xa3
	.byte	0x2
	.byte	0x23
	.uleb128 0x6c
	.uleb128 0x1e
	.4byte	.LC111
	.byte	0x5
	.2byte	0x23e
	.4byte	0xa3
	.byte	0x2
	.byte	0x23
	.uleb128 0x74
	.uleb128 0x1e
	.4byte	.LC112
	.byte	0x5
	.2byte	0x23f
	.4byte	0xcb7
	.byte	0x2
	.byte	0x23
	.uleb128 0x7c
	.uleb128 0x1e
	.4byte	.LC113
	.byte	0x5
	.2byte	0x240
	.4byte	0xcc7
	.byte	0x3
	.byte	0x23
	.uleb128 0x84
	.uleb128 0x1e
	.4byte	.LC114
	.byte	0x5
	.2byte	0x241
	.4byte	0x7e
	.byte	0x3
	.byte	0x23
	.uleb128 0x9c
	.uleb128 0x1e
	.4byte	.LC115
	.byte	0x5
	.2byte	0x242
	.4byte	0xa3
	.byte	0x3
	.byte	0x23
	.uleb128 0xa0
	.uleb128 0x1e
	.4byte	.LC116
	.byte	0x5
	.2byte	0x243
	.4byte	0xa3
	.byte	0x3
	.byte	0x23
	.uleb128 0xa8
	.uleb128 0x1e
	.4byte	.LC117
	.byte	0x5
	.2byte	0x244
	.4byte	0xa3
	.byte	0x3
	.byte	0x23
	.uleb128 0xb0
	.uleb128 0x1e
	.4byte	.LC118
	.byte	0x5
	.2byte	0x245
	.4byte	0xa3
	.byte	0x3
	.byte	0x23
	.uleb128 0xb8
	.uleb128 0x1e
	.4byte	.LC119
	.byte	0x5
	.2byte	0x246
	.4byte	0xa3
	.byte	0x3
	.byte	0x23
	.uleb128 0xc0
	.uleb128 0xb
	.4byte	0x977
	.4byte	.LC34
	.4byte	0xcd7
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xcdd
	.byte	0x1
	.uleb128 0xd
	.4byte	0xce3
	.byte	0x0
	.uleb128 0xe
	.4byte	0x98e
	.ascii	"._3\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xcdd
	.byte	0x1
	.uleb128 0xd
	.4byte	0xce3
	.byte	0x0
	.uleb128 0xf
	.ascii	"._3\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xcdd
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x21
	.4byte	.LC85
	.byte	0x5
	.2byte	0x247
	.4byte	0x83e
	.uleb128 0x20
	.4byte	0xa11
	.byte	0xf0
	.byte	0x5
	.2byte	0x24c
	.uleb128 0x1e
	.4byte	.LC120
	.byte	0x5
	.2byte	0x24e
	.4byte	0xcee
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x1e
	.4byte	.LC121
	.byte	0x5
	.2byte	0x24f
	.4byte	0xcfe
	.byte	0x2
	.byte	0x23
	.uleb128 0x78
	.uleb128 0xb
	.4byte	0x9eb
	.4byte	.LC34
	.4byte	0xd0e
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xd14
	.byte	0x1
	.uleb128 0xd
	.4byte	0xd1a
	.byte	0x0
	.uleb128 0xe
	.4byte	0xa02
	.ascii	"._4\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xd14
	.byte	0x1
	.uleb128 0xd
	.4byte	0xd1a
	.byte	0x0
	.uleb128 0xf
	.ascii	"._4\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xd14
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x21
	.4byte	.LC122
	.byte	0x5
	.2byte	0x250
	.4byte	0x9a9
	.uleb128 0xb
	.4byte	0xa38
	.4byte	.LC34
	.4byte	0xd25
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xd2b
	.byte	0x1
	.uleb128 0xd
	.4byte	0xd31
	.byte	0x0
	.uleb128 0xe
	.4byte	0xa4f
	.ascii	"._2\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xd2b
	.byte	0x1
	.uleb128 0xd
	.4byte	0xd31
	.byte	0x0
	.uleb128 0xf
	.ascii	"._2\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xd2b
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x1e
	.4byte	.LC123
	.byte	0x5
	.2byte	0x251
	.4byte	0x835
	.byte	0x2
	.byte	0x23
	.uleb128 0x58
	.uleb128 0x1e
	.4byte	.LC56
	.byte	0x5
	.2byte	0x254
	.4byte	0x429
	.byte	0x3
	.byte	0x23
	.uleb128 0x148
	.uleb128 0x1e
	.4byte	.LC124
	.byte	0x5
	.2byte	0x255
	.4byte	0x38c
	.byte	0x3
	.byte	0x23
	.uleb128 0x14c
	.uleb128 0x1e
	.4byte	.LC125
	.byte	0x5
	.2byte	0x258
	.4byte	0xd47
	.byte	0x3
	.byte	0x23
	.uleb128 0x258
	.uleb128 0x1e
	.4byte	.LC126
	.byte	0x5
	.2byte	0x25d
	.4byte	0xb2d
	.byte	0x3
	.byte	0x23
	.uleb128 0x25c
	.uleb128 0x1e
	.4byte	.LC127
	.byte	0x5
	.2byte	0x25e
	.4byte	0xd53
	.byte	0x3
	.byte	0x23
	.uleb128 0x268
	.uleb128 0x17
	.4byte	0xadd
	.byte	0x1
	.4byte	.LC34
	.4byte	.LC128
	.4byte	0xd63
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xb05
	.byte	0x1
	.uleb128 0xd
	.4byte	0xd69
	.byte	0x0
	.uleb128 0x18
	.4byte	0xaf5
	.byte	0x1
	.4byte	.LC85
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xb05
	.byte	0x1
	.uleb128 0xd
	.4byte	0xd69
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.4byte	.LC85
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xb05
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0x737
	.uleb128 0x13
	.byte	0x4
	.4byte	0x501
	.uleb128 0x14
	.byte	0x4
	.4byte	0x501
	.uleb128 0x13
	.byte	0x4
	.4byte	0xb1d
	.uleb128 0x15
	.4byte	0x501
	.uleb128 0x2
	.4byte	.LC129
	.byte	0x5
	.byte	0xf9
	.4byte	0x501
	.uleb128 0x7
	.4byte	0xbac
	.4byte	.LC130
	.byte	0xc
	.byte	0x5
	.byte	0xfd
	.uleb128 0x8
	.4byte	.LC40
	.byte	0x5
	.byte	0xfe
	.4byte	0xbac
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x8
	.4byte	.LC131
	.byte	0x5
	.byte	0xff
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x1e
	.4byte	.LC132
	.byte	0x5
	.2byte	0x100
	.4byte	0xbb2
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x17
	.4byte	0xb84
	.byte	0x1
	.4byte	.LC34
	.4byte	.LC133
	.4byte	0xbb8
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xbac
	.byte	0x1
	.uleb128 0xd
	.4byte	0xbbe
	.byte	0x0
	.uleb128 0x18
	.4byte	0xb9c
	.byte	0x1
	.4byte	.LC130
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xbac
	.byte	0x1
	.uleb128 0xd
	.4byte	0xbbe
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.4byte	.LC130
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xbac
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0xb2d
	.uleb128 0x14
	.byte	0x4
	.4byte	0xb22
	.uleb128 0x13
	.byte	0x4
	.4byte	0xb2d
	.uleb128 0x13
	.byte	0x4
	.4byte	0xbc4
	.uleb128 0x15
	.4byte	0xb2d
	.uleb128 0x22
	.4byte	0xc4b
	.4byte	.LC134
	.byte	0x10
	.byte	0x5
	.2byte	0x118
	.uleb128 0x1e
	.4byte	.LC135
	.byte	0x5
	.2byte	0x119
	.4byte	0xc4b
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x1e
	.4byte	.LC136
	.byte	0x5
	.2byte	0x11a
	.4byte	0xc4b
	.byte	0x2
	.byte	0x23
	.uleb128 0x6
	.uleb128 0x1e
	.4byte	.LC137
	.byte	0x5
	.2byte	0x11b
	.4byte	0xc5b
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.uleb128 0x17
	.4byte	0xc23
	.byte	0x1
	.4byte	.LC34
	.4byte	.LC138
	.4byte	0xc62
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xc68
	.byte	0x1
	.uleb128 0xd
	.4byte	0xc6e
	.byte	0x0
	.uleb128 0x18
	.4byte	0xc3b
	.byte	0x1
	.4byte	.LC134
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xc68
	.byte	0x1
	.uleb128 0xd
	.4byte	0xc6e
	.byte	0x0
	.uleb128 0x19
	.byte	0x1
	.4byte	.LC134
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xc68
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.4byte	0xc5b
	.4byte	0xc5b
	.uleb128 0x12
	.4byte	0x184
	.byte	0x2
	.byte	0x0
	.uleb128 0x4
	.4byte	.LC139
	.byte	0x2
	.byte	0x7
	.uleb128 0x13
	.byte	0x4
	.4byte	0xbc9
	.uleb128 0x14
	.byte	0x4
	.4byte	0xbc9
	.uleb128 0x13
	.byte	0x4
	.4byte	0xc74
	.uleb128 0x15
	.4byte	0xbc9
	.uleb128 0x11
	.4byte	0xc89
	.4byte	0x6ac
	.uleb128 0x12
	.4byte	0x184
	.byte	0x18
	.byte	0x0
	.uleb128 0x23
	.4byte	0xc94
	.uleb128 0xd
	.4byte	0xb05
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0xc89
	.uleb128 0x14
	.byte	0x4
	.4byte	0x27c
	.uleb128 0x11
	.4byte	0xcb0
	.4byte	0x6ac
	.uleb128 0x12
	.4byte	0x184
	.byte	0x19
	.byte	0x0
	.uleb128 0x4
	.4byte	.LC140
	.byte	0x8
	.byte	0x7
	.uleb128 0x11
	.4byte	0xcc7
	.4byte	0x6ac
	.uleb128 0x12
	.4byte	0x184
	.byte	0x7
	.byte	0x0
	.uleb128 0x11
	.4byte	0xcd7
	.4byte	0x6ac
	.uleb128 0x12
	.4byte	0x184
	.byte	0x17
	.byte	0x0
	.uleb128 0x13
	.byte	0x4
	.4byte	0x83e
	.uleb128 0x14
	.byte	0x4
	.4byte	0x83e
	.uleb128 0x13
	.byte	0x4
	.4byte	0xce9
	.uleb128 0x15
	.4byte	0x83e
	.uleb128 0x11
	.4byte	0xcfe
	.4byte	0x4d9
	.uleb128 0x12
	.4byte	0x184
	.byte	0x1d
	.byte	0x0
	.uleb128 0x11
	.4byte	0xd0e
	.4byte	0x91
	.uleb128 0x12
	.4byte	0x184
	.byte	0x1d
	.byte	0x0
	.uleb128 0x13
	.byte	0x4
	.4byte	0x9a9
	.uleb128 0x14
	.byte	0x4
	.4byte	0x9a9
	.uleb128 0x13
	.byte	0x4
	.4byte	0xd20
	.uleb128 0x15
	.4byte	0x9a9
	.uleb128 0x13
	.byte	0x4
	.4byte	0x835
	.uleb128 0x14
	.byte	0x4
	.4byte	0x835
	.uleb128 0x13
	.byte	0x4
	.4byte	0xd37
	.uleb128 0x15
	.4byte	0x835
	.uleb128 0x23
	.4byte	0xd47
	.uleb128 0xd
	.4byte	0x7e
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0xd4d
	.uleb128 0x14
	.byte	0x4
	.4byte	0xd3c
	.uleb128 0x11
	.4byte	0xd63
	.4byte	0xb22
	.uleb128 0x12
	.4byte	0x184
	.byte	0x2
	.byte	0x0
	.uleb128 0x13
	.byte	0x4
	.4byte	0x737
	.uleb128 0x13
	.byte	0x4
	.4byte	0xd6f
	.uleb128 0x15
	.4byte	0x737
	.uleb128 0x2
	.4byte	.LC141
	.byte	0x6
	.byte	0x1c
	.4byte	0xd7f
	.uleb128 0x7
	.4byte	0xdf1
	.4byte	.LC141
	.byte	0x8
	.byte	0x6
	.byte	0x19
	.uleb128 0x8
	.4byte	.LC142
	.byte	0x6
	.byte	0x1a
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x16
	.ascii	"rem\000"
	.byte	0x6
	.byte	0x1b
	.4byte	0x7e
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x10
	.ascii	"._5\000"
	.4byte	0xd7f
	.uleb128 0xb
	.4byte	0xdcb
	.4byte	.LC34
	.4byte	0xdf1
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xdf7
	.byte	0x1
	.uleb128 0xd
	.4byte	0xdfd
	.byte	0x0
	.uleb128 0xe
	.4byte	0xde2
	.ascii	"._5\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xdf7
	.byte	0x1
	.uleb128 0xd
	.4byte	0xdfd
	.byte	0x0
	.uleb128 0xf
	.ascii	"._5\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xdf7
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.byte	0x4
	.4byte	0xd7f
	.uleb128 0x14
	.byte	0x4
	.4byte	0xd7f
	.uleb128 0x13
	.byte	0x4
	.4byte	0xe03
	.uleb128 0x15
	.4byte	0xd7f
	.uleb128 0x2
	.4byte	.LC143
	.byte	0x6
	.byte	0x22
	.4byte	0xe13
	.uleb128 0x7
	.4byte	0xe85
	.4byte	.LC143
	.byte	0x8
	.byte	0x6
	.byte	0x1f
	.uleb128 0x8
	.4byte	.LC142
	.byte	0x6
	.byte	0x20
	.4byte	0x5a
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x16
	.ascii	"rem\000"
	.byte	0x6
	.byte	0x21
	.4byte	0x5a
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x10
	.ascii	"._6\000"
	.4byte	0xe13
	.uleb128 0xb
	.4byte	0xe5f
	.4byte	.LC34
	.4byte	0xe85
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xe8b
	.byte	0x1
	.uleb128 0xd
	.4byte	0xe91
	.byte	0x0
	.uleb128 0xe
	.4byte	0xe76
	.ascii	"._6\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xe8b
	.byte	0x1
	.uleb128 0xd
	.4byte	0xe91
	.byte	0x0
	.uleb128 0xf
	.ascii	"._6\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0xe8b
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.byte	0x4
	.4byte	0xe13
	.uleb128 0x14
	.byte	0x4
	.4byte	0xe13
	.uleb128 0x13
	.byte	0x4
	.4byte	0xe97
	.uleb128 0x15
	.4byte	0xe13
	.uleb128 0x2
	.4byte	.LC144
	.byte	0x7
	.byte	0x19
	.4byte	0xea7
	.uleb128 0x15
	.4byte	0xc5b
	.uleb128 0x24
	.ascii	"cu8\000"
	.byte	0x7
	.byte	0x1a
	.4byte	0xeb7
	.uleb128 0x15
	.4byte	0x18b
	.uleb128 0x2
	.4byte	.LC145
	.byte	0x7
	.byte	0x1b
	.4byte	0xec7
	.uleb128 0x15
	.4byte	0x91
	.uleb128 0x2
	.4byte	.LC146
	.byte	0x7
	.byte	0x1c
	.4byte	0xed7
	.uleb128 0x15
	.4byte	0xcb0
	.uleb128 0x24
	.ascii	"u8\000"
	.byte	0x7
	.byte	0x1d
	.4byte	0x18b
	.uleb128 0x24
	.ascii	"u16\000"
	.byte	0x7
	.byte	0x1e
	.4byte	0xc5b
	.uleb128 0x24
	.ascii	"u32\000"
	.byte	0x7
	.byte	0x1f
	.4byte	0x91
	.uleb128 0x24
	.ascii	"u64\000"
	.byte	0x7
	.byte	0x20
	.4byte	0xcb0
	.uleb128 0x24
	.ascii	"s8\000"
	.byte	0x7
	.byte	0x21
	.4byte	0xf11
	.uleb128 0x4
	.4byte	.LC147
	.byte	0x1
	.byte	0x6
	.uleb128 0x24
	.ascii	"s16\000"
	.byte	0x7
	.byte	0x22
	.4byte	0x686
	.uleb128 0x24
	.ascii	"s32\000"
	.byte	0x7
	.byte	0x23
	.4byte	0x7e
	.uleb128 0x24
	.ascii	"s64\000"
	.byte	0x7
	.byte	0x24
	.4byte	0x6c
	.uleb128 0x24
	.ascii	"vu8\000"
	.byte	0x7
	.byte	0x25
	.4byte	0xf44
	.uleb128 0x25
	.4byte	0x18b
	.uleb128 0x2
	.4byte	.LC148
	.byte	0x7
	.byte	0x26
	.4byte	0xf54
	.uleb128 0x25
	.4byte	0xc5b
	.uleb128 0x2
	.4byte	.LC149
	.byte	0x7
	.byte	0x27
	.4byte	0xf64
	.uleb128 0x25
	.4byte	0x91
	.uleb128 0x2
	.4byte	.LC150
	.byte	0x7
	.byte	0x28
	.4byte	0xf74
	.uleb128 0x25
	.4byte	0xcb0
	.uleb128 0x24
	.ascii	"vs8\000"
	.byte	0x7
	.byte	0x29
	.4byte	0xf84
	.uleb128 0x25
	.4byte	0xf11
	.uleb128 0x2
	.4byte	.LC151
	.byte	0x7
	.byte	0x2a
	.4byte	0xf94
	.uleb128 0x25
	.4byte	0x686
	.uleb128 0x2
	.4byte	.LC152
	.byte	0x7
	.byte	0x2b
	.4byte	0xfa4
	.uleb128 0x25
	.4byte	0x7e
	.uleb128 0x2
	.4byte	.LC153
	.byte	0x7
	.byte	0x2c
	.4byte	0xfb4
	.uleb128 0x25
	.4byte	0x6c
	.uleb128 0x2
	.4byte	.LC154
	.byte	0x7
	.byte	0x2d
	.4byte	0x440
	.uleb128 0x2
	.4byte	.LC155
	.byte	0x7
	.byte	0x2e
	.4byte	0xf18
	.uleb128 0x2
	.4byte	.LC156
	.byte	0x7
	.byte	0x2f
	.4byte	0xf23
	.uleb128 0x2
	.4byte	.LC157
	.byte	0x7
	.byte	0x30
	.4byte	0xee6
	.uleb128 0x2
	.4byte	.LC158
	.byte	0x7
	.byte	0x31
	.4byte	0xef1
	.uleb128 0x26
	.4byte	0x1105
	.ascii	"INT\000"
	.byte	0x4
	.byte	0x8
	.byte	0x23
	.uleb128 0x17
	.4byte	0x101c
	.byte	0x1
	.4byte	.LC34
	.4byte	.LC159
	.4byte	0x1105
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x110b
	.byte	0x1
	.uleb128 0xd
	.4byte	0x1111
	.byte	0x0
	.uleb128 0x27
	.4byte	0x1034
	.byte	0x1
	.ascii	"INT\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x110b
	.byte	0x1
	.uleb128 0xd
	.4byte	0x1111
	.byte	0x0
	.uleb128 0x27
	.4byte	0x1047
	.byte	0x1
	.ascii	"INT\000"
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x110b
	.byte	0x1
	.byte	0x0
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC164
	.byte	0x8
	.byte	0x26
	.4byte	.LC166
	.byte	0x1
	.uleb128 0x29
	.4byte	0x106b
	.byte	0x1
	.4byte	.LC160
	.byte	0x8
	.byte	0x27
	.4byte	.LC162
	.byte	0x1
	.uleb128 0xd
	.4byte	0x18b
	.byte	0x0
	.uleb128 0x29
	.4byte	0x1082
	.byte	0x1
	.4byte	.LC161
	.byte	0x8
	.byte	0x28
	.4byte	.LC163
	.byte	0x1
	.uleb128 0xd
	.4byte	0x18b
	.byte	0x0
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC165
	.byte	0x8
	.byte	0x2a
	.4byte	.LC167
	.byte	0x1
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC168
	.byte	0x8
	.byte	0x2b
	.4byte	.LC169
	.byte	0x1
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC170
	.byte	0x8
	.byte	0x2c
	.4byte	.LC171
	.byte	0x1
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC172
	.byte	0x8
	.byte	0x2d
	.4byte	.LC173
	.byte	0x1
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC174
	.byte	0x8
	.byte	0x2e
	.4byte	.LC175
	.byte	0x1
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC176
	.byte	0x8
	.byte	0x2f
	.4byte	.LC177
	.byte	0x1
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC178
	.byte	0x8
	.byte	0x30
	.4byte	.LC179
	.byte	0x1
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC180
	.byte	0x8
	.byte	0x31
	.4byte	.LC181
	.byte	0x1
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC182
	.byte	0x8
	.byte	0x32
	.4byte	.LC183
	.byte	0x1
	.uleb128 0x28
	.byte	0x1
	.4byte	.LC184
	.byte	0x8
	.byte	0x33
	.4byte	.LC185
	.byte	0x1
	.byte	0x0
	.uleb128 0x13
	.byte	0x4
	.4byte	0xff0
	.uleb128 0x14
	.byte	0x4
	.4byte	0xff0
	.uleb128 0x13
	.byte	0x4
	.4byte	0x1117
	.uleb128 0x15
	.4byte	0xff0
	.uleb128 0x2a
	.4byte	0x113c
	.byte	0x1
	.4byte	.LC186
	.byte	0x1
	.byte	0x75
	.4byte	.LFB3
	.4byte	.LFE3
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2b
	.ascii	"v\000"
	.byte	0x1
	.byte	0x76
	.4byte	0xee6
	.byte	0x0
	.uleb128 0x2c
	.4byte	0x1082
	.byte	0x1
	.byte	0x8a
	.4byte	.LFB5
	.4byte	.LFE5
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2c
	.4byte	0x108f
	.byte	0x1
	.byte	0x8d
	.4byte	.LFB7
	.4byte	.LFE7
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2c
	.4byte	0x109c
	.byte	0x1
	.byte	0xa3
	.4byte	.LFB9
	.4byte	.LFE9
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2c
	.4byte	0x10a9
	.byte	0x1
	.byte	0xa9
	.4byte	.LFB11
	.4byte	.LFE11
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2c
	.4byte	0x10b6
	.byte	0x1
	.byte	0xaf
	.4byte	.LFB13
	.4byte	.LFE13
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2c
	.4byte	0x10c3
	.byte	0x1
	.byte	0xb7
	.4byte	.LFB15
	.4byte	.LFE15
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2c
	.4byte	0x10d0
	.byte	0x1
	.byte	0xbd
	.4byte	.LFB17
	.4byte	.LFE17
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2c
	.4byte	0x10dd
	.byte	0x1
	.byte	0xc3
	.4byte	.LFB19
	.4byte	.LFE19
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2c
	.4byte	0x10ea
	.byte	0x1
	.byte	0xc9
	.4byte	.LFB21
	.4byte	.LFE21
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2c
	.4byte	0x10f7
	.byte	0x1
	.byte	0xd1
	.4byte	.LFB23
	.4byte	.LFE23
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2d
	.4byte	0x12b7
	.4byte	0x1047
	.byte	0x1
	.byte	0xdb
	.4byte	.LFB25
	.4byte	.LFE25
	.byte	0x1
	.byte	0x5d
	.uleb128 0x2e
	.4byte	0x122a
	.4byte	.LBB13
	.4byte	.LBE13
	.uleb128 0x2f
	.ascii	"Tmp\000"
	.byte	0x1
	.byte	0xde
	.4byte	0xf59
	.byte	0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x30
	.4byte	.Ldebug_ranges0+0x0
	.uleb128 0x31
	.4byte	.LC187
	.byte	0x1
	.byte	0xde
	.4byte	0x12b7
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.byte	0x0
	.uleb128 0x2e
	.4byte	0x125b
	.4byte	.LBB17
	.4byte	.LBE17
	.uleb128 0x2f
	.ascii	"Tmp\000"
	.byte	0x1
	.byte	0xdf
	.4byte	0xf59
	.byte	0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x32
	.4byte	.LBB18
	.4byte	.LBE18
	.uleb128 0x33
	.4byte	.LC187
	.byte	0x1
	.byte	0xdf
	.4byte	0x12b7
	.byte	0x0
	.byte	0x0
	.uleb128 0x2e
	.4byte	0x1288
	.4byte	.LBB19
	.4byte	.LBE19
	.uleb128 0x2f
	.ascii	"Tmp\000"
	.byte	0x1
	.byte	0xe0
	.4byte	0xf59
	.byte	0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x30
	.4byte	.Ldebug_ranges0+0x20
	.uleb128 0x33
	.4byte	.LC187
	.byte	0x1
	.byte	0xe0
	.4byte	0x12b7
	.byte	0x0
	.byte	0x0
	.uleb128 0x2e
	.4byte	0x12a1
	.4byte	.LBB22
	.4byte	.LBE22
	.uleb128 0x33
	.4byte	.LC187
	.byte	0x1
	.byte	0xe1
	.4byte	0x12b7
	.byte	0x0
	.uleb128 0x32
	.4byte	.LBB23
	.4byte	.LBE23
	.uleb128 0x33
	.4byte	.LC187
	.byte	0x1
	.byte	0xe2
	.4byte	0x12b7
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.byte	0x4
	.4byte	0xf59
	.uleb128 0x2d
	.4byte	0x12fa
	.4byte	0x1054
	.byte	0x1
	.byte	0xe6
	.4byte	.LFB27
	.4byte	.LFE27
	.byte	0x1
	.byte	0x5d
	.uleb128 0x34
	.4byte	.LC190
	.byte	0x1
	.byte	0xe6
	.4byte	0xedc
	.byte	0x1
	.byte	0x50
	.uleb128 0x31
	.4byte	.LC188
	.byte	0x1
	.byte	0xeb
	.4byte	0xee6
	.byte	0x1
	.byte	0x51
	.uleb128 0x31
	.4byte	.LC189
	.byte	0x1
	.byte	0xec
	.4byte	0xef1
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x2d
	.4byte	0x1337
	.4byte	0x106b
	.byte	0x1
	.byte	0xf2
	.4byte	.LFB29
	.4byte	.LFE29
	.byte	0x1
	.byte	0x5d
	.uleb128 0x34
	.4byte	.LC190
	.byte	0x1
	.byte	0xf2
	.4byte	0xedc
	.byte	0x1
	.byte	0x50
	.uleb128 0x31
	.4byte	.LC188
	.byte	0x1
	.byte	0xf7
	.4byte	0xee6
	.byte	0x1
	.byte	0x51
	.uleb128 0x31
	.4byte	.LC189
	.byte	0x1
	.byte	0xf8
	.4byte	0xef1
	.byte	0x1
	.byte	0x53
	.byte	0x0
	.uleb128 0x35
	.4byte	.LC191
	.byte	0x1
	.byte	0x4
	.4byte	0xf59
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	SYS_TIMER
	.uleb128 0x35
	.4byte	.LC192
	.byte	0x1
	.byte	0x5
	.4byte	0xef1
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	SYS_PROFILEDTIME
	.uleb128 0x35
	.4byte	.LC193
	.byte	0x1
	.byte	0x6
	.4byte	0xf59
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	SYS_PROFILETIMER
	.uleb128 0x35
	.4byte	.LC194
	.byte	0x1
	.byte	0x7
	.4byte	0xf59
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	SYS_FPS
	.uleb128 0x35
	.4byte	.LC195
	.byte	0x1
	.byte	0x8
	.4byte	0xf59
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	SYS_FRAMES
	.uleb128 0x35
	.4byte	.LC196
	.byte	0x1
	.byte	0x9
	.4byte	0x13a3
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	SYS_SOUNDTIME
	.uleb128 0x25
	.4byte	0x13a8
	.uleb128 0x4
	.4byte	.LC197
	.byte	0x1
	.byte	0x2
	.uleb128 0x35
	.4byte	.LC198
	.byte	0x1
	.byte	0xa
	.4byte	0x13a3
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	SYS_QUERYKEY
	.uleb128 0x11
	.4byte	0x13d1
	.4byte	0xee6
	.uleb128 0x12
	.4byte	0x184
	.byte	0x1b
	.byte	0x0
	.uleb128 0x31
	.4byte	.LC199
	.byte	0x1
	.byte	0x34
	.4byte	0x13e2
	.byte	0x5
	.byte	0x3
	.4byte	IntrSender
	.uleb128 0x15
	.4byte	0x13c1
	.uleb128 0x11
	.4byte	0x13f7
	.4byte	0xee6
	.uleb128 0x12
	.4byte	0x184
	.byte	0xd
	.byte	0x0
	.uleb128 0x31
	.4byte	.LC200
	.byte	0x1
	.byte	0x46
	.4byte	0x1408
	.byte	0x5
	.byte	0x3
	.4byte	IntrFlags
	.uleb128 0x15
	.4byte	0x13e7
	.uleb128 0x11
	.4byte	0x141d
	.4byte	0xfb9
	.uleb128 0x12
	.4byte	0x184
	.byte	0xd
	.byte	0x0
	.uleb128 0x31
	.4byte	.LC201
	.byte	0x1
	.byte	0x57
	.4byte	0x142e
	.byte	0x5
	.byte	0x3
	.4byte	IntrTable
	.uleb128 0x15
	.4byte	0x140d
	.uleb128 0x35
	.4byte	.LC202
	.byte	0x1
	.byte	0x68
	.4byte	0x140d
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	IntrTableBuf
	.uleb128 0x11
	.4byte	0x1456
	.4byte	0xef1
	.uleb128 0x36
	.4byte	0x184
	.2byte	0xfff
	.byte	0x0
	.uleb128 0x35
	.4byte	.LC203
	.byte	0x1
	.byte	0x69
	.4byte	0x1445
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	IntrMainBuf
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x10
	.uleb128 0x6
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0x17
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.uleb128 0x10
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.uleb128 0x26
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x18
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x1a
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x1b
	.uleb128 0x15
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x1c
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1d
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x1e
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x1f
	.uleb128 0x17
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x20
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x21
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x22
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x23
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x24
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x25
	.uleb128 0x35
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x26
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x27
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x28
	.uleb128 0x2e
	.byte	0x0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x29
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x2a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x2b
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x2c
	.uleb128 0x2e
	.byte	0x0
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x2d
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x2e
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x2f
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x30
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x32
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x33
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x34
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x35
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x36
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",%progbits
	.4byte	0x1a0
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x1469
	.4byte	0x111c
	.ascii	"IntHandler\000"
	.4byte	0x113c
	.ascii	"INT::sigDummy\000"
	.4byte	0x114d
	.ascii	"INT::sigVBlank\000"
	.4byte	0x115e
	.ascii	"INT::sigHBlank\000"
	.4byte	0x116f
	.ascii	"INT::sigVCount\000"
	.4byte	0x1180
	.ascii	"INT::sigTimer0\000"
	.4byte	0x1191
	.ascii	"INT::sigTimer1\000"
	.4byte	0x11a2
	.ascii	"INT::sigTimer2\000"
	.4byte	0x11b3
	.ascii	"INT::sigTimer3\000"
	.4byte	0x11c4
	.ascii	"INT::sigKeyPad\000"
	.4byte	0x11d5
	.ascii	"INT::sigAgbPak\000"
	.4byte	0x11e6
	.ascii	"INT::init\000"
	.4byte	0x12bd
	.ascii	"INT::disable\000"
	.4byte	0x12fa
	.ascii	"INT::enable\000"
	.4byte	0x1337
	.ascii	"SYS_TIMER\000"
	.4byte	0x1349
	.ascii	"SYS_PROFILEDTIME\000"
	.4byte	0x135b
	.ascii	"SYS_PROFILETIMER\000"
	.4byte	0x136d
	.ascii	"SYS_FPS\000"
	.4byte	0x137f
	.ascii	"SYS_FRAMES\000"
	.4byte	0x1391
	.ascii	"SYS_SOUNDTIME\000"
	.4byte	0x13af
	.ascii	"SYS_QUERYKEY\000"
	.4byte	0x1433
	.ascii	"IntrTableBuf\000"
	.4byte	0x1456
	.ascii	"IntrMainBuf\000"
	.4byte	0x0
	.section	.debug_aranges,"",%progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0x0
	.2byte	0x0
	.2byte	0x0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0x0
	.4byte	0x0
	.section	.debug_ranges,"",%progbits
.Ldebug_ranges0:
	.4byte	.LBB14-.Ltext0
	.4byte	.LBE14-.Ltext0
	.4byte	.LBB16-.Ltext0
	.4byte	.LBE16-.Ltext0
	.4byte	.LBB15-.Ltext0
	.4byte	.LBE15-.Ltext0
	.4byte	0x0
	.4byte	0x0
	.4byte	.LBB20-.Ltext0
	.4byte	.LBE20-.Ltext0
	.4byte	.LBB21-.Ltext0
	.4byte	.LBE21-.Ltext0
	.4byte	0x0
	.4byte	0x0
	.section	.debug_str,"MS",%progbits,1
.LC51:
	.ascii	"__tm_wday\000"
.LC182:
	.ascii	"sigKeyPad\000"
.LC105:
	.ascii	"_localtime_buf\000"
.LC64:
	.ascii	"_size\000"
.LC101:
	.ascii	"_cvtbuf\000"
.LC82:
	.ascii	"_ZN7__sFILEaSERKS_\000"
.LC61:
	.ascii	"_ZN7_atexitaSERKS_\000"
.LC46:
	.ascii	"__tm_min\000"
.LC119:
	.ascii	"_wcsrtombs_state\000"
.LC124:
	.ascii	"_atexit0\000"
.LC86:
	.ascii	"_errno\000"
.LC39:
	.ascii	"_Bigint\000"
.LC139:
	.ascii	"short unsigned int\000"
.LC70:
	.ascii	"_lbfsize\000"
.LC45:
	.ascii	"__tm_sec\000"
.LC36:
	.ascii	"unsigned char\000"
.LC121:
	.ascii	"_nmalloc\000"
.LC113:
	.ascii	"_signal_buf\000"
.LC202:
	.ascii	"IntrTableBuf\000"
.LC187:
	.ascii	"DmaCntp\000"
.LC151:
	.ascii	"vs16\000"
.LC190:
	.ascii	"value\000"
.LC35:
	.ascii	"__value\000"
.LC135:
	.ascii	"_seed\000"
.LC62:
	.ascii	"__sbuf\000"
.LC74:
	.ascii	"_seek\000"
.LC195:
	.ascii	"SYS_FRAMES\000"
.LC21:
	.ascii	"size_t\000"
.LC72:
	.ascii	"_read\000"
.LC25:
	.ascii	"_off64_t\000"
.LC137:
	.ascii	"_add\000"
.LC110:
	.ascii	"_mbtowc_state\000"
.LC131:
	.ascii	"_niobs\000"
.LC188:
	.ascii	"flag\000"
.LC125:
	.ascii	"_sig_func\000"
.LC96:
	.ascii	"_result\000"
.LC93:
	.ascii	"_current_locale\000"
.LC150:
	.ascii	"vu64\000"
.LC67:
	.ascii	"__sFILE\000"
.LC22:
	.ascii	"_off_t\000"
.LC177:
	.ascii	"_ZN3INT9sigTimer1Ev\000"
.LC118:
	.ascii	"_wcrtomb_state\000"
.LC152:
	.ascii	"vs32\000"
.LC123:
	.ascii	"_new\000"
.LC171:
	.ascii	"_ZN3INT9sigHBlankEv\000"
.LC66:
	.ascii	"_fpos_t\000"
.LC56:
	.ascii	"_atexit\000"
.LC116:
	.ascii	"_mbrtowc_state\000"
.LC169:
	.ascii	"_ZN3INT9sigVBlankEv\000"
.LC141:
	.ascii	"div_t\000"
.LC163:
	.ascii	"_ZN3INT6enableEh\000"
.LC155:
	.ascii	"sfp16\000"
.LC138:
	.ascii	"_ZN7_rand48aSERKS_\000"
.LC130:
	.ascii	"_glue\000"
.LC166:
	.ascii	"_ZN3INT4initEv\000"
.LC75:
	.ascii	"_close\000"
.LC194:
	.ascii	"SYS_FPS\000"
.LC167:
	.ascii	"_ZN3INT8sigDummyEv\000"
.LC128:
	.ascii	"_ZN6_reentaSERKS_\000"
.LC144:
	.ascii	"cu16\000"
.LC91:
	.ascii	"_emergency\000"
.LC156:
	.ascii	"sfp32\000"
.LC71:
	.ascii	"_cookie\000"
.LC63:
	.ascii	"_base\000"
.LC192:
	.ascii	"SYS_PROFILEDTIME\000"
.LC78:
	.ascii	"_blksize\000"
.LC23:
	.ascii	"long unsigned int\000"
.LC31:
	.ascii	"__count\000"
.LC65:
	.ascii	"_ZN6__sbufaSERKS_\000"
.LC203:
	.ascii	"IntrMainBuf\000"
.LC127:
	.ascii	"__sf\000"
.LC197:
	.ascii	"bool\000"
.LC87:
	.ascii	"_stdin\000"
.LC161:
	.ascii	"enable\000"
.LC142:
	.ascii	"quot\000"
.LC153:
	.ascii	"vs64\000"
.LC92:
	.ascii	"_current_category\000"
.LC145:
	.ascii	"cu32\000"
.LC52:
	.ascii	"__tm_yday\000"
.LC77:
	.ascii	"_nbuf\000"
.LC136:
	.ascii	"_mult\000"
.LC44:
	.ascii	"__tm\000"
.LC49:
	.ascii	"__tm_mon\000"
.LC83:
	.ascii	"short int\000"
.LC205:
	.ascii	"/cygdrive/c/codigo/m4gtool/data/m4g\000"
.LC170:
	.ascii	"sigHBlank\000"
.LC26:
	.ascii	"long long int\000"
.LC27:
	.ascii	"_ssize_t\000"
.LC53:
	.ascii	"__tm_isdst\000"
.LC140:
	.ascii	"long long unsigned int\000"
.LC108:
	.ascii	"_r48\000"
.LC33:
	.ascii	"__wchb\000"
.LC117:
	.ascii	"_mbsrtowcs_state\000"
.LC28:
	.ascii	"wint_t\000"
.LC84:
	.ascii	"char\000"
.LC143:
	.ascii	"ldiv_t\000"
.LC115:
	.ascii	"_mbrlen_state\000"
.LC97:
	.ascii	"_result_k\000"
.LC162:
	.ascii	"_ZN3INT7disableEh\000"
.LC200:
	.ascii	"IntrFlags\000"
.LC102:
	.ascii	"_unused_rand\000"
.LC186:
	.ascii	"IntHandler\000"
.LC19:
	.ascii	"__gnuc_va_list\000"
.LC85:
	.ascii	"_reent\000"
.LC168:
	.ascii	"sigVBlank\000"
.LC30:
	.ascii	"_mbstate_t\000"
.LC159:
	.ascii	"_ZN3INTaSERKS_\000"
.LC54:
	.ascii	"_ZN7_BigintaSERKS_\000"
.LC179:
	.ascii	"_ZN3INT9sigTimer2Ev\000"
.LC37:
	.ascii	"_flock_t\000"
.LC43:
	.ascii	"_wds\000"
.LC32:
	.ascii	"__wch\000"
.LC55:
	.ascii	"_ZN4__tmaSERKS_\000"
.LC199:
	.ascii	"IntrSender\000"
.LC164:
	.ascii	"init\000"
.LC132:
	.ascii	"_iobs\000"
.LC146:
	.ascii	"cu64\000"
.LC68:
	.ascii	"_flags\000"
.LC122:
	.ascii	"_unused\000"
.LC95:
	.ascii	"__cleanup\000"
.LC204:
	.ascii	"modules/int/int.cpp\000"
.LC206:
	.ascii	"GNU C++ 3.3.2\000"
.LC99:
	.ascii	"_freelist\000"
.LC129:
	.ascii	"__FILE\000"
.LC198:
	.ascii	"SYS_QUERYKEY\000"
.LC147:
	.ascii	"signed char\000"
.LC191:
	.ascii	"SYS_TIMER\000"
.LC89:
	.ascii	"_stderr\000"
.LC59:
	.ascii	"_fnargs\000"
.LC165:
	.ascii	"sigDummy\000"
.LC29:
	.ascii	"unsigned int\000"
.LC106:
	.ascii	"_gamma_signgam\000"
.LC47:
	.ascii	"__tm_hour\000"
.LC58:
	.ascii	"_fns\000"
.LC189:
	.ascii	"addr\000"
.LC88:
	.ascii	"_stdout\000"
.LC34:
	.ascii	"operator=\000"
.LC76:
	.ascii	"_ubuf\000"
.LC184:
	.ascii	"sigAgbPak\000"
.LC107:
	.ascii	"_rand_next\000"
.LC112:
	.ascii	"_l64a_buf\000"
.LC173:
	.ascii	"_ZN3INT9sigVCountEv\000"
.LC42:
	.ascii	"_sign\000"
.LC40:
	.ascii	"_next\000"
.LC172:
	.ascii	"sigVCount\000"
.LC50:
	.ascii	"__tm_year\000"
.LC185:
	.ascii	"_ZN3INT9sigAgbPakEv\000"
.LC103:
	.ascii	"_strtok_last\000"
.LC109:
	.ascii	"_mblen_state\000"
.LC69:
	.ascii	"_file\000"
.LC81:
	.ascii	"_lock\000"
.LC183:
	.ascii	"_ZN3INT9sigKeyPadEv\000"
.LC174:
	.ascii	"sigTimer0\000"
.LC176:
	.ascii	"sigTimer1\000"
.LC178:
	.ascii	"sigTimer2\000"
.LC180:
	.ascii	"sigTimer3\000"
.LC157:
	.ascii	"ufp16\000"
.LC80:
	.ascii	"_data\000"
.LC98:
	.ascii	"_p5s\000"
.LC100:
	.ascii	"_cvtlen\000"
.LC79:
	.ascii	"_offset\000"
.LC148:
	.ascii	"vu16\000"
.LC48:
	.ascii	"__tm_mday\000"
.LC60:
	.ascii	"_fntypes\000"
.LC160:
	.ascii	"disable\000"
.LC90:
	.ascii	"_inc\000"
.LC57:
	.ascii	"_ind\000"
.LC134:
	.ascii	"_rand48\000"
.LC114:
	.ascii	"_getdate_err\000"
.LC175:
	.ascii	"_ZN3INT9sigTimer0Ev\000"
.LC94:
	.ascii	"__sdidinit\000"
.LC126:
	.ascii	"__sglue\000"
.LC73:
	.ascii	"_write\000"
.LC120:
	.ascii	"_nextf\000"
.LC41:
	.ascii	"_maxwds\000"
.LC196:
	.ascii	"SYS_SOUNDTIME\000"
.LC24:
	.ascii	"long int\000"
.LC111:
	.ascii	"_wctomb_state\000"
.LC193:
	.ascii	"SYS_PROFILETIMER\000"
.LC20:
	.ascii	"va_list\000"
.LC158:
	.ascii	"ufp32\000"
.LC104:
	.ascii	"_asctime_buf\000"
.LC181:
	.ascii	"_ZN3INT9sigTimer3Ev\000"
.LC133:
	.ascii	"_ZN5_glueaSERKS_\000"
.LC149:
	.ascii	"vu32\000"
.LC154:
	.ascii	"voidptr\000"
.LC38:
	.ascii	"__ULong\000"
.LC201:
	.ascii	"IntrTable\000"
	.ident	"GCC: (GNU) 3.3.2"
