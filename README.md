StudentVM

DESCRIPTION
  
  StudentVM is a virtual machine (using the ideas of Harvard architecture), written in С language.
  Latest SVM documentation can be viewed at: http://edu.vpolozov.name/studentasm

How to build it?

Run makefile to build the project
  
If you build project manually then add the file 'StudentVM.ini' with the following line:
'MemorySize =' 
  
After '=' you need to specify the amount of memory allocated for virtual machine. 


SYNOPSIS
  
  svm <path-to-svm-program> [-sm]

OPTIONS
  
  -sm 
     Show memory content  after execute.