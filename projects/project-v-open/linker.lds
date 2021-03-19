/***************************************************
* linker.lds
* a linker script used to link RISC-V object files
* .text section starts at 0x0
*   - Jielun Tan, 02/2019
**************************************************/

OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv",
	"elf32-littleriscv")

OUTPUT_ARCH( "riscv" )
ENTRY(crt)

/* Setting memory regions */
MEMORY {
	MEM (rwx) : ORIGIN = 0x00000000, LENGTH = 0x10000
}


PHDRS {
	text PT_LOAD;
	rodata PT_LOAD;
	data PT_LOAD;
	bss PT_LOAD;
}

SECTIONS
{
  PROVIDE(_ram = ORIGIN(MEM));
  PROVIDE(_ram_end = _ram + LENGTH(MEM));

  

  .text ALIGN((ORIGIN(MEM) + 0x0), 32) : AT(ALIGN((ORIGIN(MEM) + 0x0), 32)) {
    PROVIDE(_ftext = .);
    *(.text.prologue)
    *(.text.init)
    *(.text.unlikely .text.unlikely.*)
    *(.text .text.* .gnu.linkonce.t.*)
    PROVIDE(_etext = .);
    . += 0x40; /* to create a gap between .text and .data b/c ifetch can fetch ahead from .data */
  } >MEM  :text

  .eh_frame ALIGN((ADDR(.text) + SIZEOF(.text)), 32) : AT(ALIGN((LOADADDR(.text) + SIZEOF(.text)), 32)) {
    *(.eh_frame)
  } >MEM  :text

  .rodata ALIGN((ADDR(.eh_frame) + SIZEOF(.eh_frame)), 32) : AT(ALIGN((LOADADDR(.eh_frame) + SIZEOF(.eh_frame)), 32)) ALIGN_WITH_INPUT {
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  } >MEM  :rodata

  .srodata ALIGN((ADDR(.rodata) + SIZEOF(.rodata)), 32) : AT(ALIGN((LOADADDR(.rodata) + SIZEOF(.rodata)), 32)) ALIGN_WITH_INPUT {
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata.*)
  } >MEM  :rodata

  /*.= ORIGIN(MEM);
  */
  .data ALIGN((ADDR(.srodata) + SIZEOF(.srodata)), 32) : AT(ALIGN((LOADADDR(.srodata) + SIZEOF(.srodata)), 32)) ALIGN_WITH_INPUT {
    *(.data .data.* .gnu.linkonce.d.*)
    *(.tohost) /* TODO: Support sections that aren't explicitly listed in this linker script */
  } >MEM  :data

  .sdata ALIGN((ADDR(.data) + SIZEOF(.data)), 32) : AT(ALIGN((LOADADDR(.data) + SIZEOF(.data)), 32)) ALIGN_WITH_INPUT {
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  } >MEM  :data

  PROVIDE(_data = ADDR(.data));
  PROVIDE(_data_lma = LOADADDR(.data));
  PROVIDE(_edata = .);

  .bss ALIGN((ADDR(.sdata) + SIZEOF(.sdata)), 32) : AT(ALIGN((LOADADDR(.sdata) + SIZEOF(.sdata)), 32)) ALIGN(8) {
    PROVIDE(_fbss = .);
    PROVIDE(__global_pointer$ = . + 0x7C0);
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.bss .bss.* .gnu.linkonce.b.*)
    . = ALIGN(4);
    PROVIDE(_ebss = .);
  } >MEM  :bss

  PROVIDE(_end = .);

  /*
   * heap_stack_region_usable_end: (ORIGIN(MEM) + LENGTH(MEM))
   * heap_stack_min_size: 4096
   * heap_stack_max_size: 1048576
   */
  PROVIDE(_sp = ALIGN(MIN((ORIGIN(MEM) + LENGTH(MEM)), _ebss + 1048576) - 7, 8));
  /*
   * Protect top of stack from heap, but this will not protect the heap from
   * stack overruns.
   */
  PROVIDE(_heap_end = _sp - 0x800);

  /* This section is a noop and is only used for the ASSERT */
  .stack : {
    ASSERT(_sp >= (_ebss + 4096), "Error: No room left for the heap and stack");
  }
}
