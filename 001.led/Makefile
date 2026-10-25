all: start.o
	arm-linux-gnueabi-ld -Ttext 0x0c000000 -o led.elf start.o # 0x0c000000=Stepping Stone (BL1)
	arm-linux-gnueabi-objcopy -O binary led.elf led.bin
	arm-linux-gnueabi-objdump -D led.elf > led.dis

start.o : start.S
	arm-linux-gnueabi-gcc -c -o start.o start.S # -c --compile Compile or assemble the source files, but do not link.

clean:
	rm -f *.o led.elf led.bin led.dis


