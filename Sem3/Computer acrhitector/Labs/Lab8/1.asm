section .data
    old_handler: times 8 dq 0  ; Хранилище для старого обработчика (64 бита)
    msg_caught: db "SIGTRAP caught!", 0xa
    msg_len equ $ - msg_caught

section .text
global _start
  handler_place:
    mov [toPrint], ax	; put ax in toPrint
    mov eax, 4		; sys call for sys_write
    mov ebx, 1		; file descriptor 1 (stdout)
    mov ecx, toPrint	; pointer to the character to print
    mov edx, 1		; length of the output
    int 0x80
    mov eax, 3		; sys_read
    mov ebx, 0		; standart output
    int 0x80		; call kernel
    ret

  _start:
    mov rax, 145 ; sys_rt_sigaction
    mov rdi, 5 ; SIGTRAP
    xor rsi, rsi ; NULL - не используем старую структуру sigaction
    mov rdx, handler_place
    xor rcx, rcx ; flags = 0 (без дополнительных флагов)
    mov r8, old_handler ; адрес для хранения старого обработчика
    syscall

;    pushf          ; push flags to stack
;    pop rax        ; put flags from stack to eax
;    or rax, 0x100  ; TF = 1
;    push rax       ; put eax to stack
;    popf          ; get flags from stack

;    mov ax, 100
;    add ax, 2
;    add ax, 3

;`    pushf;Disable TF
;    pop rax
;    and rax, 0xFEFF
;    push rax
;    popf
    mov ax, 120
;    int 3
    mov eax, 1		; sys call for sys_exit
    mov ebx, 0		; exit status
    int 0x80		; call kernel

section .bss
  toPrint resb 1
