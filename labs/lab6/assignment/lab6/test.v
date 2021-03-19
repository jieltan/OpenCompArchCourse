module testbench;
	logic clock, reset, enable;
	
	logic [31:0] data;
	COMMAND command;
	
	logic [$clog2(`TEST_SIZE)-1:0] write_idx, read_idx;
	logic hit;
	`DUT(CAM) #(.SIZE(`TEST_SIZE)) dut (.clock, .reset, .enable, .command, .write_idx, .read_idx, .data, .hit);
	
	always #5 clock = ~clock;
	
	task exit_on_error;
		begin
			#1;
			$display("@@@Failed at time %f", $time);
			$finish;
		end
	endtask
	
	initial begin
		
		$monitor("Command: %s, enable: %b, data: %h, write_idx: %h, read_idx: %h, hit: %b", command.name, enable, data, write_idx, read_idx, hit);
	
		clock = 0;
		write_idx = 0;
		enable = 0;
		command = READ;
		data = 0;
		
		/***RESET***/		
		reset = 1;
		@(negedge clock)
		reset = 0;
		
		/***CHECK THAT ALL ELEMENTS ARE INVALID***/
		@(negedge clock);
		assert(!hit) else #1 exit_on_error;
		
		/***INITIALIZE MEMORY****/
		command = WRITE;
		enable = 1;
		for(int i=0; i<`TEST_SIZE; i++) begin
			write_idx = i;
			data = $random;
			@(negedge clock);
		end
		
		/***OVERWRITE***/
		for(int i=0; i<(2**$clog2(`TEST_SIZE)); i++) begin
			write_idx = i;
			data = i;
			@(negedge clock);
		end
		
		/***READ VALUES***/
		command = READ;
		for(int i=0; i<`TEST_SIZE; i++) begin
			data = i;
			@(negedge clock);
			assert(hit && read_idx == i) else exit_on_error;
		end
		
		/***CHECK SIZE***/
		data = `TEST_SIZE;
		@(negedge clock);
		assert(!hit) else #1 exit_on_error;
		
		/***TEST WITH MULTIPLE COPIES OF VALUE***/
		command = WRITE;
		data = $random;
		write_idx = 0;
		@(negedge clock);
		repeat(5) begin
			write_idx = $random;
			@(negedge clock);
		end
		
		command = READ;
		@(negedge clock);
		assert(read_idx == 0) else exit_on_error;
		
		
		$display("@@@Passed");
		$finish;
		
	end
	
endmodule
