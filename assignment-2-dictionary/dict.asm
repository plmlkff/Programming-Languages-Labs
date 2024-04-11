%include "lib.inc"

global find_word
global _start

%define NODE_POINTER_SIZE 8


section .text
find_word:
	mov rdx, rsi
	xor rax, rax
	mov r12, rdi ;  Using callee-saved registers
	mov r13, rsi ;  Contains next-node ley-value
	mov r14, rsi ;  Next node address
.loop:
	test r14, r14
	jz .not_found


	lea r13, [r14 + NODE_POINTER_SIZE] ;  skip next node pointer
	mov rdi, r12
	mov rsi, r13 ;  contains node's value pointer

	call string_equals

	test rax, rax
	jne .found
	
	mov r14, [r14] ;  put the address of the next node
	jmp .loop
.not_found:
        xor rax, rax
	ret
.found:
	mov rax, r13
	ret
	
