# seek = total blocks - 1025(SDHC) or 1(SD/MMC) - 1(Signature) - 16(BL1 8K)
dd if=d-tcm.bin of=/dev/sdb bs=512 count=4 seek=7743470
