
#include <configs/smdk6410.h>

.globl _start
_start:

	bl cpu_init
	
	//bl led_flow

	/* init system clock */
	bl system_clock_init

	bl led_blink

	/* init UART */
	bl uart_asm_init

	/* init Memory */
	bl mem_ctrl_asm_init

	bl led_on

	ldr sp, _TEXT_SS_BASE	/* setup temp stack pointer */
	bl start_uboot

halt:
	b halt

_TEXT_SS_BASE:
	.word	CFG_PHY_STACK_BASE