
SECTION .data
SECTION .text

global _start
_start:
	nop
	; Code begins

	mov eax, 0abcdh

	mov ecx, 5
MORE:
	xchg ah, al
	dec ecx
	jnz MORE


	; Code ends
	nop

SECTION .bss

