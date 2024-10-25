.data
hello_str:
    .ascii "Hello, World!\n\0"

.text
.globl _main
.extern _printf
.extern _exit

_main:
	movl $10, %eax
	movl $0, %edx
	movl $3, %ecx
	idiv %ecx
	retq