%define SYSCALL_EXIT 60
%define STDIN 0
%define STDOUT 1
%define STDERR 2
%define SYSCALL_WRITE 1
%define SYSCALL_READ 0
section .text

print:
    mov rdi, STDOUT
    mov rax, SYSCALL_WRITE
    syscall
    ret

; Принимает код возврата и завершает текущий процесс
exit: 
    mov rax, SYSCALL_EXIT
    syscall

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rax, rax
.loop:
    cmp byte[rdi+rax], 0
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
    mov rsi, rdi
    mov rdx, rax
    mov rax, 1
    call print
    ret
    
; Принимает код символа и выводит его в stdout
print_char:
    push rdi
    mov rsi, rsp
    mov rdx, 1
    call print
    pop rdi
    ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    mov rdi, 0xA
    jmp print_char

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    mov rax,rdi
    mov r8, 10  
    push word 0
    mov r9, rsp
.loop:
    dec rsp
    xor rdx, rdx
    div r8
    add dl, '0'
    mov [rsp], dl
    test rax, rax
    jne .loop
.end:
    mov rdi, rsp
    push r9
    call print_string
    pop r9
    mov rsp, r9
    pop di
    ret

; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
    test rdi, rdi
    jns print_uint
    push rdi
    mov rdi, '-'
    call print_char
    pop rdi
    neg rdi
    jmp print_uint

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rax, rax
    xor rcx, rcx
.loop:
    mov dl, byte[rdi + rcx]
    cmp dl, byte[rsi + rcx]
    jnz .end
    inc rcx
    cmp dl, 0
    jnz  .loop
    inc rax
.end:
    ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    push word 0 
    mov rsi, rsp   
    mov rdx, 1
    mov rax, SYSCALL_READ
    mov rdi, STDIN
    syscall
    pop ax
    ret 

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор
read_word:
    push r12
    push r13
    push r14
    mov r12, rdi ;buffer address
    mov r13, rsi ;buffer length
    xor r14, r14 ;counter
.loop:
    call read_char
    cmp	rax, 0x20
	jz	.space_check
	cmp	rax, 0x9
	jz	.space_check
	cmp	rax, 0xA
	jz	.space_check
    cmp rax, 0
    jz .end
    cmp r14, r13
    jae .fail
    mov byte[r12 + r14], al
    inc r14
    jmp .loop
.space_check:
    test r14, r14
    jz .loop
.end: 
    mov byte[r12 + r14], 0
    mov rax, r12
    mov rdx, r14
    pop r12
    pop r13
    pop r14
    ret
.fail:
    mov rax, 0
    pop r12
    pop r13
    pop r14
    ret

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
;rdi - str pointer
parse_uint:
    xor rax, rax
    xor rcx, rcx
    xor r9, r9
    mov r8, 10
.loop:
    mov cl, byte[rdi + r9]
    cmp cl, '0'
    cmp cl, 0x30
    js .end
    cmp cl, 0x40
    jns .end
    sub rcx, '0'
    inc r9
    mul r8
    add rax, rcx
    jc .fail
    test rdx,rdx
    jz .loop      
.fail:
    xor rdx, rdx
    ret
.end:
    mov rdx, r9
    ret

; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    mov cl, byte[rdi]
    cmp cl, '-'
    jnz .notsigned
    inc rdi
    call parse_uint
    neg rax
    inc rdx
    ret
.notsigned:
    jmp parse_uint

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
;rdi - str pointer
;rsi - buf pointer
;rdx - buf length
string_copy:
    push rdi
    push rsi
    push rdx
    call string_length
    pop rdx
    pop rsi
    pop rdi
    cmp rax, rdx
    jae .fail
    xor rax, rax
.loop:
    mov cl, byte[rdi + rax]
    mov byte[rsi + rax], cl
    inc rax
    cmp cl, 0
    jnz .loop   
    ret
.fail:
    xor rax, rax
    ret

    