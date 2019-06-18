Author:				Ezra Davis
Date:				February 2014
Version:			1.0
Project ID:			Program 3: Event-Driven Simulation of a Processor Scheduling Queue in C
CS Class:			CS 2303
Programming Language:		C
OS/Hardware dependencies:	Uses the sys/stat.h library


How to build the program:	(From this directory)
	make 		#builds the program
	
	make clean	#cleans up the object files
	
	#OR:	
	make run	#builds, cleans, and runs tests


How to run the program:
	./edavis_prog3 [sources] [time-slice] <inputData.txt
	
	eg:
		./edavis_prog3 36 5 <testfiles/prog3.dat
		
	or:
	./edavis_prog3 [sources] [time-slice]
	And then input information according to the prompt

I/O:
	Takes input through the command line.
	Takes two arguments, the number of processes (sources), and the timeslice (used in round robin schedulers)
	
	It then scans the input stream for a list of trios of integers, representing the process ID of a process, its arrival time, and the CPU time required. (In 100s of milliseconds)
	
	The outputs of the program are put into the SimulationResults folder. If the folder doesn't exist, it is created.


Problem Description:
	Runs a simulation for a processor dealing with some processes with:
		a First Come First Serve scheduler
		a round robin scheduler
		a preemptive round robin scheduler
	more specifications of the problem at: http://web.cs.wpi.edu/~rek/Systems/C14/Program3_C14.pdf

Implementation Details:

	If two processIDs come in at the same time, the one with the lower process ID is handled first.
	
	In the Preemptive Round Robin Scheduler, the preemption is handled in a First In, Last Out (Stacklike FIFO) ordering.
	This means if 3 programs arrive very close together, the last one will get its preempted timeslice, then the middle, and then the first.

	The program uses a linked list to keep track of arrivals to the simulated processor, and then removes them from the list during simulation, and places the nodes of the linked list into a different list containing only completed processes.
	This is different than the recommended method in the problem specification.
	
		
Program Source:	All in the source folder
	free_list.c			fcfs.c				listcpy.c			print_eventList.c
	create_eventList.c	header.h			main.c				print_node.c
	create_node.c		insert_node.c		mean.c				print_to_file.c
	dequeue.c			insert_node_queue.c	options_to_file.c	round_robin.c
	enqueue.c			isEmpty.c			preemptive_rr.c		variance.c


Additional Files:	
	Makefile
	README.txt
	edavis_prog3 (The executable, may or may not be present)
	
	testfiles: (Holds test data)
		prog3.dat	testData.txt	testData2.txt

	ExampleSimulationResults:
		FCFS_queue.txt
		inputData.txt
		preemptive_rr_queue.txt
		round_robin_queue.txt

	Object files (.o) are located in the /objs directory.
		Building the program will populate it, and running: make clean   will remove it.

Results:		
	Test results are located in the ExampleSimulationResults folder.
	inputData.txt has information about the input processes and command line options.
	
	
Test Procedures:
	make test
	
	Which is equivalent to:
	./edavis_prog3 5 1 <testfiles/testData.txt
	./edavis_prog3 36 5 <testfiles/prog3.dat
	
	Note: the second test overwrites the output of the first. The first isn't really necessary anymore, but is a legacy.


Test Data:	
	Testfiles are located in the testfiles directory.
	They are:
		prog3.dat
		testData.txt
		testData2.txt


References:
	Makefile resources:
		http://www.cyberciti.biz/faq/linux-unix-shell-check-if-directory-empty/
		http://stackoverflow.com/questions/1814270/gcc-g-option-to-place-all-object-files-into-separate-directory
	Variance formula:
		http://en.wikipedia.org/wiki/Variance#Examples
	C's mkdir function:
		http://stackoverflow.com/questions/8719867/linux-mkdir-function-cant-authorize-full-permission
	Opening files in C:
		http://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
	Deleting files in C:
		http://www.gnu.org/software/libc/manual/html_node/Deleting-Files.html
	Assignment PDF:
		http://web.cs.wpi.edu/~rek/Systems/C14/Program3_C14.pdf
		