// This is an 8 stage (9 depending on how you look at it) pipelined 
// multiplier that multiplies 2 64-bit integers and returns the low 64 bits 
// of the result.  This is not an ideal multiplier but is sufficient to 
// allow a faster clock period than straight *
// This module instantiates 8 pipeline stages as an array of submodules.
module mult(
				input clock, reset,
				input [63:0] mcand, mplier,
				input start,
				
				output [63:0] product,
				output done
			);

  logic [63:0] mcand_out, mplier_out;
  logic [(7*64)-1:0] internal_products, internal_mcands, internal_mpliers;
  logic [6:0] internal_dones;
  
	mult_stage mstage [7:0]  (
		.clock(clock),
		.reset(reset),
		.product_in({internal_products,64'h0}),
		.mplier_in({internal_mpliers,mplier}),
		.mcand_in({internal_mcands,mcand}),
		.start({internal_dones,start}),
		.product_out({product,internal_products}),
		.mplier_out({mplier_out,internal_mpliers}),
		.mcand_out({mcand_out,internal_mcands}),
		.done({done,internal_dones})
	);

endmodule
