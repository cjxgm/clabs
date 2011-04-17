; Boot Sector
; by eXerigumo Clanjor

	org		07c00h

APP_BASE	equ		09000h
APP_OFFSET	equ		0100h

	jmp		_start

%include	"fd.asm"

_start:
	; Init segment registers
	mov		ax, cs
	mov		ds, ax

	mov		ax, 0b800h			; vga
	mov		gs, ax

	call 	cls

	xor		bx, bx				; bx = 0
	mov		ah, 00001010b		; color
	mov		si, boot_msg
	mov		cx, boot_msg_size
	call	disp_str

	mov		cx, 128
.0:
	call	fd_reset
	mov		ax, APP_BASE
	mov		es, ax
	mov		bx, APP_OFFSET
	add		bx, [p]
	mov		ax, cx
	call	fd_read_sector
	call	fd_kill_motor

	add		word [p], 512

	mov		ah, 00001010b		; color
	mov		bx, boot_msg_size*2
	mov		si, dot
	call	disp_str

	loop	.0

	jmp		APP_BASE:APP_OFFSET

	jmp		$

; === Funcs ===

disp_str:
	; ah: color
	; bx: Position
	; cx: Length
	; si: The string
	mov		al, [si]
	mov		[gs:bx], ax
	inc		si
	add		bx, 2
	loop	disp_str
	ret

cls:
	mov		cx, 80*25*2
.1:
	xor		ax, ax
	mov		bx, cx
	mov		[gs:bx], ax
	loop	.1
	ret

; === Data ===
boot_msg					db		"Booting"
boot_msg_size				equ		$ - boot_msg
dot							db		"."
p							dw		0

; === Signature ===
times	510-($-$$)		db		0
dw		0xaa55

