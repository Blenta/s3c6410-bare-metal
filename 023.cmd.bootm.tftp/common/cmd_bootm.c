/*
 * (C) Copyright 2000-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
 * Boot support
 */
#include <common.h>
#include <command.h>
#include <malloc.h>

extern void do_bootm_linux (void);

ulong load_addr = CFG_LOAD_ADDR;		/* Default Load Address */

int do_bootm (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	printf ("## Booting image at %08lx ...\n", KERNEL_ENTRY_POINT_ADDRESS);

	do_bootm_linux ();

	return 1;
}

U_BOOT_CMD(
 	bootm,	CFG_MAXARGS,	1,	do_bootm,
 	"bootm   - boot application image from memory\n",
 	"[addr [arg ...]]\n    - boot application image stored in memory\n"
 	"\tpassing arguments 'arg ...'; when booting a Linux kernel,\n"
 	"\t'arg' can be the address of an initrd image\n"
);


