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
	li	x2, data
    li  x31, 0x0a
loop:	mul	x3,	x6,	x31
	sw	x3, 0(x2)
	lw	x4, 0(x2)
	sw	x4, 0x100(x2)
	addi	x2,	x2,	0x8 #
	addi	x6,	x6,	0x1 #
	slti	x5,	x6,	16 #
	bne	x5,	x0,	loop #
	wfi

