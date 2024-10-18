global _start
section .data
section .text
  _start:

    add eax, ebx
    add eax, [ebx]
    add eax, [ebx + esi]
    add eax,[ebx + esi + 12]
    mov eax, 1
    mov ebx, 0
    int 0x80

section .bss
