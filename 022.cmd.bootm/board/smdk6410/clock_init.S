
#include <configs/smdk6410.h>
#include <s3c6410.h>

/*
 * system_clock_init: Initialize core clock and bus clock.
 * void system_clock_init(void)
 */
.globl system_clock_init
system_clock_init:

	ldr	r0, =ELFIN_CLOCK_POWER_BASE	@0x7e00f000

#ifdef	CONFIG_SYNC_MODE
	ldr	r1, [r0, #OTHERS_OFFSET]
	mov	r2, #0x40
	orr	r1, r1, r2
	str	r1, [r0, #OTHERS_OFFSET]

	nop
	nop
	nop
	nop
	nop

	ldr	r2, =0x80
	orr	r1, r1, r2
	str	r1, [r0, #OTHERS_OFFSET]

check_syncack:
	ldr	r1, [r0, #OTHERS_OFFSET]
	ldr	r2, =0xf00
	and	r1, r1, r2
	cmp	r1, #0xf00
	bne	check_syncack
#else	/* ASYNC Mode */
	nop
	nop
	nop
	nop
	nop

	ldr	r1, [r0, #OTHERS_OFFSET]
	bic	r1, r1, #0xC0
	orr	r1, r1, #0x40
	str	r1, [r0, #OTHERS_OFFSET]

wait_for_async:
	ldr	r1, [r0, #OTHERS_OFFSET]
	and	r1, r1, #0xf00
	cmp	r1, #0x0
	bne	wait_for_async

	ldr	r1, [r0, #OTHERS_OFFSET]
	bic	r1, r1, #0x40
	str	r1, [r0, #OTHERS_OFFSET]
#endif

	mov	r1, #0xff00
	orr	r1, r1, #0xff
	str	r1, [r0, #APLL_LOCK_OFFSET]
	str	r1, [r0, #MPLL_LOCK_OFFSET]
	str	r1, [r0, #EPLL_LOCK_OFFSET]
/* CLKUART(=66.5Mhz) = CLKUART_input(532/2=266Mhz) / (UART_RATIO(3)+1) */
/* CLKUART(=50Mhz) = CLKUART_input(400/2=200Mhz) / (UART_RATIO(3)+1) */
/* Now, When you use UART CLK SRC by EXT_UCLK1, We support 532MHz & 400MHz value */

#if defined(CONFIG_CLKSRC_CLKUART)
	ldr   	r1, [r0, #CLK_DIV2_OFFSET]
	bic	r1, r1, #0x70000
	orr	r1, r1, #0x30000
	str	r1, [r0, #CLK_DIV2_OFFSET]
#endif


	ldr   	r1, [r0, #CLK_DIV0_OFFSET]	/*Set Clock Divider*/
	bic	r1, r1, #0x30000
	bic	r1, r1, #0xff00
	bic	r1, r1, #0xff
	ldr	r2, =CLK_DIV_VAL
	orr	r1, r1, r2
	str	r1, [r0, #CLK_DIV0_OFFSET]

	ldr	r1, =APLL_VAL
	str	r1, [r0, #APLL_CON_OFFSET]
	ldr	r1, =MPLL_VAL
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr	r1, =0x80200203			/* FOUT of EPLL is 96MHz */
	str	r1, [r0, #EPLL_CON0_OFFSET]
	ldr	r1, =0x0
	str	r1, [r0, #EPLL_CON1_OFFSET]

	ldr	r1, [r0, #CLK_SRC_OFFSET]	/* APLL, MPLL, EPLL select to Fout */

#if defined(CONFIG_CLKSRC_CLKUART)
	ldr	r2, =0x2007
#else
	ldr	r2, =0x7
#endif
	orr	r1, r1, r2

	str	r1, [r0, #CLK_SRC_OFFSET]

	/* wait at least 200us to stablize all clock */
	mov	r1, #0x10000
1:	subs	r1, r1, #1
	bne	1b
#if 0
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #0xc0000000	/* clock setting in MMU */
	mcr	p15, 0, r0, c1, c0, 0
#endif

#ifdef CONFIG_SYNC_MODE				/* Synchronization for VIC port */
	ldr	r1, [r0, #OTHERS_OFFSET]
	orr	r1, r1, #0x20
	str	r1, [r0, #OTHERS_OFFSET]
#else
	ldr	r1, [r0, #OTHERS_OFFSET]
	bic	r1, r1, #0x20
	str	r1, [r0, #OTHERS_OFFSET]
#endif
	mov	pc, lr
