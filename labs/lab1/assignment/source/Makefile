# make          <- runs simv (after compiling simv if needed)
# make simv     <- compiles simv without running
# make dve      <- runs GUI debugger (after compiling it if needed)
# make syn      <- runs syn_simv (after synthesizing if needed then 
#                                 compiling syn_simv if needed)
# make clean    <- remove files created during compilations (but not synthesis)
# make nuke     <- remove all files created during compilation and synthesis
#
# To compile additional files, add them to the TESTBENCH or SIMFILES as needed
# Every .vg file will need its own rule and one or more synthesis scripts
# The information contained here (in the rules for those vg files) will be 
# similar to the information in those scripts but that seems hard to avoid.
#

VCS = SW_VCS=2017.12-SP2-1 vcs -sverilog +vc -Mupdate -line -full64 +define+
LIB = /afs/umich.edu/class/eecs470/lib/verilog/lec25dscc25.v

all:	simv
	./simv | tee program.out

##### 
# Modify starting here
#####

TESTBENCH = tut_test.v
SIMFILES = tut_mod.v
SYNFILES = two_bit_pred.vg


#####
# Should be no need to modify after here
#####
simv:	$(SIMFILES) $(TESTBENCH)
	$(VCS) $(TESTBENCH) $(SIMFILES) -o simv

dve:	$(SIMFILES) $(TESTBENCH) 
	$(VCS) +memcbk $(TESTBENCH) $(SIMFILES) -o dve -R -gui

.PHONY: dve

syn_simv:	$(SYNFILES) $(TESTBENCH)
	$(VCS) $(TESTBENCH) $(SYNFILES) $(LIB) -o syn_simv

syn:	syn_simv
	./syn_simv | tee syn_program.out

clean:
	rm -rvf simv *.daidir csrc vcs.key program.out \
	syn_simv syn_simv.daidir syn_program.out \
	dve *.vpd *.vcd *.dump ucli.key 

nuke:	clean
	rm -rvf *.vg *.rep *.db *.chk *.log *.out DVEfiles/
