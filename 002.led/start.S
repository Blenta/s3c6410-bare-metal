
.globl _start
_start:

	/* Peripheral Port Setup */
	ldr r0, =0x70000000		@ Base Address
	orr r0, r0, #0x13		@ Memory Size b10011 = 256MB (0x70000000-0x7fffffff)
	mcr p15,0,r0,c15,c2,4		@ Write Peripheral Port Memory Remap Register
	
	/* Disable Watchdog timer */
	ldr r0, =0x7E004000		@ WTCON - Watchdog timer control register 
	mov r1, #0			@ 0 - Disable
	str r1, [r0]
	


	/* Port M Control Register */
	/* GPM 0-3 -> LED 1-4 */

	ldr r1, =0x7F008820		@ GPMCON - Port M Configuration Register
	ldr r0, =0x1111			@ [3:0] - GPM0, [7:4] - GPM1, [11:8] - GPM2, [15:12] - GPM3, b0001 = Output 
	str r0, [r1]

	ldr r1, =0x7F008824		@ GPMDAT - Port M Data Registe
	mov r0, #7			@ [3:0] - GPM[3:0], 0 - led on / 1 - led off, 7(b0111) = LED4 on


loop:
	str r0, [r1]
	ror r0, r0, #1
	ldr r2, =0x70000000	
	cmp r0, r2 
	moveq r0, #0x70
	bl delay
	b loop

delay:
	mov r2, #0x500000
delay_loop:
	sub r2, r2, #1
	cmp r2, #0
	bne delay_loop		
	mov pc, lr


