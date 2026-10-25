
#define U_BOOT_VERSION "\n\nU-Boot by Blenta"
#define CONFIG_IDENT_STRING	" for SMDK6410\n\n"

#define CONFIG_BOOT_MOVINAND
//#define CONFIG_BOOT_NAND

#define CONFIG_ENABLE_MMU


/************************************************************
 * Memory
 ************************************************************/
#define MEMORY_BASE_ADDRESS	0x50000000

#define CFG_SS_BASE		0x0C000000
#define CFG_PHY_STACK_BASE	CFG_SS_BASE + 0x1F00


/* base address for uboot */
#ifdef CONFIG_ENABLE_MMU
#define CFG_UBOOT_BASE		0xcfe00000
#else
#define CFG_UBOOT_BASE		0x5fe00000
#endif
#define CFG_PHY_UBOOT_BASE	MEMORY_BASE_ADDRESS + 0xfe00000

/* total memory required by uboot */
#define CFG_UBOOT_SIZE		(2*1024*1024)

#ifdef CONFIG_ENABLE_MMU
#define virt_to_phys(x)	virt_to_phy_smdk6410(x)
#else
#define virt_to_phys(x)	(x)
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define DMC1_MEM_CFG		0x0001001A	/* Supports one CKE control, Chip1, Burst4, Row/Column bit */
#define DMC1_CHIP0_CFG		0x150F0
#define DMC1_MEM_CFG2		0xB45
#define DMC_DDR_32_CFG		0x0 		/* 32bit, DDR */

/* Memory Parameters */
/* DDR Parameters */
#define DDR_tREFRESH	7800	/* ns */
#define DDR_tRAS		45		/* ns (min: 45ns)*/
#define DDR_tRC 		68		/* ns (min: 67.5ns)*/
#define DDR_tRCD		23		/* ns (min: 22.5ns)*/
#define DDR_tRFC		80		/* ns (min: 80ns)*/
#define DDR_tRP 		23		/* ns (min: 22.5ns)*/
#define DDR_tRRD		15		/* ns (min: 15ns)*/
#define DDR_tWR 		15		/* ns (min: 15ns)*/
#define DDR_tXSR		120		/* ns (min: 120ns)*/
#define DDR_CASL		3		/* CAS Latency 3 */


/*
 * mDDR memory configuration
 */
#define DMC_DDR_BA_EMRS 	2
#define DMC_DDR_MEM_CASLAT	3
#define DMC_DDR_CAS_LATENCY	(DDR_CASL<<1)						//6   Set Cas Latency to 3
#define DMC_DDR_t_DQSS		1							// Min 0.75 ~ 1.25
#define DMC_DDR_t_MRD		2							//Min 2 tck
#define DMC_DDR_t_RAS		(((Startup_HCLK / 1000 * DDR_tRAS) - 1) / 1000000 + 1)	//7, Min 45ns
#define DMC_DDR_t_RC		(((Startup_HCLK / 1000 * DDR_tRC) - 1) / 1000000 + 1) 	//10, Min 67.5ns
#define DMC_DDR_t_RCD		(((Startup_HCLK / 1000 * DDR_tRCD) - 1) / 1000000 + 1) 	//4,5(TRM), Min 22.5ns
#define DMC_DDR_schedule_RCD	((DMC_DDR_t_RCD - 3) << 3)
#define DMC_DDR_t_RFC		(((Startup_HCLK / 1000 * DDR_tRFC) - 1) / 1000000 + 1) 	//11,18(TRM) Min 80ns
#define DMC_DDR_schedule_RFC	((DMC_DDR_t_RFC - 3) << 5)
#define DMC_DDR_t_RP		(((Startup_HCLK / 1000 * DDR_tRP) - 1) / 1000000 + 1) 	//4, 5(TRM) Min 22.5ns
#define DMC_DDR_schedule_RP	((DMC_DDR_t_RP - 3) << 3)
#define DMC_DDR_t_RRD		(((Startup_HCLK / 1000 * DDR_tRRD) - 1) / 1000000 + 1)	//3, Min 15ns
#define DMC_DDR_t_WR		(((Startup_HCLK / 1000 * DDR_tWR) - 1) / 1000000 + 1)	//Min 15ns
#define DMC_DDR_t_WTR		2
#define DMC_DDR_t_XP		2							//1tck + tIS(1.5ns)
#define DMC_DDR_t_XSR		(((Startup_HCLK / 1000 * DDR_tXSR) - 1) / 1000000 + 1)	//17, Min 120ns
#define DMC_DDR_t_ESR		DMC_DDR_t_XSR
#define DMC_DDR_REFRESH_PRD	(((Startup_HCLK / 1000 * DDR_tREFRESH) - 1) / 1000000) 	// TRM 2656
#define DMC_DDR_USER_CONFIG	1							// 2b01 : mDDR


/************************************************************
 * Clock
 ************************************************************/

/* input clock of PLL */
#define CONFIG_SYS_CLK_FREQ	12000000	/* the SMDK6410 has 12MHz input clock */

/**********************************
 Support Clock Settings
 **********************************
 Setting	SYNC	ASYNC
 ----------------------------------
 667_133_66	 X	  O
 533_133_66	 O	  O
 400_133_66	 X	  O
 400_100_50	 O	  O
 **********************************/
//#define CONFIG_CLK_800_133_66
//#define CONFIG_CLK_666_133_66
#define CONFIG_CLK_532_133_66
//#define CONFIG_CLK_400_133_66
//#define CONFIG_CLK_400_100_50
//#define CONFIG_CLK_OTHERS

#define CONFIG_CLKSRC_CLKUART

#define set_pll(mdiv, pdiv, sdiv)	(1<<31 | mdiv<<16 | pdiv<<8 | sdiv)

#if defined(CONFIG_CLK_666_133_66) /* FIN 12MHz, Fout 666MHz */
#define APLL_MDIV	333
#define APLL_PDIV	3
#define APLL_SDIV	1
#undef  CONFIG_SYNC_MODE /* ASYNC MODE */

#elif defined(CONFIG_CLK_532_133_66) /* FIN 12MHz, Fout 532MHz */
#define APLL_MDIV	266
#define APLL_PDIV	3
#define APLL_SDIV	1
#define CONFIG_SYNC_MODE

#elif defined(CONFIG_CLK_400_133_66) || defined(CONFIG_CLK_800_133_66) /* FIN 12MHz, Fout 800MHz */
#define APLL_MDIV	400
#define APLL_PDIV	3
#define APLL_SDIV	1
#define CONFIG_SYNC_MODE

#elif defined(CONFIG_CLK_400_100_50) /* FIN 12MHz, Fout 400MHz */
#define APLL_MDIV	400
#define APLL_PDIV	3
#define APLL_SDIV	2
#define CONFIG_SYNC_MODE

#elif defined(CONFIG_CLK_OTHERS)
/*If you have to use another value, please define pll value here*/
/* FIN 12MHz, Fout 532MHz */
#define APLL_MDIV	266
#define APLL_PDIV	3
#define APLL_SDIV	1
#define CONFIG_SYNC_MODE

#else
#error "Not Support Fequency or Mode!! you have to setup right configuration."
#endif

#define APLL_VAL	set_pll(APLL_MDIV, APLL_PDIV, APLL_SDIV)
/* prevent overflow */
#define Startup_APLL	(CONFIG_SYS_CLK_FREQ/(APLL_PDIV<<APLL_SDIV)*APLL_MDIV)

/* fixed MPLL 533MHz */
#define MPLL_MDIV	266
#define MPLL_PDIV	3
#define MPLL_SDIV	1

#define MPLL_VAL	set_pll(MPLL_MDIV, MPLL_PDIV, MPLL_SDIV)
/* prevent overflow */
#define Startup_MPLL	((CONFIG_SYS_CLK_FREQ)/(MPLL_PDIV<<MPLL_SDIV)*MPLL_MDIV)

#if defined(CONFIG_CLK_800_133_66)
#define Startup_APLLdiv		0
#define Startup_HCLKx2div	2
#elif defined(CONFIG_CLK_400_133_66)
#define Startup_APLLdiv		1
#define Startup_HCLKx2div	2
#else
#define Startup_APLLdiv		0
#define Startup_HCLKx2div	1
#endif

#define	Startup_PCLKdiv		3
#define Startup_HCLKdiv		1
#define Startup_MPLLdiv		1

#define CLK_DIV_VAL	((Startup_PCLKdiv<<12)|(Startup_HCLKx2div<<9)|(Startup_HCLKdiv<<8)|(Startup_MPLLdiv<<4)|Startup_APLLdiv)

#if defined(CONFIG_SYNC_MODE)
#define Startup_HCLK	(Startup_APLL/(Startup_HCLKx2div+1)/(Startup_HCLKdiv+1))
#else
#define Startup_HCLK	(Startup_MPLL/(Startup_HCLKx2div+1)/(Startup_HCLKdiv+1))
#endif

/************************************************************
 * UART
 ************************************************************/
/*
 * select serial console configuration
 */
#define CONFIG_SERIAL1          1	/* we use SERIAL 1 on SMDK6410 */

#define CONFIG_UART_66	/* default clock value of CLK_UART */
