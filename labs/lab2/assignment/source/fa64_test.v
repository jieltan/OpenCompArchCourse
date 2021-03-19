//TESTBENCH FOR 64 BIT ADDER
//Class:    EECS470
//Specific:    Lab 2
//Description:    This file contains the testbench for the 64-bit adder.


// Note: This testbench is heavily commented for your benefit, please
//       read through and understand _what_ it is doing

// The testbench itself is a module, so declare it as such
module testbench;

//MODULE PARAMETERS:
parameter MAX_64BIT_NUM = 64'hFFFF_FFFF_FFFF_FFFF;

// We need to define inputs and output for the module we wish to test.
// In general, inputs should be registers (since a register is a physical
// device that can hold state and can be wired from) and outputs should
// be wires (since we only need to read the value of the output, we have
// no desire or need to latch it)
logic [63:0] A,B;
logic [63:0] SUM;
logic C_IN;
logic C_OUT;

// Strictly speaking, this is an asynchronous circuit, and thus we do not
// need a clock. We'll use one to delimit test cases, as it makes looking
// at the output much easier.
logic clock;

// Need a number? It's a testbench, we can do that! Conceptually these are
// much more like variables from C, and do not necessarily correlate to any
// physical hardware (thus they can only be used in testbenches)
integer /*[63:0]*/ i,j;



// Now we declare an instance of the module we'd like to test, in this case
// the 64-bit full adder. We also wire in the signals declared above.
full_adder_64bit fa_test_64(
    .A(A),
    .B(B),
    .carry_in(C_IN),
    .S(SUM),
    .carry_out(C_OUT)
);



// "tasks" are verilog-speak for functions. These are really useful and help
// to save on a lot of repeated / duplicated work.
task exit_on_error;
    input [63:0] A, B, SUM;
    input C_IN, C_OUT;
    begin
                $display("@@@ Incorrect at time %4.0f", $time);
                $display("@@@ Time:%4.0f clock:%b A:%h B:%h CIN:%b SUM:%h COUT:%b", $time, clock, A, B, C_IN, SUM, C_OUT);
                $display("@@@ expected sum=%b", (A+B+C_IN) );
                $finish;
    end
endtask

task compare_correct_sum;
    input [63:0] A, B, SUM;
    input C_IN, C_OUT;
    begin
            // Check the answer...
            if( SUM == (A+B+C_IN) )
            begin
                // "empty" cases are legal, since the begin/end
                // block is consuming the true if-branch
            end else begin
                exit_on_error( A, B, SUM, C_IN, C_OUT );
            end

            // What doesn't this function test that it probably should?
    end
endtask





// Set up the clock to tick, notice that this block inverts clock every 5 ticks,
// so the actual period of the clock is 10, not 5.
always begin 
    #5;
    clock=~clock; 
end 



// Start the "real" testbench here. Initial is the beginning of simulated time.
initial begin 

    // Monitors can be really useful, but for larger testbenches, they can
    // dump a huge amount of text to the screen.

    // Conceptually a monitor is a "magic" printf that will print itself
    // any time one of the signals changes.

    // Try uncommenting this monitor once and running the testbench...
    //$monitor("Time:%4.0f clock:%b A:%h B:%h CIN:%b SUM:%h COUT:%b", $time, clock, A, B, C_IN, SUM, C_OUT); 



    // Recall that verilog has an "unknown" state (x) which every signal
    // starts at. In practice, most internal registers will get set by a
    // reset signal and you will only need to specify testbench signals here
    A = 64'd0;
    B = 64'd0;
    C_IN = 0;

    // Don't forget to initialize the clock! Otherwise that always block
    // above will just keep inverting "x" to "x".
    clock = 0;

    $display("STARTING TESTBENCH!");

    // Finally, we can get to actually testing things!
    // (Remember you can use non-synthesizable Verilog in testbenches)

    // ------------------------------------------------------------------------------------------------------------
    // Here, we present a method to test every possible input
    @(negedge clock);
    // For every input A, 0..2^64-1
    for (i=0; i <= 64'hFFFF_FFFF_FFFF_FFFF; i=i+1) begin 
        // For every input B, 0..2^64-1
        for (j=0; j <= 64'hFFFF_FFFF_FFFF_FFFF ; j=j+1) begin 
            // Set the inputs
            A = i;
            B = j;
            C_IN = 0;

            // Since there's no clock, we have to add a delay
            // to allow signals to propagate
            #1

            // And check the result (aren't tasks great?)
            compare_correct_sum(A, B, SUM, C_IN, C_OUT);
            @(negedge clock);

            // And for the other carry
            C_IN = 1;
            #1
            compare_correct_sum(A, B, SUM, C_IN, C_OUT);
            @(negedge clock);
        end

        // How long will it take for this line to print?
        // How many times does it have to print?
        $display("Finished one inner loop");
    end

    // ------------------------------------------------------------------------------------------------------------
    // If we wish to test some specific cases instead...
    // Usually used for corner cases testing

    // Test 1
    @(negedge clock); 
    A = 0;
    B = 0;
    C_IN = 0;
    #1
    compare_correct_sum(A, B, SUM, C_IN, C_OUT);    


    // Test 2
    @(negedge clock); 
    A = 0;
    B = 0;
    C_IN = 1;
    #1
    compare_correct_sum(A, B, SUM, C_IN, C_OUT);    



    // ------------------------------------------------------------------------------------------------------------
    // Or, we could throw probability at the problem...

    // Random Tests
    @(negedge clock);
    for (i=0; i <= 99; i=i+1) begin 
        for (j=0; j <= 99 ; j=j+1) begin
            A = {$random,$random}; // What's up with this syntax?
            B = {$random,$random};
            #1
            compare_correct_sum(A, B, SUM, C_IN, C_OUT);
            @(negedge clock);
        end
    end

    
// DON'T FORGET TO FINISH THE SIMULATION
    $display("\nENDING TESTBENCH: SUCCESS!\n");
    $finish;

end

endmodule

