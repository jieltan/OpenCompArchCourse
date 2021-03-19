module testbench();

	logic [63:0] a,b;
	logic quit, clock, start, reset;

	logic [63:0] result;
	logic done;

	wire [63:0] cres = a*b;

	wire correct = (cres===result)|~done;


	mult m0(	.clock(clock),
						.reset(reset),
						.mcand(a),
						.mplier(b),
						.start(start),
						.product(result),
						.done(done));

	always @(posedge clock)
		#2 if(!correct) begin 
			$display("Incorrect at time %4.0f",$time);
			$display("cres = %h result = %h",cres,result);
			$finish;
		end

	always begin
		#5;
		clock=~clock;
	end

	// Some students have had problems just using "@(posedge done)" because their
	// "done" signals glitch (even though they are the output of a register). This
	// prevents that by making sure "done" is high at the clock edge.
	task wait_until_done;
		forever begin : wait_loop
			@(posedge done);
			@(negedge clock);
			if(done) disable wait_until_done;
		end
	endtask



	initial begin
		$dumpvars;
		$monitor("Time:%4.0f done:%b a:%h b:%h product:%h result:%h",$time,done,a,b,cres,result);
		a=2;
		b=3;
		reset=1;
		clock=0;
		start=1;

		@(negedge clock);
		reset=0;
		@(negedge clock);
		start=0;
		wait_until_done();
		start=1;
		a=-1;
		@(negedge clock);
		start=0;
		wait_until_done();
		@(negedge clock);
		start=1;
		a=-20;
		b=5;
		@(negedge clock);
		start=0;
		wait_until_done();
		quit = 0;
		quit <= #10000 1;
		while(~quit) begin
			start=1;
			a={$random,$random};
			b={$random,$random};
			@(negedge clock);
			start=0;
			wait_until_done();
		end
		$finish;
	end

endmodule



  
  
