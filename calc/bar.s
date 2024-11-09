.globl _main
.text
_main:
    movl $11, %eax       # %rax に 11 をセット

    rcrl $1, %eax
	rcrl $1, %eax
	rcrl $1, %eax
	rcrl $1, %eax
	rcrl $1, %eax
	rcrl $1, %eax
1:
	retq