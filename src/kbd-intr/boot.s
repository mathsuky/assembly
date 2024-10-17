.code16
.text
  movw %cs,     %ax
  movw %ax,     %ds
  movw %ax,     %ss
  movl $0xFFF0, %esp
  movl $0xFFF0, %ebp
  movw $handler, %ax
  addw $0x7C00, %ax
  movw %ax,     4*9
  movw %cs,     4*9+2
  sti
1:  hlt; jmp 1b
handler:
  cli; pusha;
  call wait_OBF_1
  movw $0x60,   %dx
  inb %dx,      %al
  test $0x80,   %al
  jnz handler_exit
  movzbw %al,   %bx
  addw $0x7C00, %bx
  movb keymap(%bx), %al
  movb $0x0E, %ah
  int  $0x10
handler_exit:
  movb $0x20, %al
  outb %al, $0x20
  popa; sti; iret
wait_OBF_1:
  movw $0x64,   %dx
1:  inb  %dx,     %al
  test $0x1,    %al
  jz 1b; ret
keymap:
  .byte 'X, '*, '1, '2, '3, '4
  .byte '5, '6, '7, '8, '9, '0
.org 510
  .word 0xaa55
