
@ #include <s3c6410.h>

/*
 *************************************************************************
 *
 * CPU_init_critical registers
 *
 * setup important registers
 * setup memory timing
 *
 *************************************************************************
 */

.globl cpu_init
cpu_init:

	/*
	 * Set the cpu to SVC32 mode
	 */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr, r0

	/*
	 * Flush v4 I/D caches
	 */
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache */
	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	/*
	 * Disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	@ clear bits 13, 9:8 (--V- --RS)
	bic	r0, r0, #0x00000085	@ clear bits 7, 2, 0 (B--- -C-M)
	orr	r0, r0, #0x00000002	@ set bit 1 (A) Align
	orr	r0, r0, #0x00001000	@ set bit 12 (I) I-Cache
	mcr	p15, 0, r0, c1, c0, 0

	/* 
     * Peripheral Port Setup 
     */
	ldr r0, =0x70000000		@ Base Address
	orr r0, r0, #0x13		@ Memory Size b10011 = 256MB (0x70000000-0x7fffffff)
	mcr p15,0,r0,c15,c2,4		@ Write Peripheral Port Memory Remap Register

	/* 
     * Disable Watchdog timer 
     */
	ldr r0, =0x7E004000		@ WTCON - Watchdog timer control register 
	mov r1, #0			@ 0 - Disable
	str r1, [r0]

/*
 * GPIO
 */
#define ELFIN_GPIO_BASE		0x7f008000
#define EINT0PEND_OFFSET	0x924

	/* 
	 * External Interrupt 0 Pending Register clear
	 * 0 = Not occur 1= Occur interrupt 
	 * Each bit is cleared by writing “1”. 
	 */
	ldr	r0, =(ELFIN_GPIO_BASE+EINT0PEND_OFFSET)	/*EINT0PEND*/
	ldr	r1, [r0]
	str	r1, [r0]

/*
 * Interrupt
 */
#define ELFIN_VIC0_BASE_ADDR		(0x71200000)
#define ELFIN_VIC1_BASE_ADDR		(0x71300000)
#define oINTMOD				(0x00C)		// Interrupt Select Register VIC INT SELECT (IRQ or FIQ)
#define oINTUNMSK			(0x010)		// Interrupt Enable Register VIC INT EN (Unmask by writing 1)
#define oINTMSK				(0x014)		// Interrupt Enable Clear Register VIC INT EN CLEAR (Mask by writing 1)
#define oINTSUBMSK			(0x01C)		// Software Interrupt Clear Register VIC SOFT INT CLEAR
#define oVECTADDR			(0xF00)		// Vector Address Register VIC ADDRESS

	ldr	r0, =ELFIN_VIC0_BASE_ADDR 	@0x71200000
	ldr	r1, =ELFIN_VIC1_BASE_ADDR 	@0x71300000

	@ Disable all interrupts (VIC0 and VIC1)
	mvn	r3, #0x0
	str	r3, [r0, #oINTMSK]
	str	r3, [r1, #oINTMSK]

	@ Set all interrupts as IRQ
	mov	r3, #0x0
	str	r3, [r0, #oINTMOD]
	str	r3, [r1, #oINTMOD]

	@ Pending Interrupt Clear
	mov	r3, #0x0
	str	r3, [r0, #oVECTADDR]
	str	r3, [r1, #oVECTADDR]

    mov pc, lr
