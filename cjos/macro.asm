; Macros and constants

%ifndef __MACRO__
%define __MACRO__

;======================================================================
KERNEL_BASE:	equ	08000h
KERNEL_OFFSET:	equ	0100h

APP_BASE:		equ	09000h
APP_OFFSET:		equ	0100h

;======================================================================
; inkey
; DESCRIPTION	Input key
; 				(Like the classical "Press any key to continue")
; MODIFIES		ah
; RETURN		ax
%macro	inkey		0
	xor		ah, ah
	int		16h
%endmacro

;======================================================================
; print
; DESCRIPTION	Print message
; ARG	%1		msg
;				CAUTION: There must be %1_len
; ARG	%2		x
; ARG	%3		y
; ARG	%4		color
; MODIFIES		bp, cx, dx, bx, ax
%macro	print	4
	mov		bp, %1
	mov		cx, %1_len

	mov		dl, %2				; x
	mov		dh, %3				; y
	mov		bl, %4				; color

	mov		ax, 01301h
	xor		bh, bh
	int		10h
%endmacro

%endif

