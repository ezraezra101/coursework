# Gareth Solbeck & Ezra Davis
# HW 4 for CS 3013: Operating Systems

CC := gcc
CFLAGS := -pthread
executables := main
libs := mem_lib.c alg_clock.c alg_aging.c alg_fifo.c

all:
	@ $(MAKE) $(executables)


#Multipurpose simple make command
%: %.c $(libs)
	$(CC) $(CFLAGS) -o $@ $< $(libs)

test: $(executables)
	echo "Write tests."
	./main

debug:
	$(MAKE) $(executables) CFLAGS="-g -D DEBUG"
	lldb $(executables)

clean:
	rm $(executables)
