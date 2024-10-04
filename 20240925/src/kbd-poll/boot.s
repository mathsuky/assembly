.code16
.text
    ljmp $0x07c0, $start2
start2:	
    movw %cs,     %ax
    movw %ax,     %ds
loop:
    call read_key
    jmp loop
read_key:
    call wait_OBF_1
    movw  $0x60, %dx
    inb   %dx,   %al
    test  $0x80, %al
    jnz   skip
    movzbw  %al,   %bx
    movb  keymap(%bx), %al
    movb  $0x0E, %ah
    int   $0x10
skip:
    ret
wait_OBF_1:
    movw $0x64, %dx
1:  inb  %dx,   %al
    test $1,    %al
    jz  1b; ret
keymap:
    .byte 'X, '*, '1, '2, '3, '4
    .byte '5, '6, '7, '8, '9, '0
.org 510
    .word 0xaa55
