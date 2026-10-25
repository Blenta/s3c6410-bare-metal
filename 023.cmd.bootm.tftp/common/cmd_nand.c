/*
 * Driver for NAND support, Rick Bronson
 * borrowed heavily from:
 * (c) 1999 Machine Vision Holdings, Inc.
 * (c) 1999, 2000 David Woodhouse <dwmw2@infradead.org>
 *
 * Added 16-bit nand support
 * (C) 2004 Texas Instruments
 */

#include <common.h>

#include <s3c6410.h>


#include <command.h>
#include <watchdog.h>
#include <malloc.h>
#include <asm/byteorder.h>

#include <jffs2/jffs2.h>
#include <nand.h>

#define readl(x)	    *((unsigned int *)(x))
#define writel(v, x)	*((unsigned int *)(x)) = v



#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)

/* parition handling routines */
int mtdparts_init(void);
int id_parse(const char *id, const char **ret_id, u8 *dev_type, u8 *dev_num);
int find_dev_and_part(const char *id, struct mtd_device **dev,
                      u8 *part_num, struct part_info **part);
#endif

extern nand_info_t nand_info[];       /* info for NAND chips */

static int nand_dump_oob(nand_info_t *nand, ulong off)
{
    return 0;
}

static int nand_dump(nand_info_t *nand, ulong off)
{
    int i;
    u_char *buf, *p;
    struct nand_chip *chip = nand->priv;
    unsigned long len = nand->writesize;

    buf = malloc(nand->writesize + nand->oobsize);

    if (!buf) {
        puts("No memory for page buffer\n");
        return 1;
    }

    /* off &= ~(nand->writesize - 1); */

    i = nand_read(nand, off, (unsigned long *)&len, (u_char *)buf);

    /* if (i < 0) {
        printf("Error (%d) reading page %08x\n", i, off);
        free(buf);
        return 1;
    } */

    printf("Page %08x dump:\n", off);

    i = nand->writesize >> 4;
    p = buf;

    while (i--) {
        printf( "\t%02x %02x %02x %02x %02x %02x %02x %02x"
                "  %02x %02x %02x %02x %02x %02x %02x %02x\n",
                p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
                p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
        p += 16;
    }

    puts("OOB:\n");

    p = chip->oob_poi;
    i = nand->oobsize >> 3;

    while (i--) {
        printf( "\t%02x %02x %02x %02x %02x %02x %02x %02x\n",
                p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
        p += 8;
    }

    free(buf);

    return 0;
}

/* ------------------------------------------------------------------------- */

static inline int str2long(char *p, ulong *num)
{
    char *endptr;

    *num = simple_strtoul(p, &endptr, 16);
    return (*p != '\0' && *endptr == '\0') ? 1 : 0;
}

static int arg_off_size(int argc, char *argv[], nand_info_t *nand, ulong *off, ulong *size)
{
    int idx = nand_curr_device;
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
    struct mtd_device *dev;
    struct part_info *part;
    u8 pnum;

    if (argc >= 1 && !(str2long(argv[0], off))) {
        if ((mtdparts_init() == 0) &&
                (find_dev_and_part(argv[0], &dev, &pnum, &part) == 0)) {
            if (dev->id->type != MTD_DEV_TYPE_NAND) {
                puts("not a NAND device\n");
                return -1;
            }
            *off = part->offset;
            if (argc >= 2) {
                if (!(str2long(argv[1], size))) {
                    printf("'%s' is not a number\n", argv[1]);
                    return -1;
                }
                if (*size > part->size)
                    *size = part->size;
            } else {
                *size = part->size;
            }
            idx = dev->id->num;
            *nand = nand_info[idx];
            goto out;
        }
    }
#endif

    if (argc >= 1) {
        if (!(str2long(argv[0], off))) {
            printf("'%s' is not a number\n", argv[0]);
            return -1;
        }
    } else {
        *off = 0;
    }

    if (argc >= 2) {
        if (!(str2long(argv[1], size))) {
            printf("'%s' is not a number\n", argv[1]);
            return -1;
        }
    } else {
        *size = nand->size - *off;
    }

#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
out:
#endif
    printf("device %d ", idx);
    if (*size == nand->size)
        puts("whole chip\n");
    else
        printf("offset 0x%x, size 0x%x\n", *off, *size);
    return 0;
}


static int erase_arg_off_size(int argc, char *argv[], nand_info_t *nand, ulong *off, ulong *size)
{
    int idx = nand_curr_device;
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
    struct mtd_device *dev;
    struct part_info *part;
    u8 pnum;

    if (argc >= 1 && !(str2long(argv[0], off))) {
        if ((mtdparts_init() == 0) &&
                (find_dev_and_part(argv[0], &dev, &pnum, &part) == 0)) {
            if (dev->id->type != MTD_DEV_TYPE_NAND) {
                puts("not a NAND device\n");
                return -1;
            }
            *off = part->offset;
            if (argc >= 2) {
                if (!(str2long(argv[1], size))) {
                    printf("'%s' is not a number\n", argv[1]);
                    return -1;
                }
                if (*size > part->size)
                    *size = part->size;
            } else {
                *size = part->size;
            }
            idx = dev->id->num;
            *nand = nand_info[idx];
            goto out;
        }
    }
#endif

    if (argc >= 1)
    {
        if (!(str2long(argv[0], off)))
        {
            printf("'%s' is not a number\n", argv[0]);
            return -1;
        }

    } else
    {
        *off = 0;
    }

    if (argc >= 2)
    {
        if (!(str2long(argv[1], size)))
        {
            printf("'%s' is not a number\n", argv[1]);
            return -1;
        }

    } else
    {
        *size = nand->size - *off;
    }



    if(*off>=CFG_NAND_ERASE_LEN)
    {
        printf("Sorry  You can erase 0---%x spaces only.\n",CFG_NAND_ERASE_LEN);
        return -1;

    }

    if((*off<CFG_NAND_ERASE_LEN) && (*size>(CFG_NAND_ERASE_LEN-*off) ))
    {
        *size=CFG_NAND_ERASE_LEN-*off;
    }

    if(*off==0 && *size==nand->size)
    {
        *size = CFG_NAND_ERASE_LEN;

    }

    printf("offset 0x%x, erase size 0x%x\n", *off, *size);

#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
out:
#endif
    /*
    printf("device %d ", idx);
    if (*size == nand->size)
        puts("whole chip\n");
    else
        printf("offset 0x%x, size 0x%x\n", *off, *size);
 */
    return 0;
}

int do_nand(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
    int i, dev, ret;
    ulong addr, off, size;
    char *cmd, *s;
    nand_info_t *nand;
    int quiet = 0;

    /* at least two arguments please */
    if (argc < 2)
        goto usage;

    cmd = argv[1];

    if (strcmp(cmd, "info") == 0) {

        putc('\n');
        for (i = 0; i < CFG_MAX_NAND_DEVICE; i++) {
            if (nand_info[i].name)
                printf("Device %d: %s, sector size %lu KiB\n",
                       i, nand_info[i].name,
                       nand_info[i].erasesize >> 10);
        }
        return 0;
    }

    if (strcmp(cmd, "device") == 0) {

        if (argc < 3) {
            if ((nand_curr_device < 0) ||
                    (nand_curr_device >= CFG_MAX_NAND_DEVICE))
                puts("\nno devices available1\n");
            else
                printf("\nDevice %d: %s\n", nand_curr_device,
                       nand_info[nand_curr_device].name);
            return 0;
        }
        dev = (int)simple_strtoul(argv[2], NULL, 10);
        if (dev < 0 || dev >= CFG_MAX_NAND_DEVICE || !nand_info[dev].name) {
            puts("No such device\n");
            return 1;
        }
        printf("Device %d: %s", dev, nand_info[dev].name);
        puts("... is now current device\n");
        nand_curr_device = dev;

#ifdef CFG_NAND_SELECT_DEVICE
        /*
         * Select the chip in the board/cpu specific driver
         */
        board_nand_select_device(nand_info[dev].priv, dev);
#endif

        return 0;
    }

    if (strcmp(cmd, "beep") == 0) {/*sound beep ok*/

        uint reg_f_cfg=readl(REG_GPFCON) & 0x3FFFFFFF | (1<<30);
        writel(reg_f_cfg,REG_GPFCON);

        uint reg_f_on=readl(REG_GPFDAT) & 0xFFFF7FFF | (1<<15);
        uint reg_f_off=readl(REG_GPFDAT) & 0xFFFF7FFF;

        int i;
        for(i=0;i<20;i++){
            if((i+1)%2==0){
                writel(reg_f_off,REG_GPFDAT);
                udelay(1000000);
            }else{
                writel(reg_f_on,REG_GPFDAT);
                udelay(100000);
            }
        }

        return 0;
    }

    if (strcmp(cmd, "led-start") == 0) {/*led on  */

        unsigned int  uConValue = readl(REG_GPMCON);
        unsigned int uDatValue = ~LEDALL_OFF;

        uConValue |= 0x1111;
        writel(uConValue,REG_GPMCON);
        writel(uDatValue,REG_GPMDAT);

        return 0;
    }

    if (strcmp(cmd, "led-end") == 0) {/*led on and off */

        unsigned int  uConValue = readl(REG_GPMCON);
        unsigned int uDatValue = LEDALL_OFF;

        uConValue |= 0x1111;
        writel(uConValue,REG_GPMCON);
        writel(LEDALL_OFF,REG_GPMDAT);

        while(1)
        {
            uDatValue &= LED1_ON;
            writel(uDatValue,REG_GPMDAT);
            udelay(1000000);
            writel(LEDALL_OFF,REG_GPMDAT);


            uDatValue &= LED2_ON;
            writel(uDatValue,REG_GPMDAT);
            udelay(1000000);
            writel(LEDALL_OFF,REG_GPMDAT);


            uDatValue &= LED3_ON;
            writel(uDatValue,REG_GPMDAT);
            udelay(100000);
            writel(LEDALL_OFF,REG_GPMDAT);


            uDatValue &= LED4_ON;
            writel(uDatValue,REG_GPMDAT);
            udelay(1000000);
            writel(LEDALL_OFF,REG_GPMDAT);

            udelay(1000000);
            uDatValue=LEDALL_OFF;
       }

        return 0;
    }
    if (strcmp(cmd, "bad") != 0 && strcmp(cmd, "erase") != 0 &&
            strncmp(cmd, "dump", 4) != 0 &&
            strncmp(cmd, "read", 4) != 0 && strncmp(cmd, "write", 5) != 0 &&
            strcmp(cmd, "scrub") != 0 && strcmp(cmd, "markbad") != 0 &&
            strcmp(cmd, "biterr") != 0 &&
            strcmp(cmd, "lock") != 0 && strcmp(cmd, "unlock") != 0 )
        goto usage;

    /* the following commands operate on the current device */
    if (nand_curr_device < 0 || nand_curr_device >= CFG_MAX_NAND_DEVICE ||
            !nand_info[nand_curr_device].name) {
        puts("\nno devices available2\n");
        return 1;
    }
    nand = &nand_info[nand_curr_device];

    if (strcmp(cmd, "read.kernel") == 0) {

        ulong addr, off, size;
        addr = (ulong)0xc0008000;
        off = (ulong)0x200000;
        size = (ulong)0x500000;

        nand_read_options_t opts;
        memset(&opts, 0, sizeof(opts));
        opts.buffer	= (u_char*) addr;
        opts.length	= size;
        opts.offset	= off;
        opts.quiet      = 1;
        ret = nand_read_opts(nand, &opts);

        printf("Kernel %d bytes read: %s\n", size, ret ? "ERROR" : "OK");

        return ret == 0 ? 0 : 1;
    }

    if (strcmp(cmd, "bad") == 0) {
        printf("\nDevice %d bad blocks:\n", nand_curr_device);
        for (off = 0; off < nand->size; off += nand->erasesize)
            if (nand_block_isbad(nand, off))
                printf("  %08x\n", off);
        return 0;
    }

    /*
     * Syntax is:
     *   0    1     2       3    4
     *   nand erase [clean] [off size]
     */
    if (strcmp(cmd, "erase") == 0 || strcmp(cmd, "scrub") == 0) {
        nand_erase_options_t opts;
        /* "clean" at index 2 means request to write cleanmarker */
        int clean = argc > 2 && !strcmp("clean", argv[2]);
        int o = clean ? 3 : 2;
        int scrub = !strcmp(cmd, "scrub");

        printf("\nNAND %s: ", scrub ? "scrub" : "erase");
        /* skip first two or three arguments, look for offset and size */

        if (arg_off_size(argc - o, argv + o, nand, &off, &size) != 0)
         return 1;

     //   if (erase_arg_off_size(argc - o, argv + o, nand, &off, &size) != 0)//forlinx change here
       //     return 1;

        memset(&opts, 0, sizeof(opts));
        opts.offset = off;
        opts.length = size;
        opts.jffs2  = clean;
        opts.quiet  = quiet;

        if (scrub) {
            puts("Warning: "
                 "scrub option will erase all factory set "
                 "bad blocks!\n"
                 "         "
                 "There is no reliable way to recover them.\n"
                 "         "
                 "Use this command only for testing purposes "
                 "if you\n"
                 "         "
                 "are sure of what you are doing!\n"
                 "\nReally scrub this NAND flash? <y/N>\n");

            if (getc() == 'y' && getc() == '\r') {
                opts.scrub = 1;
            } else {
                puts("scrub aborted\n");
                return -1;
            }
        }
        ret = nand_erase_opts(nand, &opts);
        printf("%s\n", ret ? "ERROR" : "OK");

        return ret == 0 ? 0 : 1;
    }

    if (strncmp(cmd, "dump", 4) == 0) {
        if (argc < 3)
            goto usage;

        s = strchr(cmd, '.');
        off = (int)simple_strtoul(argv[2], NULL, 16);

        if (s != NULL && strcmp(s, ".oob") == 0)
            ret = nand_dump_oob(nand, off);
        else {
            ret = nand_dump(nand, off);
        }

        return ret == 0 ? 1 : 0;

    }

    /* read write */
    if (strncmp(cmd, "read", 4) == 0 || strncmp(cmd, "write", 5) == 0) {
        int read;

        if (argc < 4)
            goto usage;

        addr = (ulong)simple_strtoul(argv[2], NULL, 16);

        read = strncmp(cmd, "read", 4) == 0; /* 1 = read, 0 = write */
        printf("\nNAND %s: ", read ? "read" : "write");
        if (arg_off_size(argc - 3, argv + 3, nand, &off, &size) != 0)
            return 1;

        s = strchr(cmd, '.');
        if (s != NULL &&
                (!strcmp(s, ".jffs2") || !strcmp(s, ".e") || !strcmp(s, ".i"))) {
            if (read) {
                /* read */
                nand_read_options_t opts;
                memset(&opts, 0, sizeof(opts));
                opts.buffer	= (u_char*) addr;
                opts.length	= size;
                opts.offset	= off;
		//opts.quiet      = quiet;
                opts.quiet      = 1;
                ret = nand_read_opts(nand, &opts);
            } else {
                /* write */
                nand_write_options_t opts;
                memset(&opts, 0, sizeof(opts));
                opts.buffer	= (u_char*) addr;
                opts.length	= size;
                opts.offset	= off;
                /* opts.forcejffs2 = 1; */
                opts.pad	= 1;
                opts.blockalign = 1;
                opts.quiet      = quiet;
                ret = nand_write_opts(nand, &opts);
            }
#ifdef CFG_NAND_YAFFS_WRITE
        } else if (!read && s != NULL && + (!strcmp(s, ".yaffs2") || !strcmp(s, ".yaffs1"))) {
            nand_write_options_t opts;
            memset(&opts, 0, sizeof(opts));
            opts.buffer = (u_char*) addr;
            opts.length = size;
            opts.offset = off;
            opts.pad = 0;
            opts.blockalign = 1;
            opts.quiet = quiet;
            opts.writeoob = 1;
            opts.autoplace = 1;

            /* jsgood */
            /* if (s[6] == '1')
                opts.forceyaffs = 1; */

            ret = nand_write_opts(nand, &opts);
#endif

        } else if (!read && s != NULL && (!strcmp(s, ".uboot")) && nand->writesize == 4096) {
            size=4096;
            nand_write(nand, off, &size, (u_char *)addr);
            off+=4096;
            addr+=2048;
            nand_write(nand, off, &size, (u_char *)addr);
            off+=4096;
            addr+=2048;
            nand_write(nand, off, &size, (u_char *)addr);
            off+=4096;
            addr+=2048;
            nand_write(nand, off, &size, (u_char *)addr);
            off+=4096;
            addr+=2048;

            size=1024*1024-4*4096;
            ret = nand_write(nand, off, &size, (u_char *)addr);

        }else if(!read && s != NULL && (!strcmp(s, ".uboot")) && nand->writesize == 8192)
        {

            size=8192;

            ret=nand_write(nand, off, &size, (u_char *)addr);

            off+=8192;
            addr+=2048;
            ret=nand_write(nand, off, &size, (u_char *)addr);

            off+=8192;
            addr+=2048;
            ret=nand_write(nand, off, &size, (u_char *)addr);

            off+=8192;
            addr+=2048;
            ret=nand_write(nand, off, &size, (u_char *)addr);

            off+=8192;
            addr+=2048;

            size=1024*1024-4*8192;
            ret = nand_write(nand, off, &size, (u_char *)addr);
        }
        else {
            if (read)
                ret = nand_read(nand, off, &size, (u_char *)addr);
            else {
                ret = nand_write(nand, off, &size, (u_char *)addr);

                if (ret == 0) {
                    uint *magic = (uint*)(PHYS_SDRAM_1);
                    if ((0x24564236 == magic[0]) && (0x20764316 == magic[1]))
                        magic[0] = 0x27051956;
                }
            }
        }

        printf(" %d bytes %s: %s\n", size,
               read ? "read" : "written", ret ? "ERROR" : "OK");

        return ret == 0 ? 0 : 1;
    }

    if (strcmp(cmd, "markbad") == 0) {
        addr = (ulong)simple_strtoul(argv[2], NULL, 16);

        int ret = nand->block_markbad(nand, addr);
        if (ret == 0) {
            printf("block 0x%08lx successfully marked as bad\n",
                   (ulong) addr);
            return 0;
        } else {
            printf("block 0x%08lx NOT marked as bad! ERROR %d\n",
                   (ulong) addr, ret);
        }
        return 1;
    }
    if (strcmp(cmd, "biterr") == 0) {
        /* todo */
        return 1;
    }

    if (strcmp(cmd, "lock") == 0) {
        int tight  = 0;
        int status = 0;
        if (argc == 3) {
            if (!strcmp("tight", argv[2]))
                tight = 1;
            if (!strcmp("status", argv[2]))
                status = 1;
        }

        if (status) {
            ulong block_start = 0;
            ulong off;
            int last_status = -1;

            struct nand_chip *nand_chip = nand->priv;
            /* check the WP bit */
            nand_chip->cmdfunc (nand, NAND_CMD_STATUS, -1, -1);
            printf("device is %swrite protected\n",
                   (nand_chip->read_byte(nand) & 0x80 ?
                        "NOT " : "" ) );

            for (off = 0; off < nand->size; off += nand->writesize) {
                int s = nand_get_lock_status(nand, off);

                /* print message only if status has changed
                 * or at end of chip
                 */
                if (off == nand->size - nand->writesize
                        || (s != last_status && off != 0))	{

                    printf("%08x - %08x: %8d pages %s%s%s\n",
                           block_start,
                           off-1,
                           (off-block_start)/nand->writesize,
                           ((last_status & NAND_LOCK_STATUS_TIGHT) ? "TIGHT " : ""),
                           ((last_status & NAND_LOCK_STATUS_LOCK) ? "LOCK " : ""),
                           ((last_status & NAND_LOCK_STATUS_UNLOCK) ? "UNLOCK " : ""));
                }

                last_status = s;
            }
        } else {
            if (!nand_lock(nand, tight)) {
                puts("NAND flash successfully locked\n");
            } else {
                puts("Error locking NAND flash\n");
                return 1;
            }
        }
        return 0;
    }

    if (strcmp(cmd, "unlock") == 0) {
        if (arg_off_size(argc - 2, argv + 2, nand, &off, &size) < 0)
            return 1;

        if (!nand_unlock(nand, off, size)) {
            puts("NAND flash successfully unlocked\n");
        } else {
            puts("Error unlocking NAND flash, "
                 "write and erase will probably fail\n");
            return 1;
        }
        return 0;
    }

usage:
    printf("Usage:\n%s\n", cmdtp->usage);
    return 1;
}

U_BOOT_CMD(nand, 5, 1, do_nand,
           "nand    - NAND sub-system\n",
           "info             - show available NAND devices\n"
           "nand device [dev]     - show or set current device\n"
           "nand read[.jffs2]     - addr off|partition size\n"
           "nand write[.jffs2]    - addr off|partiton size - read/write `size' bytes starting\n"
           "    at offset `off' to/from memory address `addr'\n"
           #ifdef CFG_NAND_YAFFS_WRITE
           "nand write[.yaffs[1]] - addr off|partition size - write `size' byte yaffs image\n"
           "    starting at offset `off' from memory address `addr' (.yaffs1 for 512+16 NAND)\n"
           #endif
           "nand write[.uboot] - addr off|partition size\n"
           "nand write[.ok] - sound beep ok\n"
           "nand erase [clean] [off size] - erase `size' bytes from\n"
           "    offset `off' (entire device if not specified)\n"
           "nand bad - show bad blocks\n"
           "nand dump[.oob] off - dump page\n"
           "nand scrub - really clean NAND erasing bad blocks (UNSAFE)\n"
           "nand markbad off - mark bad block at offset (UNSAFE)\n"
           "nand biterr off - make a bit error at offset (UNSAFE)\n"
           "nand lock [tight] [status] - bring nand to lock state or display locked pages\n"
           "nand unlock [offset] [size] - unlock section\n");

