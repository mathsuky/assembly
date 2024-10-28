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
	movl $0, %ecx
	pushq $0
	pushq $0
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $1, %ecx
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $0, %ecx
	# 演算キー処理
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	addl %ecx, %eax
	movl $0, %ecx
	movl $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $2, %ecx
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
	# メモリ加算
	# 演算キー処理
	# 符号反転の処理
	testl %eax, %eax
	jns 1f
	negl %eax
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
1:
	movl $0, %edx
2:
	rcrl $1, %ecx
	jnc 3f
	addl %eax, %edx
3:
	shll $1, %eax
	testl %ecx, %ecx
	jnz 2b
	movq -8(%rbp), %rcx
	testb $1, %cl
	jz 4f
	negl %edx
4:
	movl %edx, %eax
	popq %rdx
	addl %eax, %edx
	pushq %rdx
	movl $0, %eax
	movl $0, %ecx
	movl $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $4, %ecx
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $0, %ecx
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
	# 演算キー処理
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	addl %ecx, %eax
	movl $0, %ecx
	movl $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $4, %ecx
	# メモリ加算
	# 演算キー処理
	# 符号反転の処理
	testl %eax, %eax
	jns 1f
	negl %eax
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
	movl $0, %edx
1:
	movl $0, %edi
	movl %ecx, %edx
	movl $32, %ecx
	movl $0, %esi
2:
	shll $1, %eax
	rcll $1, %edi
	shll $1, %esi
	cmpl %edx, %edi
	jl 3f
	addl $1, %esi
	subl %edx, %edi
3:
	decl %ecx
	testl %ecx, %ecx
	jnz 2b
	movq -8(%rbp), %rcx
	testb $1, %cl
	jz 4f
	negl %esi
4:
	movl %esi, %eax
	popq %rdx
	addl %eax, %edx
	pushq %rdx
	movl $0, %eax
	movl $0, %ecx
	movl $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $1, %ecx
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $5, %ecx
	# 演算キー処理
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	addl %ecx, %eax
	movl $0, %ecx
	movl $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $2, %ecx
	# 演算キー処理
	# 符号反転の処理
	testl %eax, %eax
	jns 1f
	negl %eax
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
1:
	movl $0, %edx
2:
	rcrl $1, %ecx
	jnc 3f
	addl %eax, %edx
3:
	shll $1, %eax
	testl %ecx, %ecx
	jnz 2b
	movq -8(%rbp), %rcx
	testb $1, %cl
	jz 4f
	negl %edx
4:
	movl %edx, %eax
	movl $0, %ecx
	movl $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $3, %ecx
	# メモリ加算
	# 演算キー処理
	# 符号反転の処理
	testl %eax, %eax
	jns 1f
	negl %eax
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
1:
	movl $0, %edx
2:
	rcrl $1, %ecx
	jnc 3f
	addl %eax, %edx
3:
	shll $1, %eax
	testl %ecx, %ecx
	jnz 2b
	movq -8(%rbp), %rcx
	testb $1, %cl
	jz 4f
	negl %edx
4:
	movl %edx, %eax
	popq %rdx
	addl %eax, %edx
	pushq %rdx
	movl $0, %eax
	movl $0, %ecx
	movl $0, -8(%rbp)
	# メモリ読み込み
	popq %rdx
	movl %edx, %eax
	pushq %rdx
	# 16バイト境界制約の確認
	movq %rsp, %rcx
	andq $0xF, %rcx
	cmpq $0x0, %rcx
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
