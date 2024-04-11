%include "words.inc"
%include "lib.inc"
%include "dict.inc"

global _start

%define BUFFER_SIZE 256
%define NULL_TERMINATOR_SIZE 1
%define NODE_POINTER_SIZE 8

section .rodata
can_not_read_error: db "Error while reading from input stream!", 0
word_not_found_error: db "The entered word is not included in the list!", 0

section .bss
buffer: resb BUFFER_SIZE

section .text

;  Выводит значение из словаря, найденное по ключу
;  rdi -- содержит адрес вхождения в нужный узел
print_word_from_linked_list:
	push rdi
	call string_length
	pop rdi
	lea rdi, [rdi + rax + NULL_TERMINATOR_SIZE]
	call print_string
	call print_newline
	ret
_start:
.read_line:
	mov rdi, buffer
	mov rsi, BUFFER_SIZE-NULL_TERMINATOR_SIZE
	call read_line
	test rax, rax
	jz .can_not_read
	
.try_to_find:	
	mov rdi, rax
	mov rsi, NEXT_NODE
	call find_word
	test rax, rax
	jz .word_not_found
.word_found:
	mov rdi, rax
	call print_word_from_linked_list
	call exit
.word_not_found:
	mov rdi, word_not_found_error
	call print_error
	call print_newline
	call exit
.can_not_read:
	mov rdi, can_not_read_error
	call print_error
	call print_newline
	call exit
