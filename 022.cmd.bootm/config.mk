#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

#########################################################################

# clean the slate ...
PLATFORM_RELFLAGS =
PLATFORM_CPPFLAGS =
PLATFORM_LDFLAGS =

PLATFORM_CPPFLAGS += -DCONFIG_ARM -D__ARM__

PLATFORM_RELFLAGS += -fno-strict-aliasing  -fno-common -ffixed-r8 \
	-msoft-float

# Make ARMv5 to allow more compilers to work, even though its v6.
PLATFORM_CPPFLAGS += -march=armv5t -marm
# =========================================================================
#
# Supply options according to compiler version
#
# =========================================================================
#PLATFORM_CPPFLAGS +=$(call cc-option,-mapcs-32,-mabi=apcs-gnu)
PLATFORM_RELFLAGS +=$(call cc-option,-mshort-load-bytes,$(call cc-option,-malignment-traps,))

ifndef TEXT_BASE
TEXT_BASE = 0xCFE00000
endif

#########################################################################

CONFIG_SHELL	:= $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
		    else if [ -x /bin/bash ]; then echo /bin/bash; \
		    else echo sh; fi ; fi)

HOSTCC		= gcc
HOSTCFLAGS	= -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer
HOSTSTRIP	= strip

#########################################################################
#
# Option checker (courtesy linux kernel) to ensure
# only supported compiler options are used
#
cc-option = $(shell if $(CC) $(CFLAGS) $(1) -S -o /dev/null -xc /dev/null \
		> /dev/null 2>&1; then echo "$(1)"; else echo "$(2)"; fi ;)

#
# Include the make variables (CC, etc...)
#
AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)RANLIB

ARFLAGS = crv

RELFLAGS= $(PLATFORM_RELFLAGS)
DBGFLAGS= -g # -DDEBUG
OPTFLAGS= -Os #-fomit-frame-pointer

LDSCRIPT := $(TOPDIR)/board/$(BOARD)/u-boot.lds

OBJCFLAGS += --gap-fill=0xff

gccincdir := $(shell $(CC) -print-file-name=include)

CPPFLAGS := $(DBGFLAGS) $(OPTFLAGS) $(RELFLAGS)		\
	-D__KERNEL__ -DTEXT_BASE=$(TEXT_BASE)		\

CPPFLAGS += -I$(TOPDIR)/include
CPPFLAGS += -fno-builtin -ffreestanding -nostdinc 	\
	-isystem $(gccincdir) -pipe $(PLATFORM_CPPFLAGS)

CFLAGS := $(CPPFLAGS) -Wall -Wstrict-prototypes


# $(CPPFLAGS) sets -g, which causes gcc to pass a suitable -g<format>
# option to the assembler.
AFLAGS_DEBUG :=

AFLAGS := $(AFLAGS_DEBUG) -D__ASSEMBLY__ $(CPPFLAGS)

LDFLAGS += -Bstatic -T $(LDSCRIPT) -Ttext $(TEXT_BASE) $(PLATFORM_LDFLAGS)


#########################################################################

export	CONFIG_SHELL HPATH HOSTCC HOSTCFLAGS CROSS_COMPILE \
	AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP \
	MAKE
export	TEXT_BASE PLATFORM_CPPFLAGS PLATFORM_RELFLAGS CPPFLAGS CFLAGS AFLAGS
