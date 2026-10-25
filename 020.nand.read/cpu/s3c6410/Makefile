
include $(TOPDIR)/config.mk

LIB	= lib$(SOC).a

SOBJS	= start.o cpu_init.o mem_init.o mmu_init.o clock_init.o uart_init.o nand_init.o stack_bss_init.o led.o 

COBJS	= cpu.o interrupts.o serial.o d-tcm.o speed.o hs_mmc.o nand_cp.o nand.o smdk6410.o

include $(TOPDIR)/target.mk

