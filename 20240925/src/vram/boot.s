.code16
.text
start:		
    ljmp $0x07c0, $start2
start2:	
    movw %cs,    %ax
    movw %ax,    %ds
    movw %ax,    %ss
#
    movw $0xB800, %cx
    movw %cx,    %es
    movw $340,   %bx
    movb $'A,    %es:(%bx)
    incw %bx
    movb $0x0C, %es:(%bx)
#
exit: hlt; jmp halt

.org 510
    .word 0xaa55
