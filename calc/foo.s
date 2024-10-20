.text
.globl _main
_main:
  pushq %rbp
  movq %rsp, %rbp
  movl $0, %eax
  movl $0, %ebx
  imull $10, %ebx, %ebx
  addl $1, %ebx
  addl %ebx, %eax
  movl $0, %ebx
  movl %eax, %edi
  call _printf
  movl $0, %eax
  leave
  ret
