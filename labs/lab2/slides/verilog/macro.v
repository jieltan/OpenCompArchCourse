`define DEBUG
`define LOCKED 1'b0
`define UNLOCKED 1'b1
module turnstile(
	input coin, push,
	input clock, reset
	`ifdef DEBUG
		,output logic state
	`endif
	);
	`ifndef DEBUG
		logic state;
	`endif
	always_comb begin
		next_state=state;
		if (state==`LOCKED&&coin)	next_state = `UNLOCKED;
		if (state==`UNLOCKED&&push)	next_state = `LOCKED;
	end
	always_ff @(posedge clock) begin
		if (reset)	state <= #1 `LOCKED;
		else		state <= #1 next_state;
	end
endmodule
