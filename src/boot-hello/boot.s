.code16
.text
start:		
    ljmp $0x07c0, $start2
start2:	
    movw %cs,    %ax
    movw %ax,    %ds
    movw %ax,    %ss

    movw $mesg,  %bp
loop:
    movb (%bp),  %al
    cmpb $0,     %al
    je   exit
    movb $0xe,   %ah
    movb $0x12,  %bl
    int  $0x10
    incw %bp
    jmp  loop 
exit:
    hlt
    jmp exit
mesg:	
    .ascii "Hello\0"
.org 510
    .word 0xaa55
