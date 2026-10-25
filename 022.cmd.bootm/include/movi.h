#ifndef __MOVI_H__
#define __MOVI_H__

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

/* TCM function for bl2 load */
#define CopyMovitoMem(a,b,c,d,e)	(((int(*)(int, uint, ushort, uint *, int))(*((uint *)(TCM_BASE + 0x8))))(a,b,c,d,e))

/* partition information */
#define PART_SIZE_BL		(128 * 1024)
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

struct movi_offset_t {
	uint	last;
	uint	bl1;
	uint	env;
	uint	bl2;
	uint	kernel;
	uint	rootfs;
};

/* external functions */
extern void hsmmc_set_gpio(void);
extern void hsmmc_reset (void);
extern int hsmmc_init (void);
extern int movi_init(void);
extern void movi_set_capacity(void);
extern int movi_set_ofs(uint last);
extern void movi_write (uint addr, uint start_blk, uint blknum);
extern void movi_read (uint addr, uint start_blk, uint blknum);

extern ulong virt_to_phy_smdk6410(ulong addr);

/* external variables */
extern uint movi_hc;
extern struct movi_offset_t ofsinfo;

extern int movi_card_size(void);

#endif /*__MOVI_H__*/
