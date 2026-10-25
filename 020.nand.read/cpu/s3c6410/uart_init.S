
#include <configs/smdk6410.h>

/*
 * uart_asm_init: Initialize UART in asm mode, 115200bps fixed.
 * void uart_asm_init(void)
 */
.globl uart_asm_init
uart_asm_init:

/*
 * GPIO
 */
#define ELFIN_GPIO_BASE		0x7f008000
#define GPACON_OFFSET		0x00
#define GPBCON_OFFSET		0x20


	/* set GPIO to enable UART */
	@ GPIO setting for UART
	ldr	r0, =ELFIN_GPIO_BASE
	ldr	r1, =0x220022   /*  UART 0, 1  */
	str   	r1, [r0, #GPACON_OFFSET]

	ldr	r1, =0x2222     /*  UART 2, 3  */
	str   	r1, [r0, #GPBCON_OFFSET]

/*
 * UART
 */
#define ELFIN_UART_BASE		0x7F005000

#define ELFIN_UART0_OFFSET	0x0000
#define ELFIN_UART1_OFFSET	0x0400
#define ELFIN_UART2_OFFSET	0x0800
#define ELFIN_UART3_OFFSET	0x0c00

#ifdef CONFIG_SERIAL1
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + ELFIN_UART0_OFFSET)
#elif defined(CONFIG_SERIAL2)
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + ELFIN_UART1_OFFSET)
#elif defined(CONFIG_SERIAL3)
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + ELFIN_UART2_OFFSET)
#elif defined(CONFIG_SERIAL4)
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + ELFIN_UART3_OFFSET)
#else
#define ELFIN_UART_CONSOLE_BASE (ELFIN_UART_BASE + ELFIN_UART0_OFFSET)
#endif

#define ULCON_OFFSET		0x00
#define UCON_OFFSET		    0x04
#define UFCON_OFFSET		0x08
#define UMCON_OFFSET		0x0C
#define UTRSTAT_OFFSET		0x10
#define UTXH_OFFSET	        0x20
//#define URXH_OFFSET       0x24
#define UBRDIV_OFFSET		0x28
#define UDIVSLOT_OFFSET		0x2C

	ldr	r0, =ELFIN_UART_CONSOLE_BASE		@0x7F005000
	mov	r1, #0x0
	str	r1, [r0, #UFCON_OFFSET]     /* Disable FIFO */
	str	r1, [r0, #UMCON_OFFSET]     /* Disable Modem */

	mov	r1, #0x3                	@was 0. /* b11 = 8-bits  */
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

	ldr	r1, =0x4f
	str	r1, [r0, #UTXH_OFFSET]		@'O'

 1:
 	ldr     r1, [r0, #UTRSTAT_OFFSET]
 	cmp     r1, #0x2
 	bne     1b
	
    ldr	r1, =0x4b
    str	r1, [r0, #UTXH_OFFSET]      @'K'

	mov	pc, lr
