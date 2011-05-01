
;======================================================================
; Convert position to memory address
%define pos(x,y) ((y<<8)+(y<<6)+x)

;======================================================================
times	100h	db	0		; This is a must

;======================================================================
	; Set up registers
	mov		ax, cs
	mov		ds, ax
	mov		es, ax
	mov		fs, ax

	mov		ss, ax			;`. Set up stack
	mov		sp, stack_top	;/

	mov		ax, 0a000h		;`. Set up video adapter
	mov		gs, ax			;/

	call	_start

	jmp		$				; Loop forever

;======================================================================
; The stack
stack:		times 1024	db	0
stack_top:	equ			$-stack

%include	"macro.asm"
%include	"unreal.asm"

;======================================================================
; The main routine
_start:
	call	enable_unreal

	; Set up VGA
	xor		ah, ah
	mov		al, 13h			; 320x200 256 colors mode
	int		10h

	; Plot pixels
	xor		al, al
.0:
	mov		cx, 320*100
	xor		bp, bp
.loop0:
	mov		byte [gs:bp], al			; Red
	inc		bp
	inc		al
	cmp		al, 16
	jb		.cont_loop0
	xor		al, al
.cont_loop0:
	loop	.loop0

	mov		byte [gs:pos(10,20)], 4

	; Wait for a key
	inkey
	cmp		al, 27
	je		.done
	cmp		al, 32
	je		.next
	mov		al, ah
	jmp		.0

.next:
	mov		ax, 10		;`.
	mov		bx, 20		; | Draw a line from
	mov		cx, 70		; | (10, 20) to (70, 40).
	mov		dx, 40		; |
	call	draw_line	;/
	inkey

.done:
	; Return to normal mode
	xor		ah, ah
	mov		al, 03h			; Normal mode
	int		10h

	ret


draw_line:
	mov		cx, 200
	mov		bx, 0
.0:
	mov		bx, cx
	shl		bx, 8
	mov		ax, cx
	shl		ax, 6
	add		bx, ax
	add		bx, cx
	mov		byte [gs:bx], 4
	loop	.0
	ret
