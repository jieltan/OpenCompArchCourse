`ifndef __AND_2_V__
`define __AND_2_V__

module and2(
	input [1:0] a,
	output logic x
);
	assign x=a[0] & a[1];
endmodule

`endif
