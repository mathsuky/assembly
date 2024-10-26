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
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	addl %ebx, %eax
	jo overflow
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $0, %ebx
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	cmpl $0, %ebx
	je division_by_zero
	xorl %edx, %edx
	cltd
	idivl %ebx
	movl $0, %ebx
	movl $0, %ecx
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
