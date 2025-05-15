section .data
    arg0 db "/usr/bin/showkey", 0  ; Путь к showkey (может отличаться)
    arg1 db "-s", 0
    arg2 db "-a", 0

    argv dd arg0, arg1, 0
    argv2 dd arg0, arg2, 0
section .text
    global _start

_start:
    ; fork
    mov rax, 57 ; sys_fork
    syscall

    ; Дочерний процесс (pid == 0)
    test rax, rax
    jz child

parent:
    ; execve(filename, argv, envp)
    mov eax, 11        ; System call number for execve
    mov ebx, arg0       ; Address of the command string
    mov ecx, argv2   ; command line arguments
    xor edx, edx       ; No environment variables
    int 0x80

    ; Код, который никогда не выполнится после успешного execve
    mov rax, 60 ; sys_exit
    xor rdi, rdi
    syscall


child:
    mov eax, 11        ; System call number for execve
    mov ebx, arg0       ; Address of the command string
    mov ecx, argv   ; command line arguments
    xor edx, edx       ; No environment variables
    int 0x80

    mov rax, 60 ; sys_exit
    xor rdi, rdi
    syscall
