.data
hello_str:
    .ascii "Hello, World!\n\0"
L_fmt:
    .ascii "eax: %d, ebx: %d, ecx: %d, r8d: %d\n\0"
CF_set_msg:
    .ascii "Carry flag is set\n\0"
CF_clear_msg:
    .ascii "Carry flag is clear\n\0"

.text
.globl _main
.extern _printf
.extern _exit

_main:
# 割られる数 $eax, 割られる数を1ビットずつ写すためのレジスタ　$ebx, 割る数 スタック(edx), 割り算の結果を格納するレジスタ $r8d
    pushq %rbp
    movq %rsp, %rbp

    movl $10, %eax
    movl $0, %ebx
    pushq $3
    movl $32, %ecx
    movl $0, %r8d

get_bits:
    shll $1, %eax
    rcll $1, %ebx
    popq %rdx
    shll $1, %r8d
    cmpl %edx, %ebx
    jl tag
    addl $1, %r8d
    subl %edx, %ebx
tag:
    pushq %rdx
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %r8
    leaq L_fmt(%rip), %rdi  # printf のフォーマット文字列を %rdi にセット
    movl %eax, %esi         # %esi に %eax の値をセット
    movl %ebx, %edx         # %edx に %ebx の値をセット
    movl %ecx, %ecx         # %ecx に %ecx の値をセット
    movl %r8d, %r8d         # %r8d に %r8d の値をセット
    xorl %eax, %eax         # %eax を 0 にセット
    call _printf            # printf を呼び出す
    popq %r8
    popq %rcx
    popq %rbx
    popq %rax

    loop get_bits

    # プログラムの終了
    movl $0, %edi           # 終了コードを 0 に設定
    call _exit              # プログラムを終了

    leave                   # スタックフレームの解除
    retq