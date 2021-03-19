#####################################
# crt.s
# C Runtime setup for our environment
# allocates the stack pointer
# initializes all registers
# provide a simpler exit() function
#   - Jielun Tan, 02/2019
####################################

.global crt
.section .text.prologue, "ax"
.align 4
crt:
	nop
	la ra, exit
	la sp, _sp
	mv s0, sp
	la gp, __global_pointer$
	li tp, 0
	li t0, 0
	li t1, 0
	li t2, 0
	li s1, 0
	li a0, 0
	li a1, 0
	li a2, 0
	li a3, 0
	li a4, 0
	li a5, 0
	li a6, 0
	li a7, 0
	li s2, 0
	li s3, 0
	li s4, 0
	li s5, 0
	li s6, 0
	li s7, 0
	li s8, 0
	li s9, 0
	li s10, 0
	li s11, 0
	li t3, 0
	li t4, 0
	li t5, 0
	li t6, 0
	j main

.global exit
.section .text
.align 4
exit:
	la sp, _sp
	sw a0, -8(sp)
	nop
	wfi
