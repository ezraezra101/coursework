AUTHOR: Ezra Davis & Gareth Solbeck

DATE: Friday February 6th

PROJECT ID: Project 2: Kernel Interception

CS COURSE: CS3013

PROGRAMMING LANGUAGE: C

OS/HARDWARE DEPENDENCIES: Ubuntu 14.04 with three additional system calls as specified in project 0; a single core environment.

###### PROBLEM DESCRIPTION:
In assignment PDF. Allows admins to Smite and Unsmite users' processes. Creates a "virus" scanner that detects the word "virus" in any file being read and logs opening and closing of files.

----------

# Using the program

## Part 1: On-Access Anti-Virus Scanner
*All commands should be run from the part1 folder.*
###### Building the program

`make`

###### Installing the program

`sudo insmod antivirus.ko`

Uninstall: `sudo rmmod antivirus.ko`

###### Running the program:

Part 1 runs whenever a file is opened. Output can be read in `/var/log/syslog`.

###### Testing the program

`make test`

An excerpt from the kernel log can be found in `./syslog-results`


## Part 2: Smiting (and Unsmiting) Unruly Users
*All commands should be run from the part2 folder.*

###### Building the program

`make build`

###### Installing the program

`sudo insmod smite_module`

Uninstall: `sudo rmmod smite_module`

###### Running the program:

`sudo ./smite [userid] > [file_for_saving_pids]`

`sudo ./unsmite < [file_for_saving_pids]`

###### Testing the program

1. After installing the module, run a smitable process as another user (we recommend our spin program).
2. While it is still running, run `./smite [userid] > smite_output.txt`.
3. Verify that the process has been smited (`ps -aux | grep [program]` is helpful here).
4. Run `./unsmite < smite_output.txt`.
5. The process should now continue.
6. Check `/var/log/syslog` for output.

We have streamlined this for our workflow with `make test`. However, `make test` may have varying results on other systems.

----------

# Files

###### Source files:

* README.txt
* part1/
	* Makefile      - Makefile for the 'antivirus'
	* antivirus.c   - Source for a kernel module that intercepts calls to open, close, and read, and logs information about those calls.
	* test_module.c - The example module included in the project specification.
	* testcalls.c  - Test calls from Project 0, to test the example module.

* part2/
	* Makefile       - Makefile for smiting and unsmiting.
	* smite.c        - Code for the user-space command line executable permitting smiting
	* unsmite.c      - Code for the user-space command line executable permitting unsmiting
	* smite_module.c - The source for the kernel module allowing processes to be smited and unsmited.
	* spin.c         - Source for a executable that is eminently smitable.

###### Temporary files:
* part1/:
	* antivirus.mod.c, Module.symvers, modules.order, and hidden files
* part2/:
	* smite_module.mod.c, Module.symvers, modules.order, smite_module.mod.o, smite_module.o and hidden files

###### Executables/Kernel modules:
* part1/
	* antivirus.ko - Compiled antivirus module file

* part2/
	* spin     - Program used for testing smite
	* smite    - Uses the smite call to mark processes as unrunnable, and lists them in a format that unsmite can accept.
	* unsmite  - Uses the unsmite call to return given processes to their (specified) original state.
	* smite_module.ko - Compiled module file for smiting and unsmiting.

###### Testing (see "Testing the program" sections above):
* part1/
	* cleanfile.txt
	* infectedfile.txt
	* syslog-results
* part2/
	* smite_output.txt
	* syslog-results
