
#include <configs/smdk6410.h>

#include <linux/types.h>        /* ulong */

#include <linux/bitops.h>       /* ffs */

#include <asm/setup.h>          /* ATAG_CORE tag_size */
#include <asm/u-boot.h>         /* bd_t */
#include <asm/global_data.h>	/* DECLARE_GLOBAL_DATA_PTR */

#include <asm/u-boot-arm.h>     /* _armboot_start _bss_start _bss_end */

/*
 * General Purpose Utilities
 */
#define min(X, Y)				\
	({ typeof (X) __x = (X), __y = (Y);	\
		(__x < __y) ? __x : __y; })

#define max(X, Y)				\
	({ typeof (X) __x = (X), __y = (Y);	\
		(__x > __y) ? __x : __y; })

/*
 * Macros to transform values
 * into environment strings.
 */
#define XMK_STR(x)	#x
#define MK_STR(x)	XMK_STR(x)

extern ulong load_addr;		/* Default Load Address */
extern char	BootFile[128];		/* Boot file name			*/
