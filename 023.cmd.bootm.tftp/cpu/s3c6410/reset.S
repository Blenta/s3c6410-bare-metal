
#include <configs/smdk6410.h>

/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *
 *************************************************************************
 */

/*
 * the actual reset code
 */
.globl reset
reset:

	bl cpu_init 

	/*
	 * Go setup Memory and board specific bits prior to relocation.
	 */
	bl	lowlevel_init	/* go setup pll,mux,memory */

	/* when we already run in ram, we don't need to relocate U-Boot.
	 * and actually, memory controller must be configured before U-Boot
	 * is running in ram.
	 */
	
	ldr	r0, =0xff000fff

	bic	r1, pc, r0		/* r0 <- current base addr of code */
	ldr	r2, _TEXT_BASE		/* r1 <- original base addr in ram */
	bic	r2, r2, r0		/* r0 <- current base addr of code */
	cmp     r1, r2                  /* compare r0, r1                  */
	beq     after_copy		/* r0 == r1 then skip flash copy   */


#ifdef CONFIG_BOOT_NAND
	mov	r0, #0x1000
	bl	copy_from_nand
#endif

#ifdef CONFIG_BOOT_MOVINAND
	ldr	sp, _TEXT_PHY_BASE
	bl	movi_bl2_copy
	b	after_copy
#endif

after_copy:

	bl enable_mmu

skip_hw_init:

	bl stack_setup

	ldr	pc, _start_armboot

_start_armboot:
	.word start_armboot

_TEXT_BASE:
	.word	TEXT_BASE

_TEXT_PHY_BASE:
	.word	CFG_PHY_UBOOT_BASE
