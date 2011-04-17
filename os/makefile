ASM = nasm

debug: os
	qemu -boot a -fda floppy.img

os: boot_sector app

boot_sector: boot.bin floppy.img
	dd if=boot.bin of=floppy.img bs=512 count=1 conv=notrunc

app: app.bin floppy.img
	dd if=app.bin of=floppy.img seek=1 bs=512 count=1 conv=notrunc

floppy.img:
	dd bs=512 count=2880 if=/dev/zero of=$@

boot.bin: boot.asm fd.asm
	$(ASM) -o $@ $<

app.bin: app.asm
	$(ASM) -o $@ $<
