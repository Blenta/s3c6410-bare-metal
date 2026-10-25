
.globl _start
_start:

	bl cpu_init
	
	bl led_flow

	/* init system clock */
	bl system_clock_init
	
	bl led_blink
	bl led_flow
	bl led_on

halt:
	b halt
