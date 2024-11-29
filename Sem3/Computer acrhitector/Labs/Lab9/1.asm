section .data
    device db "/dev/input/event0", 0  ; Устройство ввода (может быть другое)
    buf resb 24                        ; Буфер для хранения события

section .text
    global _start
_start:
    ; Открываем устройство
    mov rax, 2                        ; syscall: open
    lea rdi, [device]                 ; указатель на строку
    mov rsi, 0                        ; O_RDONLY
    syscall                            ; вызов

    ; Сохраняем ответ (дескриптор файла) в rdi
    mov rdi, rax                      ; сохраняем файловый дескриптор

read_loop:
    ; Читаем событие
    mov rax, 0                        ; syscall: read
    mov rsi, buf                      ; указатель на буфер
    mov rdx, 24                       ; размер буфера
    syscall                            ; вызов

    mov eax, 4			; sys call for sys_write
    mov ebx, 1			; file descriptor 1 (stdout)
    mov ecx, buf		; pointer to the character to print
    mov edx, 24			; length of the output
    int 0x80; call kernel

    jmp read_loop                     ; продолжаем чтение

end_program:
    ; Закрываем устройство
    mov rax, 3                        ; syscall: close
    syscall                            ; вызов

    ; Завершение программы
    mov rax, 60                       ; syscall: exit
    xor rdi, rdi                      ; статус 0
    syscall                            ; вызов
