;======================================================================
; Init
	org		07c00h

	mov		ax, cs
	mov		ds, ax

	mov		ax, 0b800h	;`. Video memory
	mov		gs, ax		;/

	call	_start
	jmp		$

;======================================================================
; Some datas and includes
%include	"macro.asm"
%include	"fd.asm"

msg_err:		db	"Failed!"
msg_err_len:	equ	$-msg_err

msg_done:		db	"Well done!"
msg_done_len:	equ	$-msg_done

msg_boot:		db	"Booting..."
msg_boot_len:	equ	$-msg_boot

;======================================================================
; Main routine
_start:
	; Print "booting"
	mov		ax, cs				;`. Point es to current cs
	mov		es, ax				;/
	print	msg_boot, 0, 0, 00001010b	; Print msg_boot at (0, 0) in
										; light green

	; Read datas
	mov		ax, KERNEL_BASE		;`.
	mov		es, ax				; | Destination ex:bx
	xor		bx, bx				;/

	mov		ax, 1				; Read from the 2nd sector
	call	fd_read_64k

	; Print "done" and wait for key
	mov		ax, cs				;`. Point es to current cs
	mov		es, ax				;/
	print	msg_done, 0, 1, 00001110b	; Print msg_done at (0, 1) in
										; light yellow

	inkey

	call	vga

	jmp		KERNEL_BASE:KERNEL_OFFSET		; Let's go!

;======================================================================
; A boring animation
vga:
	mov ax,012h    ;VGA mode
	int 10h        ;640 x 480 16 colors.

	mov		ax, 0a000h		; video memory in graph mode
	mov		es, ax

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
	cmp		dh, 042h
	jne		.loop

	jmp		.done
.error:
	mov		ax, cs
	mov		es, ax
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

.gohome:
	ret

;======================================================================
; Wait for some cpu cycles
wait:
	mov		ah, 86h
	mov		cx, cs
	mov		dx, time
	int		15h
	int		15h
	int		15h
	int		15h
	int		15h
	int		15h

	ret
time:	dw	500000h

;======================================================================
; The end sign of a boot sector
	times	510-($-$$)	db	0
	dw		0xaa55

