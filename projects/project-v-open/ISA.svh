////////////////////////////////////////////////////////////////////////////////////////////
// ISA.svh
// Jielun Tan (jieltan@umich.edu)
// 07/13/2018
//
// This file contains global definitions for all RISC-V ISA related things
// This decoding format is mostly taken from Bespoke Silicon Group (Michael Taylor's Group)
// which I guess is taken from VSCALE???
// Currently this contains instructions for RV32IMA and RV64IMA
// Any extensions added in the future should be appended here
////////////////////////////////////////////////////////////////////////////////////////////

`ifndef __ISA_SVH__
`define __ISA_SVH__

// RV32 Opcodes
`define RV32_LOAD     7'b0000011 //load, self-explanatory
`define RV32_STORE    7'b0100011 //store, self-explanatory
//`define RV32_MADD     7'b1000011 //FUSED multiply and add, no long valid?

// we have branch instructions ignore the low bit so that we can place the prediction bit there
// RISC-V by default has the low bits set to 11 in the icache, so we can use those creatively
// note this relies on all code using ==? and casez.

`define RV32_BRANCH   7'b1100011

`define RV32_LOAD_FP  7'b0000111 //floating point load
`define RV32_STORE_FP 7'b0100111 //floating point store
//`define RV32_MSUB     7'b1000111 //FUSED multiply and sub, no longer valid?
`define RV32_JALR_OP  7'b1100111 //jump and link with return
//`define RV32_CUSTOM_0 7'b0001011
//`define RV32_CUSTOM_1 7'b0101011
//`define RV32_NMSUB    7'b1001011 //FUSED negative multiply and sub, no longer valid?
//                    7'b1101011 is reserved
`define RV32_FENCE    7'b0001111 //FENCE instruction for enforcing memory consistency
`define RV32_AMO      7'b0101111 //atomic memory operation
//`define RV32_NMADD    7'b1001111 //FUSED negative multiply and sub, no longer valid?
`define RV32_JAL_OP   7'b1101111 //just jump and link
`define RV32_OP_IMM   7'b0010011
`define RV32_OP       7'b0110011
`define RV32_OP_FP    7'b1010011
`define RV32_SYSTEM   7'b1110011
`define RV32_AUIPC_OP 7'b0010111
`define RV32_LUI_OP   7'b0110111
//                    7'b1010111 is reserved
//                    7'b1110111 is reserved
`define RV64_OP_IMM_W 7'b0011011 //is RV64-specific, used for 32 bit imm operations
`define RV64_OP_W     7'b0111011 //is RV64-specific, used for 32 bit operations
`define RV32_CUSTOM_2 7'b1011011
`define RV32_CUSTOM_3 7'b1111011

`define RV32_CSRRW_FUN3 3'b001
`define RV32_CSRRS_FUN3 3'b010
`define RV32_CSRRC_FUN3 3'b011

`define RV32_CSRRWI_FUN3 3'b101
`define RV32_CSRRSI_FUN3 3'b110
`define RV32_CSRRCI_FUN3 3'b111

//MUL_DIV defines
`define MD_MUL_FUN3       3'b000
`define MD_MULH_FUN3      3'b001
`define MD_MULHSU_FUN3    3'b010
`define MD_MULHU_FUN3     3'b011
`define MD_DIV_FUN3       3'b100
`define MD_DIVU_FUN3      3'b101
`define MD_REM_FUN3       3'b110
`define MD_REMU_FUN3      3'b111

//FENCE defines
`define RV32_FENCE_FUN3   3'b000
`define RV32_FENCE_I_FUN3 3'b001


// Some useful RV32 instruction macros
`define RV32_Rtype(op, funct3, funct7) {``funct7``, {5{1'b?}},  {5{1'b?}},``funct3``, {5{1'b?}},``op``}
`define RV32_Itype(op, funct3)         {{12{1'b?}},{5{1'b?}},``funct3``,{5{1'b?}},``op``} 
`define RV32_Stype(op, funct3)         {{7{1'b?}},{5{1'b?}},{5{1'b?}},``funct3``,{5{1'b?}},``op``}
`define RV32_Utype(op)                 {{20{1'b?}},{5{1'b?}},``op``}

// We have to delete the white space in micro definition, otherwise Design
// Compiler would issue warings. PS: I guess nothing is issuing warnings?
// Mike Taylor's group might be tripping there...
// RV32IM Instruction encodings
// Note that these instructions is the EXACT SAME for RV64IM, except that
// all operand register would be 64 bits instead, they are labeled RV32 for
// legacy reasons, but really they can both be for 32 or 64 bit
// There are some special 64 bit instructions that will only produce 32 bit
// results, more on that later
`define RV32_LUI       `RV32_Utype(`RV32_LUI_OP)
`define RV32_AUIPC     `RV32_Utype(`RV32_AUIPC_OP)
`define RV32_JAL       `RV32_Utype(`RV32_JAL_OP)
`define RV32_JALR      `RV32_Itype(`RV32_JALR_OP, 3'b000)
`define RV32_BEQ       `RV32_Stype(`RV32_BRANCH, 3'b000)
`define RV32_BNE       `RV32_Stype(`RV32_BRANCH, 3'b001)
`define RV32_BLT       `RV32_Stype(`RV32_BRANCH, 3'b100)
`define RV32_BGE       `RV32_Stype(`RV32_BRANCH, 3'b101)
`define RV32_BLTU      `RV32_Stype(`RV32_BRANCH, 3'b110)
`define RV32_BGEU      `RV32_Stype(`RV32_BRANCH, 3'b111)
`define RV32_LB        `RV32_Itype(`RV32_LOAD, 3'b000)
`define RV32_LH        `RV32_Itype(`RV32_LOAD, 3'b001)
`define RV32_LW        `RV32_Itype(`RV32_LOAD, 3'b010)
`define RV32_LBU       `RV32_Itype(`RV32_LOAD, 3'b100)
`define RV32_LHU       `RV32_Itype(`RV32_LOAD, 3'b101)
`define RV32_SB        `RV32_Stype(`RV32_STORE, 3'b000)
`define RV32_SH        `RV32_Stype(`RV32_STORE, 3'b001)
`define RV32_SW        `RV32_Stype(`RV32_STORE, 3'b010)
`define RV32_ADDI      `RV32_Itype(`RV32_OP_IMM,3'b000)
`define RV32_SLTI      `RV32_Itype(`RV32_OP_IMM, 3'b010)
`define RV32_SLTIU     `RV32_Itype(`RV32_OP_IMM, 3'b011)
`define RV32_XORI      `RV32_Itype(`RV32_OP_IMM, 3'b100)
`define RV32_ORI       `RV32_Itype(`RV32_OP_IMM, 3'b110)
`define RV32_ANDI      `RV32_Itype(`RV32_OP_IMM, 3'b111)
`define RV32_SLLI      `RV32_Rtype(`RV32_OP_IMM, 3'b001, 7'b0000000)
`define RV32_SRLI      `RV32_Rtype(`RV32_OP_IMM, 3'b101, 7'b0000000)
`define RV32_SRAI      `RV32_Rtype(`RV32_OP_IMM, 3'b101, 7'b0100000)
`define RV32_ADD       `RV32_Rtype(`RV32_OP,3'b000,7'b0000000)
`define RV32_SUB       `RV32_Rtype(`RV32_OP, 3'b000, 7'b0100000)
`define RV32_SLL       `RV32_Rtype(`RV32_OP, 3'b001, 7'b0000000)
`define RV32_SLT       `RV32_Rtype(`RV32_OP, 3'b010, 7'b0000000)
`define RV32_SLTU      `RV32_Rtype(`RV32_OP, 3'b011, 7'b0000000)
`define RV32_XOR       `RV32_Rtype(`RV32_OP, 3'b100, 7'b0000000)
`define RV32_SRL       `RV32_Rtype(`RV32_OP, 3'b101, 7'b0000000)
`define RV32_SRA       `RV32_Rtype(`RV32_OP, 3'b101, 7'b0100000)
`define RV32_OR        `RV32_Rtype(`RV32_OP, 3'b110, 7'b0000000)
`define RV32_AND       `RV32_Rtype(`RV32_OP, 3'b111, 7'b0000000)
`define RV32_FENCE_     `RV32_Itype(`RV32_FENCE, `RV32_FENCE_FUN3)
`define RV32_FENCE_I   `RV32_Itype(`RV32_FENCE, `RV32_FENCE_I_FUN3)

//RV32M
`define RV32_MUL       `RV32_Rtype(`RV32_OP, `MD_MUL_FUN3   , 7'b0000001) 
`define RV32_MULH      `RV32_Rtype(`RV32_OP, `MD_MULH_FUN3  , 7'b0000001) 
`define RV32_MULHSU    `RV32_Rtype(`RV32_OP, `MD_MULHSU_FUN3, 7'b0000001) 
`define RV32_MULHU     `RV32_Rtype(`RV32_OP, `MD_MULHU_FUN3 , 7'b0000001) 
`define RV32_DIV       `RV32_Rtype(`RV32_OP, `MD_DIV_FUN3   , 7'b0000001) 
`define RV32_DIVU      `RV32_Rtype(`RV32_OP, `MD_DIVU_FUN3  , 7'b0000001) 
`define RV32_REM       `RV32_Rtype(`RV32_OP, `MD_REM_FUN3   , 7'b0000001) 
`define RV32_REMU      `RV32_Rtype(`RV32_OP, `MD_REMU_FUN3  , 7'b0000001) 
//RV32A
`define RV32_LR_W       `RV32_Rtype(`RV32_AMO, 3'b010, 7'b00010??)
`define RV32_SC_W       `RV32_Rtype(`RV32_AMO, 3'b010, 7'b00011??)
`define RV32_AMOSWAP_W  `RV32_Rtype(`RV32_AMO, 3'b010, 7'b00001??)
`define RV32_AMOADD_W   `RV32_Rtype(`RV32_AMO, 3'b010, 7'b00000??)
`define RV32_AMOXOR_W   `RV32_Rtype(`RV32_AMO, 3'b010, 7'b00100??)
`define RV32_AMOAND_W   `RV32_Rtype(`RV32_AMO, 3'b010, 7'b01100??)
`define RV32_AMOOR_W    `RV32_Rtype(`RV32_AMO, 3'b010, 7'b01000??)
`define RV32_AMOMIN_W   `RV32_Rtype(`RV32_AMO, 3'b010, 7'b10000??)
`define RV32_AMOMAX_W   `RV32_Rtype(`RV32_AMO, 3'b010, 7'b10100??)
`define RV32_AMOMINU_W  `RV32_Rtype(`RV32_AMO, 3'b010, 7'b11000??)
`define RV32_AMOMAXU_W  `RV32_Rtype(`RV32_AMO, 3'b010, 7'b11100??)
//System calls
`define ECALL           {25'b0, `RV32_SYSTEM}
`define EBREAK          {12'b1, 13'b0, `RV32_SYSTEM}
`define WFI             {12'b000100000101, 13'b0, `RV32_SYSTEM}

`define RV32_CSRRW      `RV32_Itype(`RV32_SYSTEM, `RV32_CSRRW_FUN3)
`define RV32_CSRRS      `RV32_Itype(`RV32_SYSTEM, `RV32_CSRRS_FUN3)
`define RV32_CSRRC      `RV32_Itype(`RV32_SYSTEM, `RV32_CSRRC_FUN3)
                                                                 
`define RV32_CSRRWI     `RV32_Itype(`RV32_SYSTEM, `RV32_CSRRWI_FUN3)
`define RV32_CSRRSI     `RV32_Itype(`RV32_SYSTEM, `RV32_CSRRSI_FUN3)
`define RV32_CSRRCI     `RV32_Itype(`RV32_SYSTEM, `RV32_CSRRCI_FUN3)

//RV64I
// The instructions that end with W indicates that it will only produce a 32 bit result
// The only difference in shift is that there's an extra bit to the shift amount because
// of increased register width
`define RV64_LWU        `RV32_Itype(`RV32_LOAD, 3'b110)
`define RV64_LD         `RV32_Itype(`RV32_LOAD, 3'b011)
`define RV64_SD         `RV32_Stype(`RV32_STORE, 3'b011)
`define RV64_SLL        `RV32_Rtype(`RV32_OP, 3'b001, 7'b000000?)
`define RV64_SRL        `RV32_Rtype(`RV32_OP, 3'b101, 7'b000000?)
`define RV64_SRA        `RV32_Rtype(`RV32_OP, 3'b101, 7'b010000?)
`define RV64_ADDIW      `RV32_Itype(`RV64_OP_IMM_W, 3'b000)
`define RV64_SLLIW      `RV32_Rtype(`RV64_OP_IMM_W, 3'b001, 7'b0000000)
`define RV64_SRLIW      `RV32_Rtype(`RV64_OP_IMM_W, 3'b101, 7'b0000000)
`define RV64_SRAIW      `RV32_Rtype(`RV64_OP_IMM_W, 3'b101, 7'b0100000)
`define RV64_SUBW       `RV32_Rtype(`RV64_OP_W, 3'b001, 7'b0000000)
`define RV64_ADDW       `RV32_Rtype(`RV64_OP_W, 3'b101, 7'b0000000)
`define RV64_SLLW       `RV32_Rtype(`RV64_OP_W, 3'b001, 7'b0000000)
`define RV64_SRLW       `RV32_Rtype(`RV64_OP_W, 3'b101, 7'b0000000)
`define RV64_SRAW       `RV32_Rtype(`RV64_OP_W, 3'b101, 7'b0100000)
//RV64M
`define RV64_MULW       `RV32_Rtype(`RV32_OP_W, `MD_MUL_FUN3   , 7'b0000001) 
`define RV64_DIVW       `RV32_Rtype(`RV32_OP_W, `MD_DIV_FUN3   , 7'b0000001) 
`define RV64_DIVUW      `RV32_Rtype(`RV32_OP_W, `MD_DIVU_FUN3  , 7'b0000001) 
`define RV64_REMW       `RV32_Rtype(`RV32_OP_W, `MD_REM_FUN3   , 7'b0000001) 
`define RV64_REMUW      `RV32_Rtype(`RV32_OP_W, `MD_REMU_FUN3  , 7'b0000001) 
//RV64A, really the same thing as RV32A with 1 bit difference...
`define RV64_LR_D       `RV32_Rtype(`RV32_AMO, 3'b011, 7'b00010??)
`define RV64_SC_D       `RV32_Rtype(`RV32_AMO, 3'b011, 7'b00011??)
`define RV64_AMOSWAP_D  `RV32_Rtype(`RV32_AMO, 3'b011, 7'b00001??)
`define RV64_AMOADD_D   `RV32_Rtype(`RV32_AMO, 3'b011, 7'b00000??)
`define RV64_AMOXOR_D   `RV32_Rtype(`RV32_AMO, 3'b011, 7'b00100??)
`define RV64_AMOAND_D   `RV32_Rtype(`RV32_AMO, 3'b011, 7'b01100??)
`define RV64_AMOOR_D    `RV32_Rtype(`RV32_AMO, 3'b011, 7'b01000??)
`define RV64_AMOMIN_D   `RV32_Rtype(`RV32_AMO, 3'b011, 7'b10000??)
`define RV64_AMOMAX_D   `RV32_Rtype(`RV32_AMO, 3'b011, 7'b10100??)
`define RV64_AMOMINU_D  `RV32_Rtype(`RV32_AMO, 3'b011, 7'b11000??)
`define RV64_AMOMAXU_D  `RV32_Rtype(`RV32_AMO, 3'b011, 7'b11100??)

// RV32 Immediate signed/unsigned(U is technically unsigned) extension macros
`define RV32_signext_Iimm(instr) {{21{``instr``[31]}},``instr``[30:20]}
`define RV32_signext_Simm(instr) {{21{``instr``[31]}},``instr[30:25],``instr``[11:7]}
`define RV32_signext_Bimm(instr) {{20{``instr``[31]}},``instr``[7],``instr``[30:25],``instr``[11:8], {1'b0}}
`define RV32_signext_Uimm(instr) {``instr``[31:12], {12{1'b0}}}
`define RV32_signext_Jimm(instr) {{12{``instr``[31]}},``instr``[19:12],``instr``[20],``instr``[30:21], {1'b0}} 

// RV32 12bit Immediate injection/extraction, replace the Imm content with specified value
// for injection, input immediate value index starting from 1
// * store the sign bit (bit 31) of branches into bit of the stored instruction for use as prediction bit
// You probably will never need this...

`define RV32_Bimm_12inject1(instr,value) {``value``[12], ``value``[10:5], ``instr``[24:12],\
                                          ``value``[4:1],``value``[11],``instr``[6:1],``instr``[31]}
`define RV32_Jimm_12inject1(instr,value) {1'b0, ``value``[10:1], ``value``[11], 7'b0,``value``[12], ``instr``[11:0]}

`define RV32_Bimm_12extract(instr) {``instr``[31], ``instr``[7], ``instr``[30:25], ``instr``[11:8]};
`define RV32_Jimm_12extract(instr) {``instr``[12], ``instr``[20],``instr``[30:21] };

localparam RV32_instr_width_gp    = 32;
localparam RV32_reg_data_width_gp = 32;
localparam RV32_reg_addr_width_gp = 5;
localparam RV32_shamt_width_gp    = 5;
localparam RV32_opcode_width_gp   = 7;
localparam RV32_funct3_width_gp   = 3;
localparam RV32_funct7_width_gp   = 7;
localparam RV32_Iimm_width_gp     = 12;
localparam RV32_Simm_width_gp     = 12;
localparam RV32_Bimm_width_gp     = 12;
localparam RV32_Uimm_width_gp     = 20;
localparam RV32_Jimm_width_gp     = 20;

`endif
