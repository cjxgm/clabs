
SECTION .data
sss:	db		"Hello", 0
SECTION .text

global _start
_start:
	nop
	; Code begins

	mov eax, sss
	xor ebx, ebx	  ; Clear EBX
loop:
	mov bl, [eax]
	inc eax
	add byte [eax], 0 ; test if it's zero
	jnz loop		  ; Not zero? Loop!
	xor ebx, ebx	  ; Clear EBX

	; Code ends
	nop
	jmp $-1 ; Never go through

SECTION .bss

