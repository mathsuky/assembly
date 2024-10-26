.data
hello_str:
    .ascii "Hello, World!\n\0"
L_fmt: 
    .ascii "Result: %ld\n\0"     # 出力フォーマット

.text
.globl _main
.extern _printf
.extern _exit

_main:
    pushq %rbp                 # ベースポインタを保存
    movq %rsp, %rbp            # RSPをRBPに保存

    # EAXとEBXに数値を代入
    movl $10, %eax             # EAX = 10
    movl $20, %ebx             # EBX = 20

    # スタックにEAXとEBXをプッシュ (64ビットのスタック上に32ビット値として保存)
    pushq %rax                 # スタックに10をプッシュ（64ビット幅でプッシュ）
    pushq %rbx                 # スタックに20をプッシュ（64ビット幅でプッシュ）

    # スタックから値を取り出して加算
    popq %rcx                  # RCXにスタックトップの20を取り出す（64ビット幅）
    popq %rdx                  # RDXにスタックの次の10を取り出す（64ビット幅）
    addl %ecx, %edx            # EDX = EDX + ECX (10 + 20)

	movl $0, %edi
	call _exit
	leave
	ret