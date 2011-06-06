; Floppy Disk Driver
; by eXerigumo Clanjor

;======================================================================
; Reset fd
; MODIFIES	ah, dl
fd_reset:
	xor		ah, ah		;`. ah = dl = 0
	xor		dl, dl		;/
	int		13h			; Reset floppy drive
	ret

;======================================================================
; Kill fd's motor
fd_kill_motor:
	; TODO
	ret

;======================================================================
; Read <ax> sector from fd into es:bx
; MODIFIES	ax, cx, dx
fd_read_sector:
	mov		dl, 18		;`. ax / 18 = al ...... ah
	div		dl			;/  18 sectors / track

	xor		dl, dl		; dl (DrvNo.) = 0 (floppy)

	mov		dh, al		;`. dh = al & 1
	and		dh, 1		;/  Track Head No.

	mov		cl, ah		;`. cl = ah + 1
	inc		cl			;/  Starting sector

	mov		ch, al		;`. ch = al >> 1, e.g. al / 2 = ch
	shr		ch, 1		;/  Track

.0:
	mov		ah, 2
	mov		al, 1
	int		13h
	jc		.0

	ret

;======================================================================
; Read 64KB datas from <ax> sector in fd into es:bx
; MODIFIES	ax, cx
fd_read_64k:
	mov		cx, 128		; 64KB -> 128 sectors
.read:
	push	ax
	push	cx

	call	fd_read_sector
	add		bx, 0200h		; 512 (200h) bytes per sector

	pop		cx
	pop		ax
	inc		ax				; ax++
	loop	.read

	ret

