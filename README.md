## Overview

This is the repository associated with our SIGCSE '21 paper "RISC-V Reward: Building Out-of-Order Processors in a Computer Architecture Design Course with an Open-Source ISA". 

* What's in the repo: 
	* the paper and slides (Creative Commons, woohoo!) 
	* current (as of 2021) project and lab assignments in PDF and LaTeX
	* a pointer to the repo containing the in-order RISC-V CPU developed for the course
	* link to the current course web page 

* What's not in the repo:
	* An out-of-order RISC-V CPU (more on this below)
	* Project/lab solutions

### Abstract

*"We describe our experience teaching an undergraduate capstone (and elective graduate course) in computer architecture with a semester-long project in which teams of five students design and implement an out-of-order (OoO) pipelined processor core using the open-source RISC-V instruction set. The course content includes OoO scheduling algorithms for instructions to exploit instruction-level parallelism (ILP), example microarchitectures, caching, prefetching, and virtual memory. The labs and projects help students gain proficiency with the SystemVerilog language.*

*Students use the concepts learned in class to design processors with the goals of achieving correctness and high performance for a suite of test programs representative of different data structures and algorithms. Using RISC-V enables students to validate and benchmark their designs by compiling test programs using GCC with a custom linker. By collaborating as a team, students learn how to write and debug a large code base over the two-month project.*

*We explain the project content and process in detail, identify the challenges involved for students and the necessary instructor support, and share statistics and student feedback about the project. We have open-sourced our lab and project materials to enable others to teach similar courses."*

## Some words about access and distribution
Our paper and CPU design are released under the Creative Commons license because we want other people to be able to reuse our work! Please feel free to copy, in whole or in part, anything from the CPU design or paper for use in your own non-commercial course or project. All we ask is that you attribute the original credit to us (this can be as simple as a comment in the HDL).

As of this writing most course documents are available on the course web site. However we do not have the ability to release these documents under the Creative Commons license because we do not have the permission of all the authors.

## Course Web Site

As of 2021, most (all?) assignments/slides/starter code for the course is available at:
http://www.eecs.umich.edu/eecs/courses/eecs470/?page=home.php

If you can't find what you're looking for, email us and we'll try to help.

## FAQ
Q: **Wait, I don't see an out-of-order CPU here. Isn't that the whole point of this class?**

A: The students build their OoO design from the in-order CPU design we provide. We have not publicly released a fully-functional OoO design because we want students to create their own design. If you'd like to see an example of a completed project, please get in touch with us by email and we would be happy to send you the code.

Q: **How do I build the project?**

Currently you will need to replace the build commands in the Makefile in all the projects with your technology library and simulator to properly compile and simulate


