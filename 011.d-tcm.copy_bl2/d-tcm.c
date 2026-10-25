
#include <configs/smdk6410.h>

#include <linux/types.h>

#include <serial.h>
#include <d-tcm.h>

int memory_card_info(void)
{
    serial_puts("Card Type: ");
    if (movi_card_type() == 0x2) {
        serial_puts("MMC");
    } else if (movi_card_type() == 0x4) {
        serial_puts("SD");
    }
    serial_puts(", Sector Mode: ");
    if (movi_sector_mode()) {
        serial_puts("Yes");
    } else {
        serial_puts("No");
    }
    serial_putc('\n');
    
	return (0);
}

int movi_card_type(void)
{
	return (MOVI_SDHC_INFO & 0x6);
}

int movi_sector_mode(void)
{
	return (MOVI_SDHC_INFO & 0x1);
}

int movi_bl1_copy(void)
{
	return CopyMMCtoMem(HSMMC_CHANNEL, MOVI_BL1_POS, MOVI_BL1_BLKCNT, (uint *)BL1_BASE, MOVI_INIT_REQUIRED);
}

int movi_bl2_copy(void)
{
	return CopyMMCtoMem(HSMMC_CHANNEL, MOVI_BL2_POS, MOVI_BL2_BLKCNT, (uint *)BL2_BASE, MOVI_INIT_REQUIRED);
}