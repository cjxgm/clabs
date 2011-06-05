[bits 16]

times 0100h		db	0				; This is a must for we load it into
									; xxxx:0000 but run it at xxxx:0100.

;----------------------------------------------------------------------
; Prepare
	mov		ax, cs
	mov		ds, ax

	mov		ax, 0b800h
	mov		gs, ax

	jmp		_start

;----------------------------------------------------------------------
; Datas here
%include 'pmode.inc'
%include 'macro.asm'

gdt:		dq	0					; Null entry
;						Base, Limit, Attr
gdt_code:	Descriptor	0,	0FFFFFh, DA_C  |DA_32	; 32-bit code seg.
gdt_data:	Descriptor	0,	0FFFFFh, DA_DRW|DA_32	; 32-bit data seg.
gdt_len:	equ	$ - gdt

gdt_desc:	dw	gdt_len - 1
			dd	(KERNEL_BASE<<4) + gdt

selector_code:	equ	gdt_code - gdt	; 08h * 1
selector_data:	equ	gdt_data - gdt	; 08h * 2

;----------------------------------------------------------------------
; The main routine
_start:
	mov		al, 'R'
	mov		ah, 00001100b
	mov		[gs:(80*2 + 0)*2], ax

	; Prepare protected mode
	cli								; Disable interrupts
	lgdt	[gdt_desc]				; Load gdt descriptor!

	mov		eax, cr0				;`. Set the first bit of cr0,
	or		eax, 1					; | and we are now in protected
	mov		cr0, eax				;/  mode! ^_^

	; Flush CS by doing a far jump
	jmp		dword selector_code:(flush+(KERNEL_BASE<<4))

[bits 32]

flush:
	mov		ax, selector_data
	mov		ds, ax
	mov		ss, ax
	mov		esp, 090000h

	mov		byte [0b8000h + 2*(80*2 + 1) + 0], 'P'
	mov		byte [0b8000h + 2*(80*2 + 1) + 1], 00001100b
	
	jmp		$+(KERNEL_BASE<<4)

;----------------------------------------------------------------------
times 64*1024 - ($-$$)	db	0		; Fill up 64k
