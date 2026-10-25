
#include <s3c6410.h>

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
	 * set the cpu to SVC32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3
	msr	cpsr,r0

	/*
	 * flush v4 I/D caches
	 */
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache */
	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	@ clear bits 13, 9:8 (--V- --RS)
	bic	r0, r0, #0x00000087	@ clear bits 7, 2:0 (B--- -CAM)
	orr	r0, r0, #0x00000002	@ set bit 2 (A) Align
	orr	r0, r0, #0x00001000	@ set bit 12 (I) I-Cache
	mcr	p15, 0, r0, c1, c0, 0

	/* 
     * Peripheral Port Setup 
     */
	ldr r0, =0x70000000		@ Base Address
	orr r0, r0, #0x13		@ Memory Size b10011 = 256MB (0x70000000-0x7fffffff)
	mcr p15,0,r0,c15,c2,4		@ Write Peripheral Port Memory Remap Register

	/* 
     * Disable Watchdog
     */
	ldr	r0, =0x7e000000		@0x7e004000
	orr	r0, r0, #0x4000
	mov	r1, #0
	str	r1, [r0]

	/* 
	 * External Interrupt 0 Pending Register clear
	 * 0 = Not occur 1= Occur interrupt 
	 * Each bit is cleared by writing “1”. 
	 */
	ldr	r0, =(ELFIN_GPIO_BASE+EINTPEND_OFFSET)	/*EINTPEND*/
	ldr	r1, [r0]
	str	r1, [r0]

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

