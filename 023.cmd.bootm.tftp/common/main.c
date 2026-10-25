/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Add to readline cmdline-editing by
 * (C) Copyright 2005
 * JinHua Luo, GuangDong Linux Center, <luo.jinhua@gd-linux.com>
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

#include <common.h>
#include <watchdog.h>
#include <command.h>

#include <hush.h>

#define MAX_DELAY_STOP_STR 32

#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
static int abortboot(int);
#endif

#undef DEBUG_PARSER

char        console_buffer[CFG_CBSIZE];		/* console I/O buffer	*/

#ifndef CONFIG_CMDLINE_EDITING
static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen);
static char erase_seq[] = "\b \b";		/* erase sequence	*/
static char   tab_seq[] = "        ";		/* used to expand TABs	*/
#endif /* CONFIG_CMDLINE_EDITING */

#define	endtick(seconds) (get_ticks() + (uint64_t)(seconds) * get_tbclk())


/***************************************************************************
 * Watch for 'delay' seconds for autoboot stop or autoboot delay string.
 * returns: 0 -  no key string, allow autoboot
 *          1 - got key string, abort
 */
#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)

static __inline__ int abortboot(int bootdelay)
{
	int abort = 0;

	printf("Hit any key to stop autoboot: %2d ", bootdelay);

#if defined CONFIG_ZERO_BOOTDELAY_CHECK
	/*
	 * Check if key already pressed
	 * Don't check if bootdelay < 0
	 */
	if (bootdelay >= 0) {
		if (tstc()) {	/* we got a key press	*/
			(void) getc();  /* consume input	*/
			puts ("\b\b\b 0");
			abort = 1; 	/* don't auto boot	*/
		}
	}
#endif

	while ((bootdelay > 0) && (!abort)) {
		int i;

		--bootdelay;
		/* delay 100 * 10ms */
		for (i=0; !abort && i<100; ++i) {
			if (tstc()) {	/* we got a key press	*/
				abort  = 1;	/* don't auto boot	*/
				bootdelay = 0;	/* no more delay	*/
				(void) getc();  /* consume input	*/
				break;
			}
			udelay (10000);
		}

		printf ("\b\b\b%2d ", bootdelay);
	}

	putc ('\n');

	return abort;
}
#endif	/* CONFIG_BOOTDELAY >= 0  */
static void ExecuteCmd(char *cmd)
{
	parse_string_outer(cmd, FLAG_PARSE_SEMICOLON | FLAG_EXIT_FROM_LOOP);
}

void arm_sdfuse(void)
{
	unsigned char select;
	while(1)
	{
		printf("\n##### flash from SDcard  #####\n");
		printf("\n##### Select the fuction #####\n");
		printf("[1] Flash all image\n");
		printf("[2] Flash u-boot\n");
		printf("[3] Flash kernel\n");
		printf("[4] Flash system\n");			
		printf("[5] Exit\n");
		printf("Enter your Selection:");
	
		select = getc();
		printf("%c\n", select >= ' ' && select <= 127 ? select : ' ');	
	
		switch(select) 
		{
			case '1':
				ExecuteCmd("nand erase");
				ExecuteCmd("fatload mmc 0:1 0x50008000 u-boot.bin");
				ExecuteCmd("nand write.uboot 0x50008000 0 0x200000");
				ExecuteCmd("fatload mmc 0:1 0x50008000 zImage");
				ExecuteCmd("nand write.e 0x50008000 0x500000 0x500000");
				ExecuteCmd("fatload mmc 0:1 0x50008000 rootfs.yaffs2");
				ExecuteCmd("nand write.yaffs2 0x50008000 0x1e00000 $filesize");
				break;
			
			case '2':
				ExecuteCmd("nand erase 0 0x200000");
				ExecuteCmd("fatload mmc 0:1 0x50008000 u-boot.bin");
				ExecuteCmd("nand write.uboot 0x50008000 0 0x200000");
				break;
					
			case '3':
				ExecuteCmd("nand erase 0x500000 0x1900000");
				ExecuteCmd("fatload mmc 0:1 0x50008000 zImage");
				ExecuteCmd("nand write.e 0x50008000 0x500000 0x500000");
				break;

			case '4':
				ExecuteCmd("nand write.e 0x50008000 0x1e00000 0x10");
				ExecuteCmd("nand erase 0x1e00000");
				ExecuteCmd("fatload mmc 0:1 0x50008000 rootfs.yaffs2");
				ExecuteCmd("nand write.yaffs2 0x50008000 0x1e00000 $filesize");
				break;
			case '5':
				return;
			default:
				break;
		}
	}
}

#ifdef CONFIG_BOOT_NAND
void NAND_ARMMenu(void)
{
	unsigned char select;
	char *command;	
	char cramfs_cmdline[256]="setenv bootargs ";

	while(1) {
		printf("\n");
		printf("###################### User Menu for OK6410#####################\n");	
		
		printf("[1] Format the nand flash\n");		
//		printf("[2] Burn image from SD card\n");
		printf("[2] Burn image from USB\n");	
		printf("[3] configure the lcd size\n");							
		printf("[4] Boot the system\n");
		printf("[5] Reboot the u-boot\n");
		printf("[6] Exit to command line\n");								

		printf("-----------------------------Select---------------------------------\n");
		printf("Enter your Selection:");

		select = getc();
		printf("%c\n", select >= ' ' && select <= 127 ? select : ' ');

		switch(select) 
		{	
			//[1] Format the nand flash						
			case '1':
				ExecuteCmd("nand scrub");
				break;	
			//[2] Burn image from SD card
//			case '2':
//				arm_sdfuse();
//				break;
			//[3] Use USBsdfuse
			case '2':
				arm_USBfuse();
				break;	
			//[4] configure lcdsize
			case '3':
			lcd_size_config();
			break;	
			//[5] Boot the system		
			case '4':
				ExecuteCmd("nand read.i 0xc0008000 0x200000 0x500000;bootm 0xc0008000");
				break;
			//[6] Reboot the u-boot
			case '5':
				ExecuteCmd("reset");
				break;
			case '6':
				return;
			default:
				break;
		}
	}
}
#endif
#ifdef CONFIG_BOOT_MOVINAND
void ARMMenu(void)
{
	unsigned char select;
	char *command;	
	char cramfs_cmdline[256]="setenv bootargs ";

	while(1) {
		printf("\n");
		printf("###################### User Menu for OK6410#####################\n");	
		
		printf("[1] Boot the Kernel from NAND	\n");		
		printf("[2] Boot the Kernel from SD Card \n");	
		printf("[3] Burn image from SD card\n");
		printf("[4] Reboot the u-boot\n");
		printf("[5] Exit to command line\n");								

		printf("-----------------------------Select-----------------------------\n");
		printf("Enter your Selection:");

		select = getc();
		printf("%c\n", select >= ' ' && select <= 127 ? select : ' ');

		switch(select) 
		{	
			//[1] Boot the Kernel from NAND						
			case '1':
				ExecuteCmd("nand read.i 0xc0008000 0x200000 0x500000;bootm 0xc0008000");
				break;	
			//[2] Boot the Kernel from SD Card (HS-MMC)
			case '2':
				ExecuteCmd("movi read kernel 0xc0008000;bootm 0xc0008000");
				break;
			//[3] Burn image from SD card
			case '3':
				arm_sdfuse();
				break;	
			//[4] Reboot the u-boot
			case '4':
				ExecuteCmd("reset");
				break;
			case '5':
				return;
			default:
				break;
		}
	}
}
#endif
/****************************************************************************/

void main_loop (void)
{
	char *s;
	int bootdelay;

	u_boot_hush_start ();

	bootdelay = CONFIG_BOOTDELAY;

	printf ("\n### main_loop entered: bootdelay=%d\n", bootdelay);

	s = "nand read.i 0xc0008000 0x200000 0x500000;bootm 0xc0008000";

	printf ("### main_loop: bootcmd=\"%s\"\n\n", s ? s : "<UNDEFINED>");

	if (bootdelay >= 0 && s && !abortboot (bootdelay)) {
		parse_string_outer(s, FLAG_PARSE_SEMICOLON |
				    FLAG_EXIT_FROM_LOOP);
	}

#ifdef CONFIG_BOOT_MOVINAND
           ARMMenu();
#endif
#ifdef CONFIG_BOOT_NAND
           NAND_ARMMenu();
#endif
	/*
	 * Main Loop for Monitor Command Processing
	 */

	parse_file_outer();
	/* This point is never reached */
	for (;;);
}

/****************************************************************************/
/*
 * cmdline-editing related codes from vivi.
 * Author: Janghoon Lyu <nandy@mizi.com>
 */

#define putnstr(str,n)	do {			\
		printf ("%.*s", n, str);	\
	} while (0)

#define CTL_CH(c)		((c) - 'a' + 1)

#define MAX_CMDBUF_SIZE		256

#define CTL_BACKSPACE		('\b')
#define DEL			((char)255)
#define DEL7			((char)127)
#define CREAD_HIST_CHAR		('!')

#define getcmd_putch(ch)	putc(ch)
#define getcmd_getch()		getc()
#define getcmd_cbeep()		getcmd_putch('\a')

#define HIST_MAX		20
#define HIST_SIZE		MAX_CMDBUF_SIZE

static int hist_max = 0;
static int hist_add_idx = 0;
static int hist_cur = -1;
unsigned hist_num = 0;

char* hist_list[HIST_MAX];
char hist_lines[HIST_MAX][HIST_SIZE];

#define add_idx_minus_one() ((hist_add_idx == 0) ? hist_max : hist_add_idx-1)

static void hist_init(void)
{
	int i;

	hist_max = 0;
	hist_add_idx = 0;
	hist_cur = -1;
	hist_num = 0;

	for (i = 0; i < HIST_MAX; i++) {
		hist_list[i] = hist_lines[i];
		hist_list[i][0] = '\0';
	}
}

static void cread_add_to_hist(char *line)
{
	strcpy(hist_list[hist_add_idx], line);

	if (++hist_add_idx >= HIST_MAX)
		hist_add_idx = 0;

	if (hist_add_idx > hist_max)
		hist_max = hist_add_idx;

	hist_num++;
}

static char* hist_prev(void)
{
	char *ret;
	int old_cur;

	if (hist_cur < 0)
		return NULL;

	old_cur = hist_cur;
	if (--hist_cur < 0)
		hist_cur = hist_max;

	if (hist_cur == hist_add_idx) {
		hist_cur = old_cur;
		ret = NULL;
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

static char* hist_next(void)
{
	char *ret;

	if (hist_cur < 0)
		return NULL;

	if (hist_cur == hist_add_idx)
		return NULL;

	if (++hist_cur > hist_max)
		hist_cur = 0;

	if (hist_cur == hist_add_idx) {
		ret = "";
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

#define BEGINNING_OF_LINE() {			\
	while (num) {				\
		getcmd_putch(CTL_BACKSPACE);	\
		num--;				\
	}					\
}

#define ERASE_TO_EOL() {				\
	if (num < eol_num) {				\
		int tmp;				\
		for (tmp = num; tmp < eol_num; tmp++)	\
			getcmd_putch(' ');		\
		while (tmp-- > num)			\
			getcmd_putch(CTL_BACKSPACE);	\
		eol_num = num;				\
	}						\
}

#define REFRESH_TO_EOL() {			\
	if (num < eol_num) {			\
		wlen = eol_num - num;		\
		putnstr(buf + num, wlen);	\
		num = eol_num;			\
	}					\
}

static void cread_add_char(char ichar, int insert, unsigned long *num,
	       unsigned long *eol_num, char *buf, unsigned long len)
{
	unsigned long wlen;

	/* room ??? */
	if (insert || *num == *eol_num) {
		if (*eol_num > len - 1) {
			getcmd_cbeep();
			return;
		}
		(*eol_num)++;
	}

	if (insert) {
		wlen = *eol_num - *num;
		if (wlen > 1) {
			memmove(&buf[*num+1], &buf[*num], wlen-1);
		}

		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
		while (--wlen) {
			getcmd_putch(CTL_BACKSPACE);
		}
	} else {
		/* echo the character */
		wlen = 1;
		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
	}
}

static void cread_add_str(char *str, int strsize, int insert, unsigned long *num,
	      unsigned long *eol_num, char *buf, unsigned long len)
{
	while (strsize--) {
		cread_add_char(*str, insert, num, eol_num, buf, len);
		str++;
	}
}

static int cread_line(char *buf, unsigned int *len)
{
	unsigned long num = 0;
	unsigned long eol_num = 0;
	unsigned long rlen;
	unsigned long wlen;
	char ichar;
	int insert = 1;
	int esc_len = 0;
	int rc = 0;
	char esc_save[8];

	while (1) {
		rlen = 1;
		ichar = getcmd_getch();

		if ((ichar == '\n') || (ichar == '\r')) {
			putc('\n');
			break;
		}

		/*
		 * handle standard linux xterm esc sequences for arrow key, etc.
		 */
		if (esc_len != 0) {
			if (esc_len == 1) {
				if (ichar == '[') {
					esc_save[esc_len] = ichar;
					esc_len = 2;
				} else {
					cread_add_str(esc_save, esc_len, insert,
						      &num, &eol_num, buf, *len);
					esc_len = 0;
				}
				continue;
			}

			switch (ichar) {

			case 'D':	/* <- key */
				ichar = CTL_CH('b');
				esc_len = 0;
				break;
			case 'C':	/* -> key */
				ichar = CTL_CH('f');
				esc_len = 0;
				break;	/* pass off to ^F handler */
			case 'H':	/* Home key */
				ichar = CTL_CH('a');
				esc_len = 0;
				break;	/* pass off to ^A handler */
			case 'A':	/* up arrow */
				ichar = CTL_CH('p');
				esc_len = 0;
				break;	/* pass off to ^P handler */
			case 'B':	/* down arrow */
				ichar = CTL_CH('n');
				esc_len = 0;
				break;	/* pass off to ^N handler */
			default:
				esc_save[esc_len++] = ichar;
				cread_add_str(esc_save, esc_len, insert,
					      &num, &eol_num, buf, *len);
				esc_len = 0;
				continue;
			}
		}

		switch (ichar) {
		case 0x1b:
			if (esc_len == 0) {
				esc_save[esc_len] = ichar;
				esc_len = 1;
			} else {
				puts("impossible condition #876\n");
				esc_len = 0;
			}
			break;

		case CTL_CH('a'):
			BEGINNING_OF_LINE();
			break;
		case CTL_CH('c'):	/* ^C - break */
			*buf = '\0';	/* discard input */
			return (-1);
		case CTL_CH('f'):
			if (num < eol_num) {
				getcmd_putch(buf[num]);
				num++;
			}
			break;
		case CTL_CH('b'):
			if (num) {
				getcmd_putch(CTL_BACKSPACE);
				num--;
			}
			break;
		case CTL_CH('d'):
			if (num < eol_num) {
				wlen = eol_num - num - 1;
				if (wlen) {
					memmove(&buf[num], &buf[num+1], wlen);
					putnstr(buf + num, wlen);
				}

				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('k'):
			ERASE_TO_EOL();
			break;
		case CTL_CH('e'):
			REFRESH_TO_EOL();
			break;
		case CTL_CH('o'):
			insert = !insert;
			break;
		case CTL_CH('x'):
			BEGINNING_OF_LINE();
			ERASE_TO_EOL();
			break;
		case DEL:
		case DEL7:
		case 8:
			if (num) {
				wlen = eol_num - num;
				num--;
				memmove(&buf[num], &buf[num+1], wlen);
				getcmd_putch(CTL_BACKSPACE);
				putnstr(buf + num, wlen);
				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('p'):
		case CTL_CH('n'):
		{
			char * hline;

			esc_len = 0;

			if (ichar == CTL_CH('p'))
				hline = hist_prev();
			else
				hline = hist_next();

			if (!hline) {
				getcmd_cbeep();
				continue;
			}

			/* nuke the current line */
			/* first, go home */
			BEGINNING_OF_LINE();

			/* erase to end of line */
			ERASE_TO_EOL();

			/* copy new line into place and display */
			strcpy(buf, hline);
			eol_num = strlen(buf);
			REFRESH_TO_EOL();
			continue;
		}
		default:
			cread_add_char(ichar, insert, &num, &eol_num, buf, *len);
			break;
		}
	}
	*len = eol_num;
	buf[eol_num] = '\0';	/* lose the newline */

	if (buf[0] && buf[0] != CREAD_HIST_CHAR)
		cread_add_to_hist(buf);
	hist_cur = hist_add_idx;

	return (rc);
}

/****************************************************************************/

/*
 * Prompt for input and read a line.
 * If  CONFIG_BOOT_RETRY_TIME is defined and retry_time >= 0,
 * time out when time goes past endtime (timebase time in ticks).
 * Return:	number of read characters
 *		-1 if break
 *		-2 if timed out
 */
int readline (const char *const prompt)
{
	char *p = console_buffer;
	unsigned int len=MAX_CMDBUF_SIZE;
	int rc;
	static int initted = 0;

	if (!initted) {
		hist_init();
		initted = 1;
	}

	puts (prompt);

	rc = cread_line(p, &len);
	return rc < 0 ? rc : len;
}

/****************************************************************************/

