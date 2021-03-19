# make          <- runs simv (after compiling simv if needed)
# make simv     <- compile simv if needed (but do not run)
# make int      <- runs int_simv interactively (after compiling it if needed)
# make syn      <- runs syn_simv (after synthesizing if needed then 
#                                 compiling synsimv if needed)
# make syn_int  <- runs syn_int_simv interactively (much like make syn)
# make clean    <- remove files created during compilations (but not synthesis)
# make nuke     <- remove all files created during compilation and synthesis
#
# To compile additional files, add them to the TESTBENCH or SIMFILES as needed
# Every .vg file will need its own rule and one or more synthesis scripts
# The information contained here (in the rules for those vg files) will be 
# similar to the information in those scripts but that seems hard to avoid.
#

VCS = SW_VCS=2015.09 vcs -sverilog +vc -Mupdate -line -full64

all:    simv
#	./simv | tee program.out

##### 
# Modify starting here
#####

TESTBENCH = sys_defs.vh test_cd.v
SIMFILES = fsm_cd.v
SYNFILES = pa2_fsm.vg 
LIB = /afs/umich.edu/class/eecs470/lib/verilog/lec25dscc25.v


pa2_fsm.vg:	fsm_cd.v fsm_cd.tcl 
	dc_shell-t -f fsm_cd.tcl | tee synth.out


#####
# Should be no need to modify after here
#####

# updated interactive debugger "DVE", using the latest version of VCS
# awdeorio fall 2011
dve:	$(SIMFILES) $(TESTBENCH) 
	$(VCS) +memcbk $(TESTBENCH) $(SIMFILES) -o dve -R -gui

simv:	$(SIMFILES) $(TESTBENCH)
	$(VCS) $(TESTBENCH) $(SIMFILES)	-o simv -R | tee program.out

syn_simv:	$(SYNFILES) $(TESTBENCH)
	$(VCS) $(TESTBENCH) $(SYNFILES) $(LIB) -o syn_simv -R | tee syn_program.out

syn:	syn_simv
#	./syn_simv | tee syn_program.out

clean:
	rm -rvf simv *.daidir csrc vcs.key program.out \
	  syn_simv syn_simv.daidir syn_program.out \
          dve *.vpd *.vcd *.dump ucli.key 

nuke:	clean
	rm -rvf *.vg *.rep *.db *.chk *.log *.out DVEfiles/
