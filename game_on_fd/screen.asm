
; Screen Management Module
; by eXerigumo Clanjor

;----------------------------------------------------------------------
; Initialize gs and es, make gs -> video memory, es -> off-screen
; buffer.
screen_init:
	push	ax

	mov		ax, 0b800h
	mov		gs, ax

	mov		ax, 04000h
	mov		es, ax

	pop		ax
	ret

;----------------------------------------------------------------------
; Clear off-screen buffer
screen_clear:
	push	ax
	push	di
	push	cx

	xor		ax, ax					;`.	ax = 0;
	xor		di, di					; |	for (di=0,cx=80*24; cx; cx--,di+=2)
	mov		cx, 80*24				; |		es[di] = ax;
	repne	stosw					;/

	pop		cx
	pop		di
	pop		ax
	ret

;----------------------------------------------------------------------
; Make off-screen buffer visible (e.g. copy them to the video memory)
screen_flip:
	push	ax
	push	di
	push	cx

	xor		di, di					;`.
	mov		cx, 80*24				; |	di = 0;
.0:									; |
	mov		ax, word [es:di]		; |	for (cx=80*24; cx; cx--) {
	mov		word [gs:di], ax		; |		gs[di] = es[di];
	inc		di						; |		di += 2;
	inc		di						; |	}
	loop	.0						;/

	pop		cx
	pop		di
	pop		ax
	ret

;----------------------------------------------------------------------
; Make off-screen buffer visible (e.g. copy them to the video memory)
; But, only copy the rect from (x0, y0) to (x1, y1).
; ch	x0
; cl	y0
; dh	x1
; dl	y1
screen_flip_rect:
	push	ax
	push	bx
	push	cx
	push	dx

.0:
	cmp		cl, dl					;`.	for (; cl<=dl; cl++) {
	ja		.0_end					;/
	push	cx
.1:
	cmp		ch, dh					;`.		for (; ch<=dh; ch++) {
	ja		.1_end					;/

	xor		bh, bh					;`.
	mov		bl, cl					; |			// bx = (cl * 80 + ch) * 2
	mov		ax, bx					; |			bx = cl; ax = bx;
	shl		bx, 6					; |			bx <<= 6; ax <<= 4;
	shl		ax, 4					; |			bx += ax;
	add		bx, ax					; |			ax = ch;
	xor		ah, ah					; |			bx += ax;
	mov		al, ch					; |			bx <<= 1;
	add		bx, ax					; |
	shl		bx, 1					;/

	mov		ax, word [es:bx]		;`.			gs[bx] = es[bx]
	mov		word [gs:bx], ax		;/

	inc		ch						;`.		}
	jmp		.1						;/
.1_end:
	pop		cx
	inc		cl						;`.	}
	jmp		.0						;/
.0_end:
	pop		dx
	pop		cx
	pop		bx
	pop		ax
	ret

disp_al:
	push	ax
	push	bx

	mov		bl, al

	and		al, 0F0h
	shr		al, 4
	cmp		al, 9
	jg		.0
	add		al, '0'
	jmp		.1
.0:	add		al, 'A' - 10
.1:	mov		ah, 00001100b
	mov		word [gs:0], ax

	mov		al, bl
	and		al, 00Fh
	cmp		al, 9
	jg		.2
	add		al, '0'
	jmp		.3
.2:	add		al, 'A' - 10
.3:	mov		ah, 00001100b
	mov		word [gs:2], ax

	pop		bx
	pop		ax
	ret

screen_delay:
	mov		cx, 0FFFFh
.0:
	%rep	126
	nop
	%endrep
	loop	.0
	ret
