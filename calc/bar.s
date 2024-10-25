.data
hello_str:
    .ascii "Hello, World!\n\0"

.text
.globl _main
.extern _printf
.extern _exit

_main:
    movl $-10, %eax      # 被除数として-10をeaxに設定
    cltd                  # eaxの符号をedxに拡張
    movl $3, %ecx        # 除数として3をecxに設定
    idiv %ecx            # eaxをecxで除算
    retq                  # 戻る
