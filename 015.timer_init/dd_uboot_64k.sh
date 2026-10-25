# seek = total blocks - 1025(SDHC) or 1(SD/MMC) - 1(Signature) - 16(BL1 8K) - 128(BL2 64K)
#dd if=u-boot-movi.bin of=/dev/sdb bs=512 count=145 seek=250734 #128M 64K
dd if=u-boot-movi.bin of=/dev/sdb bs=512 count=145 seek=7743342 #4G 64K
