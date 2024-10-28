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
	addl $5, %ecx
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $5, %ecx
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	# 演算キー処理
	addl %ecx, %eax
	movl $0, %ecx
	movl $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $1, %ecx
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	# 演算キー処理
	movl $0, %ebx
	movl %ecx, %edx
	movl $32, %ecx
	movl $0, %r8d
divide_loop:
	shll $1, %eax
	rcll $1, %ebx
	shll $1, %r8d
	cmpl %edx, %ebx
	jl divide_shift
	addl $1, %r8d
	subl %edx, %ebx
divide_shift:
	loop divide_loop
	movl %r8d, %eax
	movl $0, %ecx
	movl $0, -8(%rbp)
	movl %ecx, %edx
	sall $3, %ecx
	sall $1, %edx
	addl %edx, %ecx
	addl $5, %ecx
	# 符号反転の処理
	movq -8(%rbp), %rdx
	testb $1, %dl
	jz 1f
	negl %ecx
1:
	# 演算キー処理
	movl $0, %ebx
	movl %ecx, %edx
	movl $32, %ecx
	movl $0, %r8d
divide_loop:
	shll $1, %eax
	rcll $1, %ebx
	shll $1, %r8d
	cmpl %edx, %ebx
	jl divide_shift
	addl $1, %r8d
	subl %edx, %ebx
divide_shift:
	loop divide_loop
	movl %r8d, %eax
	movl $0, %ecx
	movl $0, -8(%rbp)
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
