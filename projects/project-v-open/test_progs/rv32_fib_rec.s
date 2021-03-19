/*
	TEST PROGRAM #4: compute nth fibonacci number recursively

	int output;
	
	void
	main(void)
	{
	   output = fib(14); 
	}

	int
	fib(int arg)
	{
	    if (arg == 0 || arg == 1)
		return 1;

	    return fib(arg-1) + fib(arg-2);
	}
*/
	
	data = 0x400
	stack = 0x1000
    li  x8, 1
	li	x31, stack
	
	li	x17, 14
	jal	x27,	fib #

	li	x2, data
	sw	x1, 0(x2)	
	wfi
	
fib:	beq	x17,	x0,	fib_ret_1 # arg is 0: return 1

	#cmpeq	x2,	x17,	1 # arg is 1: return 1
	beq	x17,	x8,	fib_ret_1 #

	addi	x31,	x31,	-32 # allocate stack frame
	sw	x27, 24(x31)	

	sw	x17, 0(x31)	

	addi	x17,	x17,	-1 # arg = arg-1
	jal	x27,	fib # call fib
	sw	x1, 8(x31)	

	lw	x17, 0(x31)	
	addi	x17,	x17,	-2 # arg = arg-2
	jal	x27,	fib # call fib

	lw	x2, 8(x31)	
	add	x1,	x2,	x1 # fib(arg-1)+fib(arg-2)

	lw	x27, 24(x31)	
	addi	x31,	x31,	32 # deallocate stack frame
	jalr x0, x27, 0
	
fib_ret_1:
	li	x1,	1 # set return value to 1
	jalr x0, x27, 0
	
