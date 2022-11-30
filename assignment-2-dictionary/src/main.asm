%include "src/colon.inc"
%include "src/words.inc"
%include "src/lib.inc"

%define BUFFER_SIZE 256

extern find_word

section .rodata
    start_message: db "Enter key: ", 0
    result_message: db "Value: ", 0
    error_overflow_message: db "Overflow", 0
    error_not_found_message: db "Value not found", 0
    

section .bss
    buffer: resb BUFFER_SIZE

section .text
global _start
_start:
    mov rdi, start_message ;Init program
    call print_string

    mov rdi, buffer        ;Read input
    mov rsi, BUFFER_SIZE
    call read_word
   
    push rdx                ;save length

    test rax, rax           ;check if overflow
    jz .overflow

    mov rdi, rax            ;trying to find key in dict
    mov rsi, NEXT_ELEMENT   
    call find_word         
                          
    test rax, rax           ;check if key exist in dict
    jz .not_found           

    push rax                ;visual stuff
    mov rdi, result_message
    call print_string
    pop rax

    pop rdx                 ;get length

    mov rdi, rax            ;get and print value
    add rdi, 8
    add rdi, rdx
    inc rdi
    call print_string
    call print_newline
    mov rdi, 0  ;success
    call exit
.overflow:
    mov rdi, error_overflow_message
    jmp .error
.not_found:
    mov rdi, error_not_found_message
.error:
    call print_error
    call print_error_newline
    mov rdi, 1 ;fail
    call exit
    