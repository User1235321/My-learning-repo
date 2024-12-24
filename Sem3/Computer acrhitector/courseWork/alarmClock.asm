global _start

section .data
  curtime dq 0
  freq equ 500
  beep_sec equ 1
  beep_nsec equ 0

section .bss
  sleep_time: resb 16 ; for timespec

section .text
_start:
  readNum:
    mov rax, 3 ; systeam read
    mov rbx, 0 ; default input
    mov rcx, number
    mov rdx, 8 ; number of bytes
    int 0x80

    ; convert string to num
    mov rsi, number
    mov rax, 0
    read_loop:
      movzx rdx, byte [rsi] ; put in rdx byte from number
      cmp rdx, 0xA ; compare rdx with '\n'
      je end_read_loop ; if (rdx == '\n') end
      sub rdx, '0'
      imul rax, 10 ; rax = ( rax * 10 ) + rdx
      add rax, rdx
      inc rsi ; go to next number byte
      jmp read_loop
    end_read_loop:
    push rax

  main:
    ; get current time
    mov rax, 0xc9
    mov rdi, curtime
    syscall

    mov rdx, [curtime]
    pop rax
    add rdx, rax ; add to timer number from user

    timeloop:
      mov rax, 0xc9
      mov rdi, curtime
      syscall

      mov rax, [curtime]

      cmp rbx, rax
      jae afterPrint ; if a second has passed since the previous check, the output occurs

      print:
        push rax
        push rbx
        push rcx
        push rdx

        mov rbx, qword [curtime] ; rbx is now times
        mov rax, rdx ; rax is start time
        sub rax, rbx ; ras is time diff

        printBigNum:
          mov rcx, 0
          decomposition:
            mov rbx, 10
            mov rdx, 0
            div rbx                    ; rax = rdx:rax / rbx; rdx = rax % rbx
            push rdx
            add rcx, 1; ++rcx
            cmp rax, 0                 ; (rax > 0) ? (ZF = 1) : (ZF = 0)
            jnz decomposition         ; while (ZF != 1) call jmp decomposition
          outputLoop:
            pop rax
            add rax, '0'               ; char(9) is '\t', but char(9 + '0') is '9'
            push rcx
            basePrint:
              mov [number], rax
              mov rax, 4
              mov rbx, 1
              mov rcx, number
              mov rdx, 1
              int 0x80
            pop rcx
            loop outputLoop

        mov rax, 10 ; put '\n' in rax
        mov [number], rax
        mov rax, 4
        mov rbx, 1
        mov rcx, number
        mov rdx, 1
        int 0x80

        pop rdx
        pop rcx
        pop rbx
        pop rax

        mov rbx, rax

      afterPrint:
        cmp qword [curtime], rdx
        jb timeloop

  beep:
    mov rax, 2 ; open
    mov rdi, console_path ; /dev/console
    mov rsi, 0 ; O_RDONLY
    mov rdx, 0
    syscall
    mov r10, rax ; save descriptor in r10

    ; ioctl for sound generation
    mov rax, 16    ; ioctl
    mov rdi, r10    ; /dev/console
    mov rsi, 0x4B2F ; KIOCSOUND for ioctl
    mov rdx, freq   ; frequency
    syscall

    ; timespec
    mov qword [sleep_time], beep_sec
    mov qword [sleep_time + 8], beep_nsec

    ; pause
    mov rax, 35    ; syscall nanosleep
    mov rdi, sleep_time ; timespec
    mov rsi, 0
    syscall

    ; sound off
    mov rax, 16    ; ioctl
    mov rdi, r10    ; /dev/console
    mov rsi, 0x4B2F ; KIOCSOUND for ioctl
    mov rdx, 0      ; frequency adress 0
    syscall

    ; close /dev/console
    mov rax, 3    ; close
    mov rdi, r10
    syscall

  exit:
    mov rdi, 0
    mov rax, 60
    syscall

section .bss
  number resd 1

section .data
  console_path db "/dev/console",0
