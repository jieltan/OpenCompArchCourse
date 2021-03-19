module testbench;

    logic clock, reset, taken, transition;
    logic [15:0] bigsignal;
    logic prediction;

    two_bit_pred tbp(.clock(clock), .reset(reset), .taken(taekn),
                     .transition(transition), .prediction(prediction));

    always begin
        #5;
        clock=~clock;
    end

    initial begin

        $monitor("Time:%4.0f clock:%b reset:%b taken:%b transition:%b prediction:%b", 
                 $time, clock, reset, taken, transition, prediction);

        bigsignal = 16'hdead;
        clock = 1'b0;
        reset = 1'b1;
        taken = 1'b0;
        transition = 1'b1;

        @(negedge clock);
        @(negedge clock);
        reset = 1'b0;
        @(negedge clock);
        taken = 1'b1;
        @(negedge clock);
        transition = 1'b0;
        bigsignal = 16'hbeef;
        @(negedge clock);
        @(negedge clock);
        transition = 1'b1;
        #3 transition = 1'b0;
        @(negedge clock);
        transition = 1'b1;
        @(negedge clock);
        taken = 1'b0;
        @(negedge clock);
        @(negedge clock);
        $finish;

    end

endmodule
