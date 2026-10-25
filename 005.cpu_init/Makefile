
objs := start.o led.o cpu_init.o

all: $(objs)
	arm-linux-gnueabi-ld -Ttext 0x0c000000 -o cpu_init.elf $^ # 0x0c000000=Stepping Stone (BL1)
	arm-linux-gnueabi-objcopy -O binary cpu_init.elf cpu_init.bin
	arm-linux-gnueabi-objdump -D cpu_init.elf > cpu_init.dis

%.o : %.S
	arm-linux-gnueabi-gcc -c -o $@ $< # -c --compile Compile or assemble the source files, but do not link.

clean:
	rm *.o cpu_init.elf cpu_init.bin cpu_init.dis
