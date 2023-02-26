	.text
	.align 2
	.code 32
	.arm

	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@ Check TIM2CNT_L and reset it if greater than r0
@@ argument r0 = timerTarget	
@@ return nonzero if timer was reset
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.globl CheckBPMClock
CheckBPMClock:
	ldr r1, =0x04000108					@ Load TIM2CNT_L value
	ldrh r1, [r1]						@

	ldr r2, =0x04000108					@ Load a copy of TIM2CNT_L value
	ldrh r2, [r2]						@
	
	cmp r0, r1							@ Is timerTarget < current timer value?
	bgt CBPM_10							@ then jump to CBPM_10
	
	mov r0, #1							@ return 1
	bx lr								@
	
CBPM_10:
	@@ Nothing to do, return zero
	mov r0, #0
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.end
