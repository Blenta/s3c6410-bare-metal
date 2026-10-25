
#include <configs/smdk6410.h>
#include <s3c6410.h>

/*
 * uart_asm_init: Initialize UART in asm mode, 115200bps fixed.
 * void uart_asm_init(void)
 */
.globl uart_asm_init
uart_asm_init:
	/* set GPIO to enable UART */
	@ GPIO setting for UART
	ldr	r0, =ELFIN_GPIO_BASE
	ldr	r1, =0x220022
	str   	r1, [r0, #GPACON_OFFSET]

	ldr	r1, =0x2222
	str   	r1, [r0, #GPBCON_OFFSET]

	ldr	r0, =ELFIN_UART_CONSOLE_BASE		@0x7F005000
	mov	r1, #0x0
	str	r1, [r0, #UFCON_OFFSET]
	str	r1, [r0, #UMCON_OFFSET]

	mov	r1, #0x3                	@was 0.
	str	r1, [r0, #ULCON_OFFSET]

#if defined(CONFIG_CLKSRC_CLKUART)
	ldr	r1, =0xe45			/* UARTCLK SRC = 11 => EXT_UCLK1*/
#else
	ldr	r1, =0x245			/* UARTCLK SRC = x0 => PCLK */
#endif

	str	r1, [r0, #UCON_OFFSET]

#if defined(CONFIG_UART_50)
	ldr	r1, =0x1A
#elif defined(CONFIG_UART_66)
	ldr	r1, =0x22
#else
	ldr	r1, =0x1A
#endif
	str	r1, [r0, #UBRDIV_OFFSET]

#if defined(CONFIG_UART_50)
	ldr	r1, =0x3
#elif defined(CONFIG_UART_66)
	ldr	r1, =0x1FFF
#else
	ldr	r1, =0x3
#endif
	str	r1, [r0, #UDIVSLOT_OFFSET]

	ldr	r1, =0x4f4f4f4f
	str	r1, [r0, #UTXH_OFFSET]		@'O'

	mov	pc, lr
