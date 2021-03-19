# Stripped down Makefile

VCS = SW_VCS=2011.03 vcs -sverilog +vc -Mupdate -line -full64
LIB = /afs/umich.edu/class/eecs470/lib/verilog/lec25dscc25.v
TEST = isr_test.v
VG = ISR.vg

test: simv
	./simv

simv: $(TEST) $(VG) $(LIB)
	$(VCS) $^ -o $@

dve:	$(TEST) $(VG) $(LIB)
	$(VCS) +memcbk $^ -o dve -R -gui

clean:
	rm -rvf simv *.daidir csrc vcs.key program.out \
	  syn_simv syn_simv.daidir syn_program.out \
	  int_simv int_simv.daidir syn_int_simv syn_int_simv.daidir \
          dve *.vpd *.vcd *.dump ucli.key 

.PHONY: test clean
