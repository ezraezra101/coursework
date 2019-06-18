Ezra Davis
November 16

Network homework assignment 1: Socket Programming

To make:
	running `make` will build both httpserver and httpclient

	`make clean` will delete both executables.

To run the client:
	`./httpclient [-p] hostname/path port`
	E.G.:
		./httpclient hibou.cs.wpi.edu/~kven/courses/CS3516-B15/home.html 80

To run the server:
	`./httpserver port`
	E.G.:
		./httpserver 12345

	Press Control-C to quit.


Testing local round trip time vs. remote round trip time:
	1. Modify test_round_trip_time.sh to point to your current computer and appropriate port.
	2. Run either:
		./test_round_trip_time.sh
		or
		make round_trip_time
