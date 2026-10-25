TEXT_BASE = 0x0C000000

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

objs := start.o cpu_init.o clock_init.o uart_init.o mem_init.o
objs += led.o serial.o
objs += main.o

all: $(objs)
	$(LD) -Ttext 0x0C000000 -o mem_init.elf $^ # 0x0c000000=Stepping Stone (BL1)
	$(OBJCOPY) -O binary mem_init.elf mem_init.bin
	$(OBJDUMP) -D mem_init.elf > mem_init.dis

%.o:	%.S
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o mem_init.elf mem_init.bin mem_init.dis
