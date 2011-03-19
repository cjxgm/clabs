
SECTION .data
	str1	db		"KANGAROO"
	str1len	equ		$-str1
SECTION .text

global _start
_start:
	nop
	; Code begins

	mov eax, str1
	mov ebx, str1len
MORE:
	add byte[eax], 32
	inc eax
	dec ebx
	jnz MORE


	; Code ends
	nop

SECTION .bss

