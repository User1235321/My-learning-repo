section .data
    freq    equ 500         ; Частота звука (Гц)
    delay_sec equ 0        ; Задержка в секундах
    delay_nsec equ 500000000  ; Задержка в наносекундах (0.5 секунды)

section .bss
    sleep_time:  resb 16      ; Зарезервировать место для структуры timespec

section .text
    global _start

_start:
    ; 1. Открыть /dev/console
    mov rax, 2     ; syscall open
    mov rdi, console_path ; путь к /dev/console
    mov rsi, 0     ; флаг O_RDONLY
    mov rdx, 0     ; режим не используется
    syscall
    mov r10, rax   ; Сохраняем файловый дескриптор в r10

    cmp rax, 0     ; Проверка на ошибку
    jl  exit     ; Если ошибка - выход

    ; 2. Вызов ioctl для генерации звука
    mov rax, 16    ; syscall ioctl
    mov rdi, r10    ; файловый дескриптор /dev/console
    mov rsi, 0x4B2F ; KIOCSOUND код ioctl
    mov rdx, freq   ; адрес частоты
    syscall

    ; 3. Заполнение структуры timespec
    mov qword [sleep_time], delay_sec
    mov qword [sleep_time + 8], delay_nsec

    ; 4. Пауза
    mov rax, 35    ; syscall nanosleep
    mov rdi, sleep_time ; Указатель на структуру timespec
    mov rsi, 0      ; Для остатка от времени (не используется)
    syscall

    ; 5. Выключение звука (гарантированное выключение)
    mov rax, 16    ; syscall ioctl
    mov rdi, r10    ; файловый дескриптор /dev/console
    mov rsi, 0x4B2F ; KIOCSOUND код ioctl
    mov rdx, 0      ; адрес частоты 0
    syscall

    ; Дополнительная пауза для гарантированного отключения звука
    mov rax, 35    ; syscall nanosleep
    mov rdi, sleep_time ; Указатель на структуру timespec
    mov rsi, 0      ; Для остатка от времени (не используется)
    syscall

    ; 6. Закрыть /dev/console
    mov rax, 3    ; syscall close
    mov rdi, r10
    syscall

exit:
    ; Выход из программы
    mov rax, 60   ; syscall exit
    mov rdi, 0
    syscall

section .data
    console_path db "/dev/console",0
