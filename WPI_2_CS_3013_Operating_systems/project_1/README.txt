Author:                     Ezra Davis & Gareth Solbeck
Date:                       26-01-2014
Project ID:                 Project 1: Linux Shell
CS Class:                   CS 3013
Programming Language:       C
OS/Hardware dependencies:   *nix (tested on Ubuntu 14.04 and Mac OSX Yosemite)

Problem Description:        Write a basic Linux shell that supports
                                background tasks


Overall Design:
    Data representation:    shell2 stores jobs in a linked list, defined in
                                joblist.c

    Algorithms:             joblist.c includes typical linked list functions
                               (e.g: add, remove, isEmpty)

Interfaces:
    User:                   runCommand takes in arguments from the user
                            shell, shell2 interactively take input from
                                the user

    File/DB:                shell, shell2 support redirected files as input


How to build the program:   make clean all

How to run the program:     ./runCommand [command] [args ...]
                            ./shell
                            ./shell2 [--no-stats]
                                --no-stats sets a flag which disables the
                                    display of process data

How to test the program:    make test
														Test results are logged to the output_*.txt files listed below.

Program Source:
    Part 1:                 runCommand.c
    Part 2:                 shell.c
    Part 3:                 shell2.c, joblist.c, shell2.h


Additional Files:
    Test files:             test_runCommand.sh, output_runCommand.txt, 
                                test_shell.txt, output_shell.txt
                                test_shell2.txt, output_shell2.txt,
                                test_shell2_no-stats.txt,
                                output_shell2_no-stats.txt,
    Other:                  Makefile, README.txt

Test Procedures:            runCommand was run using basic shell commands,
                                including some with 2 or more args.
                                Afterwards, a shell script was written to
                                automate this.
                            shell and shell2 were tested informally depending
                                on the functionality being addressed, and then
                                using the test files as input later on.

Test Data/Results:                  See "Additional files" above
