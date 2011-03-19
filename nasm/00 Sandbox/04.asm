
SECTION .data
snip:		db	'KANGAROO', 0
SECTION .text

global _start
_start:
	nop
	; Code begins

			mov eax, snip
MORE:
			add byte [eax], 32
			inc eax
			add byte[eax], 0
			jnz MORE

	; Code ends
	nop
	jmp $-1 ; Never go through

SECTION .bss

