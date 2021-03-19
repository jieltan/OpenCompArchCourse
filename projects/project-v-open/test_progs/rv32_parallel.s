/*
	TEST PROGRAM #4: compute first 16 multiples of 8 (embarrassingly parallel)


	long output[16];
	
	void
	main(void)
	{
	  long i;
	  for (i=0; i < 16; i++,a++,b++,c++)
	    output[i] = ((i + i) + (i + i)) + ((i + i) + (i + i));
	}
*/
	data = 0x1000
    li  x17, 1
	li	x3, 100000
	li	x4, data
loop:	add	x6,	x3,	x3 #
	add	x9,	x3,	x3 #
	add	x8,	x3,	x3 #
	mulhsu x5,	x3,	x3 #
	mul x7,	x5,	x6 #
	mulh x10,	x8,	x9 #
	mulhu x11,	x7,	x10 #
	sw	x7, 0(x4)
	addi	x4,	x4,	0x8 #
	addi	x17,	x17,	0x1 #
    addi    x3,     x3,     1000
	slti	x2,	x17,	16 #
	bne	x2,	x0,	loop #
	wfi

