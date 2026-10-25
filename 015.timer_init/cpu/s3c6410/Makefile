
include $(TOPDIR)/config.mk

LIB	= lib$(SOC).a

SOBJS	= cpu_init.o mem_init.o mmu_init.o clock_init.o uart_init.o led.o start.o 

COBJS	= cpu.o interrupts.o serial.o d-tcm.o speed.o

include $(TOPDIR)/target.mk

