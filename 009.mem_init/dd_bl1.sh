# seek = total blocks - 1025(SDHC) or 1(SD/MMC) - 1(Signature) - 16(BL1 8K)
dd if=mem_init.bin of=/dev/sdb bs=512 count=3 seek=7743470
