`include "and2.v"
module and4(
	input [3:0] in,
	output logic out
);
	logic [1:0] tmp;
	and2 left(.a(in[1:0]),.x(tmp[0]));
	and2 right(.a(in[3:2]),.x(tmp[1]));
	and2 top(.a(tmp),.x(out));
endmodule
