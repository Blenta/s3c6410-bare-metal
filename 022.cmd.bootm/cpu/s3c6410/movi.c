#include <common.h>

#ifdef CONFIG_MOVINAND

#include <s3c6410.h>

#include <movi.h>
#include <asm/io.h>

uint movi_hc = 0;

void movi_set_capacity(void)
{
	if (movi_sector_mode())
		movi_hc = 1;
}

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
    printf (", Card Size: %d MB", movi_card_size());
    serial_puts("\n\n");
    
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

int movi_set_ofs(uint last)
{
	int changed = 0;

	if (ofsinfo.last != last) {
		ofsinfo.last 	= last - (eFUSE_SIZE / MOVI_BLKSIZE);
		ofsinfo.bl1	= ofsinfo.last - MOVI_BL1_BLKCNT;
		ofsinfo.env	= ofsinfo.bl1 - MOVI_ENV_BLKCNT;
		ofsinfo.bl2	= ofsinfo.bl1 - MOVI_UBOOT_BLKCNT;
		ofsinfo.kernel	= ofsinfo.bl2 - MOVI_ZIMAGE_BLKCNT;
		ofsinfo.rootfs	= ofsinfo.kernel - MOVI_ROOTFS_BLKCNT;
		changed = 1;
	}

	return changed;
}

int movi_init(void)
{
	hsmmc_set_gpio();
	hsmmc_reset();
	if (hsmmc_init()) {
		printf("\nCard Initialization failed.\n");
		return -1;
	}
	return 1;
}

int movi_card_size(void)
{
	return (int) (MOVI_TOTAL_BLKCNT / 2 / 1024);
}

void movi_bl2_copy(void)
{
	CopyMovitoMem(HSMMC_CHANNEL, MOVI_BL2_POS, MOVI_BL2_BLKCNT, (uint *)BL2_BASE, MOVI_INIT_REQUIRED);
}

int movi_kernel_copy(uint addr)
{
	return CopyMovitoMem(HSMMC_CHANNEL, MOVI_ZIMAGE_POS, MOVI_ZIMAGE_BLKCNT, (uint *)addr, MOVI_INIT_REQUIRED);
}

#endif
