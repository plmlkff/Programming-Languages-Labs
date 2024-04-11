section .text

global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global read_char
global read_word
global parse_uint
global parse_int
global string_equals
global string_copy

;inp: rdi rsi rdx rcx r8 r9
;ret: rax and rdx

%define SYS_WRITE 1
%define SYS_READ 0
%define SYS_EXIT 60
%define OUTPUT_STREAM_DESCRIPTOR 1
%define INPUT_STREAM_DESCRIPTOR 0
%define TABULATION_CODE 0x9
 
; Принимает код возврата и завершает текущий процесс
exit:
    mov rax, SYS_EXIT
    syscall

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rax, rax
.loop:
    cmp byte [rdi + rax], 0
    je .end
    inc rax
    jmp .loop
.end:
    ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    push rdi
    call string_length
    pop rdi
    mov rdx, rax
    mov rsi, rdi
    mov rax, SYS_WRITE
    mov rdi, OUTPUT_STREAM_DESCRIPTOR
    syscall
    ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    mov rdi, "\n"
    jmp print_char
    
; Принимает код символа и выводит его в stdout
print_char:
    push rdi
    mov rsi, rsp
    mov rax, SYS_WRITE
    mov rdi, OUTPUT_STREAM_DESCRIPTOR
    mov rdx, 1
    syscall
    pop rdi
    ret

; Выводит знаковое 8-байтовое число в десятичном формате
print_int:
.check_rdx:
    test rdi, rdi
    jns print_uint
.process_neg:
    push rdi
    mov rdi, '-'
    call print_char
    pop rdi
    neg rdi
    jmp print_uint

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    mov r8, 10
    mov rax, rdi
    mov r9, rsp
    dec rsp
    mov byte[rsp], 0
    dec rsp
.loop:
    xor rdx, rdx
    div r8
    add rdx, '0'

    mov byte[rsp], dl
    dec rsp

    test rax, rax
    jne .loop

.end:
    inc rsp
    mov rdi, rsp
    push r9
    call print_string
    pop r9
    mov rsp, r9
    ret

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rdx, rdx

.loop:
    mov r8b, byte[rdi+rdx]
    cmp r8b, byte[rsi+rdx]
    jne .wrong

    cmp byte[rdi+rdx], 0
    je .success

    inc rdx
    jmp .loop

.success:
    mov rax, 1
    ret
.wrong:
    xor rax, rax
    ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    xor rax, rax
    dec rsp
    mov rdi, INPUT_STREAM_DESCRIPTOR
    mov rsi, rsp
    mov rdx, 1
    syscall
    cmp rax, -1
    je .null_terminator
    test rax, rax
    jne .end
.null_terminator:
    mov byte[rsp], 0

.end:
    mov al, byte[rsp]
    inc rsp
    ret

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
xor rdx, rdx
xor r8, r8

.wait_loop:
    push rsi
    push rdi
    push rdx
    call read_char
    pop rdx
    pop rdi
    pop rsi
    cmp rax, " "
    je .wait_loop
    cmp rax, TABULATION_CODE
    je .wait_loop
    cmp rax, "\n"
    je .wait_loop

.read_loop:
    cmp rdx, rsi
    ja .bad_end

    test rax, rax
    je .good_end
    cmp rax, 0x20
    je .good_end
    cmp rax, 0x9
    je .good_end
    cmp rax, 0xA
    je .good_end

    mov byte[rdi + rdx], al
    inc rdx
    push rsi
    push rdi
    push rdx
    call read_char
    pop rdx
    pop rdi
    pop rsi
    jmp .read_loop

.bad_end:
    mov rax, 0
    ret
.good_end:
    mov byte[rdi + rdx], 0
    mov rax, rdi
    ret
 

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
    xor rax, rax
    xor rsi, rsi
    xor r8, r8
    mov r9, 10

.load_char:
    mov r8b, byte[rdi+rsi]
    test r8b, r8b
    je .end
    sub r8b, '0'
    jl .end
    cmp r8b, 9
    jg .end
    mul r9
    add rax, r8
    inc rsi
    jmp .load_char
.end:
    mov rdx, rsi
    ret




; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
xor rax, rax
.load_val:
    cmp byte[rdi], '+'
    je .pos
    cmp byte[rdi], '-'
    je .neg
    cmp byte[rdi], '0'
    jl .Nan
    cmp byte[rdi], '9'
    jg .Nan
    jmp parse_uint
.neg:
    inc rdi
    call parse_uint
    neg rax
    inc rdx
    ret
.pos:
    inc rdi
    call parse_uint
    inc rdx
    ret
.Nan:
    xor rdx, rdx
    ret

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
    xor rax, rax
    xor r8, r8
    xor r9, r9
.loop:
    inc r8
    cmp r8, rdx
    jg .failed_end
    dec r8
    mov r9b, byte[rdi + r8]
    mov byte[rsi + r8], r9b
    test r9b, r9b
    je .end
    inc r8
    jmp .loop
.failed_end:
    xor r8, r8
.end:
    mov rax, r8
    ret
