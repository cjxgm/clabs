mov		bl, ah
mov		bh, al

mov		al, [11111111b]
mov		ah, [11111111b]
mov		ax, [11111111b]
mov		bx, [11111111b]
mov		cx, [11111111b]
mov		dx, [11111111b]
mov		[11111111b], al
mov		ah, 11000011b
mov		dx, 1100001111111111b

nop
