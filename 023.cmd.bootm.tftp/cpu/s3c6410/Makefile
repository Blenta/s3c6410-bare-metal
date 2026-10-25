
include $(TOPDIR)/config.mk

LIB	= lib$(SOC).a

SOBJS	= cpu_init.o mem_init.o mmu_init.o stack_bss_init.o reset.o start.o 

COBJS	= serial.o interrupts.o movi.o hs_mmc.o nand_cp.o nand.o cpu.o speed.o 

include $(TOPDIR)/target.mk
