.data
L_fmt:
	.ascii "%d\n\0"
.text
.globl _main
.extern _exit
_main:
	pushq %rbp
	movq %rsp, %rbp
	movl $0, %eax
	movl $0, %ebx
	movl $0, %ecx
	pushq $0
	imull $10, %ebx, %ebx
	addl $1, %ebx
	imull $10, %ebx, %ebx
	addl $0, %ebx
	# 符号反転の処理
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	# 演算キー処理
	addl %ebx, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $2, %ebx
	# メモリ加算
	# 符号反転の処理
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	# 演算キー処理
	imull %ebx, %eax
	popq %rdx
	addl %eax, %edx
	pushq %rdx
	movl $0, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $4, %ebx
	imull $10, %ebx, %ebx
	addl $0, %ebx
	# 符号反転の処理
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	# 演算キー処理
	addl %ebx, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $4, %ebx
	# メモリ加算
	# 符号反転の処理
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	# 演算キー処理
	xorl %edx, %edx
	cltd
	idivl %ebx
	popq %rdx
	addl %eax, %edx
	pushq %rdx
	movl $0, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $1, %ebx
	imull $10, %ebx, %ebx
	addl $5, %ebx
	# 符号反転の処理
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	# 演算キー処理
	addl %ebx, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $2, %ebx
	# 符号反転の処理
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	# 演算キー処理
	imull %ebx, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $3, %ebx
	# メモリ加算
	# 符号反転の処理
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	# 演算キー処理
	imull %ebx, %eax
	popq %rdx
	addl %eax, %edx
	pushq %rdx
	movl $0, %eax
	movl $0, %ebx
	movl $0, %ecx
	# メモリ読み込み
	popq %rdx
	movl %edx, %eax
	pushq %rdx
	# 16バイト境界制約の確認
	movq %rsp, %rbx
	andq $0xF, %rbx
	cmpq $0x0, %rbx
	je end
	andq $0xFFFFFFFFFFFFFFF0, %rsp
end:
	leaq L_fmt(%rip), %rdi
	movl %eax, %esi
	xorl %eax, %eax
	call _printf
	movl $0, %edi
	call _exit
	leave
	ret
