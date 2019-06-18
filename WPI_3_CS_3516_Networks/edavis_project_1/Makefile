# Makefile by Ezra Davis

# Compiler
CC= gcc
COMPILER_OPTS= -Wall

# Libraries
LINK= -pthread
INCLUDE=

# Variables for test
HOSTNAME=ccc1.wpi.edu
PORT=12345
TESTSERVER=www.google.com


all: httpclient httpserver

%: %.c
	$(CC) $(COMPILER_OPTS) $(INCLUDE) $(LINK) $< -o $@

# A basic and not terribly helpful test
test: httpclient httpserver
	@rm -f log
	@echo "\n    Removed old logfile"
	@echo "TESTS aren't implemented!"
	
	./httpclient www.google.com 80
	./httpserver $(PORT)

	@echo "\n\n"

round_trip_time: all
	./test_round_trip_time.sh

clean:
	rm -f httpclient httpserver