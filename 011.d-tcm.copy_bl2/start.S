
#include <configs/smdk6410.h>

.globl _start
_start:

	bl cpu_init

	/* init system clock */
	bl system_clock_init

	@ bl led_flow

	/* init UART */
	bl uart_asm_init

	/* init Memory */
	bl mem_ctrl_asm_init
	
	/* copy BL2 to memory */
	bl movi_bl2_copy
	b  after_copy


halt:
	b halt



.globl after_copy
after_copy:

	bl led_blink
	bl enable_mmu
	bl led_on
	
	ldr	sp, =(CFG_UBOOT_BASE + CFG_UBOOT_SIZE - 0xc)
	ldr	pc, _start_uboot


_start_uboot:
	.word start_uboot

