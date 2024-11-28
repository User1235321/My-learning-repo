section .data
    old_handler: times 8 dq 0  ; Хранилище для старого обработчика (64 бита)
    msg_caught: db "SIGINT caught!", 0xa
    msg_len equ $ - msg_caught

section .text
    global _start

    ; Обработчик сигнала SIGINT
    signal_handler:
        mov rax, 1 ; sys_write
        mov rdi, 1 ; stdout
        mov rsi, msg_caught
        mov rdx, msg_len
        syscall

        ; Возвращаемся в основной поток.  Важно, чтобы обработчик не завершал программу напрямую, если это не требуется.
        mov rax, 0x3c ; sys_exit
        xor rdi, rdi ; exit code 0
        syscall

    _start:
        ; Сохраняем старый обработчик SIGINT
        mov rax, 145 ; sys_rt_sigaction
        mov rdi, 2 ; SIGINT
        xor rsi, rsi ; NULL - не используем старую структуру sigaction
        mov rdx, signal_handler
        xor rcx, rcx ; flags = 0 (без дополнительных флагов)
        mov r8, old_handler ; адрес для хранения старого обработчика
        syscall

        ; Основной цикл программы (например, бесконечный цикл)
        loop:
            pause ; Ждем прерывания
            jmp loop

        ; Конец программы (эта строка не будет достигнута в случае обработки SIGINT)
        mov rax, 60
        xor rdi, rdi
        syscall
