.code16
.text
start:		
    ljmp $0x07c0, $start2
start2:	
    movw %cs,    %ax
    movw %ax,    %ds
    movw %ax,    %ss
#
    movb $0xED, %al
    movw $0x60, %dx
    outb %al,   %dx
    movw $0x64, %dx
wait:
    inb  %dx,   %al
    test $2,    %al
    jnz  wait
    movw $0x60, %dx
    movb $2,    %al
    outb %al,   %dx
#
exit: hlt; jmp halt

.org 510
    .word 0xaa55
