typedef enum logic { LOCKED, UNLOCKED } ts_state;

module turnstile(
	input wire coin, push,
	input wire clock, reset,
	output ts_state state
	);

	ts_state next_state;

	always_comb begin
		next_state=state;
		if (state==LOCKED && coin)		next_state = UNLOCKED;
		if (state==UNLOCKED && push)	next_state = LOCKED;
	end
	always_ff @(posedge clock) begin
		if (reset)	state <= #1 LOCKED;
		else		state <= #1 next_state;
	end
endmodule
