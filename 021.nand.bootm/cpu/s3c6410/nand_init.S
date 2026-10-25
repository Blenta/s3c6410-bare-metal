
#include <configs/smdk6410.h>
#include <s3c6410.h>

/*
 * Nand Interface Init for SMDK6410 
 */
.globl nand_asm_init
nand_asm_init:
	ldr	r0, =ELFIN_NAND_BASE
	ldr	r1, [r0, #NFCONF_OFFSET]
	orr	r1, r1, #0x70
	orr	r1, r1, #0x7700
	str     r1, [r0, #NFCONF_OFFSET]

	ldr	r1, [r0, #NFCONT_OFFSET]
	orr	r1, r1, #0x03
	str     r1, [r0, #NFCONT_OFFSET]

	mov	pc, lr


/*
 * copy U-Boot to SDRAM and jump to ram (from NAND or OneNAND)
 * r0: size to be compared
 * Load 1'st 2blocks to RAM because U-boot's size is larger than 1block(128k) size
 */
	.globl copy_from_nand
copy_from_nand:
	mov	r10, lr		/* save return address */

	mov	r9, r0
	/* get ready to call C functions */
	ldr	sp, _TEXT_PHY_BASE	/* setup temp stack pointer */
	sub	sp, sp, #12
	mov	fp, #0			/* no previous frame, so fp=0 */
	mov	r9, #0x1000
	bl	copy_uboot_to_ram

3:	tst 	r0, #0x0
	bne	copy_failed

	ldr	r0, =0x0c000000
	ldr	r1, _TEXT_PHY_BASE
1:	ldr	r3, [r0], #4
	ldr	r4, [r1], #4
	teq	r3, r4
	bne	compare_failed	/* not matched */
	subs	r9, r9, #4
	bne	1b

4:	mov	lr, r10		/* all is OK */
	mov	pc, lr

copy_failed:
	nop			/* copy from nand failed */
	b	copy_failed

compare_failed:
	nop			/* compare failed */
	b	compare_failed

_TEXT_PHY_BASE:
	.word	CFG_PHY_UBOOT_BASE
