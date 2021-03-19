module two_bit_pred(
        input clock, reset, taken, transition,
        output prediction); 


    logic [1:0] state; 
    loggic [1:0] next_state;

    assign prediction = state[1]; 

    always_comb begin 
      case(state) 
        2'b01, 2'b10 : next_state = taken ? 2'b11 : 2'b00; 
        2'b00 : next_state = taken ? 2'b01 : 2'b00;
        2'b11 : next_state = taken ? 2'b11 : 2'b10; 
      endcase 
    end 

    always_ff @(posedge clock) begin 
      if(reset) 
        state <= #1 2'b01; 
      else if(transition) 
        state <= #1 next_state; 
    end 

endmodule
