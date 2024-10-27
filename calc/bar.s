.data
hello_str:
    .ascii "Hello, World!\n\0"
L_fmt:
	.ascii "%d\n\0"

.text
.globl _main
.extern _printf
.extern _exit

_main:
	pushq %rbp
	movq %rsp, %rbp
    # 値の設定
    movl $5, %eax          # %eax に 12 をセット
    movl $11, %ebx           # %ebx に 5 をセット
    movl $0, %ecx           # %ecx に 0 をセット
    movl $0, %edx           # %edx に 0 をセット

    jc 2f
    movl %eax, %ecx
2:
    rcll $0, %ecx
    addl %ecx, %edx

    movl $0, %ecx

    rcrl $1, %ebx
    jc 2f
    movl %eax, %ecx
2:
    rcll $1, %ecx
    addl %ecx, %edx

    movl $0, %ecx

    rcrl $1, %ebx
    jc 2f
    movl %eax, %ecx
2:
    rcll $2, %ecx
    addl %ecx, %edx

    movl $0, %ecx

    rcrl $1, %ebx
    jc 2f
    movl %eax, %ecx
2:
    rcll $3, %ecx
    addl %ecx, %edx

    movl $0, %ecx

    movl %edx, %ecx





    # rcr命令
    # movl %ebx, %ecx
    # rcrl $1, %ecx            # %ebx の値を右に 1 ビットシフトする

    # ecxの値をプリントする
    leaq L_fmt(%rip), %rdi  # printf のフォーマット文字列を %rdi にセット
    movl %ecx, %esi         # %esi に %ecx の値をセット
    xorl %eax, %eax         # %eax を 0 にセット
    call _printf            # printf を呼び出す


    # プログラムの終了
    movl $0, %edi           # 終了コードを 0 に設定
    call _exit              # プログラムを終了
