
#include <configs/smdk6410.h>

#include <linux/types.h>        /* ulong */
#include <asm/u-boot-arm.h>     /* _armboot_start _bss_start _bss_end */

#include <serial.h>
#include <d-tcm.h>

const char version_string[] = U_BOOT_VERSION" (" __DATE__ " - " __TIME__ ")"CONFIG_IDENT_STRING;


void start_uboot (void)
{
	printf ("\n\n%s\n\n", version_string);
	printf ("U-Boot code: %08lX -> %08lX  BSS: -> %08lX\n", _uboot_start, _bss_start, _bss_end);

	print_cpuinfo();
	
	interrupt_init();
	
	udelay(0xF0000000);
	printf("delayed: 0xF0000000us\n\n");

	memory_card_info();

	for (;;);
}


