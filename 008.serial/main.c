
#include <configs/smdk6410.h>

#include <serial.h>

const char version_string[] = U_BOOT_VERSION" (" __DATE__ " - " __TIME__ ")"CONFIG_IDENT_STRING;

void start_uboot (void)
{
	serial_puts(version_string);

	for (;;);
}

