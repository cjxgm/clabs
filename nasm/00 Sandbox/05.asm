
SECTION .data
snip:		db	'KANGAROO', 0
SECTION .text

global _start
_start:
	nop
	; Code begins

	mov ax, 12h
	mov bx, 56h
	mov cx, 9ah

	push ax
	push bx
	push cx
	pop ax
	pop bx
	pop cx

	; Code ends
	nop
	jmp $-1 ; Never go through

SECTION .bss

