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
	li	x4, data
loop1: andi x31, x3, 1	
    bne	x31,	x0,	loop2 #
	sw	x3, 0(x4)
	addi	x4,	x4,	0x8 #
loop2:	addi	x3,	x3,	0x1 #
	slti	x2,	x3,	16 #
	bne	x2,	x0,	loop1 #
	wfi

