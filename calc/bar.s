.text
.globl _main
_main:
    movq $0, %rax
    imulq $10, %rax
    addq $9, %rax
    imulq $10, %rax
    addq $9, %rax
    imulq $10, %rax
    addq $8, %rax
    imulq $10, %rax
    addq $2, %rax
    imulq $10, %rax
    addq $4, %rax
    imulq $10, %rax
    addq $4, %rax
    retq