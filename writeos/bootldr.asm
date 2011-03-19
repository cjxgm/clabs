[bits 16]
org 0x7c00

start:
	mov ax, cs
	mov ds, ax
	mov es, ax

	mov al, 0x3
	mov ah, 0
	int 0x10

	mov si, msg
	call print

	mov ah, 0
	int 0x16

	mov si, endmsg
	call print

	jmp $

print:
	pusha
	cont:
		lodsb
		or al, al
		jz dne
		mov ah, 0x0e
		mov bx, 0
		int 0x10
		jmp cont
	dne:
		popa
		ret

msg db "Boot OK!", 10, 13, "Press any key to continue...", 0
endmsg db 10, 13, "Welcome!", 10, 13, 0
times 510-($-$$) db 0
dw 0xaa55
