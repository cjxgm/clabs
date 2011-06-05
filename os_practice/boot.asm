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

msg_ready:		db	"Ready."
msg_ready_len:	equ	$-msg_ready

msg_boot:		db	"Booting..."
msg_boot_len:	equ	$-msg_boot

msg_inkey:		db	"Press any key to continue..."
msg_inkey_len:	equ	$-msg_inkey

;======================================================================
; Main routine
_start:
	; Clear screen
	cls

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

	; Print "done", print "press any key" and wait for key
	mov		ax, cs				;`. Point es to current cs
	mov		es, ax				;/
	print	msg_ready, 0, 1, 00001110b	; Print msg_ready at (0, 1) in
										; light yellow
	;print	msg_inkey, 0, 2, 00001001b
	;inkey

	jmp		KERNEL_BASE:KERNEL_OFFSET		; Let's go!

;======================================================================
; The end sign of a boot sector
	times	510-($-$$)	db	0
	dw		0xaa55

