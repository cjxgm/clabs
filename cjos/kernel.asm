; Boot Sector
; by eXerigumo Clanjor

times 100h db 0
	jmp		_start

times 1024 db 0
STACK	equ		$

_start:
	mov		ax, cs
	mov		ds, ax
	mov		ss, ax
	mov		sp, STACK

	mov		ax, 0b800h			; vga
	mov		es, ax

	call 	cls

	xor		bx, bx

	mov		ah, 00001010b		; color
	mov		si, BootMsg
	mov		cx, BootMsgLen
	call	DispStr

abc:
	mov		al, '.'
	mov		[es:bx], ax
	add		bx, 2

	mov		dl, [es:80*2]
	inc		dl
	mov		[es:80*2], dl

	inc		ah
	cmp		ah, 00001111b
	jb		.4
	xor		ah, ah
.4:
	cmp		bx, 80*25*2
	jb		.3
	mov		bx, 10000b*2
.3:
	call	draw
	jmp		abc

; === Funcs ===

DispStr:
	mov		al, [si]
	mov		[es:bx], ax
	inc		si
	add		bx, 2
	loop	DispStr
	ret

cls:
	mov		cx, 80*25*2
.1:
	xor		ax, ax
	mov		bx, cx
	mov		[es:bx], ax
	loop	.1
	ret

draw:
	ret

; === Data ===
BootMsg					db		"Welcome!"
BootMsgLen				equ		$-BootMsg
x						dw		1
y						dw		1

times 1024 dw	0x1234

times	65536-($-$$)		db		0

