
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
	
	bl led_flow
	bl led_blink
	bl led_on

halt:
	b halt
