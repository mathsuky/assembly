.data
hello_str:
    .ascii "Hello, World!\n\0"
L_fmt:
    .ascii "%d\n\0"
CF_set_msg:
    .ascii "Carry flag is set\n\0"
CF_clear_msg:
    .ascii "Carry flag is clear\n\0"

.text
.globl _main
.extern _printf
.extern _exit

_main:
    pushq %rbp
    movq %rsp, %rbp
    # 値の設定
    movl $5, %eax          # %eax に 5 をセット
    movl $7, %ebx         # %ebx に -3 をセット
    movl $0, %ecx          # %ecx に 0 をセット
    movl $0, %edx          # %edx に 0 をセット

    jc CF_set_0
    leaq CF_clear_msg(%rip), %rdi
    jmp print_msg_0
CF_set_0:
    leaq CF_set_msg(%rip), %rdi
print_msg_0:
    call _printf
    rcrl $1, %ebx
    jc CF_set_1
    leaq CF_clear_msg(%rip), %rdi
    jmp print_msg_1
CF_set_1:
    leaq CF_set_msg(%rip), %rdi
print_msg_1:
    call _printf
    rcrl $1, %ebx
    jc CF_set_2
    leaq CF_clear_msg(%rip), %rdi
    jmp print_msg_2
CF_set_2:
    leaq CF_set_msg(%rip), %rdi
print_msg_2:
    call _printf
    rcrl $1, %ebx
    jc CF_set_3
    leaq CF_clear_msg(%rip), %rdi
    jmp print_msg_3
CF_set_3:
    leaq CF_set_msg(%rip), %rdi
print_msg_3:
    call _printf




    # ecxの値をプリントする
    leaq L_fmt(%rip), %rdi  # printf のフォーマット文字列を %rdi にセット
    movl %ebx, %esi         # %esi に %ecx の値をセット
    xorl %eax, %eax         # %eax を 0 にセット
    call _printf            # printf を呼び出す

    # プログラムの終了
    movl $0, %edi           # 終了コードを 0 に設定
    call _exit              # プログラムを終了

    leave                   # スタックフレームの解除
    retq