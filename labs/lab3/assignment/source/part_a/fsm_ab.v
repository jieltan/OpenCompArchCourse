module fsm_ab(
				input clock, reset, in,
				output logic out
				`ifdef DEBUG_OUT
				, output state_out
				`endif
			);

	logic [1:0] next_state;
	logic [1:0] state;

	`ifdef DEBUG_OUT
	assign state_out = state;
	`endif

	always_comb begin
		case(state)
			2'b00: begin
				out = 0;
				if(in) next_state = 2'b01;
				else next_state = 2'b00;
			end
			2'b01: begin
				out = 0;
				if(in) next_state = 2'b01;
				else next_state = 2'b10;
			end
			2'b10: begin
				out = 0;
				if(in) next_state = 2'b11;
				else next_state = 2'b00;
			end
			2'b11: begin
				out = 1;
				if(in) next_state = 2'b01;
				else next_state = 2'b10;
			end
		endcase
	end

	always_ff @(posedge clock) begin
		if(reset) begin
			state = 2'b00;
		end
		else begin
			state = next_state;
		end
	end


endmodule
