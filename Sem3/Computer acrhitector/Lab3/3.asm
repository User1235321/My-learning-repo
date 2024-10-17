global _start
section .data
  arr db 170, 171, 172, 173, 174, 175, 176, 177, 178, 179
  arr2 db 10 dup(180)
section .text
  print:
    mov eax, 4
    mov ebx, 1
    mov ecx, toPrint
    mov edx, 1
    int 0x80
    ret

  prePrint:
    mov bl, 10
    div bl
    mov [toPrint], ah
    push ax
    call print
    pop ax
    cmp ax, 0
    jnz prePrint
    ret
  printTemplate:
    mov rcx, 10
    fullPrint:
      mov ax, [rbx]
      add rbx, 1
      push rcx
      push rbx
      call prePrint
      mov ax, 32
      mov [toPrint], ax
      call print
      pop rbx
      pop rcx
      loop fullPrint
    mov ax, 10
    mov [toPrint], ax
    call print
    ret

  _start:
    mov rbx, arr
    call printTemplate
    mov rbx, arr2
    call printTemplate

    cld
    lea esi, arr
    lea edi, arr2
    mov ecx, 10
    rep movsb

    mov rbx, arr2
    call printTemplate

    mov eax, 1
    mov ebx, 0
    int 0x80

section .bss
  toPrint resb 1
