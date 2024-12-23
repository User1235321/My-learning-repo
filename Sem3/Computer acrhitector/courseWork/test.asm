section .data
  audio_dev db "/dev/audio",0

section .bss
  freq resd 1
  duration resd 1


section .text
  global _start

_start:
  ; Открываем /dev/audio
  mov eax, 5
  mov ebx, audio_dev
  mov ecx, 02
  mov edx, 0
  int 80h

  ; Настройка частоты (440 Гц - ля)
  mov dword [freq], 440

  ; длительность писка 
  mov dword [duration], 1000 ; миллисекунды

  ; Пишем данные в /dev/audio
  mov eax, 4 ; write
  mov ebx, eax; file descriptor
  mov ecx, freq ; pointer to data
  mov edx, 4  ; length of data
  int 80h

  ;Закрываем /dev/audio
  mov eax,6
  mov ebx,eax
  int 80h


  ; Завершение программы
  mov eax, 1
  xor ebx, ebx
  int 80h

