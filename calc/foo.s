.data
L_fmt:
	.ascii "%d\n\0"
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
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $7, %ecx
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $3, %ecx
	# 演算キー処理
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	addl %ecx, %eax
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $2, %ecx
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $4, %ecx
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
	subl %ecx, %eax
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $7, %ecx
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $1, %ecx
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
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $3, %ecx
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $8, %ecx
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
	# 演算キー処理
	# 符号反転の処理
	testl %eax, %eax
	jns 1f
	negl %eax
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
1:
	xorl %edx, %edx
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
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $2, %ecx
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $6, %ecx
	# 演算キー処理
	# 符号反転の処理
	testl %eax, %eax
	jns 1f
	negl %eax
	movq -8(%rbp), %rdx
	addq $1, %rdx
	movq %rdx, -8(%rbp)
	xorl %edx, %edx
1:
	xorl %edi, %edi
	movl %ecx, %edx
	movl $32, %ecx
	xorl %esi, %esi
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
	xorl %ecx, %ecx
	movq $0, -8(%rbp)
	addq $16, %rsp
	leave
end:
	leaq L_fmt(%rip), %rdi
	movl %eax, %esi
	xorl %eax, %eax
	call _printf
	movl $0, %edi
	call _exit
	ret
