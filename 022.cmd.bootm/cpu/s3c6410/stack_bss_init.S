
#include <configs/smdk6410.h>

/* Set up the stack	and clear bss */
.globl stack_setup
stack_setup:

#ifdef CONFIG_MEMORY_UPPER_CODE
	ldr	sp, =(CFG_UBOOT_BASE + CFG_UBOOT_SIZE - 0xc)
#else
	ldr	r0, _TEXT_BASE                  /* upper 128 KiB: relocated uboot   */
	sub	r0, r0, #CFG_MALLOC_LEN         /* malloc area                      */
	sub	r0, r0, #CFG_GBL_DATA_SIZE      /* bdinfo                        */
	sub	sp, r0, #12                     /* leave 3 words for abort-stack    */

#endif

clear_bss:
	ldr	r0, _bss_start		/* find start of bss segment        */
	ldr	r1, _bss_end		/* stop here                        */
	mov r2, #0x00000000		/* clear                            */

clbss_l:
	str	r2, [r0]            /* clear loop...                    */
	add	r0, r0, #4
	cmp	r0, r1
	ble	clbss_l

	mov	pc, lr

_TEXT_BASE:
	.word	TEXT_BASE

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word _end

