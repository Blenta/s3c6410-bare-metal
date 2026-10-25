/*
 * Memory Setup stuff - taken from blob memsetup.S
 *
 * Copyright (C) 1999 2000 2001 Erik Mouw (J.A.K.Mouw@its.tudelft.nl) and
 *                     Jan-Derk Bakker (J.D.Bakker@its.tudelft.nl)
 *
 * Modified for the Samsung SMDK2410 by
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#include <configs/smdk6410.h>
#include <s3c6410.h>

_TEXT_BASE:
	.word	TEXT_BASE

	.globl lowlevel_init
lowlevel_init:
	mov	r12, lr

	ldr	r0, =ELFIN_GPIO_BASE
	ldr     r1, =0x55555555	
	str     r1, [r0, #GPKCON0_OFFSET]

	ldr     r1, =0x55555555
	str     r1, [r0, #GPKCON1_OFFSET]

	ldr     r1, =0x22222666
	str     r1, [r0, #GPLCON0_OFFSET]
	
	ldr     r1, =0x04000000
	str     r1, [r0, #GPFCON_OFFSET]
        ldr     r1, =0x2000
	str     r1, [r0, #GPFDAT_OFFSET]
	


	/* LED on only #8 */
	ldr	r0, =ELFIN_GPIO_BASE  
	ldr	r1, =0x00111111
	str	r1, [r0, #GPMCON_OFFSET]

	ldr	r1, =0x00000555
	str	r1, [r0, #GPMPUD_OFFSET]

	ldr	r1, =0x002a
	str	r1, [r0, #GPMDAT_OFFSET]



	ldr	r1, =0  /*0x55555555 phantom*/
	str	r1, [r0, #MEM1DRVCON_OFFSET]

	/* init system clock */
	bl system_clock_init

	/* for UART */
	bl uart_asm_init


ldr	r0, =ELFIN_UART_BASE
ldr	r1, =0x4b4b4b4b
str	r1, [r0, #UTXH_OFFSET]


#if defined(CONFIG_NAND)
    /* simple init for NAND */
	bl nand_asm_init
#endif

	bl	mem_ctrl_asm_init

#if 1
        ldr     r0, =(ELFIN_CLOCK_POWER_BASE+RST_STAT_OFFSET)
        ldr     r1, [r0]
        bic     r1, r1, #0xfffffff7
        cmp     r1, #0x8
        beq     wakeup_reset

#endif

1:
	ldr	r0, =ELFIN_UART_BASE
	ldr	r1, =0x4b4b4b4b
	str	r1, [r0, #UTXH_OFFSET]

	mov	lr, r12
	mov	pc, lr
#if 1
wakeup_reset:

	/*Clear wakeup status register*/
	ldr	r0, =(ELFIN_CLOCK_POWER_BASE+WAKEUP_STAT_OFFSET)
	ldr	r1, [r0]
	str	r1, [r0]

        /*LED test*/
        ldr     r0, =ELFIN_GPIO_BASE
        ldr     r1, =0x3000
        str     r1, [r0, #GPNDAT_OFFSET]

	/*Load return address and jump to kernel*/
	ldr	r0, =(ELFIN_CLOCK_POWER_BASE+INF_REG0_OFFSET)
	ldr	r1, [r0]	/* r1 = physical address of s3c6400_cpu_resume function*/
	mov	pc, r1		/*Jump to kernel (sleep-s3c6400.S)*/
	nop
	nop
#endif

