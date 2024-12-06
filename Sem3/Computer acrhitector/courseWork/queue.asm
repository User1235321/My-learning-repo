section .data
section .text
    global _start

_start:

  mov rax, 60 ; sys_exit
  xor rdi, rdi
  syscall
