.section .text
.align 4
nop
li a0, 0
li a1, 12314
li a2, 342
li sp, 2048
sw a1, 0(sp)
addi sp, sp, 4
add a0, a2, a0
add a0, a2, a1
sw a0, 0(sp)
addi sp, sp, -16
lw a0, 0(sp)
sub a0, a2, a0
nop
nop
wfi



