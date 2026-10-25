
#include <configs/smdk6410.h>

.globl enable_mmu
enable_mmu:
	/* enable domain access */
	ldr	r5, =0x0000ffff
	mcr	p15, 0, r5, c3, c0, 0		@ load domain access register

	/* Set the TTB register */
	ldr	r0, _mmu_table_base
	ldr	r1, =CFG_PHY_UBOOT_BASE
	ldr	r2, =0xfff00000
	bic	r0, r0, r2
	orr	r1, r0, r1
	mcr	p15, 0, r1, c2, c0, 0

	/* Enable the MMU */
mmu_on:
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #1			/* Set CR_M to enable MMU */
	mcr	p15, 0, r0, c1, c0, 0
	nop
	nop
	nop
	nop

    mov pc, lr

_mmu_table_base:
	.word mmu_table

/*
 * MMU Table for SMDK6400
 */

	/* form a first-level section entry */
.macro FL_SECTION_ENTRY base,ap,d,c,b
	.word (\base << 20) | (\ap << 10) | \
	      (\d << 5) | (1<<4) | (\c << 3) | (\b << 2) | (1<<1)
.endm
.section .mmudata, "a"
	.align 14
	// the following alignment creates the mmu table at address 0x4000.
	.globl mmu_table
mmu_table:
	.set __base,0
	// 1:1 mapping for debugging
	.rept 0xA00
	FL_SECTION_ENTRY __base,3,0,0,0
	.set __base,__base+1
	.endr

	// access is not allowed.
	.rept 0xC00 - 0xA00
	.word 0x00000000
	.endr


    // 128MB or 256MB for SDRAM 0xC0000000 -> 0x50000000
	.set __base, 0x500
    #ifdef FORLINX_BOOT_RAM128
        .rept 0xC80 - 0xC00
    #else
        .rept 0xD00 - 0xC00
    #endif
	FL_SECTION_ENTRY __base,3,0,1,1
	.set __base,__base+1
	.endr

        // access is not allowed.
    #ifdef FORLINX_BOOT_RAM128
        .rept 0x1000 - 0xC80
    #else
        .rept 0x1000 - 0xD00
    #endif

	.word 0x00000000
	.endr

