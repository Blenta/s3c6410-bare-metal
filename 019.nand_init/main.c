
#include <configs/smdk6410.h>

#include <linux/types.h>        /* ulong */
#include <asm/u-boot-arm.h>     /* _armboot_start _bss_start _bss_end */

#include <serial.h>
#include <d-tcm.h>

const char version_string[] = U_BOOT_VERSION" (" __DATE__ " - " __TIME__ ")"CONFIG_IDENT_STRING;

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
	printf ("U-Boot code: %08lX -> %08lX  BSS: -> %08lX\n", _uboot_start, _bss_start, _bss_end);
	return (0);
}

void start_uboot (void)
{
	display_banner();
	print_cpuinfo();
	checkboard();

	/* armboot_start is defined in the board-specific linker script */
#ifdef CONFIG_MEMORY_UPPER_CODE /* by scsuh */
	mem_malloc_init (CFG_UBOOT_BASE + CFG_UBOOT_SIZE - CFG_MALLOC_LEN - CFG_STACK_SIZE);
#else
	mem_malloc_init (_uboot_start - CFG_MALLOC_LEN);
#endif

	interrupt_init();
	
	memory_card_info();

	serial_puts ("SD/MMC:  ");
	movi_set_capacity();
	hsmmc_set_gpio();
	hsmmc_reset();
	if (hsmmc_init()) {
		printf("\nCard Initialization failed.\n");
		return -1;
	}

	serial_puts ("\nNAND:    ");
	nand_init();

	for (;;);
}


