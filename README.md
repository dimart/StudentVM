StudentVM (SVM)

How to build it?
  RUN Makefile TO BUILD THE PROJECT
  IF YOU BUILD MANUALLY THEN ADD THE FILE StudentVM.ini WITH THE FOLLOWING LINE:
  MemorySize = 
  AFTER = YOU NEED TO SPECIFY THE AMOUNT OF MEMORY ALLOCATED FOR VIRTUAL MACHINE. 
  (Measured in cells) 

DESCRIPTION
  StudentVM is a virtual machine (using the ideas of Harvard architecture), written in С language.
  Latest SVM documentation can be viewed at: http://edu.vpolozov.name/studentasm

SYNOPSIS
  svm <path-to-svm-program> [-sm]

OPTIONS
  -sm 
     Show memory content  after execute.