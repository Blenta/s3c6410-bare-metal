
TOPDIR		:= $(CURDIR)

export	TOPDIR

#########################################################################

ARCH   = arm
CPU    = s3c64xx
BOARD  = smdk6410
VENDOR = samsung
SOC    = s3c6410


export	ARCH CPU BOARD VENDOR SOC

#########################################################################

ifndef CROSS_COMPILE
ifeq ($(ARCH),arm)
CROSS_COMPILE = arm-linux-gnueabi-
endif
endif

# load other configuration
include $(TOPDIR)/config.mk

#########################################################################
# U-Boot objects....order is important (i.e. start must be first)


LIBS = board/$(BOARD)/lib$(BOARD).a
LIBS +=	cpu/$(SOC)/lib$(SOC).a
LIBS += lib_$(ARCH)/lib$(ARCH).a
LIBS += lib_generic/libgeneric.a

LIBS += drivers/nand/libnand.a
LIBS += common/libcommon.a

LIBS += net/libnet.a

.PHONY : $(LIBS)

#########################################################################
#########################################################################

ALL = u-boot.bin u-boot-movi.bin

all:		$(ALL)

u-boot.bin:	u-boot.elf
		$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@
		$(OBJDUMP) -d $< > u-boot.dis

u-boot-movi.bin:	u-boot.bin
		./mkmovi_128k

u-boot.dis:	u-boot.elf
		$(OBJDUMP) -d $< > $@

u-boot.elf:	$(LIBS) $(LDSCRIPT)
		UNDEF_SYM=`$(OBJDUMP) -x $(LIBS) |sed  -n -e 's/.*\(__u_boot_cmd_.*\)/-u\1/p'|sort|uniq`;\
		cd $(TOPDIR) && $(LD) $(LDFLAGS) $$UNDEF_SYM \
			--start-group $(LIBS) --end-group \
			-Map u-boot.map -o u-boot.elf


$(LIBS):
		$(MAKE) -C $(dir $@)


#########################################################################


clean:
	find $(TOPDIR) -type f \
		\( -name '*.o'  -o -name '*.a' -o -name '.depend' \) -print \
		| xargs rm -f
	rm -f u-boot.* u-boot-movi.bin
	

backup:
	F=`basename $(TOPDIR)` ; cd .. ; \
	tar -cJpf `date "+$$F-%Y-%m-%d-%T.tar.gz"` $$F

#########################################################################
