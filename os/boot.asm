
;======================================================================
; Input key
; (Like the classical "Press any key to continue")
%macro	inkey		0
	xor		ah, ah
	int		16h
%endmacro

;======================================================================
; Init
	org		07c00h

	mov		ax, cs
	mov		ds, ax

	mov		ax, 0a000h		; video memory in graph mode
	mov		es, ax

	call	_start
	jmp		$

msg_err:		db	"Unable to init VGA!"
msg_err_len:	equ	$-msg_err
msg_done:		db	"Well done!"
msg_done_len:	equ	$-msg_done

;======================================================================
; Main routine
_start:
	; Init VGA
	;mov		ax, 04F02h
	;mov		bx, 0101h		; graph 640x480 256 colors
	;int		10h
	;cmp		ah, 0			; When succeeded, ah == 0
	;jne		.error			; When ah != 0, show error message

	inkey

	mov ax,012h    ;VGA mode
	int 10h        ;640 x 480 16 colors.

	mov dx,03C4h   ;dx = indexregister
	mov ax,0F02h   ;INDEX = MASK MAP,
	out dx,ax      ;write all the bitplanes.
	mov di,0       ;DI pointer in the video memory.
	mov cx,38400   ;(640 * 480)/8 = 38400
	mov ax,000h    ;write to every pixel.
	rep stosb      ;fill the screen

	mov		dh, 000h
.loop:
	mov		ax, 1010h
	mov		bx, 0h
	xor		cx, cx
	int		10h

	push	dx
	call	wait
	pop		dx
	inc		dh
	cmp		dh, 040h
	jne		.loop

	jmp		.done
.error:
	; Print msg_err
	mov		bp, msg_err
	mov		cx, msg_err_len
	mov		ax, 01301h
	mov		bx, 000ch
	mov		dx, 0000h		; at (00, 00)
	int		10h
	jmp		.gohome

.done:
	; Restore to text mode
	xor		ah, ah
	mov		al, 03h
	int		10h

	; Print msg_done
	mov		bp, msg_done
	mov		cx, msg_done_len
	mov		ax, 01301h
	mov		bx, 000ch
	mov		dx, 0000h		; at (00, 00)
	int		10h

.gohome:
	ret

;======================================================================
; Wait for some cpu cycles
wait:
	jmp		.better
	; first time...
	mov		cx, 0ffffh
.0:
	nop
	loop	.0

	; second time...
	mov		cx, 0ffffh
.1:
	nop
	loop	.1

	; third time...
.2:
	nop
	loop	.2
.better:
	mov		ah, 86h
	mov		cx, cs
	mov		dx, time
	int		15h

	ret
time:	dw	500000h

;======================================================================
; The end sign of a boot sector
	times	510-($-$$)	db	0
	dw		0xaa55

