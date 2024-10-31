.data
L_fmt:
	.ascii "%d\n\0"
char_E:
	.ascii "E\n\0"
.text
.globl main
main:
	pushq %rbp
	movq  %rsp, %rbp
	movl $0, %eax
	movl $0, %ebx
	movl $0, %ecx
	movl $0, %edx
 	movl $0, %esi
	movl $0, %edi
	movl $3, %ebx
	addl  %ebx, %eax
	 jo exit_E

	movl $0, %ebx
	movl $4, %ebx
	 imull %ebx, %eax
	 jo exit_E

	movl $0, %ebx
	addl %eax, %ecx
	movl $0, %ebx
	movl $0, %eax
	 jo exit_E

	leaq L_fmt(%rip), %rdi
	movslq %ecx, %rsi
	call  printf
	addq  $16, %rsp
	leave
	ret
	movl $0, %ebx
	movl $0, %eax
	 jo exit_E

	addl  %ebx, %eax
	 jo exit_E

	leaq L_fmt(%rip), %rdi
	movslq %eax, %rsi
	call  printf
	addq  $16, %rsp
	leave
	ret
exit_E:
	leaq char_E(%rip), %rdi
	call printf
	addq $16, %rsp
	movl $1, %edi
	call exit
.section .note.GNU-stack,"",@progbits
