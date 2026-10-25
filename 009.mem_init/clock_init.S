
#include <configs/smdk6410.h>
@ #include <s3c6410.h>

#define ELFIN_CLOCK_POWER_BASE	0x7e00f000

#define APLL_LOCK_OFFSET	0x00
#define MPLL_LOCK_OFFSET	0x04
#define EPLL_LOCK_OFFSET	0x08
#define APLL_CON_OFFSET		0x0C
#define MPLL_CON_OFFSET		0x10
#define EPLL_CON0_OFFSET	0x14
#define EPLL_CON1_OFFSET	0x18
#define CLK_SRC_OFFSET		0x1C
#define CLK_DIV0_OFFSET		0x20
#define CLK_DIV2_OFFSET		0x28
#define OTHERS_OFFSET		0x900

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
	orr	r1, r1, #0xff /* 0xFFFF = enable APLL MPLL EPLL */
	str	r1, [r0, #APLL_LOCK_OFFSET]
	str	r1, [r0, #MPLL_LOCK_OFFSET]
	str	r1, [r0, #EPLL_LOCK_OFFSET]
/* CLKUART(=66.5Mhz) = CLKUART_input(532/2=266Mhz) / (UART_RATIO(3)+1) */
/* CLKUART(=50Mhz) = CLKUART_input(400/2=200Mhz) / (UART_RATIO(3)+1) */
/* Now, When you use UART CLK SRC by EXT_UCLK1, We support 532MHz & 400MHz value */

#if defined(CONFIG_CLKSRC_CLKUART)
	ldr   	r1, [r0, #CLK_DIV2_OFFSET]
	bic	r1, r1, #0x70000
	orr	r1, r1, #0x30000	/* UART_RATIO [19:16] = 0x03 */
	str	r1, [r0, #CLK_DIV2_OFFSET]
#endif


	ldr   	r1, [r0, #CLK_DIV0_OFFSET]	/*Set Clock Divider*/
	bic	r1, r1, #0x30000
	bic	r1, r1, #0xff00
	bic	r1, r1, #0xff
	ldr	r2, =CLK_DIV_VAL
	orr	r1, r1, r2
	str	r1, [r0, #CLK_DIV0_OFFSET]	/* MPLL_RATIO[4]=1, HCLK_RATIO[8]=1, HCLKX2_RATIO[11:9]=1, PCLK_RATIO[15:12]=3, SECUR_RATIO[19:18]=1, JPEG_RATIO[27:24]=1 */

	ldr	r1, =APLL_VAL				/* MDIV = 266, PDIV = 3, SDIV = 1 */
	str	r1, [r0, #APLL_CON_OFFSET]
	ldr	r1, =MPLL_VAL				/* MDIV = 266, PDIV = 3, SDIV = 1 */
	str	r1, [r0, #MPLL_CON_OFFSET]

	/* FOUT of EPLL is 0x102 = 96MHz, 0x203 = 24MHz */
	ldr	r1, =0x80200102				/* [31]= PLL enable control, MDIV[23:16]=0x20, PDIV[13:8]=0x0001, SDIV[2:0]=0x010 */
	str	r1, [r0, #EPLL_CON0_OFFSET]
	ldr	r1, =0x0					/* KDIV [15:0] = 0 */
	str	r1, [r0, #EPLL_CON1_OFFSET]

	ldr	r1, [r0, #CLK_SRC_OFFSET]	/* APLL, MPLL, EPLL select to Fout */

#if defined(CONFIG_CLKSRC_CLKUART)
	ldr	r2, =0x2007					/* UART_SEL[13] EPLL_SEL[2] MPLL_SEL[1] APLL_SEL[0] */
#else
	ldr	r2, =0x7
#endif
	orr	r1, r1, r2					/* UART_SEL = 1:DOUTMPLL, EPLL_SEL = 1:FOUTEPLL, MPLL_SEL = 1:FOUTMPLL, APLL_SEL = 1:FOUTAPLL*/

	str	r1, [r0, #CLK_SRC_OFFSET]

	/* wait at least 200us to stablize all clock */
	mov	r1, #0x10000
1:	subs	r1, r1, #1
	bne	1b

#ifdef CONFIG_SYNC_MODE				/* Synchronization for VIC port */
	ldr	r1, [r0, #OTHERS_OFFSET]
	orr	r1, r1, #0x20
	str	r1, [r0, #OTHERS_OFFSET]
#else
	ldr	r1, [r0, #OTHERS_OFFSET]
	bic	r1, r1, #0x20
	str	r1, [r0, #OTHERS_OFFSET]
#endif

	mov pc, lr

