%include "src/lib.inc"
section .text
global find_word
find_word:
    ;rdi - key pointer
    ;rsi - list pointer
.loop: 
    add rsi, 8          ;get label
    push rdi
    push rsi
    call string_equals  ;check if 
    pop rsi
    pop rdi
    sub rsi, 8

    test rax, rax       ;check if string equals
    jnz .success        

    mov rsi, [rsi]      ;get next element
    test rsi, rsi       ;null check
    jnz .loop 
.fail:
    xor rax, rax
    ret
.success:
    mov rax, rsi
    ret
