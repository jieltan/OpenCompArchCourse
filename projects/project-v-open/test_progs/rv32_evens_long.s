/*
	TEST PROGRAM #2: compute even numbers that are less than 16


	long output[16];
	
	void
	main(void)
	{
	  long i,j;
	
	  for (i=0,j=0; i < 16; i++)
	    {
	      if ((i & 1) == 0)
	        output[j++] = i;
	    }
	}
*/
	data = 0x1000
	li	x3, 0
	nop
	nop
	nop
    andi x31, x3, 1
	nop
	li	x4, data
	nop
	nop
	nop
	nop
loop1:	bne	x31,	x0,	loop2 #
	nop
	nop
	nop
	nop
	sw	x3, 0(x4)
	nop
	nop
	nop
	nop
	addi	x4,	x4,	0x8 #
	nop
	nop
	nop
	nop
loop2:	addi	x3,	x3,	0x1 #
	nop
	nop
	nop
	nop
	slti	x2,	x3,	16 #
	nop
	nop
    andi x31, x3, 1
	nop
	nop 
	bne	x2,	x0,	loop1 #
	nop
	nop
	nop
	nop
	wfi
	nop
	nop
	nop
	nop

