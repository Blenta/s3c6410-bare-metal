
#define U_BOOT_VERSION "\n\nU-Boot by Blenta"
#define CONFIG_IDENT_STRING	" for SMDK6410\n\n"

#define CFG_SS_BASE		0x0C000000
#define CFG_PHY_STACK_BASE	CFG_SS_BASE + 0x1F00

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
