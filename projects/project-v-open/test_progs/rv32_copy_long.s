/*
	TEST PROGRAM #1: copy memory contents of 16 elements starting at
			 address 0x1000 over to starting address 0x1100. 
	

	long output[16];

	void
	main(void)
	{
	  long i;
	  *a = 0x1000;
          *b = 0x1100;
	 
	  for (i=0; i < 16; i++)
	    {
	      a[i] = i*10; 
	      b[i] = a[i]; 
	    }
	}
*/
	data = 0x1000
	li	x6, 0
	nop
	nop
	nop
	nop
	li	x2, data
	nop
	nop
	nop
	nop
    li x31, 0x0a
loop:	mul	x3,	x6,	x31
	nop
	nop
	nop
	nop
	sw	x3, 0(x2)
	nop
	nop
	nop
	nop
	lw	x4, 0(x2)
	nop
	nop
	nop
	nop
	sw	x4, 0x100(x2)
	nop
	nop
	nop
	nop
	addi	x2,	x2,	0x8 #
	nop
	nop
	nop
	nop
	addi	x6,	x6,	0x1 #
	nop
	nop
	nop
	nop
	slti	x5,	x6,	16 #
	nop
	nop
	nop
	nop
	bne	x5,	x0,	loop #
	nop
	nop
	nop
	nop
	wfi
	nop
	nop
	nop
	nop

