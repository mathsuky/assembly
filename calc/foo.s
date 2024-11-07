.data
L_fmt:
	.ascii "%d\n\0"
L_err:
	.ascii "E\n\0"
.text
.globl _main
.extern _exit
_main:
	pushq %rbp
	movq %rsp, %rbp
	xorl %eax, %eax
	xorl %ecx, %ecx
	pushq $0
	pushq $0
	imull $10, %ecx, %ecx
	addl $1, %ecx
	jo overflow
	imull $10, %ecx, %ecx
	addl $0, %ecx
	jo overflow
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	# 演算キー処理
	addl %ecx, %eax
	jo overflow
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	imull $10, %ecx, %ecx
	addl $2, %ecx
	jo overflow
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
	# メモリ加算
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	# 演算キー処理
	imull %ecx, %eax
	jo overflow
	popq %rdx
	addl %eax, %edx
	jo overflow
	pushq %rdx
	xorl %eax, %eax
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	imull $10, %ecx, %ecx
	addl $4, %ecx
	jo overflow
	imull $10, %ecx, %ecx
	addl $0, %ecx
	jo overflow
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	# 演算キー処理
	addl %ecx, %eax
	jo overflow
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	imull $10, %ecx, %ecx
	addl $4, %ecx
	jo overflow
	# メモリ加算
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	# 演算キー処理
	cmpl $0, %ecx
	je division_by_zero
	cltd
	idivl %ecx
	popq %rdx
	addl %eax, %edx
	jo overflow
	pushq %rdx
	xorl %eax, %eax
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	imull $10, %ecx, %ecx
	addl $1, %ecx
	jo overflow
	imull $10, %ecx, %ecx
	addl $5, %ecx
	jo overflow
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	# 演算キー処理
	addl %ecx, %eax
	jo overflow
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	imull $10, %ecx, %ecx
	addl $2, %ecx
	jo overflow
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	# 演算キー処理
	imull %ecx, %eax
	jo overflow
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	imull $10, %ecx, %ecx
	addl $3, %ecx
	jo overflow
	# メモリ加算
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	# 演算キー処理
	imull %ecx, %eax
	jo overflow
	popq %rdx
	addl %eax, %edx
	jo overflow
	pushq %rdx
	xorl %eax, %eax
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	# メモリ読み込み
	popq %rdx
	movl %edx, %eax
	pushq %rdx
	addq $16, %rsp
	leave
end:
	leaq L_fmt(%rip), %rdi
	movl %eax, %esi
	xorl %eax, %eax
	call _printf
	movl $0, %edi
	call _exit
overflow:
	leaq L_err(%rip), %rdi
	call _printf
	movl $1, %edi
	call _exit
division_by_zero:
	leaq L_err(%rip), %rdi
	call _printf
	movl $1, %edi
	call _exit
