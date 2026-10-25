#ifndef __D_TCM_H__
#define __D_TCM_H__

#define HSMMC_CHANNEL		0
#define MOVI_INIT_REQUIRED	0

#define	TCM_BASE		0x0C004000
#define BL1_BASE		(CFG_PHY_UBOOT_BASE)
#define BL2_BASE		(CFG_PHY_UBOOT_BASE)
#define KERNEL_BASE		(CFG_KERNEL_LOAD_ADDR)

/* movinand definitions */
#ifdef CONFIG_BOOT_MOVINAND
#define MOVI_TOTAL_BLKCNT	*((volatile unsigned int*)(TCM_BASE - 0x4))
#define MOVI_SDHC_INFO		*((volatile unsigned int*)(TCM_BASE - 0x8)) 
#else
#define MOVI_TOTAL_BLKCNT	7744512 //250880 // 7744512 // 7744512 // 15564800 /* static movinand total block count: for writing to movinand when nand boot */
#define MOVI_SDHC_INFO	0
#endif

/* size information */
#define MOVI_BLKSIZE		512

#define BL1_SIZE		(8 * 1024)
#define eFUSE_SIZE		(1 * 1024)	// 0.5k eFuse, 0.5k reserved`

/* partition information */
#define PART_SIZE_BL		(64 * 1024)
#define PART_SIZE_UBOOT		(256 * 1024)
#define PART_SIZE_KERNEL	(6 * 1024 * 1024)
#define PART_SIZE_ROOTFS	(10 * 1024 * 1024)

#define MOVI_LAST_BLKPOS	(MOVI_TOTAL_BLKCNT - (eFUSE_SIZE / MOVI_BLKSIZE))
#define MOVI_BL1_BLKCNT		(BL1_SIZE / MOVI_BLKSIZE)
#define MOVI_ENV_BLKCNT		(CFG_ENV_SIZE / MOVI_BLKSIZE)
#define MOVI_BL2_BLKCNT		(PART_SIZE_BL / MOVI_BLKSIZE)
#define MOVI_UBOOT_BLKCNT	(PART_SIZE_UBOOT / MOVI_BLKSIZE)
#define MOVI_ZIMAGE_BLKCNT	(PART_SIZE_KERNEL / MOVI_BLKSIZE)
#define MOVI_ROOTFS_BLKCNT	(PART_SIZE_ROOTFS / MOVI_BLKSIZE)
#define MOVI_BL1_POS		(MOVI_LAST_BLKPOS - MOVI_BL1_BLKCNT)
#define MOVI_BL2_POS		(MOVI_LAST_BLKPOS - MOVI_BL1_BLKCNT - MOVI_BL2_BLKCNT)
#define MOVI_ZIMAGE_POS		(MOVI_LAST_BLKPOS - MOVI_BL1_BLKCNT - MOVI_UBOOT_BLKCNT - MOVI_ZIMAGE_BLKCNT)


/* TCM function for bl2 load */
/**
 * This Function copies a block of page to destination memory.( 8-bit ECC only )
 * Note: 512 Page Nand Only
 * @param uint blcok : Source block address number to copy.
 * @param uint page : Source page address number to copy.
 * @param uchar *buffer : Target Buffer pointer.
 * @return int32 - Success or failure.
 */
#define NF8_ReadPage(a,b,c) (((int(*)(uint, uint, uchar*))(*((uint *)(TCM_BASE))))(a,b,c)) 

/**
 * This Function copies a block of page to destination memory( 8-bit ECC only )
 * Note: 2048, 4096 Page Nand Only. 
 * @param uint blcok : Source block address number to copy.
 * @param uint page : Source page address number to copy.
 * @param uchar *buffer : Target Buffer pointer.
 * @return int32 - Success or failure.
 */
#define NF8_ReadPage_Adv(a,b,c) (((int(*)(uint, uint, uchar*))(*((uint *)(TCM_BASE + 0x4))))(a,b,c)) 

/**
 * This Function copies SD/MMC Card Data to memory.
 * Always use EPLL source clock.
 * @param channel : HSMMC Controller channel number ( Not support. Depend on GPN15, GPN14 and GPN13 )
 * @param StartBlkAddress : Source card(SD/MMC) Address.(It must block address.)
 * @param blockSize : Number of blocks to copy.
 * @param memoryPtr : Buffer to copy from.
 * @param with_init : reinitialize or not
 * @return unsigend int - Success or failure.
*/
#define CopyMMCtoMem(z,a,b,c,e) (((int(*)(int, uint, ushort, uint *, int))(*((uint *)(TCM_BASE + 0x8))))(z,a,b,c,e))

void movi_set_capacity(void);
int movi_card_type(void);
int movi_sector_mode(void);
int memory_card_info(void);
int movi_card_size(void);

#endif /*__D_TCM_H__*/
