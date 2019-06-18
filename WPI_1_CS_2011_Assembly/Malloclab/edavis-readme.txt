# Malloclab readme

Author:
* Ezra Davis (edavis@wpi.edu)
Class:
* CS 2011 with Prof. Lauer, section D04, 2014

Project:
* malloclab


This implementation of a malloc package uses binning to increase throughput, at the expense of utilization (compared to this implementation with NEXT_FIT preprocessor directive defined).

Binning:
--------
It uses 255 bins, starting at the smallest possible block size (16 bytes), and increasing from there. One bin is for any blocks that are too large for the other bins.
* The special large-allocation bin is navigated with first-fit searching.

Each bin is a doubly linked list (doubly linked for ease of removal from the bin).

Coalescing:
-----------
The implementation coalesces free nodes immediately, using boundary tag coalescing. This is by far simpler to implement, and occurs in constant time, so it does not significantly affect throughput (though the headers and footers affects utilization).

Boundary tags:
--------------
Each block has a header and footer (1 word each) that indicates the size of the block and its current allocation status.


A useful reference:
	http://gee.cs.oswego.edu/dl/html/malloc.html

	(Bryant and O'Hallaron's "Computer Systems: a Programmer's Perspective" second edition was invaluable as well.)

Where the base implementation from the textbook came from:
	http://csapp.cs.cmu.edu/public/ics2/code/vm/malloc/mm.c
