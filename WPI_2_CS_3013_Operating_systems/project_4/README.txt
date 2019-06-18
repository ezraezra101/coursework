Gareth Solbeck & Ezra Davis
HW 4 for CS 3013: Operating Systems

Purpose:
    To emulate a virtual memory system with three classes of storage:
        RAM, SSD, and the hard disk.
    For more information, see the original project specification

Build:
    make

Test:
    ./main [num_threads] [clock|aging|fifo]

API:
    See project specification or mem_lib.h

Algorithms:
    We implement three algorithms: clock, aging, and FIFO.
    However, the FIFO algorithm implemented is an oversimplication on FIFO;
        rather than having a stack of pages accessed, it simply loops through
        RAM, keeping track of its pointer once it evicts something. This means
        that it fails to handle freed pages as FIFO would.
    This was created as a basic algorithm to use while writing the page handler
        and page table. We recommend the use of the clock or aging algorithms instead.

Files:
    Algorithm source files:
     - alg_aging.c
     - alg_clock.c
     - alg_fifo.c
    Virtual memory API source:
     - mem_lib.c
    Testing program and supplements:
     - debug.c
     - main.c
    API header for user programs:
     - mem_lib.h
    Internal header files:
     - alg_aging.h
     - alg_clock.h
     - alg_fifo.h
     - debug.h
     - header.h
    Makefile:
     - Makefile
    Text files:
     - test_output.txt
     - README.txt
     - testing_methodology.txt
