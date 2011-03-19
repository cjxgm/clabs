ASM = nasm
ASMFLAGS = -f bin

myfirst: myfirst_prepare
	qemu -fda myfirst.flp

myfirst_prepare: myfirst.bin
	dd status=noxfer conv=notrunc if=myfirst.bin of=myfirst.flp

myfirst.bin: myfirst.asm
	$(ASM) $(ASMFLAGS) -o $@ $<
