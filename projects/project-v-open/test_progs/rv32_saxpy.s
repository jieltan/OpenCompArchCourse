/*
	TEST PROGRAM #6: integer SAXPY

	long x[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3, 2, 3 };
	long y[] = { 1, 4, 1, 4, 2, 1, 3, 5, 6, 2, 3, 7, 3, 0, 9, 5, 0, 4 };

	main(void)
	{
          long a = 9999;
	  long i;
	 
	  for (i=0; i < 18; i++)
	    {
	      y[i] = a*x[i] + y[i];
	    }
	}
*/

	j	start
    nop	
	.dword 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3, 2, 3
	.dword 1, 4, 1, 4, 2, 1, 3, 5, 6, 2, 3, 7, 3, 0, 9, 5, 0, 4
	.align 4
start:	
	li	x6, 0
	li	x7, 9999
	li	x1, 8
	li	x2, 152

loop:	lw	x3, 0(x1)	
	mul	x3,	x3,	x7 # r2 <- a * x[i]
	lw	x4, 0(x2)	
	add	x3,	x4,	x3 # r2 += y[i]
	sw	x3, 0(x2)	

	
	addi	x1,	x1,	8 #
	addi	x2,	x2,	8 #

	
	addi	x6,	x6,	1 #
	sltu	x5,	x6,	18 #
	bne	x5,	x0,	loop #

	
	wfi
