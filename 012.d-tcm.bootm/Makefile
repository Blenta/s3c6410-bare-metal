TEXT_BASE = 0xCFE00000

#CROSS_COMPILE = arm-linux-
CROSS_COMPILE = arm-linux-gnueabi-

LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc

OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

PLATFORM_RELFLAGS += -fno-strict-aliasing  -fno-common -ffixed-r8 -msoft-float

PLATFORM_CPPFLAGS =
PLATFORM_CPPFLAGS += -DCONFIG_ARM -D__ARM__
PLATFORM_CPPFLAGS += -march=armv5t -marm

DBGFLAGS = -g # -DDEBUG
OPTFLAGS = -Os #-fomit-frame-pointer
RELFLAGS= $(PLATFORM_RELFLAGS)

CPPFLAGS := $(DBGFLAGS) $(OPTFLAGS) $(RELFLAGS) -D__KERNEL__ -DTEXT_BASE=$(TEXT_BASE) 

gccincdir := $(shell $(CC) -print-file-name=include)

CPPFLAGS += -Iinclude
CPPFLAGS += -fno-builtin -ffreestanding -nostdinc -isystem $(gccincdir) -pipe $(PLATFORM_CPPFLAGS)

AFLAGS := -D__ASSEMBLY__ $(CPPFLAGS)

CFLAGS := $(CPPFLAGS) -Wall -Wstrict-prototypes

export	CROSS_COMPILE LD CC OBJCOPY OBJDUMP
export	TEXT_BASE PLATFORM_CPPFLAGS PLATFORM_RELFLAGS CPPFLAGS CFLAGS AFLAGS

# lib_s3c6410
objs	= start.o cpu_init.o mem_init.o mmu_init.o clock_init.o uart_init.o nand_init.o stack_bss_init.o led.o 
objs	+= cpu.o serial.o d-tcm.o

# lib_arm
objs	+= _udivsi3.o _divsi3.o 
objs	+= div0.o armlinux.o

# lib_generic
objs	+= dlmalloc.o string.o

objs 	+= main.o

#########################################################################

all: $(objs)
	$(LD) -Ttext $(TEXT_BASE) -o u-boot.elf $^ 
	$(OBJCOPY) -O binary u-boot.elf u-boot.bin
	$(OBJDUMP) -D u-boot.elf > u-boot.dis

%.o:	%.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o u-boot.elf u-boot.bin u-boot.dis u-boot-movi.bin
