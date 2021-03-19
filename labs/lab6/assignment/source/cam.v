module CAM #(parameter SIZE=8) (

	input clock, reset,
	input enable,
	
	input COMMAND command,
	
	input [31:0] data,
	
	input [$clog2(SIZE)-1:0] write_idx,
	
	output logic [$clog2(SIZE)-1:0] read_idx,
	output logic hit
	);
	
	// Fill in design here
		
	
endmodule
			