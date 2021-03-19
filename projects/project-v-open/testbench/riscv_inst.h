#ifndef INST_H
#define INST_H

#include <iostream>
#include <stdint.h> // <cstdint> is C++11, oh my
#include <cstdio>
#include <string>
#include <climits>

int32_t sign(uint32_t value) {
  if (value <= INT_MAX)
    return static_cast<int32_t>(value);
  if (value >= INT_MIN)
    return static_cast<int32_t>(value - INT_MIN) + INT_MIN;
  return (int32_t)value;
}

int64_t sign64(uint32_t value) {
  uint64_t mask = 0xFFFFFFFFull << 32;
  uint64_t sign = 0x80000000 & value;
  if (sign)
    return mask + value;
  else
    return value;
}

struct inst_t {
  bool is_valid;
  uint32_t opcode;
  uint32_t funct3;
  uint32_t funct7;
  uint32_t funct12;
  uint32_t rs1;
  uint32_t rs2;
  uint32_t rd;
  
  uint32_t imm_value;
  enum op_t {
    LUI,         // LUI rd imm: rd <- imm;
    ADDI,         // ADDI rd rs1 imm: rd <- rs1 + imm
    SLTI,        // SLTI rd rs1 imm: rd <- (rs1 < [signex]imm)
    SLTIU,        // SLTIU rd rs1 imm: rd <- (rs1 < imm)
    ANDI,        // ANDI rd rs1 imm: rd <- rs1 & imm
    ORI,        // ORI rd rs1 imm: rd <- rs1 | imm
    XORI,        // XORI rd rs1 imm: rd <- rs1 ^ imm
    SLLI,        // SLLI rd rs1 imm: rd <- rs1 << imm
    SRLI,        // SRLI rd rs1 imm: rd <- rs1 >> imm
    SRAI,        // SRAI rd rs1 imm: rd <- rs1 >>> imm [signex MSB]
    AUIPC,        // AUIPC rd imm: rd <- pc + imm
    ADD,        // ADD rd rs1 rs2: rd <- rs1 + rs2
    SLT,        // SLT rd rs1 rs2: rd <- (rs1 < [signex]rs2)
    SLTU,        // SLTU rd rs1 rs2: rd <- (rs1 < rs2)
    AND,        // AND rd rs1 rs2: rd <- rs1 & rs2
    OR,          // OR rd rs1 rs2: rd <- rs1 | rs2
    XOR,        // XOR rd rs1 rs2: rd <- rs1 ^ rs2
    SLL,        // SLL rd rs1 rs2: rd <- rs1 << rs2
    SRL,        // SRL rd rs1 rs2: rd <- rs1 >> rs2
    SRA,        // SRA rd rs1 rs2: rd <- rs1 >>> rs2
    SUB,        // SUB rd rs1 rs2: rd <- rs1 - rs2
    NOP,        // ADDI x0 x0 0
    JAL,        // JAL rd imm: pc <- pc + imm, rd <- oldpc + 4
    JALR,        // JALR rd rs1 imm: pc <- rs1 + imm, rd <- oldpc + 4
    BEQ,        // BEQ rs1 rs2 imm: if (rs1 == rs2) pc <- imm
    BNE,        // BNE rs1 rs2 imm: if (rs1 != rs2) pc <- imm
    BLT,        // BLT rs1 rs2 imm: if (rs1 < [signex]rs2) pc <- imm
    BLTU,        // BLTU rs1 rs2 imm: if (rs1 < rs2) pc <- imm
    BGE,        // BGE rs1 rs2 imm: if (rs1 >= [signex]rs2) pc <- imm
    BGEU,        // BGEU rs1 rs2 imm: if (rs1 >= rs2) pc <- imm
    // LOAD,      // LOAD rd rs1 imm: rd <- [rs1 + imm]
    LB,
    LH,
    LW,
    LBU,
    LHU,
    // STORE,      // STORE rs1 rs2 imm: [rs1 + imm] <- rs2
    SB,
    SH,
    SW,
    MUL,        // MUL rd rs1 rs2: rd <- (rs1 * rs2)[31:0]
    MULH,        // MULH rd rs1 rs2: rd <- ([signex]rs1 * [signex]rs2)[63:32]
    MULHU,        // MULHU rd rs1 rs2: rd <- (rs1 * rs2)[63:32]
    MULHSU,        // MULHSU rd rs1 rs2: rd <- ([signex]rs1 * rs2)[63:32]
    WFI,        // WFI: halt
    INVALID,
    NUM_OP_T
  };
  enum riscv_t {
              // 31        25 24     20 19     15 14        12 11    7 6        0
    RType,        //    funct7       rs2       rs1       funct3       rd     opcode
    IType,        //        immediate          rs1       funct3       rd     opcode 
    SType,        //     imm         rs2       rs1       funct3       imm    opcode
    UType,        //                      immediate          rd     opcode
    PType,        //         funct12       rs1     funct3    rd     opcode
    NUM_RISCV_T
  };
  enum imm_t {
    Rimm,        // Just no imm
    Iimm,
    Simm,
    Bimm,        // Inst encoding same as S, but B imm different
    Uimm,
    Jimm,        // Inst encoding same as U, but J imm different
    NUM_IMM_T
  };
  uint32_t inst;
  op_t op;
  riscv_t riscv;
  imm_t imm;
  const char* str;

  inst_t() {
    is_valid = false;
    rs1 = 0;
    rs2 = 0;
    rd = 0;
    imm_value = 0;
        str = NULL;
  }

  bool is_nop() {
    if (op == ADDI) {
      if (rd == 0 && rs1 == 0 && imm_value == 0) {
        return true;
      }
    }
    return false;
  }


  void decode(uint32_t inst_) {
    is_valid = true;
    inst = inst_;
    opcode = inst & 0x7f;
    funct3 = (inst >> 12) & 0x7;
    funct7 = inst >> 25;
    funct12 = inst >> 20; 
    str = "invalid";
    op = INVALID;
    
    switch (opcode) {
      case 0x37: {
        str = "LUI";
        op = LUI;
        riscv = UType;
        imm = Uimm;
        break;
      }
      case 0x17: { 
        str = "auipc";
        op = AUIPC;
        riscv = UType;
        imm = Uimm;
        break;
      }
      case 0x6f: { 
        str = "jal";
        op = JAL;
        riscv = UType;
        imm = Jimm;
        break;
      }
      case 0x67: { 
        str = "jalr";
        op = JALR;
        riscv = IType;
        imm = Iimm;
        break;
      }
      case 0x63: { // branch
        riscv = SType;
        imm = Bimm;
        switch (funct3) {
          case 0b000: {
            str = "beq";
            op = BEQ;
            break;
          }
          case 0b001: { 
            str = "bne";
            op = BNE;
            break;
          }
          case 0b100: { 
            str = "blt";
            op = BLT;
            break;
          }
          case 0b101: {
            str = "bge";
            op = BGE;
            break;
          }
          case 0b110: {
            str = "bltu";
            op = BLTU;
            break;
          }
          case 0b111: {
            str = "bgeu";
            op = BGEU;
            break;
          }
          default: {
            str = "invalid";
            op = INVALID;
            break;
          }
        }
        break;
      }
      case 0x03: { // load
        riscv = IType;
        imm = Iimm;
        switch (funct3) {
          case 0b000: {
            str = "lb";
            op = LB;
            break;
          }
          case 0b001: {
            str = "lh";
            op = LH;
            break;
          }
          case 0b010: {
            str = "lw";
            op = LW;
            break;
          }
          case 0b100: { 
            str = "lbu";
            op = LBU;
            break;
          }
          case 0b101: {
            str = "lhu";
            op = LHU;
            break;
          }
          default: {
            str = "invalid";
            op = INVALID;
            break;
          }
        }
        break;
      }
      case 0x23: { // store
        riscv = SType;
        imm = Simm;
        switch (funct3) {
          case 0b000: { 
            str = "sb";
            op = SB;
            break;
          }
          case 0b001: { 
            str = "sh";
            op = SH;
            break;
          }
          case 0b010: { 
            str = "sw";
            op = SW;
            break;
          }
          default: { 
            str = "invalid";
            op = INVALID;
            break;
          }
        }
        break;
      }
      case 0x13: { // immediate
        riscv = IType;
        imm = Iimm;
        switch (funct3) {
          case 0b000: {
            str = "addi";
            op = ADDI;
            break;
          }
          case 0b010: {
            str = "slti";
            op = SLTI;
            break;
          }
          case 0b011: { 
            str = "sltiu";
            op = SLTIU;
            break;
          }
          case 0b100: {
            str = "xori";
            op = XORI;
            break;
          }
          case 0b110: { 
            str = "ori";
            op = ORI;
            break;
          }
          case 0b111: {
            str = "andi";
            op = ANDI;
            break;
          }
          case 0b001: {
            if (funct7 == 0x00) {
              str = "slli";
              op = SLLI;
            }
            else {
              str = "invalid";
              op = INVALID;
            }
            break;
          }
          case 0b101: {
            if (funct7 == 0x00) {
              str = "srli";
              op = SRLI;
            }
            else if (funct7 == 0x20) {
              str = "srai";
              op = SRAI;
            }
            else {
              str = "invalid";
              op = INVALID;
            }
            break;
          }
        }
        break;
      }
      case 0x33: { // arithmetic
        riscv = RType;
        imm = Rimm;
        switch (funct7 << 4 | funct3) {
          case 0x000: {
            str = "add";
            op = ADD;
            break;
          }
          case 0x200: {
            str = "sub";
            op = SUB;
            break;
          }
          case 0x001: {
            str = "sll";
            op = SLL;
            break;
          }
          case 0x002: {
            str = "slt";
            op = SLT;
            break;
          }
          case 0x003: {
            str = "sltu";
            op = SLTU;
            break;
          }
          case 0x004: {
            str = "xor";
            op = XOR;
            break;
          }
          case 0x005: {
            str = "srl";
            op = SRL;
            break;
          }
          case 0x205: {
            str = "sra";
            op = SRA;
            break;
          }
          case 0x006: {
            str = "or";
            op = OR;
            break;
          }
          case 0x007: {
            str = "and";
            op = AND;
            break;
          }
          
          case 0x010: {
            str = "mul";
            op = MUL;
            break;
          }
          case 0x011: {
            str = "mulh";
            op = MULH;
            break;
          }
          case 0x012: {
            str = "mulhsu";
            op = MULHSU;
            break;
          }
          case 0x013: {
            str = "mulhu";
            op = MULHU;
            break;
          }
          case 0x014: {
            str = "div";
            op = INVALID;
            break;
          }  
          case 0x015: {
            str = "divu";
            op = INVALID;
            break;
          } 
          case 0x016: {
            str = "rem";
            op = INVALID;
            break;
          }  
          case 0x017: {
            str = "remu";
            op = INVALID;
            break;
          } 
          default: {
            str = "invalid";
            op = INVALID;
            break;
          }
        }
        break;
      }
      case 0x0f: {
        str = "fence";
        op = INVALID;
        break;
      } 
      case 0x73: { // system
        riscv = PType;
        imm = Rimm;
        switch (funct3) {
          case 0b000:
            
            switch (funct12) {
              case 0x000: {
                str = "ecall";
                op = INVALID;
                break;
              }
              case 0x001: {
                str = "ebreak";
                op = INVALID;
                break;
              }
              case 0x105: {
                str = "wfi";
                op = WFI;
                break;
              } 
              default: {
                str = "system";
                op = INVALID;
                break;
              }
            }
            break;
          case 0b001: {
            str = "csrrw";
            op = INVALID;
            break;
          }
          case 0b010: {
            str = "csrrs";
            op = INVALID;
            break;
          }
          case 0b011: {
            str = "csrrc";
            op = INVALID;
            break;
          }
          case 0b101: {
            str = "csrrwi";
            op = INVALID;
            break;
          }
          case 0b110: {
            str = "csrrsi";
            op = INVALID;
            break;
          }
          case 0b111: {
            str = "csrrci";
            op = INVALID;
            break;
          }
          default: {
            str = "invalid";
            op = INVALID;
            break;
          }
        }
        break;
      }
      default: {
        str = "invalid";
        op = INVALID;
        break;
      }
    }
    analyze();
  }

  void analyze() {
    rs2 = (inst >> 20) & 0x1F;
    rs1 = (inst >> 15) & 0x1F;
    rd = (inst >> 7) & 0x1F;
    uint32_t sign = (inst >> 31) & 0x1;
    uint32_t imask = (((1u << 22) - 1) << 11);
    uint32_t bmask = (((1u << 21) - 1) << 12);
    uint32_t jmask = (((1u << 13) - 1) << 20);
    
    switch(imm) {
      case Iimm: {
        // inst[30:20]
        // SLLI SRLI SRAI - shamt
        if (opcode == 0x13 && (funct3 == 0b001 || funct3 == 0b101)) {
          imm_value = (inst >> 20) & 0x1F;
        } else {
          imm_value = (inst >> 20) & 0x7FF;
          if (sign) {
            // 22{inst[31]}
            imm_value |= imask;
          }
        }
        break;
      }
      case Simm: {
        // inst[30:25] inst[11:7]
        imm_value = (inst >> 7) & 0x1F;
        imm_value |= ((inst >> 25) & 0x3F) << 5;
        if (sign) {
          imm_value |= imask;
        }
        break;
      }
      case Bimm: {
        // inst[7] inst[30:25] inst[11:8] 0
        imm_value = (inst >> 8) & 0xF;
        imm_value |= ((inst >> 25) & 0x3F) << 4;
        imm_value |= ((inst >> 7) & 0x1) << 10;
        imm_value <<= 1;
        if (sign) {
          imm_value |= bmask;
        }
        break;
      }
      case Uimm: {
        // inst[31:12]
        imm_value = (inst >> 12) & 0xFFFFF;
        // NOTE: Actually UType use shift left 12 bit value
        // imm_value <<= 12;
        break;
      }
      case Jimm: {
        // inst[19:12] inst[20] inst[30:21] 0
        imm_value = (inst >> 21) & 0x3FF;
        imm_value |= ((inst >> 20) & 0x1) << 10;
        imm_value |= ((inst >> 12) & 0xFF) << 11;
        imm_value <<= 1;
        if (sign) {
          imm_value |= jmask;
        }
        break;
      }
      default: break;
    }
  }

  struct select_t {
    riscv_t riscv;
    imm_t imm;
    uint32_t opcode;
    uint32_t funct3;
    uint32_t funct7;
    uint32_t funct12;
  };

    void assign(select_t sel) {
        riscv = sel.riscv;
        imm = sel.imm;
        opcode = sel.opcode;
        funct3 = sel.funct3;
        funct7 = sel.funct7;
        funct12 = sel.funct12;
    }

  select_t select(op_t iop) {
    select_t sel;
    switch(iop) {
      // IType
      case ADDI:
        sel.funct3 = 0b000;
        sel.opcode = 0x13;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case SLTI:
        sel.funct3 = 0b010;
        sel.opcode = 0x13;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case SLTIU:
        sel.funct3 = 0b011;
        sel.opcode = 0x13;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case ANDI:
        sel.funct3 = 0b111;
        sel.opcode = 0x13;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case ORI:
        sel.funct3 = 0b110;
        sel.opcode = 0x13;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case XORI:
        sel.funct3 = 0b100;
        sel.opcode = 0x13;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case SLLI:
        sel.funct3 = 0b001;
        sel.funct7 = 0x00;
        sel.opcode = 0x13;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case SRLI:
        sel.funct3 = 0b101;
        sel.funct7 = 0x00;
        sel.opcode = 0x13;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case SRAI:
        sel.funct3 = 0b101;
        sel.funct7 = 0x20;
        sel.opcode = 0x13;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      // UType
      case LUI:
        sel.opcode = 0x37;
        sel.imm = Uimm;
        sel.riscv = UType;
        break;
      case AUIPC:
        sel.opcode = 0x17;
        sel.imm = Uimm;
        sel.riscv = UType;
        break;
      // RType
      case ADD:
        sel.funct3 = 0b000;
        sel.funct7 = 0x00;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case SLT:
        sel.funct3 = 0b010;
        sel.funct7 = 0x00;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case SLTU:
        sel.funct3 = 0b011;
        sel.funct7 = 0x00;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case AND:
        sel.funct3 = 0b111;
        sel.funct7 = 0x00;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case OR:
        sel.funct3 = 0b110;
        sel.funct7 = 0x00;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case XOR:
        sel.funct3 = 0b100;
        sel.funct7 = 0x00;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case SLL:
        sel.funct3 = 0b001;
        sel.funct7 = 0x00;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case SRL:
        sel.funct3 = 0b101;
        sel.funct7 = 0x00;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case SRA:
        sel.funct3 = 0b101;
        sel.funct7 = 0x20;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case SUB:
        sel.funct3 = 0b000;
        sel.funct7 = 0x20;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      // UType
      case JAL:
        sel.opcode = 0x6f;
        sel.imm = Uimm;
        sel.riscv = UType;
        break;
      // IType
      case JALR:
        sel.funct3 = 0b000;
        sel.opcode = 0x67;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      // SType
      case BEQ:
        sel.funct3 = 0b000;
        sel.opcode = 0x63;
        sel.imm = Bimm;
        sel.riscv = SType;
        break;
      case BNE:
        sel.funct3 = 0b001;
        sel.opcode = 0x63;
        sel.imm = Bimm;
        sel.riscv = SType;
        break;
      case BLT:
        sel.funct3 = 0b100;
        sel.opcode = 0x63;
        sel.imm = Bimm;
        sel.riscv = SType;
        break;
      case BLTU:
        sel.funct3 = 0b110;
        sel.opcode = 0x63;
        sel.imm = Bimm;
        sel.riscv = SType;
        break;
      case BGE:
        sel.funct3 = 0b101;
        sel.opcode = 0x63;
        sel.imm = Bimm;
        sel.riscv = SType;
        break;
      case BGEU:
        sel.funct3 = 0b111;
        sel.opcode = 0x63;
        sel.imm = Bimm;
        sel.riscv = SType;
        break;
      // IType
      case LB:
        sel.funct3 = 0b000;
        sel.opcode = 0x03;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case LH:
        sel.funct3 = 0b001;
        sel.opcode = 0x03;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case LW:
        sel.funct3 = 0b010;
        sel.opcode = 0x03;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case LBU:
        sel.funct3 = 0b100;
        sel.opcode = 0x03;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      case LHU:
        sel.funct3 = 0b101;
        sel.opcode = 0x03;
        sel.imm = Iimm;
        sel.riscv = IType;
        break;
      // SType
      case SB:
        sel.funct3 = 0b000;
        sel.opcode = 0x23;
        sel.imm = Simm;
        sel.riscv = SType;
        break;
      case SH:
        sel.funct3 = 0b001;
        sel.opcode = 0x23;
        sel.imm = Simm;
        sel.riscv = SType;
        break;
      case SW:
        sel.funct3 = 0b010;
        sel.opcode = 0x23;
        sel.imm = Simm;
        sel.riscv = SType;
        break;
      // RType
      case MUL:
        sel.funct3 = 0b000;
        sel.funct7 = 0x01;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case MULH:
        sel.funct3 = 0b001;
        sel.funct7 = 0x01;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case MULHU:
        sel.funct3 = 0b011;
        sel.funct7 = 0x01;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      case MULHSU:
        sel.funct3 = 0b010;
        sel.funct7 = 0x01;
        sel.opcode = 0x33;
        sel.imm = Rimm;
        sel.riscv = RType;
        break;
      // PType
      case WFI:
        sel.opcode = 0x73;
        sel.funct3 = 0b000;
        sel.funct12 = 0x105;
        sel.imm = Rimm;
        sel.riscv = PType;
        break;
      default: break;
    }
    return sel;
  }

  uint32_t encode() {
    uint32_t instv = 0;
    switch (riscv) {
      case RType: {
        instv |= opcode;
        instv |= rd << 7;
        instv |= funct3 << 12;
        instv |= rs1 << 15;
        instv |= rs2 << 20;
        instv |= funct7 << 25;
        break;
      }
      case IType: {
        instv |= opcode;
        instv |= rd << 7;
        instv |= funct3 << 12;
        instv |= rs1 << 15;
        if (op == SLLI || op == SRLI || op == SRAI) {
          instv |= (imm_value & 0x1F) << 20;
          instv |= funct7 << 25;
        } else {
          instv |= imm_value << 20;
        }
        break;
      }
      case SType: {
        instv |= opcode;
        instv |= funct3 << 12;
        instv |= rs1 << 15;
        instv |= rs2 << 20;
        if (imm == Simm) {
          instv |= (imm_value & 0x1F) << 7;
          instv |= ((imm_value >> 5) & 0x3F) << 25;
        } else if (imm == Bimm) {
          instv |= ((imm_value >> 11) & 0x1) << 7;
          instv |= ((imm_value >> 1) & 0xF) << 8;
          instv |= ((imm_value >> 5) & 0x3F) << 25;
          instv |= ((imm_value >> 12) & 0x1) << 31;
        } else {
          printf("Impossible.\n");
        }
        break;
      }
      case UType: {
        instv |= opcode;
        instv |= rd << 7;
        if (imm == Uimm) {
          instv |= (imm_value & 0xFFFFF) << 12;
        } else if (imm == Jimm) {
          instv |= ((imm_value >> 12) & 0xFF) << 12;
          instv |= ((imm_value >> 11) & 0x1) << 20;
          instv |= ((imm_value >> 1) & 0x3FF) << 21;
          instv |= ((imm_value >> 20) & 0x1) << 31;
        }
        break;
      }
      case PType: {
        instv |= opcode;
        instv |= rd << 7;
        instv |= funct3 << 12;
        instv |= rs1 << 15;
        instv |= funct12 << 20;
        break;
      }
      default: break;
    }
    return instv;
  }

  int32_t sign_imm() {
    return sign(imm_value);
  }

};

#endif