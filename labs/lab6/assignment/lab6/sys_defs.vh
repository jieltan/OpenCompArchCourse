`ifndef __SYS_DEFS_VH__
`define __SYS_DEFS_VH__

`ifdef  SYNTH_TEST
`define DUT(mod) mod``_svsim
`else
`define DUT(mod) mod
`endif

typedef enum logic {READ, WRITE} COMMAND;

`define TEST_SIZE 8

`endif