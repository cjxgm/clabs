[bits 16]

times 0100h		db	0				; This is a must for we load it into
									; xxxx:0000 but run it at xxxx:0100.

;----------------------------------------------------------------------
; Prepare
	mov		ax, cs
	mov		ds, ax
	mov		ss, ax
	mov		sp, stack_top

	call	screen_init

	jmp		_start

	jmp		$

;----------------------------------------------------------------------
; Datas here
stack:		times	1024	db	0
stack_top:	equ		$-1

%include 'macro.asm'
%include 'screen.asm'

;----------------------------------------------------------------------
; The main routine
_start:
	; Write a 'R'
	mov		al, 'R'
	mov		ah, 00001100b
	mov		[gs:(80*2 + 0)*2], ax
	inkey

	call	screen_clear
	call	screen_flip
	inkey

	xor		ax, ax
.0:
	xor		di, di
	mov		cx, 80*24
.fuck_screen:
	mov		[es:di], ax
	add		ax, 5
	inc		di
	inc		di
	loop	.fuck_screen

	mov		cx, 0000h
	mov		dx, 0000h
.1:
	cmp		dh, 79
	ja		.0
	;inkey
	call	screen_delay
	call	screen_delay
	call	screen_delay
	call	screen_delay
	call	screen_flip_rect
	inc		dh
	push	dx
	and		dh, 00010010b
	pop		dx
	jnz		.1
	inc		dl
	jmp		.1

	ret

;----------------------------------------------------------------------
times 64*1024 - ($-$$)	db	0		; Fill up 64k
