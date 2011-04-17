
fd_reset:
	xor		ah, ah		;`. ah = dl = 0
	xor		dl, dl		;/
	int		13h			; Reset floppy drive
	ret

fd_kill_motor:
	; TODO
	ret

fd_read_sector:
	; cl	amount
	; ax	from which sector
	; es:bx	data will be read into that

	mov		dl, 18		;`. ax / 18 = al ...... ah
	div		dl			;/  18 sectors / track

	xor		dl, dl		; dl (DrvNo.) = 0 (floppy)

	mov		ah, al		;`. dh = al & 1
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
