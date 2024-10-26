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
	movl $0, %eax
	movl $0, %ebx
	movl $0, %ecx
	pushq $0
	imull $10, %ebx, %ebx
	addl $2, %ebx
	jo overflow
	imull $10, %ebx, %ebx
	addl $1, %ebx
	jo overflow
	imull $10, %ebx, %ebx
	addl $4, %ebx
	jo overflow
	imull $10, %ebx, %ebx
	addl $7, %ebx
	jo overflow
	imull $10, %ebx, %ebx
	addl $4, %ebx
	jo overflow
	imull $10, %ebx, %ebx
	addl $8, %ebx
	jo overflow
	imull $10, %ebx, %ebx
	addl $3, %ebx
	jo overflow
	imull $10, %ebx, %ebx
	addl $6, %ebx
	jo overflow
	imull $10, %ebx, %ebx
	addl $4, %ebx
	jo overflow
	imull $10, %ebx, %ebx
	addl $7, %ebx
	jo overflow
	addl $1, %ecx
	# メモリ加算
	# 符号反転の処理
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	# 演算キー処理
	addl %ebx, %eax
	jo overflow
	popq %rdx
	addl %eax, %edx
	jo overflow
	pushq %rdx
	movl $0, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $2, %ebx
	jo overflow
	# メモリ減算
	# 符号反転の処理
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	# 演算キー処理
	addl %ebx, %eax
	jo overflow
	popq %rdx
	subl %eax, %edx
	jo overflow
	pushq %rdx
	movl $0, %eax
	movl $0, %ebx
	movl $0, %ecx
	# メモリ読み込み
	popq %rdx
	movl %edx, %eax
	pushq %rdx
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
