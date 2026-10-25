
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

COBJS  = main.o

LIBS  =	cpu/$(SOC)/lib$(SOC).a
LIBS += lib_$(ARCH)/lib$(ARCH).a
LIBS += lib_generic/libgeneric.a


.PHONY : $(LIBS)

#########################################################################
#########################################################################

ALL = u-boot.bin u-boot-movi.bin

all:		$(ALL)

u-boot.bin:	u-boot.elf
		$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@
		$(OBJDUMP) -d $< > u-boot.dis

u-boot-movi.bin:	u-boot.bin
		./mkmovi_64k

u-boot.dis:	u-boot.elf
		$(OBJDUMP) -d $< > $@

u-boot.elf:	 $(COBJS) $(LIBS) $(LDSCRIPT)
		cd $(TOPDIR) && $(LD) $(LDFLAGS) $(COBJS) \
			--start-group $(LIBS) --end-group \
			-Map u-boot.map -o u-boot.elf


$(LIBS):
		$(MAKE) -C $(dir $@)

#########################################################################

%.o:	%.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<


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
