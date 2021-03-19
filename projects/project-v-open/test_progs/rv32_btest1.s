/*
   btest1.s: Hammer on the branch prediction logic somewhat.
             This test is a series of 64 code-blocks that check a register
             and update a bit-vector by or'ing with 2^block#.  The resulting
             bit-vector sequence is 0xbeefbeefbaadbaad stored in mem line 2000

             Do not expect a decent prediction rate on this test.  No branches
             are re-visited (though a global predictor _may_ do reasonably well)
             The intent of this benchmark is to test control flow.

             Note: 'call_pal 0x000' is an instruction that is not decoded by
                   simplescalar3.  It is being used in this instance as a way
                   to pad the space between (almost) basic blocks with invalid
                   opcodes.
 */
data = 0x3E80

	li	x30, 0
	li	x1, 0
	li	x2, 1
B0:	slli	x21,	x2,	0 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B32 #
	j   bad

	wfi
	wfi
B1:	slli	x21,	x1,	1 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B33 #
	j   bad

	wfi
	wfi
B2:	slli	x21,	x2,	2 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B34 #
	j   bad

	wfi
	wfi
B3:	slli	x21,	x2,	3 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B35 #
	j   bad

	wfi
	wfi
B4:	slli	x21,	x1,	4 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B36 #
	j   bad

	wfi
	wfi
B5:	slli	x21,	x2,	5 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B37 #
	j   bad

	wfi
	wfi
B6:	slli	x21,	x1,	6 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B38 #
	j   bad

	wfi
	wfi
B7:	slli	x21,	x2,	7 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B39 #
	j   bad

	wfi
	wfi
B8:	slli	x21,	x1,	8 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B40 #
	j   bad

	wfi
	wfi
B9:	slli	x21,	x2,	9 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B41 #
	j   bad

	wfi
	wfi
B10:	slli	x21,	x1,	10 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B42 #
	j   bad

	wfi
	wfi
B11:	slli	x21,	x2,	11 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B43 #
	j   bad

	wfi
	wfi
B12:	slli	x21,	x2,	12 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B44 #
	j   bad

	wfi
	wfi
B13:	slli	x21,	x2,	13 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B45 #
	j   bad

	wfi
	wfi
B14:	slli	x21,	x1,	14 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B46 #
	j   bad

	wfi
	wfi
B15:	slli	x21,	x2,	15 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B47 #
	j   bad

	wfi
	wfi
B16:	slli	x21,	x2,	16 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B48 #
	j   bad

	wfi
	wfi
B17:	slli	x21,	x1,	17 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B49 #
	j   bad

	wfi
	wfi
B18:	slli	x21,	x2,	18 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B50 #
	j   bad

	wfi
	wfi
B19:	slli	x21,	x2,	19 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B51 #
	j   bad

	wfi
	wfi
B20:	slli	x21,	x1,	20 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B52 #
	j   bad

	wfi
	wfi
B21:	slli	x21,	x2,	21 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B53 #
	j   bad

	wfi
	wfi
B22:	slli	x21,	x1,	22 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B54 #
	j   bad

	wfi
	wfi
B23:	slli	x21,	x2,	23 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B55 #
	j   bad

	wfi
	wfi
B24:	slli	x21,	x1,	24 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B56 #
	j   bad

	wfi
	wfi
B25:	slli	x21,	x2,	25 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B57 #
	j   bad

	wfi
	wfi
B26:	slli	x21,	x1,	26 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B58 #
	j   bad

	wfi
	wfi
B27:	slli	x21,	x2,	27 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B59 #
	j   bad

	wfi
	wfi
B28:	slli	x21,	x2,	28 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B60 #
	j   bad

	wfi
	wfi
B29:	slli	x21,	x2,	29 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B61 #
	j   bad

	wfi
	wfi
B30:	slli	x21,	x1,	30 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B62 #
	j   bad

	wfi
	wfi
B31:	slli	x21,	x2,	31 #
	or	x30,	x21,	x30 #
	beq	x1,	x0,	B63 #
	j   bad

	wfi
	wfi
B32:	slli	x21,	x2,	0 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B1

	wfi
	wfi
B33:	slli	x21,	x2,	1 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B2

	wfi
	wfi
B34:	slli	x21,	x2,	2 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B3

	wfi
	wfi
B35:	slli	x21,	x2,	3 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B4

	wfi
	wfi
B36:	slli	x21,	x1,	4 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B5

	wfi
	wfi
B37:	slli	x21,	x2,	5 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B6

	wfi
	wfi
B38:	slli	x21,	x2,	6 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B7

	wfi
	wfi
B39:	slli	x21,	x2,	7 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B8

	wfi
	wfi
B40:	slli	x21,	x1,	8 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B9

	wfi
	wfi
B41:	slli	x21,	x2,	9 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B10

	wfi
	wfi
B42:	slli	x21,	x2,	10 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B11

	wfi
	wfi
B43:	slli	x21,	x2,	11 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B12

	wfi
	wfi
B44:	slli	x21,	x2,	12 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B13

	wfi
	wfi
B45:	slli	x21,	x2,	13 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B14

	wfi
	wfi
B46:	slli	x21,	x1,	14 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B15

	wfi
	wfi
B47:	slli	x21,	x2,	15 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B16

	wfi
	wfi
B48:	slli	x21,	x2,	16 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B17

	wfi
	wfi
B49:	slli	x21,	x2,	17 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B18

	wfi
	wfi
B50:	slli	x21,	x2,	18 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B19

	wfi
	wfi
B51:	slli	x21,	x2,	19 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B20

	wfi
	wfi
B52:	slli	x21,	x1,	20 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B21

	wfi
	wfi
B53:	slli	x21,	x2,	21 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B22

	wfi
	wfi
B54:	slli	x21,	x2,	22 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B23

	wfi
	wfi
B55:	slli	x21,	x2,	23 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B24

	wfi
	wfi
B56:	slli	x21,	x1,	24 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B25

	wfi
	wfi
B57:	slli	x21,	x2,	25 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B26

	wfi
	wfi
B58:	slli	x21,	x2,	26 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B27

	wfi
	wfi
B59:	slli	x21,	x2,	27 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B28

	wfi
	wfi
B60:	slli	x21,	x2,	28 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B29

	wfi
	wfi
B61:	slli	x21,	x2,	29 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B30

	wfi
	wfi
B62:	slli	x21,	x1,	30 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   B31

	wfi
	wfi
B63:	slli	x21,	x2,	31 #
	or	x30,	x21,	x30 #
	beq	x2,	x0,	bad #
	j   end

	wfi
	wfi
end:	li	x21, data
	sw	x30,  0(x21)

	wfi
	wfi
bad:    li x0, 0xbeef
    li x0, 0xbeef
    li x0, 0xbeef
    li x0, 0xbeef
    wfi
