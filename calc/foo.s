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
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	addl %ebx, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $2, %ebx
	testb $1, %cl
	jz 1f
	negl %ebx
1:
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
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	addl %ebx, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $4, %ebx
	testb $1, %cl
	jz 1f
	negl %ebx
1:
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
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	addl %ebx, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $2, %ebx
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	imull %ebx, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $3, %ebx
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	imull %ebx, %eax
	popq %rdx
	addl %eax, %edx
	pushq %rdx
	movl $0, %eax
	movl $0, %ebx
	movl $0, %ecx
	popq %rdx
	movl %edx, %eax
	pushq %rdx
	leaq L_fmt(%rip), %rdi
	movl %eax, %esi
	xorl %eax, %eax
	call _printf
	movl $0, %edi
	call _exit
	leave
	ret
