
DA_32:		EQU		4000h	; 32-bit

DA_DPL0:	EQU		00h		; DPL = 0
DA_DPL1:	EQU		20h		; DPL = 1
DA_DPL2:	EQU		40h		; DPL = 2
DA_DPL3:	EQU		60h		; DPL = 3

DA_DR		EQU		90h		; Data, Readable
DA_DRW		EQU		92h		; Data, Readable & Writable
DA_DRWA		EQU		93h		; Data, Readable & Writable, Accessed
DA_C		EQU		98h		; Code
DA_CR		EQU		9Ah		; Code, Readable
DA_CC		EQU		9Ch		; Code, Confirming
DA_CCR		EQU		9Eh		; Code, Readable, Confirming

DA_LDT		EQU		82h		; Local Descriptor Table
DA_TaskGate	EQU		85h		; Task Gate
DA_386TSS	EQU		89h		; 386 Task State Stack ???
DA_386CGate	EQU		8Ch		; 386 Call Gate
DA_386IGate	EQU		8Eh		; 386 Int. Gate
DA_386TGate	EQU		8Fh		; 386 Trap Gate

SA_RPL0		EQU		0		;`.
SA_RPL1		EQU		1		; | RPL
SA_RPL2		EQU		2		; |
SA_RPL3		EQU		3		;/

SA_TIG		EQU		0		;`. TI
SA_TIL		EQU		4		;/

;----------------------------------------------------------------------
; usage: Descriptor Base, Limit, Attr
;        Base:  dd
;        Limit: dd (low 20 bits available)
;        Attr:  dw (lower 4 bits of higher byte are always 0)
%macro Descriptor 3
	dw	%2 & 0FFFFh							; Limit1
	dw	%1 & 0FFFFh							; Base1
	db	(%1 >> 16) & 0FFh					; Base2
	dw	((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)	; Attr1 + Limit2 + Attr2
	db	(%1 >> 24) & 0FFh					; Base3
%endmacro

;----------------------------------------------------------------------
; usage: Gate Selector, Offset, DCount, Attr
;        Selector:  dw
;        Offset:    dd
;        DCount:    db
;        Attr:      db
%macro Gate 4
	dw	(%2 & 0FFFFh)						; Offset1
	dw	%1									; Selector
	dw	(%3 & 1Fh) | ((%4 << 8) & 0FF00h)	; Attr
	dw	((%2 >> 16) & 0FFFFh)				; Offset2
%endmacro

