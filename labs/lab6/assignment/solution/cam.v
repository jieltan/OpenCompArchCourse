module CAM #(parameter SIZE=8) (

	input clock, reset,
	input enable,
	
	input COMMAND command,
	
	input [31:0] data,
	
	input [$clog2(SIZE)-1:0] write_idx,
	
	output logic [$clog2(SIZE)-1:0] read_idx,
	output logic hit
	);
	
	logic [SIZE-1:0] [31:0] content;
	logic [SIZE-1:0] valid;
	
	always_ff @(posedge clock) begin
		if(reset) begin
			for(int i=0; i<SIZE; i++) begin
				content[i] = 32'b0;
				valid[i] = 0;
			end
		end else if(enable && command == WRITE && write_idx < SIZE) begin
			content[write_idx] = data;
			valid[write_idx] = 1;
		end
	end
	
	always_comb begin
		hit = 0;
		read_idx = 0;
		if(enable && command == READ) begin
			for(int i=0; i<SIZE; i++) begin
				if(content[i] == data && valid[i]) begin
					hit = 1;
					read_idx = i;
					break;
				end
			end
		end
	end
		
	
endmodule
			