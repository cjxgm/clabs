ASM = nasm

debug: cjos
	qemu -boot a -fda floppy.img

clean:
	rm -f *.bin
	rm -f floppy.img

cjos: boot_sector kernel

boot_sector: boot.bin floppy.img
	dd if=boot.bin of=floppy.img bs=512 count=1 conv=notrunc

kernel: kernel.bin floppy.img
	dd if=kernel.bin of=floppy.img seek=1 bs=512 count=128 conv=notrunc

floppy.img:
	dd bs=512 count=2880 if=/dev/zero of=$@

boot.bin: boot.asm fd.asm const.asm
	$(ASM) -o $@ $<

kernel.bin: kernel.asm
	$(ASM) -o $@ $<
