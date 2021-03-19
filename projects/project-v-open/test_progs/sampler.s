.section .text
.align 4
	nop	
	li sp, 2048
## Branch tests ##
	li t0, 0x1 #TODO: this will be test number 
	li t6, 0
	li t1, 1
	li t2, 2
	bne t1,t2, bt1
	nop
	nop
	nop
	wfi
bt1:
	addi t6, t6, 1
	li t1, 0
	li t2, 0
	bne t1, t2, bt2
	addi t6, t6, 1
bt2:
	beq t1, t2, bt3
	nop
	wfi
bt3:
	addi t6, t6, 1
	addi t1, t1, 1
	bltu t1, t0, bt4
	blt  t1, t0, bt4
	bge  t0, t1, bt4
	bgeu t0, t1, bt4 
	addi t6, t6, 1
	bge	 t1, t0, bt4
	nop
	wfi
bt4:
	lui t1, 0xfffff
	lui t0, 0x7ffff
	bgeu t1, t0, btt1
	nop
	wfi
btt1:
	bltu t0, t1, btt2
	nop
	wfi
btt2:
	blt t1, t0, btt3
	nop
	wfi
btt3:
	bge t0, t1, btt4
	nop
	wfi
btt4:
	jal btt5 
	nop
	wfi
btt5:
	li t0, 0
	la t1, btt6
	jalr t0,t1,0
linkaddr:
	wfi
btt6:
	la t1, linkaddr
	bne t0, t1, linkaddr

## Immediate tests ##
	li t0, 0x1 #TODO: this will be the test number	
	li t6 , 0 #zero out
	ori t6, t6, -2048 
	ori t1, t1, -1 
	li t2, 0
	addi t2, t2, 3
	andi t2, t2, 1
	li t3, 0
	xori t3, t3, -1 
	bne t3, t1, FAIL
	andi t1, t1, -2048
	bne t1,t6, FAIL
	slti t4, t1, 1	
	sltiu t5, t1, 1
	bne t4,t5, im_hop
	bge t1, t1, FAIL
im_hop:	
	li t2, 0x1
	slli t2, t2, 12
	lui t1, 1
	bne t2, t1, FAIL
	lui t1, 0xfffff
	srli t1, t1, 31
	li t2, 1
	bne t2, t1, FAIL
	lui t1, 0xfffff
	srai t1, t1, 31
	lui t2, 0xfffff
	ori t2, t2, -1
	bne t2, t1, FAIL 
## Memory tests ##
	li t0, 0x2 #TODO: testname	
	li t1, 255
	sb t1, 0(sp) 
	lb t2, 0(sp)
	bge t2, t1, FAIL
	lbu t2, 0(sp)
	bne t1,t2, FAIL	
	ori t1, t1, -1 
	lui t1, 0xf	
	sh t1, 0(sp)
	lh t2, 0(sp)
	bge t2, t1, FAIL
	lhu t2, 0(sp)
	bne t2, t2, FAIL	
	ori t1, t1, -1 
	lui t1, 0x7ffff
	sw t1, 0(sp)
	lw t2, 0(sp)
	bne t1, t2, FAIL
## Arithimetic between register tests ##
	li t0, 0x3
	li t1, 5
	li t2, 0
	add t2, t1, t1
	slli t1, t1, 1	
	bne t1, t2, FAIL
	li t1, 0x3
	li t2, 0x4
	or t1, t1,t2
	li t3, 0x7
	bne t1,t3, FAIL	
	li t1, 3
	sub t1, t1, t2
	li t2, -1
	bne t1,t2, FAIL
	# TODO: Finish out with arithmetic instructions, they have been enurmated 
	# in the previous section with immeadiates, just need to test them w/ reg args
## MULT Instructions ##
	li t0, 0x4
	li t1, 14
	li t2, 40
	mul t3, t2, t1
	li t4, 560
	bne t4, t3, FAIL
	lui t1, 0x7ff00
	lui t2, 0x55555
	mulhu t5, t1, t2
	mul t4, t1, t2
	lui t1, 0xfff00
    lui t2, 0xf5555
	mulh t5, t1,t2
	mul t4, t1,t2
	wfi
FAIL: 
	wfi
