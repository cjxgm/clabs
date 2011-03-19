ASM = nasm
ASMFLAGS = -f bin

write_to_floappy: bootldr.bin
	dd if=bootldr.bin of=floappy.img bs=512 count=1 conv=notrunc

bootldr.bin: bootldr.asm
	$(ASM) $(ASMFLAGS) -o $@ $<
