
#include <configs/smdk6410.h>

#include <linux/types.h>

int checkboard(void)
{
	printf("Board:   SMDK6410\n");
	printf("DRAM:    %d MB\n", (uint) (PHYS_SDRAM_1_SIZE / 1024 / 1024));
	return (0);
}

#ifdef CONFIG_ENABLE_MMU
ulong virt_to_phy_smdk6410(ulong addr)
{
	if ((0xc0000000 <= addr) && (addr < 0xd0000000))
		return (addr - 0xc0000000 + 0x50000000);
	else
		printf("do not support this address : %08lx\n", addr);

	return addr;
}
#endif