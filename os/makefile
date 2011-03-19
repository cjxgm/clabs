ASM = nasm

debug: os
	qemu -fda floppy.img

os: boot_sector

boot_sector: boot.bin floppy.img
	dd if=boot.bin of=floppy.img bs=512 count=1 conv=notrunc

floppy.img:
	dd bs=512 count=2880 if=/dev/zero of=$@

boot.bin: boot.asm
	$(ASM) -o $@ $<
