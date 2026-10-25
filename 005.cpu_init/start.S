
.globl _start
_start:

	bl cpu_init
	
	bl led_flow
	bl led_blink
	bl led_on

halt:
	b halt
