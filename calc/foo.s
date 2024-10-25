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
	movl $0, %edx
	movl $0, %ecx
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
電卓に存在しない文字Pが入力されました。この入力は無視されます。
	imull $10, %ebx, %ebx
	addl $4, %ebx
	imull $10, %ebx, %ebx
	addl $0, %ebx
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	imull %ebx, %eax
	movl $0, %ebx
	movl $0, %ecx
	imull $10, %ebx, %ebx
	addl $4, %ebx
電卓に存在しない文字Pが入力されました。この入力は無視されます。
	imull $10, %ebx, %ebx
	addl $1, %ebx
	imull $10, %ebx, %ebx
	addl $5, %ebx
	testb $1, %cl
	jz 1f
	negl %ebx
1:
	xorl %edx, %edx
	idivl %ebx
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
電卓に存在しない文字Pが入力されました。この入力は無視されます。
電卓に存在しない文字Rが入力されました。この入力は無視されます。
	leaq L_fmt(%rip), %rdi
	movl %eax, %esi
	xorl %eax, %eax
	call _printf
	movl $0, %edi
	call _exit
	leave
	ret
