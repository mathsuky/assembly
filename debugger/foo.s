.text
.globl _main
.p2align 4, 0x90
_main:
movl $999, -4(%rbp)
leaq L_.str(%rip), %rdi
movl -4(%rbp), %esi
movb $0, %al
# ここで%rspは16の倍数が必要
callq _printf
retq
.cstring
L_.str:
.asciz "%d\n"