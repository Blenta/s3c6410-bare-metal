/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * Copyright (C) 2001  Erik Mouw (J.A.K.Mouw@its.tudelft.nl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307	 USA
 *
 */

#include <configs/smdk6410.h>
#include <linux/types.h>

#include <asm/setup.h>          /* ATAG_CORE tag_size */
#include <asm/u-boot.h>         /* bd_t */
#include <asm/global_data.h>	/* DECLARE_GLOBAL_DATA_PTR */

#include <asm/byteorder.h>

DECLARE_GLOBAL_DATA_PTR;

#if defined (CONFIG_SETUP_MEMORY_TAGS) || defined (CONFIG_CMDLINE_TAG)
static void setup_start_tag (bd_t *bd);

# ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags (bd_t *bd);
# endif
static void setup_commandline_tag (bd_t *bd, char *commandline);

static void setup_end_tag (bd_t *bd);

static struct tag *params;
#endif /* CONFIG_SETUP_MEMORY_TAGS || CONFIG_CMDLINE_TAG */

void do_bootm_linux (void)
{
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

	/* board_init */
	gd->bd->bi_arch_number = MACH_TYPE;
	gd->bd->bi_boot_params = (PHYS_SDRAM_1+0x100);

	/* dram_init */
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	bd_t *bd = gd->bd;

#ifdef CONFIG_CMDLINE_TAG
	char *commandline = "root=/dev/mtdblock2 rootfstype=yaffs2 init=/linuxrc console=ttySAC0,115200";
#endif

	void (*theKernel)(int zero, int arch, uint params);

	theKernel = (void (*)(int, int, uint))KERNEL_ENTRY_POINT_ADDRESS;

	printf ("## Transferring control to Linux (at address %08lx) ...\n", (ulong) theKernel);

#if defined (CONFIG_SETUP_MEMORY_TAGS) || defined (CONFIG_CMDLINE_TAG)
	setup_start_tag (bd);
#ifdef CONFIG_SETUP_MEMORY_TAGS
	setup_memory_tags (bd);
#endif
#ifdef CONFIG_CMDLINE_TAG
	setup_commandline_tag (bd, commandline);
#endif
	setup_end_tag (bd);
#endif

	/* we assume that the kernel is in place */
	printf ("\nStarting kernel ...\n\n");

	cleanup_before_linux ();

	theKernel (0, bd->bi_arch_number, bd->bi_boot_params);
}


#if defined (CONFIG_SETUP_MEMORY_TAGS) || defined (CONFIG_CMDLINE_TAG)
static void setup_start_tag (bd_t *bd)
{
	params = (struct tag *) bd->bi_boot_params;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}


#ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags (bd_t *bd)
{
	int i;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		params->hdr.tag = ATAG_MEM;
		params->hdr.size = tag_size (tag_mem32);

		params->u.mem.start = bd->bi_dram[i].start;
		params->u.mem.size = bd->bi_dram[i].size;

		params = tag_next (params);
	}
}
#endif /* CONFIG_SETUP_MEMORY_TAGS */


static void setup_commandline_tag (bd_t *bd, char *commandline)
{
	char *p;

	if (!commandline)
		return;

	/* eat leading white space */
	for (p = commandline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy (params->u.cmdline.cmdline, p);

	params = tag_next (params);
}


static void setup_end_tag (bd_t *bd)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}

#endif /* CONFIG_SETUP_MEMORY_TAGS || CONFIG_CMDLINE_TAG */
