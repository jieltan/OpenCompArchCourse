/*
	TEST PROGRAM #3: compute first 16 fibonacci numbers
			 with forwarding and stall conditions in the loop


	long output[16];
	
	void
	main(void)
	{
	  long i, fib;
	
	  output[0] = 1;
	  output[1] = 2;
	  for (i=2; i < 16; i++)
	    output[i] = output[i-1] + output[i-2];
	}
*/
	
	data = 0x1000
	li	x4, data
	nop
	nop
	nop
	nop
	lui	x5, 0x1
	nop
	nop
	nop
	nop
	addi x5, x5, 8
	nop
	nop
	nop
	nop
	lui	x6, 0x1
	nop
	nop
	nop
	nop
	addi x6, x6, 16
	nop
	nop
	nop
	nop
	li	x10, 2
	nop
	nop
	nop
	nop
	li	x2, 1
	nop
	nop
	nop
	nop
	sw	x2, 0(x4)
	nop
	nop
	nop
	nop
	sw	x2, 0(x5)
	nop
	nop
	nop
	nop
loop:	lw	x2, 0(x4)
	nop
	nop
	nop
	nop
	lw	x3, 0(x5)
	nop
	nop
	nop
	nop
	add	x3,	x3,	x2 #
	nop
	nop
	nop
	nop
	addi	x4,	x4,	0x8 #
	nop
	nop
	nop
	nop
	addi	x5,	x5,	0x8 #
	nop
	nop
	nop
	nop
	addi	x10,	x10,	0x1 #
	nop
	nop
	nop
	nop
	slti	x11,	x10,	16 #
	nop
	nop
	nop
	nop
	sw	x3, 0(x6)
	addi	x6,	x6,	0x8 #
	nop
	nop
	nop
	nop
	bne	x11,	x0,	loop #
	nop
	nop
	nop
	nop
	wfi
	nop
	nop
	nop
	nop
