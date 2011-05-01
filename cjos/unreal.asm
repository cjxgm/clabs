; This code is used to enable
; Unreal Mode for accessing big memory.
;
; by eXerigumo Clanjor

%ifndef __UNREAL__
%define __UNREAL__

[bits 16]

enable_unreal:
	; Convert physical address of gdt_table to linear one.
	; For example, cs:bx = (cs<<4) + bx.
	xor		eax, eax
	mov		ax, ds
	shl		eax, 4
	add		eax, gdt_table
	mov		dword [gdt_ptr + 2], eax

	; Load gdt
	lgdt	[gdt_ptr]

	; Close interrupt
	cli

	; Enable A20
	in		al, 92h
	or		al, 00000010b
	out		92h, al

	; Enable protected Mode
	; e.g. set bit 0 of cr0
	mov		eax, cr0
	or		eax, 00000001b		; Set PE = 1
	mov		cr0, eax

	jmp		.0					; Flush jump
.0:
	; Set up segment registers
	mov		ax, flat_data_seg-gdt_table ; Selector
	mov		ds, ax
	mov		es, ax

	; Disable protected mode
	mov		eax, cr0
	xor		eax, 00000001b		; Flip PE, set it to 0
	mov		cr0, eax

	jmp		.1					; Flush jump
.1:
	ret
	
gdt_table:		dw		00000h, 00000h, 00000h, 00000h
flat_data_seg:	dw		0FFFFh, 00000h, 09200h, 000CFh
GDT_SIZE:		equ		$ - gdt_table

gdt_ptr:		dw		GDT_SIZE - 1, 0, 0

%endif

