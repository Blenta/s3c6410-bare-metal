/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
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

/*
 * To match the U-Boot user interface on ARM platforms to the U-Boot
 * standard (as on PPC platforms), some messages with debug character
 * are removed from the default U-Boot build.
 *
 * Define DEBUG here if you want additional info as shown below
 * printed upon startup:
 *
 * U-Boot code: 00F00000 -> 00F3C774  BSS: -> 00FC3274
 * IRQ Stack: 00ebff7c
 * FIQ Stack: 00ebef7c
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <version.h>
#include <asm/io.h>
#include <s3c6410.h>


#if defined(CONFIG_BOOT_MOVINAND)
#include <movi.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

const char version_string[] =
	U_BOOT_VERSION" (" __DATE__ " - " __TIME__ ")"CONFIG_IDENT_STRING;

/*
 * Begin and End of memory area for malloc(), and current "brk"
 */
static ulong mem_malloc_start = 0;
static ulong mem_malloc_end = 0;
static ulong mem_malloc_brk = 0;

static
void mem_malloc_init (ulong dest_addr)
{
	mem_malloc_start = dest_addr;
	mem_malloc_end = dest_addr + CFG_MALLOC_LEN;
	mem_malloc_brk = mem_malloc_start;

	/* memset ((void *) mem_malloc_start, 0,
			mem_malloc_end - mem_malloc_start); */
}

void *sbrk (ptrdiff_t increment)
{
	ulong old = mem_malloc_brk;
	ulong new = old + increment;

	if ((new < mem_malloc_start) || (new > mem_malloc_end)) {
		return (NULL);
	}
	mem_malloc_brk = new;

	return ((void *) old);
}

static int display_banner (void)
{
	printf ("\n\n%s\n\n", version_string);
	printf ("U-Boot code: %08lX -> %08lX  BSS: -> %08lX\n",
	       _armboot_start, _bss_start, _bss_end);
	return (0);
}

void start_armboot (void)
{
	display_banner();
	print_cpuinfo();
	checkboard();

	/* armboot_start is defined in the board-specific linker script */
#ifdef CONFIG_MEMORY_UPPER_CODE /* by scsuh */
	mem_malloc_init (CFG_UBOOT_BASE + CFG_UBOOT_SIZE - CFG_MALLOC_LEN - CFG_STACK_SIZE);
#else
	mem_malloc_init (_armboot_start - CFG_MALLOC_LEN);
#endif

	interrupt_init();

#if defined(CONFIG_NAND)

	puts ("NAND:    ");
	nand_init();		/* go init the NAND */

#endif

#if defined(CONFIG_BOOT_MOVINAND)

	puts ("SD/MMC:  ");
	movi_set_capacity();
	movi_set_ofs(MOVI_TOTAL_BLKCNT);
	movi_init();

#endif

	/* Pointer is writable since we allocated a register for it */
#ifdef CONFIG_MEMORY_UPPER_CODE /* by scsuh */
	ulong gd_base;

	gd_base = CFG_UBOOT_BASE + CFG_UBOOT_SIZE - CFG_MALLOC_LEN - CFG_STACK_SIZE - sizeof(gd_t);
	gd = (gd_t*)gd_base;
#else
	gd = (gd_t*)(_armboot_start - CFG_MALLOC_LEN - sizeof(gd_t));
#endif

	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");

	memset ((void*)gd, 0, sizeof (gd_t));
	gd->bd = (bd_t*)((char*)gd - sizeof(bd_t));
	memset (gd->bd, 0, sizeof (bd_t));


	puts ("Net:     ");

	/* IP Address */
	gd->bd->bi_ip_addr = getenv_IPaddr ((char *)MK_STR(CONFIG_IPADDR));

	/* MAC Address */
	{
		ulong reg;
		char *s, *e;
		char tmp[64];

		s = MK_STR(CONFIG_ETHADDR);

		for (reg = 0; reg < 6; ++reg) {
			gd->bd->bi_enetaddr[reg] = s ? simple_strtoul (s, &e, 16) : 0;
			if (s)
				s = (*e) ? e + 1 : e;
		}
	}

	load_addr = CFG_KERNEL_LOAD_ADDR;
	copy_filename (BootFile, "zImage", sizeof (BootFile));

	dm9000_pre_init();
	eth_initialize(gd->bd);

	/* main_loop() can return to retry autoboot, if so just run it again. */
	for (;;) {
		main_loop ();
	}

	/* NOTREACHED - no way out of command loop except booting */
}
